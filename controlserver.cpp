#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sstream>
#include "controlserver.h"
#include "mower.h"


ControlServer::ControlServer()
{

}

void ControlServer::run(void)
{
    int i;
    fd_set master;    // master file descriptor list
    fd_set read_fds;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int newfd;        // newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char buf[256];    // buffer for client data
    int nbytes;

    char remoteIP[INET6_ADDRSTRLEN];

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&read_fds);

    listener = ControlServerInit(CONTROL_PORT);

    // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one

    // main loop
    while(true)
    {
        read_fds = master; // copy it
        if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        // run through the existing connections looking for data to read
        for(i = 0; i <= fdmax; i++)
        {
            if (FD_ISSET(i, &read_fds))  // we got one!!
            {
                if (i == listener)
                {
                    // handle new connections
                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener,(struct sockaddr *)&remoteaddr,&addrlen);

                    if (newfd == -1)
                        perror("accept");
                    else
                    {
                        FD_SET(newfd, &master); // add to master set
                        if (newfd > fdmax)    // keep track of the max
                            fdmax = newfd;

                        printf("selectserver: new connection from %s on socket %d\n",
                               inet_ntop(remoteaddr.ss_family,
                                         get_in_addr((struct sockaddr*)&remoteaddr),
                                         remoteIP, INET6_ADDRSTRLEN), newfd);
                    }
                }
                else
                {
                    // handle data from a client
                    if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0)
                    {
                        // got error or connection closed by client
                        if (nbytes == 0) //connection closed
                            printf("selectserver: socket %d hung up\n", i);
                        else
                            perror("recv");

                        close(i); // bye!
                        FD_CLR(i, &master); // remove from master set
                    }
                    else // we got some data...
                        ParseCommand(buf);
                }
            }
        }
    }
}

void ControlServer::ParseCommand(char buf[])
{
    string command, value;
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss.str(string(buf));
    ss >> command >> value;

    //command should probably be an enum
    //then we could switch() it
    if(command=="mode")
    {
        if(value=="autonomous")
        {
            Autonomous=1;
            printf("Changing to AUTO\r\n");
        }
        else
        {
            Autonomous=0;
            printf("Changing to MANUAL\r\n");
        }
    }
    else if(command=="halt")
    {
        //stop blades, motors, etc...
        motors.setMotorEnable(false);
    }

    else if (command=="Report_Location")
    {

    }

    else if (command =="Report_Battery_Status")
    {

    }

    else
        printf("Bad input.  What is: %s?", buf);

}

