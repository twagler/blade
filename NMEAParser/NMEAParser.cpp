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

void NMEAParser::read_serial()
{
    struct termios tio;
    struct termios stdio;
    int tty_fd;
    unsigned char c = 0;

    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag=0;
    stdio.c_oflag=0;
    stdio.c_cflag=0;
    stdio.c_lflag=0;
    stdio.c_cc[VMIN]=1;
    stdio.c_cc[VTIME]=0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       // make the reads non-blocking

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

    tty_fd=open(SERIAL_DEVICE, O_RDWR);
    cfsetospeed(&tio,B115200);            // 115200 baud
    cfsetispeed(&tio,B115200);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);

    while(true)
    {
        read(tty_fd,&c,1);
        ParseRecursive(c);
    }
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
    static const unsigned int NMEA_SEQUENCE_MAX_LENGTH = 81;

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

GPSInfo& NMEAParser::GetActualGPSInfo()
{
    return m_GPSInfo;
}

/*
GPGGA Sentence format

$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M, ,*47
   |   |	  |			 |			 | |  |	  |		  |      | |
   |   |	  |			 |			 | |  |	  |		  |		 | checksum data
   |   |	  |			 |			 | |  |	  |		  |		 |
   |   |	  |			 |			 | |  |	  |		  |		 empty field
   |   |	  |			 |			 | |  |	  |		  |
   |   |	  |			 |			 | |  |	  |		  46.9,M Height of geoid (m) above WGS84 ellipsoid
   |   |	  |			 |			 | |  |	  |
   |   |	  |			 |			 | |  |	  545.4,M Altitude (m) above mean sea level
   |   |	  |			 |			 | |  |
   |   |	  |			 |			 | |  0.9 Horizontal dilution of position (HDOP)
   |   |	  |			 |			 | |
   |   |	  |			 |			 | 08 Number of satellites being tracked
   |   |	  |			 |			 |
   |   |	  |			 |			 1 Fix quality:	0 = invalid
   |   |	  |			 |							1 = GPS fix (SPS)
   |   |	  |			 |							2 = DGPS fix
   |   |	  |			 |							3 = PPS fix
   |   |	  |			 |							4 = Real Time Kinematic
   |   |	  |			 |							5 = Float RTK
   |   |	  |			 |							6 = estimated (dead reckoning) (2.3 feature)
   |   |	  |			 |							7 = Manual input mode
   |   |	  |			 |							8 = Simulation mode
   |   |	  |			 |
   |   |	  |			 01131.000,E Longitude 11 deg 31.000' E
   |   |	  |
   |   |	  4807.038,N Latitude 48 deg 07.038' N
   |   |
   |   123519 Fix taken at 12:35:19 UTC
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
    m_GPSInfo.m_latitude = latitude;
    m_GPSInfo.m_longitude = longitude;
    m_GPSInfo.m_altitude = altitude;
    m_GPSInfo.m_nSentences++;
    m_GPSInfo.m_signalQuality = quality;
    m_GPSInfo.m_satelitesInUse = satelitesInUse;
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
    //	if(*p1 != 'A')
    //		return;
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
    if((p2 = strchr(p1, '*')) == NULL)
        return;
    if(p2 - p1 > 1)
        return;
    if(*p1 == 'W')
        latitude = -latitude;
    else if(*p1 != 'E' && *p1 != '*')
        return;

    // Set the values of m_GPSInfo
    m_GPSInfo.m_latitude = latitude;
    m_GPSInfo.m_longitude = longitude;
    m_GPSInfo.m_nSentences++;
}

void NMEAParser::ProcessGPZDA(const char *buf, const unsigned int bufSize)
{

}
