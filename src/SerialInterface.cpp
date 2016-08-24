//
// Created by discord on 23/08/16.
//

#include "SerialInterface.hpp"

SerialInterface::SerialInterface(char *serialport) : serialPort(serialport)
{
    fileStream = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    if (fileStream == -1)
    {
        //ERROR - CAN'T OPEN SERIAL PORT
        throw new std::exception();
    }

}

void SerialInterface::begin(int baudrate)
{
    struct termios options;
    tcgetattr(fileStream, &options);
    options.c_cflag = ((tcflag_t) baudrate) | CS8 | CLOCAL | CREAD;		//<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fileStream, TCIFLUSH);
    tcsetattr(fileStream, TCSANOW, &options);
}

void SerialInterface::setTimeout(int time)
{
    timeout = time;
}

void SerialInterface::write(char byte)
{

}

char SerialInterface::read()
{
    return 0;
}

void SerialInterface::flush()
{

}

int SerialInterface::readBytes(char * buffer, int nb)
{
    return 0;
}
