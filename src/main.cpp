//
// Created by discord on 03/09/16.
//

#include "DynamixelInterface.h"
#include "DynamixelMotor.h"

void delay(int time)
{
    //TODO
}


int main()
{
    DynamixelInterface &interface=*createSerialInterface((char *) "/dev/ttyUSB0");
    DynamixelMotor motor(interface, 0); //TODO check ID for AX12

    interface.begin(1000000);

    motor.init();
    motor.led(HIGH);
    delay(1000);
    motor.led(LOW);
    delay(1000);

    //switch motor to 9600Bd
    motor.communicationSpeed(9600);

    //switch interface speed to 9600Bd
    delay(100);
    interface.begin(9600);

    motor.led(HIGH);
    delay(1000);
    motor.led(LOW);
    delay(1000);

    //switch back to 1MBd
    motor.communicationSpeed(1000000);

    delay(100);
    interface.begin(1000000);

    motor.led(HIGH);
    delay(1000);
    motor.led(LOW);
    delay(1000);
}