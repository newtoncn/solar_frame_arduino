/*
  motors.h - Library for linear actuator code.
*/

#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <Azande.h>"

/*
 * Class has 3 functions: move forward, backward, and stop.
 * requires: Relevant 4 pin values, desired speed, and setting for relay pin.
 * All these arguments are inititalized in Solar_arduino tab.
 */
class Actuator {
  public:
    Actuator(int Megamoto_EnablePin, int PWMPinA, int PWMPinB, int relayPin, int pin_relaySetting);
    ~Actuator();
    void forwardActuator();       // Moves actuator forward.
    void reverseActuator();       // Moves actuator backward.
    void stopActuator();          // Stops actuator.
    void speedSet(int speedOrder);// Sets/changes actuator speed.
    float getSpeed();             // Returns speed
    float getSpeedAsPercentage(); // Returns speed as percentage
    int getRelaySetting();        // Get pin setting as identifier
  private:
    int pin_enable;
    int pin_PWM_A;
    int pin_PWM_B;
    int relayPin_;
    int relaySetting_;  
    float speed_actuate; // Operation speed
    float speed_stop;    // Stop speed.
    int rampUpTime;
};


#endif
