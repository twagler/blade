#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include "networkserver.h"
#include "gps_receiver.h"
#include "navigation.h"

class MainController : public QObject
{
    Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);
    void init();

private:
    MyServer myNetworkServer;
    GPS_receiver myGPS_recevier;
    Navigation myNavigator;

signals:

public slots:

private slots:


};

#endif // MAINCONTROLLER_H
