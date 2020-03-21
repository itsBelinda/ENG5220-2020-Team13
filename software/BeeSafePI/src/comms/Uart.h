#ifndef BEESAFEPI_UART_H
#define BEESAFEPI_UART_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>

class Uart{
public:
    //Constructors and destructor
     Uart();
    ~Uart();

private:
    //std::string& device;
    //std::string& message;

};
#endif //BEESAFEPI_UART_H

