#include "networkserver.h"

MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
}

void MyServer::startServer()
{
    int port = 50001;

    if(!this->listen(QHostAddress::Any,port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    MyThread *thread = new MyThread(socketDescriptor, this);

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(datapush(QByteArray)), this, SLOT(receivedata(QByteArray)));

    thread->start();
}

//This slot catches the incomming data from ALL connected clients/threads
//Probably want to implement something to make that a little less troublesome
void MyServer::receivedata(QByteArray data)
{
    QList<QByteArray> list = data.split(' ');

    QByteArray command_str, value_str;

    if(list.size() <2)
        return;
    list[0] = command_str;
    list[1] = value_str;

    int command = command_str.toInt();
    int value = value_str.toInt();

    switch(command)
    {
    case MODE:
        qDebug() << "Changing MODE to: ";
        if(value==MODE_AUTO)
        {
            //Autonomous=1;
            qDebug() <<"AUTO\r\n";
        }
        else //anything else defaults to manual control
        {
            //Autonomous=0;
            qDebug() << "MANUAL\r\n";
        }
        break;

    case GET:
        qDebug() << "GET received...\r\n";
        if(value == CURR_LOC)
        {
            //send the current location GPS object
        }
        else if(value == PREV_LOC)
        {
            //send the previous location GPS object
        }
        else if(value == NEXT_LOC)
        {
            //send the next waypoint GPS object
        }

        break;

    default:
        qDebug() << "Bad input.  What is: " << data << "\r\n";
        break;
    }
}
