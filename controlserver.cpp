#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sstream>
#include "controlserver.h"
#include "mower.h"


ControlServer::ControlServer()
{

}

int ControlServer::set_previous_location(GPS prev)
{
    try
    {
        my_previous_location = prev;
    }
    catch (int n)
    {}
    return 0;
}

int ControlServer::set_current_location(GPS curr)
{
    my_current_location = curr;
    return 0;
}

int ControlServer::set_next_location(GPS next)
{
    my_next_location = next;
    return 0;
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
        for(int i=0;i<256;i++)
            buf[i] = 0;

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

                        ParseCommand(buf, i);

                }
            }
        }
    }
}

void ControlServer::ParseCommand(char buf[], int fd)
{
    int command, value, bytes_sent;
    std::string dataToSend;
    stringstream ss(stringstream::in | stringstream::out);
    ss.str("");
    ss.str(string(buf));
    ss >> command >> value;

    switch(command)
    {
    case MODE:
        printf("Changing MODE to: ");
        if(value==MODE_AUTO)
        {
            Autonomous=1;
            printf("AUTO\r\n");
        }
        else //anything else defaults to manual control
        {
            Autonomous=0;
            printf("MANUAL\r\n");
        }
        break;

    case GET:
        printf("GET received...\r\n");
        if(value == CURR_LOC)
        {
            //send the current location GPS object

            dataToSend = my_current_location.toString();

            if ((bytes_sent = send(fd,dataToSend.c_str(),dataToSend.size(),0)) <=0)
            {} //do some error handling
            else
            {}//yay

        }
        else if(value == PREV_LOC)
        {
            //send the previous location GPS object
            dataToSend = my_previous_location.toString();

            if ((bytes_sent = send(fd,dataToSend.c_str(),dataToSend.size(),0)) <=0)
            {} //do some error handling
            else
            {}//yay
        }
        else if(value == NEXT_LOC)
        {
            //send the next location GPS object
            dataToSend = my_next_location.toString();

            if ((bytes_sent = send(fd,dataToSend.c_str(),dataToSend.size(),0)) <=0)
            {} //do some error handling
            else
            {}//yay
        }


        break;

    //default:
    //   printf("Bad input.  What is: %s?", buf);
     //   break;
    }
}

