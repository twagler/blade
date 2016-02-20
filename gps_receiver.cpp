#include "gps_receiver.h"


GPS_receiver::GPS_receiver(QObject *parent) : QObject(parent)
{
    myNMEASource = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode, this);
    myNMEAsocket = new QTcpSocket(this);
    //myLocation = new QGeoCoordinate();

    connect(myNMEAsocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(printError(QAbstractSocket::SocketError)));

    connect(myNMEASource, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(updateReceived()));

}

void GPS_receiver::init()
{
    myNMEAsocket->connectToHost(NMEASOURCE_HOST, NMEASOURCE_HOST_PORT, QTcpSocket::ReadOnly);
    myNMEASource->setDevice(myNMEAsocket);
    myNMEASource->startUpdates();

}

void GPS_receiver::printError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
        case QAbstractSocket::RemoteHostClosedError:
            qDebug() << "The remote host closed the socket.\r\n";
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug() << "The host was not found.  Please check the host name and port settings.\r\n";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug() << "The connection was refused by the peer.  Make sure the server is running and check that the host name and port settings are correct.\r\n";
            break;
        default:
            qDebug() << "The following error occurred:" << myNMEAsocket->errorString() << "\r\n";
    }
}

void GPS_receiver::updateReceived()
{
    myLocation = (myNMEASource->lastKnownPosition().coordinate());
    emit publish_update(myLocation);
}
