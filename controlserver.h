#ifndef CONTROLSERVER_H
#define CONTROLSERVER_H

class ControlServer
{

public:

    ControlServer();
    void run();
    void ParseCommand(char[]);

private:
    int initialize();

};

#endif
