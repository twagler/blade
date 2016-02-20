#ifndef GPS_RECEIVER_H
#define GPS_RECEIVER_H

#define NMEASOURCE_HOST "192.168.2.52"
#define NMEASOURCE_HOST_PORT    50002

#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QNmeaPositionInfoSource>
#include <QtNetwork/QTcpSocket>


class GPS_receiver : public QObject
{
    Q_OBJECT
public:
    explicit GPS_receiver(QObject *parent = 0);
    void init();

private:
    QGeoCoordinate myLocation;
    QTcpSocket *myNMEAsocket;
    QNmeaPositionInfoSource *myNMEASource;

signals:
    void publish_update(QGeoCoordinate);

public slots:


private slots:
    void printError(QAbstractSocket::SocketError socketError);
    void updateReceived();

};

#endif // GPS_RECEIVER_H
