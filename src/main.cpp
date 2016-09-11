//
// Created by discord on 03/09/16.
//

#include <thread>
#include <stdlib.h>
#include "../include/DynamixelInterface.h"
#include "../include/DynamixelMotor.h"

void delay(int time)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
}


int main()
{

    DynamixelInterface *interface=createSerialInterface((char *) "/dev/ttyUSB0");
    DynamixelMotor motor(*interface, 1); //TODO check ID for AX12

    delay(100);
    interface->begin(9600);
    delay(100);

    motor.write(0x04, (int)207);
    motor.write(0x10, (uint8_t)1);

    std::cout << std::to_string((int)motor.init()) << std::endl;

    //motor.setID(0x01);

    motor.enableTorque();

    // reset to middle position
    motor.jointMode();
    motor.speed(256);
    motor.goalPosition(0x1ff);

    motor.write(0x19, true);

    std::cout << std::to_string((int)motor.currentPosition()) << std::endl;


    // delay(5000);
    //motor.wheelMode();
}