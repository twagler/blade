using namespace std;

#include "mower.h"

//use an array of waypoints for now.  A more robust mapping mechanism will be required
double LATwaypoint[] = {38.637431,   38.638,  38.639,  37.971432,  37.971417,  37.971495,  37.971497,  37.971432};
double LONwaypoint[] = {-86.921618, -86.921618, -86.921620, -87.529695, -87.529813, -87.529810, -87.529698, -87.529695};

//vector<GPS> waypoints;

mutex gps_lock;
condition_variable cv_gps;
GPS gps;

mutex drive_lock;
condition_variable cv_drive;

bool Autonomous = true;
bool first = true;
char adjustment;
char targetspeed = 55;

MotorDriver motors;

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int GPSclientInit(const char* server, const char* port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);

    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;

}

int ControlServerInit(const char* port)
{
    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int rv;
    int listener;     // listening socket descriptor
    struct addrinfo hints, *ai, *p;

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, port, &hints, &ai)) != 0)
    {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
        {
            continue;
        }

        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL)
    {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); // all done with this

    // listen
    if (listen(listener, 10) == -1)
    {
        perror("listen");
        exit(3);
    }
    return listener;
}

void ReadGPS_NMEA()
{
    cout << "Starting NMEA GPS Reception thread...\r\n";
    GPS temp;
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    sockfd = GPSclientInit(GPS_SERVER, GPS_PORT);
    while(1) //change to while gps enabled later
    {
        NMEAParser parser;
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
        {
            perror("NMEAParser recv error...exiting\r\n");
            exit(1);
        }
        temp = parser.Parse(buf,numbytes);
        //for debugging now.  later, we should require quality be '4'
        if(temp.getSignalQuality() != 0)
        {
            gps_lock.lock();
            gps = temp;
            gps_lock.unlock();
            cv_gps.notify_one();
        }
    }
    close(sockfd);
    return;
}

void SetSpeeds()
{
    cout << "Starting speed setting thread...\r\n";

    char leftspeed, rightspeed;

    while(true)
    {
        unique_lock<mutex> lk_drive(drive_lock);
        cv_drive.wait(lk_drive);

        leftspeed = targetspeed - adjustment;
        rightspeed = targetspeed + adjustment;

        lk_drive.unlock();

        if (leftspeed < MIN_SPEED)                 //cap min motor speed
            leftspeed = MIN_SPEED;
        else if (leftspeed>MAX_SPEED)
            leftspeed = MAX_SPEED;                  //cap max motor speed

        if (rightspeed<MIN_SPEED)
            rightspeed = MIN_SPEED;
        else if (rightspeed>MAX_SPEED)
            rightspeed = MAX_SPEED;

        if(!Autonomous)
        {
            leftspeed = 0;
            rightspeed = 0;
        }

        if (motors.getEnable())
        {
            motors.setSpeeds(leftspeed, rightspeed);
            motors.sendSpeeds();
        }
        //cout << "Motors: (" << (int)leftspeed << ","
        //     << (int)rightspeed << ")\r\n";
    }
}
float gps_distance(GPS one, GPS two)
{
    double dlong = (two.getLongitude() - one.getLongitude())*d2r;
    double dlat = (two.getLatitude() - one.getLatitude())*d2r;
    double a1 = sin(dlat/2.0);
    double a2 = sin(dlong/2.0);
    double a = (a1*a1) + cos(one.getLatitude()*d2r) *
            cos(two.getLatitude()*d2r) * (a2*a2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double d = 2.54 * 12 * 5280 * 3956 * c; //cm, in, ft, scaler

    return d;
}


