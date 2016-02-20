#ifndef SABERTOOTHDRIVER_H
#define SABERTOOTHDRIVER_H

#include <QObject>


class SabertoothDriver : public QObject
{
    Q_OBJECT
public:
    explicit SabertoothDriver(QObject *parent = 0);

signals:

public slots:
};

#endif // SABERTOOTHDRIVER_H
