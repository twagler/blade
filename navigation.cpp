#include <QtMath>
#include "navigation.h"

Navigation::Navigation(QObject *parent) : QObject(parent)
{

}

void Navigation::CalculcateCorrection(QGeoCoordinate current_coordinate)
{
    current_distance_to_destination_coordinate = current_coordinate.distanceTo(destination_coordinate);

    if(current_distance_to_destination_coordinate < ARRIVED)
    {
        //update destination_coordinate, path length, others
        //emit signal
    }
    else //calculate error from path
    {

        float theta;
        float magnitude;
        float delta_heading;

        theta = starting_coordinate.azimuthTo(current_coordinate);

        //qDebug() << "Heading from start: " << theta << "\r\n";

        delta_heading = destination_heading - theta;

        magnitude = current_coordinate.distanceTo(starting_coordinate) * qSin(delta_heading);

        //emit signal with error to
    }
}
