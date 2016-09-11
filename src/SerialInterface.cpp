//
// Created by discord on 23/08/16.
//

#include <thread>
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
    struct serial_rs485 rs485conf = serial_rs485();
    rs485conf.flags |= SER_RS485_ENABLED;
    rs485conf.delay_rts_before_send = 0x00000004;
    ioctl(fileStream, TIOCSRS485, &rs485conf);
    struct termios options;
    tcgetattr(fileStream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(fileStream, TCIFLUSH);
    tcsetattr(fileStream, TCSANOW, &options);
    rs485_receiver(1);
}

void SerialInterface::setTimeout(int time)
{
    timeout = time;
}

void SerialInterface::writeByte(uint8_t byte)
{
    rs485_receiver(0);

    ssize_t count = write(fileStream, &byte, 1);
    if(count < 0)
        std::cout << "Can't write in Serial Port !" << std::endl;

    std::cout << "wrote : " << (int)byte << std::endl;
    rs485_receiver(1);


}

char SerialInterface::readByte()
{
    uint8_t res = 0;

    //std::this_thread::sleep_for(std::chrono::milliseconds(10));

    ssize_t count = read(fileStream, &res, 1);
    if(count < 0)
        std::cout << "Can't read in Serial Port !" << std::endl;
    if(count == 0)
        std::cout << "No data avail." << std::endl;

    std::cout << "read" << std::endl;

    return res;
}

void SerialInterface::flush()
{
    //UNUSED
}

int SerialInterface::readBytes(uint8_t * buffer, int nb)
{

   // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ssize_t count = read(fileStream, buffer, (size_t) nb);

    if(count < 0)
        std::cout << "Can't read in Serial Port !" << std::endl;
    if(count == 0)
        std::cout << "No data avail." << std::endl;

    std::cout << "read " << nb << std::endl;

    for(int i=0 ; i<nb ; i++)
        std::cout << i << " : " << (int)buffer[i] << std::endl;

    return (int) count;
}

void SerialInterface::closeSerial()
{
    close(fileStream);
}

int SerialInterface::writeBytes(uint8_t * buffer, int nb) {
    rs485_receiver(0);
    ssize_t count = write(fileStream, buffer, nb);
    if (count < 0)
        std::cout << "Can't write in Serial Port !" << std::endl;

    std::cout << "wrote " << nb << std::endl;
    rs485_receiver(1);

    return (int) count;
}

void SerialInterface::rs485_receiver(int on)
{
    struct termios options;

    tcgetattr(fileStream, &options);
    if(on)
        options.c_cflag |= CREAD;
    else
        options.c_cflag &= ~CREAD;
    tcsetattr(fileStream, TCSANOW, &options);
    fcntl(fileStream, F_SETFL, FNDELAY);

    int bits;
    ioctl(fileStream, TIOCMGET, & bits);

    if (on)
        bits |= TIOCM_RTS;
    else
        bits &= ~TIOCM_RTS;
    ioctl(fileStream, TIOCMSET, & bits);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

}