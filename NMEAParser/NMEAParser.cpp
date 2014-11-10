// NMEAParser.cpp: implementation of the NMEAParser class.
//
//////////////////////////////////////////////////////////////////////

#include "NMEAParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NMEAParser::NMEAParser()
{}

NMEAParser::~NMEAParser()
{}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr2(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int NMEAParser::TCPinit()
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(SERVER, PORT, &hints, &servinfo)) != 0) {
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

    inet_ntop(p->ai_family, get_in_addr2((struct sockaddr *)p->ai_addr),
              s, sizeof s);

    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    return sockfd;

}

int NMEAParser::read_RTKLIBserver()
{
    int sockfd, numbytes;
    char buf[MAXDATASIZE];
    char * pch;
    sockfd = this->TCPinit();
    while(1)
    {
        if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1)
        {
            perror("NMEAParser recv error...exiting\r\n");
            exit(1);
        }
        //The above recv will grab multiple lines at once
        //We need to split the buf on the '\n' character

        Parse(buf,numbytes);

    }

    close(sockfd);
    return 0;
}

int axtoi( const char *hexStg )
{
    int n = 0;         // position in string
    int m = 0;         // position in digit[] to shift
    int count;         // loop index
    int intValue = 0;  // integer value of hex string
    int digit[5];      // hold values to convert
    while (n < 4)
    {
        if (hexStg[n]=='\0')
            break;
        if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ) //if 0 to 9
            digit[n] = hexStg[n] & 0x0f;            //convert to int
        else if (hexStg[n] >='a' && hexStg[n] <= 'f') //if a to f
            digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
        else if (hexStg[n] >='A' && hexStg[n] <= 'F') //if A to F
            digit[n] = (hexStg[n] & 0x0f) + 9;      //convert to int
        else break;
        n++;
    }
    count = n;
    m = n - 1;
    n = 0;
    while(n < count)
    {
        intValue = intValue | (digit[n] << (m << 2));
        m--;   // adjust the position to set
        n++;   // next digit to process
    }
    return (intValue);
}

void NMEAParser::Parse(const char *buf, const unsigned int bufSize)
{
    for( unsigned int i = 0; i < bufSize; i++ )
        ParseRecursive(buf[i]);
}

void NMEAParser::ParseRecursive(const char ch)
{	
    enum NMEAParserState { SearchForSOS = 1,
                           RetrieveAddressField,
                           ReceiveSentenceData,
                           GetFirstChecksumCharacter,
                           GetSecondChecksumCharacter,
                           WaitForST,
                           ValidSentence };

    static const unsigned int ADDRESS_FIELD_MAX_LENGTH = 10;
    static const unsigned int NMEA_SEQUENCE_MAX_LENGTH = 101;

    static NMEAParserState m_State = SearchForSOS;
    static unsigned int m_CalcChecksum;
    static char m_Checksum[3];
    static char m_NMEASequence[NMEA_SEQUENCE_MAX_LENGTH];
    static unsigned int m_NMEASequenceIndex;
    static char m_AddressField[ADDRESS_FIELD_MAX_LENGTH];
    static unsigned int m_AddressFieldIndex;

    switch( m_State )
    {
    case SearchForSOS:
    {
        if( ch == '$' )
        {
            m_AddressFieldIndex = 0;
            m_NMEASequenceIndex = 0;
            m_CalcChecksum = 0;
            m_State = RetrieveAddressField;
        }
        break;
    }

    case RetrieveAddressField:
    {
        if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 )
            m_State = SearchForSOS;
        else
        {
            m_NMEASequence[m_NMEASequenceIndex++] = ch;
            m_CalcChecksum ^= ch;
            if( ch == ',' )
            {
                m_AddressField[m_AddressFieldIndex] = '\0';
                m_State = ReceiveSentenceData;
            }
            else if( m_AddressFieldIndex == ADDRESS_FIELD_MAX_LENGTH - 1 ||
                     !isalpha(ch) || islower(ch) )
                m_State = SearchForSOS;
            else
                m_AddressField[m_AddressFieldIndex++] = ch;
        }
        break;
    }

    case ReceiveSentenceData:
    {
        if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 )
            m_State = SearchForSOS;
        else
        {
            m_NMEASequence[m_NMEASequenceIndex++] = ch;
            if( ch == '*' )
                m_State = GetFirstChecksumCharacter;
            else if( ch == 10 )
                m_State = WaitForST;
            else if( ch == 13 )
            {
                m_NMEASequence[m_NMEASequenceIndex++] = ch;
                m_NMEASequence[m_NMEASequenceIndex] = '\0';
                ParseNMEASentence( m_AddressField, m_NMEASequence, m_NMEASequenceIndex );
                m_State = SearchForSOS;
            }
            else
                m_CalcChecksum ^= ch;
        }
        break;
    }

    case GetFirstChecksumCharacter:
    {
        if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
                ( !isdigit(ch) && ( ch < 'A' || ch > 'F' ) ) )
            m_State = SearchForSOS;
        else
        {
            m_NMEASequence[m_NMEASequenceIndex++] = ch;
            m_Checksum[0] = ch;
            m_State = GetSecondChecksumCharacter;
        }
        break;
    }

    case GetSecondChecksumCharacter:
    {
        if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
                ( !isdigit(ch) && ( ch < 'A' || ch > 'F' ) ) )
            m_State = SearchForSOS;
        else
        {
            m_NMEASequence[m_NMEASequenceIndex++] = ch;
            m_Checksum[1] = ch;
            m_Checksum[2] = '\0';
            unsigned int iChecksum = axtoi( m_Checksum );
            if( iChecksum == m_CalcChecksum )
                m_State = WaitForST;
            else
                m_State = SearchForSOS;
        }
        break;
    }

    case WaitForST:
    {
        if( m_NMEASequenceIndex == NMEA_SEQUENCE_MAX_LENGTH - 1 ||
                (ch != 10 && ch != 13) )
            m_State = SearchForSOS;
        else if(ch == 13)
        {
            m_NMEASequence[m_NMEASequenceIndex++] = ch;
            m_NMEASequence[m_NMEASequenceIndex] = '\0';
            ParseNMEASentence( m_AddressField, m_NMEASequence, m_NMEASequenceIndex );
            m_State = SearchForSOS;
        }
        break;
    }

    case ValidSentence:
        break;

    default:
        break;

    }

}

void NMEAParser::ParseNMEASentence(const char *addressField,
                                   const char *buf, const unsigned int bufSize)
{

    if( strcmp(addressField, "GPGGA") == 0 )
    {
        ProcessGPGGA(buf, bufSize);
    }
    else if( strcmp(addressField, "GPGSA") == 0 )
    {
        ProcessGPGSA(buf, bufSize);
    }
    else if( strcmp(addressField, "GPGSV") == 0 )
    {
        ProcessGPGSV(buf, bufSize);
    }
    else if( strcmp(addressField, "GPRMB") == 0 )
    {
        ProcessGPRMB(buf, bufSize);
    }
    else if( strcmp(addressField, "GPRMC") == 0 )
    {
        ProcessGPRMC(buf, bufSize);
    }
    else if( strcmp(addressField, "GPZDA") == 0 )
    {
        ProcessGPZDA(buf, bufSize);
    }
}

GPS& NMEAParser::GetActualGPSInfo()
{
    return myGPSInfo;
}

/*
  GPGGA Sentence format

  $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M, ,*47
    |    |      |		   |		   | |  |	|		|      | |
    |    |      |		   |		   | |  |	|		|	   | checksum data
    |    |      |		   |		   | |  |	|		|	   |
    |    |      |		   |		   | |  |	|		|	   empty field
    |    |      |		   |		   | |  |	|		|
    |    |      |		   |		   | |  |	|		46.9,M Height of geoid (m) above WGS84 ellipsoid
    |    |      |		   |		   | |  |	|
    |    |      |		   |		   | |  |	545.4,M Altitude (m) above mean sea level
    |    |      |		   |		   | |  |
    |    |      |		   |		   | |  0.9 Horizontal dilution of position (HDOP)
    |    |      |		   |		   | |
    |    |      |		   |		   | 08 Number of satellites being tracked
    |    |      |		   |		   |
    |    |      |		   |		   1 Fix quality:	0 = invalid
    |    |      |		   |							1 = GPS fix (SPS)
    |    |      |		   |							2 = DGPS fix
    |    |      |		   |							3 = PPS fix
    |    |      |		   |							4 = Real Time Kinematic
    |    |      |		   |							5 = Float RTK
    |    |      |		   |							6 = estimated (dead reckoning) (2.3 feature)
    |    |      |		   |							7 = Manual input mode
    |    |      |		   |							8 = Simulation mode
    |    |      |		   |
    |    |      |		   01131.000,E Longitude 11 deg 31.000' E
    |    |      |
    |    |      4807.038,N Latitude 48 deg 07.038' N
    |    |
    |    123519 Fix taken at 12:35:19 UTC
    |
    GGA Global Positioning System Fix Data

*/
void NMEAParser::ProcessGPGGA(const char *buf, const unsigned int bufSize)
{
    // To disable handling this sentence uncomment the next line
    // return;


    char auxBuf[10];
    const char *p1 = buf, *p2;

    // GGA
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if(bufSize < 6)
        return;
    strncpy(auxBuf, buf, 5);
    auxBuf[5] = '\0';
    if(strcmp(auxBuf, "GPGGA") != 0 || buf[5] != ',')
        return;
    p1 += 6;

    // Time
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    unsigned int hour, min, sec;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    hour = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    min = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    sec = atoi(auxBuf);
    p1 = p2 + 1;

    // Latitude
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    if((p2 = strchr(auxBuf, '.')) == NULL)
        return;
    if(p2-auxBuf < 2)
        return;
    double latitude = atof(p2 - 2) / 60.0;
    auxBuf[p2 - 2 - auxBuf] = '\0';
    latitude += atof(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 - p1 != 1)
        return;
    if(*p1 == 'S')
        latitude = -latitude;
    else if(*p1 != 'N')
        return;
    p1 = p2 + 1;

    // Longitude
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    if((p2 = strchr(auxBuf, '.')) == NULL)
        return;
    double longitude = atof(p2 - 2) / 60.0;
    auxBuf[p2 - 2 - auxBuf] = '\0';
    longitude += atof(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 - p1 != 1)
        return;
    if(*p1 == 'W')
        longitude = -longitude;
    else if(*p1 != 'E')
        return;
    p1 = p2 + 1;

    // GPS quality
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    unsigned int quality = atoi(auxBuf);

    // Satellites in use
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    unsigned int satelitesInUse = atoi(auxBuf);

    // HDOP
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double hdop = atoi(auxBuf);

    // Altitude
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double altitude = atoi(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 - p1 != 1)
        return;
    if(*p1 != 'M')
        return;
    p1 = p2 + 1;

    // Height of geoid
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double heightGeoid = atoi(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    //	if(p2 - p1 != 1)
    //		return;
    //	if(*p1 != 'M')
    //		return;
    p1 = p2 + 1;

    // Empty field
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    p1 = p2 + 1;

    // Last Empty field
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) != NULL)
        return;
    if((p2 = strchr(p1, '*')) == NULL)
        return;

    // Set the values of m_GPSInfo
    myGPSInfo.setLatitude(latitude);
    myGPSInfo.setLongitude(longitude);
    myGPSInfo.setAltitude(altitude);
    myGPSInfo.setSignalQuality(quality);
    myGPSInfo.setSatelitesInUse(satelitesInUse);
    myGPSInfo.incrementSentenceCount();

    //m_GPSInfo.m_time

}

void NMEAParser::ProcessGPGSA(const char *buf, const unsigned int bufSize)
{

}

void NMEAParser::ProcessGPGSV(const char *buf, const unsigned int bufSize)
{

}

void NMEAParser::ProcessGPRMB(const char *buf, const unsigned int bufSize)
{

}

/*
  Format
  $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    |	 |		| |			 |			 |	   |	 |		|	   |
    |	 |		| |			 |			 |	   |	 |		|	   *6A Checksum data
    |	 |		| |			 |			 |	   |	 |		|
    |	 |		| |			 |			 |	   |	 |		003.1,W Magnetic Variation
    |	 |		| |			 |			 |	   |	 |
    |	 |		| |			 |			 |	   |	 230394 Date - 23rd of March 1994
    |	 |		| |			 |			 |	   |
    |	 |		| |			 |			 |	   084.4 Track angle in degrees
    |	 |		| |			 |			 |
    |	 |		| |			 |			 022.4 Speed over the ground in knots
    |	 |		| |			 |
    |	 |		| |			 01131.000,E Longitude 11 deg 31.000' E
    |	 |		| |
    |	 |		| 4807.038,N Latitude 48 deg 07.038' N
    |	 |		|
    |	 |		A Status A=active or V=Void
    |	 |
    |	 123519 Fix taken at 12:35:19 UTC
    |
    RMC Recommended Minimum sentence C

*/
void NMEAParser::ProcessGPRMC(const char *buf, const unsigned int bufSize)
{
    char auxBuf[10];
    const char *p1 = buf, *p2;

    // GGA
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if(bufSize < 6)
        return;
    strncpy(auxBuf, buf, 5);
    auxBuf[5] = '\0';
    if(strcmp(auxBuf, "GPRMC") != 0 || buf[5] != ',')
        return;
    p1 += 6;

    // Time
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    unsigned int hour, min, sec;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    hour = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    min = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    sec = atoi(auxBuf);
    p1 = p2 + 1;


    // Status
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 == p1)
        return;
    if(*p1 != 'A')
        return;
    p1 = p2 + 1;

    // Latitude
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    if((p2 = strchr(auxBuf, '.')) == NULL)
        return;
    if(p2-auxBuf < 2)
        return;
    double latitude = atof(p2 - 2) / 60.0;
    auxBuf[p2 - 2 - auxBuf] = '\0';
    latitude += atof(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 - p1 != 1)
        return;
    if(*p1 == 'S')
        latitude = -latitude;
    else if(*p1 != 'N')
        return;
    p1 = p2 + 1;

    // Longitude
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    if((p2 = strchr(auxBuf, '.')) == NULL)
        return;
    double longitude = atof(p2 - 2) / 60.0;
    auxBuf[p2 - 2 - auxBuf] = '\0';
    longitude += atof(auxBuf);
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    if(p2 - p1 != 1)
        return;
    if(*p1 == 'W')
        longitude = -longitude;
    else if(*p1 != 'E')
        return;
    p1 = p2 + 1;

    // Ground speed
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double groundSpeed = atof(auxBuf);

    // Course over ground (degrees)
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double courseOverGround = atof(auxBuf);

    // Date
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    unsigned int day, month, year;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    day = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    month = atoi(auxBuf);
    p1 += 2;
    strncpy(auxBuf, p1, 2);
    auxBuf[2] = '\0';
    year = 2000 + atoi(auxBuf);
    p1 = p2 + 1;

    // Magnetic variation
    if((unsigned int)(p1 - buf) >= bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    auxBuf[p2 - p1] = '\0';
    p1 = p2 + 1;
    double magneticVariation = atof(auxBuf);

    if(*p1 == 'W')
        latitude = -latitude;
    else if(*p1 != 'E' && *p1 != '*')
        return;

    //Mode

    if((unsigned int)(p1 - buf) >=bufSize)
        return;
    if((p2 = strchr(p1, ',')) == NULL)
        return;
    strncpy(auxBuf, p1, p2 - p1);
    p1 = p2 + 1;
    char mode;
    if(*p1 == 'A')
        mode = 'A'; //Autonomous
    else if(*p1 == 'D')
        mode = 'D'; //Differential
    else if(*p1 == 'E')
        mode = 'E'; //Estimated
    else if(*p1 == 'N')
        mode = 'N'; //Not Valid
    else if(*p1 == 'S')
        mode = 'S'; //Simulated
    else
        return;

    //Checksum
    if((p2 = strchr(p1, '*')) == NULL)
        return;
    if(p2 - p1 > 1)
        return;

    // Set the values of m_GPSInfo
    myGPSInfo.setLatitude(latitude);
    myGPSInfo.setLongitude(longitude);
    myGPSInfo.incrementSentenceCount();
}

void NMEAParser::ProcessGPZDA(const char *buf, const unsigned int bufSize)
{

}
