
#include "DynamixelInterface.h"


DynamixelInterface *createSerialInterface(char *serialPort)
{
    SerialInterface *s = new SerialInterface(serialPort);
	return new DynamixelInterfaceImpl<SerialInterface>(*s);
}

DynamixelInterface *createSerialInterface(char *serialPort, uint8_t aDirectionPin)
{
	SerialInterface *s = new SerialInterface(serialPort);
    return new DynamixelInterfaceImpl<SerialInterface>(*s, aDirectionPin);
}


namespace
{
class DynSoftwareSerial : public SerialInterface
{
	public:
	DynSoftwareSerial(char *serialPort):
		SerialInterface(serialPort)
	{}
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
	//aStream.disableTx();
	//aStream.listen();
}

template<>
void setWriteMode<DynSoftwareSerial>(DynSoftwareSerial &aStream)
{
	//aStream.stopListening();
	//aStream.enableTx();
}


