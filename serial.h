#ifndef SERIAL_H
#define SERIAL_H

class serial
{
public:
    serial();
    int serial_initialize(const char*);
    int serial_write(char*);
    char* serial_read();
    int serial_close(int);

private:
    int my_serial_fd;
    int my_baud_rate;
    int my_parity;
    char* my_port_name;
};

#endif // SERIAL_H
