
#include "DynamixelInterface.h"


DynamixelInterface *createSerialInterface(char *serialPort)
{
	return new DynamixelInterfaceImpl<HardwareSerial>(serialPort);
}

DynamixelInterface *createSerialInterface(char *serialPort, uint8_t aDirectionPin)
{
	return new DynamixelInterfaceImpl<HardwareSerial>(serialPort, aDirectionPin);
}


namespace
{
class DynSoftwareSerial
{
	public:
	DynSoftwareSerial(char *serialPort):
		serialPort(serialPort)
	{
        fileStream = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
        if (fileStream == -1)
        {
            //ERROR - CAN'T OPEN SERIAL PORT
            throw new std::exception();
        }
        struct termios options;
        tcgetattr(fileStream, &options);
        options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
        options.c_iflag = IGNPAR;
        options.c_oflag = 0;
        options.c_lflag = 0;
        tcflush(fileStream, TCIFLUSH);
        tcsetattr(fileStream, TCSANOW, &options);
	}

	private:
	char * serialPort;
    int fileStream = -1;
};
}
DynamixelInterface *createSoftSerialInterface(char *serialPort)
{
	DynSoftwareSerial &serial=*new DynSoftwareSerial(serialPort);
	return new DynamixelInterfaceImpl<DynSoftwareSerial>(serial, DynamixelInterfaceImpl<DynSoftwareSerial>::NO_DIR_PORT, true);
}

DynamixelInterface *createSoftSerialInterface(char *serialPort, uint8_t aDirectionPin)
{
	DynSoftwareSerial &serial=*new DynSoftwareSerial(serialPort);
	return new DynamixelInterfaceImpl<DynSoftwareSerial>(serial, aDirectionPin, true);
}


template<>
void setReadMode<DynSoftwareSerial>(DynSoftwareSerial &aStream)
{
	aStream.disableTx();
	aStream.listen();
}

template<>
void setWriteMode<DynSoftwareSerial>(DynSoftwareSerial &aStream)
{
	aStream.stopListening();
	aStream.enableTx();
}


