//
// Created by discord on 23/08/16.
//

#include "../include/SerialInterface.hpp"

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

void SerialInterface::writeByte(uint8_t byte)
{
    ssize_t count = write(fileStream, &byte, 1);
    if(count < 0)
        std::cout << "Can't write in Serial Port !" << std::endl;

 /*   uint8_t res = 0;
    count = read(fileStream, &res, 1);
    if(count < 0)
        std::cout << "Could not evacuate byte after write" << std::endl;*/

}

char SerialInterface::readByte()
{
    uint8_t res = 0;

    ssize_t count = read(fileStream, &res, 1);
    if(count < 0)
        std::cout << "Can't read in Serial Port !" << std::endl;
    if(count == 0)
        std::cout << "No data avail." << std::endl;

    return res;
}

void SerialInterface::flush()
{
    //UNUSED
}

int SerialInterface::readBytes(uint8_t * buffer, int nb)
{
    ssize_t count = read(fileStream, buffer, (size_t) nb);

    if(count < 0)
        std::cout << "Can't read in Serial Port !" << std::endl;
    if(count == 0)
        std::cout << "No data avail." << std::endl;

    return (int) count;
}

void SerialInterface::closeSerial()
{
    close(fileStream);
}

int SerialInterface::writeBytes(uint8_t * buffer, int nb) {
    ssize_t count = write(fileStream, buffer, nb);
    if (count < 0)
        std::cout << "Can't write in Serial Port !" << std::endl;

   /* uint8_t * buf;
    ssize_t count2 = read(fileStream, buf, nb);
    if(count2 < 0 || count != count2)
        std::cout << "Could not evacuate all bytes after write" << std::endl;*/

    return (int) count;
}