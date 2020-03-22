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

class Uart{
    //-----------emums------------
    enum class PortState { CLOSED, OPEN };
    enum class BaudRate { BR9600, BR115200 };
    enum class Device { TTYS0, TTYAMA0 };

public:
    //Constructors and destructor
    Uart(const std::string &devName, BaudRate baud);
    Uart();
    ~Uart();

public:
    //-----getters and setters------
    void setDevice(const std::string &devName);
    void setBaudRate(BaudRate baud);

    //--------other methods-----------
    void openPort();
    void closePort();

    void read(std::string& readInData);
    void write(std::string& writeOutData);

private:
    //--------variables--------
    std::string deviceName;
    BaudRate baudRate;
    int device;
    PortState portState;

    std::vector<char> readData;
    short readDataSize;

    static const short max_bufferSize = 1024;
    //--------methods---------
    void conf();

};
#endif //BEESAFEPI_UART_H


