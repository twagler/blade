#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <QObject>

class UltraSonicSensor : public QObject
{
    Q_OBJECT
public:
    explicit UltraSonicSensor(QObject *parent = 0);

signals:

public slots:
};

#endif // ULTRASONICSENSOR_H
