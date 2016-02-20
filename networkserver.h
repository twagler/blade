#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QTcpServer>
#include "networkserverthread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer();

private:
    enum commands
    {
        MODE,
                MODE_AUTO,
                MODE_MANUAL,
        START,
                START_1,
                START_2,
        STOP,
        GET,
                CURR_LOC,
                PREV_LOC,
                NEXT_LOC,

    };

signals:

public slots:
    void receivedata(QByteArray);

protected:
    void incomingConnection(qintptr socketDescriptor);

};
#endif
