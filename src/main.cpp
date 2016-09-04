//
// Created by discord on 03/09/16.
//

#include <thread>
#include "DynamixelInterface.h"
#include "DynamixelMotor.h"

void delay(int time)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}


int main()
{
    for(DynamixelID i=227 ; i<256 ; i++)
    {
        DynamixelInterface *interface=createSerialInterface((char *) "/dev/ttyUSB0");
        DynamixelMotor motor(*interface, i); //TODO check ID for AX12

        std::cout << "      " << (int)i << std::endl;
        delay(100);
        interface->begin(9600);
        delay(100);

        motor.init();
        motor.enableTorque();

        // reset to middle position
        motor.jointMode();
        motor.speed(256);
        motor.goalPosition(0x1ff);
        delay(5000);
        motor.wheelMode();
    }
}