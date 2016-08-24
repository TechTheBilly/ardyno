//
// Created by discord on 23/08/16.
//

#ifndef ARDYNO_SERIALINTERFACE_HPP
#define ARDYNO_SERIALINTERFACE_HPP

#import "Dynamixel.h"

class SerialInterface {

    SerialInterface(char *serialport);

    void begin(int); //USE TCFLAGS DEFINES !!

    void setTimeout(int);

    void write(char);

    char read();

    void flush();

    int readBytes(char*, int);

private:
    char * serialPort;
    int fileStream = -1;
    int timeout = -1;
};


#endif //ARDYNO_SERIALINTERFACE_HPP
