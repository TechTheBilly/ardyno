//
// Created by discord on 23/08/16.
//

#ifndef ARDYNO_SERIALINTERFACE_HPP
#define ARDYNO_SERIALINTERFACE_HPP

#include "Dynamixel.h"

class SerialInterface {

public:

    SerialInterface(char *serialport);

    void begin(int); //USE TCFLAGS DEFINES !!

    void setTimeout(int);

    void writeByte(uint8_t);

    char readByte();

    void flush();

    int readBytes(uint8_t *, int);

    int writeBytes(uint8_t *, int);

    void closeSerial();

private:

    char * serialPort;
    int fileStream = -1;
    long timeout = 0;
};


#endif //ARDYNO_SERIALINTERFACE_HPP
