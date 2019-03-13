/*
  motors.h - Library for linear actuator code.
*/
#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>


class LINEAR_ACTUATOR {
  public:
    LINEAR_ACTUATOR(int Megamoto_EnablePin, int PWMPinA, int PWMPinB);
    ~LINEAR_ACTUATOR();
    void forwardActuator();   // Moves actuator forward.
    void reverseActuator();   // Moves actuator backward.
    void stopActuator();      // Stops actuator.
  private:
    int pin_enable;
    int pin_PWM_A;
    int pin_PWM_B;  
    int speed_full;   // Max speed.
    int speed_stop;   // Stop speed.
};

class AZIMUTH_MOTOR {

};

#endif
