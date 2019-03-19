/*
  motors.h - Library for linear actuator code.
  Note: If need separate functions later for altitudinal and azimuthal, uncomment AZIMUTH_MOTOR and fill in, and rename ACUATOR
*/
#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>


class ACTUATOR {
  public:
    ACTUATOR(int Megamoto_EnablePin, int PWMPinA, int PWMPinB, float speedSetting);
    ~ACTUATOR();
    void forwardActuator();   // Moves actuator forward.
    void reverseActuator();   // Moves actuator backward.
    void stopActuator();      // Stops actuator.
  private:
    int pin_enable;
    int pin_PWM_A;
    int pin_PWM_B;  
    float speed_actuate; // Operation speed
    float speed_stop;    // Stop speed.
    int rampUpTime;
};

/* 
class AZIMUTH_MOTOR {

}; */

#endif
