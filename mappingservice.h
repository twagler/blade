#ifndef MAPPINGSERVICE_H
#define MAPPINGSERVICE_H

#include <QObject>
#include <QtPositioning/QGeoCoordinate>

class MappingService : public QObject
{
    Q_OBJECT
public:
    explicit MappingService(QObject *parent = 0);
private:
    QList<QGeoCoordinate> my_list;
    int my_iterator;
signals:
    void send_waypoint(QGeoCoordinate);

public slots:
    void Request_Waypoint_Received();
};

#endif // MAPPINGSERVICE_H
