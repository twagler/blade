#include <QtMath>
#include "navigation.h"

Navigation::Navigation(QObject *parent) : QObject(parent)
{
    startup=true;

}

void Navigation::Init()
{
    emit request_coordinate();
}

void Navigation::CalculcateCorrection(QGeoCoordinate current_coordinate)
{
    qDebug() << "Current coordinate: " << current_coordinate << endl;
    current_distance_to_destination_coordinate = current_coordinate.distanceTo(destination_coordinate);
    qDebug() << "Distance to target: " << current_distance_to_destination_coordinate << endl;

    if(current_distance_to_destination_coordinate < ARRIVED)
    {
            //TODO: make sure this only hits once per waypoint
        emit request_coordinate();
    }
    else //calculate error from path
    {

        float theta;
        float magnitude;
        float delta_heading;

        theta = starting_coordinate.azimuthTo(current_coordinate);

        qDebug() << "Azimuth from start to finish: " << destination_heading << endl;
        qDebug() << "Azimuth from start to current: " << theta << endl;

        delta_heading = destination_heading - theta;

        qDebug() << "Difference in those two: " << delta_heading << endl;

        magnitude = current_coordinate.distanceTo(starting_coordinate) * qSin(delta_heading);

        qDebug() << "qSin(delta_heading) * distance from start = " << magnitude << endl;

        //emit signal with error to the
    }
}

void Navigation::received_waypoint(QGeoCoordinate waypoint)
{
    if(startup)
    {
        destination_coordinate = waypoint;
        startup = false;
    }

    starting_coordinate = destination_coordinate;
    destination_coordinate = waypoint;

    current_pathlength = starting_coordinate.distanceTo(destination_coordinate);

    destination_heading = starting_coordinate.azimuthTo(destination_coordinate);

    qDebug() << "Starting Coordinate: " << starting_coordinate << endl;
    qDebug() << "Destination Coordinate: " << destination_coordinate << endl;
    qDebug() << "Current Path Length: " << current_pathlength << endl;
    qDebug() << "Destination Heading: " << destination_heading << endl;

}

