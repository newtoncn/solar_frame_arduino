/*
  motors.h - Library for linear actuator code.
*/
#include "motors.h"

#include <Arduino.h>

// Class LINEAR_ACTUATOR. Has 3 functions: move fwd, backwd, and stop.
// Constructor
LINEAR_ACTUATOR::LINEAR_ACTUATOR(int Megamoto_EnablePin, int PWMPinA, int PWMPinB)
{
  pin_enable = Megamoto_EnablePin;
  pin_PWM_A = PWMPinA;
  pin_PWM_B = PWMPinB;
  speed_full = 255;
  speed_stop = 0;
}

// Destructor
LINEAR_ACTUATOR::~LINEAR_ACTUATOR()
{
}

void LINEAR_ACTUATOR::stopActuator() {
  analogWrite(pin_PWM_A, speed_stop);
  analogWrite(pin_PWM_B, speed_stop);
  digitalWrite(pin_enable, LOW); 
}

void LINEAR_ACTUATOR::forwardActuator() {
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_A, speed_stop);
  analogWrite(pin_PWM_B, speed_full);
}

void LINEAR_ACTUATOR::reverseActuator() {
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_A, speed_full);
  analogWrite(pin_PWM_B, speed_stop);
}
