

#ifndef DYNAMIXEL_INTERFACE_H
#define DYNAMIXEL_INTERFACE_H

#include "SerialInterface.hpp"


template<class T>
static void setReadMode(T &aStream);

template<class T>
static void setWriteMode(T &mStream);


template<class T>
class DynamixelInterfaceImpl:public DynamixelInterface
{
	private:
	/** \brief Switch stream to read (receive)) mode */
	void readMode()
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			writePin(mDirectionPin, LOW);
		}
		else
		{
		//	setReadMode(mStream);
		}
	}
	
	/** \brief Switch stream to write (send) mode */
	void writeMode()
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			writePin(mDirectionPin, HIGH);
		}
		else
		{
			//setWriteMode(mStream);
		}
	}
	
	public:
	
	/**
	 * \brief Constructor
	 * \param[in] aStreamController : stream controller that abstract real stream
	 * \param[in] aDirectionPin : direction pin, use NO_DIR_PORT if you do not one (default)
	 * \param[in] aTranferOwnership : if true, the stream will be deleted in the destructor
	*/
	DynamixelInterfaceImpl(T &aStream, uint8_t aDirectionPin=NO_DIR_PORT, bool aTranferOwnership=false):
		mStream(aStream), mDirectionPin(aDirectionPin), mStreamOwner(aTranferOwnership)
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			writePin(mDirectionPin, LOW);
			setMode(mDirectionPin, OUTPUT);
		}
	}
	
	/**
	 * \brief Destructor
	 * Delete stream if it is owned by the instance
	 */
	~DynamixelInterfaceImpl()
	{
		if(mStreamOwner)
			delete &mStream;
	}
	
	/**
	 * \brief Start interface
	 * \param[in] aBaud : Baudrate
	 *
	 * Start the interface with desired baudrate, call once before using the interface
	*/
	void begin(unsigned long aBaud)
	{
		mStream.begin(aBaud);
		//mStream.setTimeout(50); //warning : response delay seems much higher than expected for some operation (eg writing eeprom)
		//readMode();
	}
	
	/**
	 * \brief Send a packet on bus
	 * \param[in] aPacket : Packet to send
	 *
	 * The function wait for the packet to be completly sent (using Stream.flush)
	*/
	void sendPacket(const DynamixelPacket &aPacket)
	{
		writeMode();
	
		mStream.writeByte(0xFF);
		mStream.writeByte(0xFF);
		mStream.writeByte(aPacket.mID);
		mStream.writeByte(aPacket.mLength);
		mStream.writeByte(aPacket.mInstruction);
		uint8_t n=0;
		if(aPacket.mAddress!=255)
		{
			mStream.writeByte(aPacket.mAddress);
			++n;
		}
		if(aPacket.mDataLength!=255)
		{
			mStream.writeByte(aPacket.mDataLength);
			++n;
		}
		if(aPacket.mLength>(2+n))
		{
			if(aPacket.mIDListSize==0)
			{
				mStream.writeBytes(aPacket.mData, aPacket.mLength-2-n);
			}
			else
			{
				uint8_t *ptr=aPacket.mData;
				for(uint8_t i=0; i<aPacket.mIDListSize; ++i)
				{
					mStream.writeByte(aPacket.mIDList[i]);
					mStream.writeBytes(ptr, aPacket.mDataLength);
					ptr+=aPacket.mDataLength;
				}
			}
		}
		mStream.writeByte(aPacket.mCheckSum);
		mStream.flush();
		readMode();
	}
	/**
	 * \brief Receive a packet on bus
	 * \param[out] aPacket : Received packet. mData field must be previously allocated
	 *
	 * The function wait for a new packet on the bus. Timeout depends of timeout of the underlying stream.
	 * Return error code in case of communication error (timeout, checksum error, ...)
	*/
	void receivePacket(DynamixelPacket &aPacket)
	{
		static uint8_t buffer[3];
		aPacket.mIDListSize=0;
		aPacket.mAddress=255;
		aPacket.mDataLength=255;
		if(mStream.readBytes(buffer,2)<2)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(mStream.readBytes(buffer, 3)<3)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		aPacket.mID=buffer[0];
		aPacket.mLength=buffer[1];
		aPacket.mStatus=buffer[2];
		if(aPacket.mLength>2 && mStream.readBytes(aPacket.mData, aPacket.mLength - 2) < (aPacket.mLength - 2))
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(mStream.readBytes(&(aPacket.mCheckSum),1)<1)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(aPacket.checkSum()!=aPacket.mCheckSum)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_CHECKSUM_ERROR;
		}
	}
	
	static const uint8_t NO_DIR_PORT=255;
	
	private:
	
	T &mStream;
	const uint8_t mDirectionPin;
	bool mStreamOwner;
};

#endif
