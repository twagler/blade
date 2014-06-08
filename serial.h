#ifndef SERIAL_H
#define SERIAL_H

class serial
{
public:
    serial();
    int serial_initialize();
    int serial_write(char*);
    char* serial_read();
    int serial_close(int);

private:
    int my_serial_fd;
};

#endif // SERIAL_H
