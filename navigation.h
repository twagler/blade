#ifndef NAVIGATION_H
#define NAVIGATION_H

#define ARRIVED 0.35 //m

#include <QObject>
#include <QtPositioning/QGeoCoordinate>

class Navigation : public QObject
{
    Q_OBJECT
public:
    explicit Navigation(QObject *parent = 0);

private:
    float kP, kI, kD;
    QGeoCoordinate starting_coordinate, destination_coordinate;

    //distances in meters set once per waypoint
    float current_pathlength,
          delta_LAT,
          delta_LON;

    //heading in degrees set once per waypoint
    float destination_heading;

    //distances in meters set once per GPS update

    float current_distance_to_destination_coordinate,
          current_distance_to_starting_coordinate,
          current_path_error;
signals:

public slots:
    void CalculcateCorrection(QGeoCoordinate);
};

#endif // NAVIGATION_H
