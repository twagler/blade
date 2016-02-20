#include "maincontroller.h"
#include <QTcpServer>

MainController::MainController(QObject *parent) : QObject(parent)
{
    //myNetworkServer = new NetworkServer(this);

    //MyServer server;

    //myNetworkServer.startServer();

    /*myNetworkServer = new QTcpServer(this);

    connect(myNetworkServer, SIGNAL(newConnection()), this, SLOT(handleconnection()));

    if(!myNetworkServer->listen())
        qDebug() << "FAILED TO LISTEN\r\n";
    else
    {

        qDebug() << "Network Server status: " << myNetworkServer->isListening() << "\r\n";

        qDebug() << "IP: " << myNetworkServer->serverAddress() << "\r\n";

        qDebug() << "Port: " << myNetworkServer->serverPort() << "\r\n";

        qDebug() << "Errors: " << myNetworkServer->errorString() << "\r\n";

    }
    //FIXME: Initialize the GPS receiver with the IP and port of the server
    //myGPS_recevier = new GPS_receiver(IP,PORT,this);

    myGPS_recevier = new GPS_receiver(this);

    myGPS_recevier->init();
*/


    //exec();

}

void MainController::init()
{
    myNetworkServer.startServer();
    myGPS_recevier.init();

    connect(&myGPS_recevier, SIGNAL(publish_update(QGeoCoordinate)), &myNavigator, SLOT(CalculcateCorrection(QGeoCoordinate)));
}
