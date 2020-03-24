#ifndef BEESAFEPI_UART_H
#define BEESAFEPI_UART_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <termios.h>

#define BAUD_RATE 115200
#define DEVICE "/dev/ttyS0"

class Uart{
    //-----------enums------------
    enum class PortState { CLOSED, OPEN };

public:
    //Constructors and destructor
    Uart();
    ~Uart();

    std::string getReadBuffer();

public:
    //-----getters and setters------

    //--------other methods-----------
    void read(std::string& readInData);
    void write(std::string& writeOutData);

private:
    //--------variables--------
    int device;
    PortState portState;

    std::vector<char> readData;
    short readDataSize;

    static const short max_bufferSize = 1024;
    //--------methods---------
    void conf();

};
#endif //BEESAFEPI_UART_H



