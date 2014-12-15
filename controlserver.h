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

    enum commands
    {
        MODE,
            MODE_AUTO,
            MODE_MANUAL,
        START,
            START_1,
            START_2,
        STOP
    };

};

#endif
