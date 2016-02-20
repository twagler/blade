#include "mappingservice.h"

MappingService::MappingService(QObject *parent) : QObject(parent)
{
    my_list.append(QGeoCoordinate(-12.18835,96.83397));
    my_list.append(QGeoCoordinate(-12.18836,96.834));
    my_list.append(QGeoCoordinate(-12.1883504,96.85));
    my_list.append(QGeoCoordinate(-12.189,96.836));

    my_iterator = 0;
}

void MappingService::Request_Waypoint_Received()
{
    emit send_waypoint(my_list.at(my_iterator));
    my_iterator++;
}
