/*
  motors.h - Library for linear actuator code.
*/
#include "motors.h"

#include <Arduino.h>

// Class LINEAR_ACTUATOR. Has 3 functions: move fwd, backwd, and stop.
// Constructor
ACTUATOR::ACTUATOR(int Megamoto_EnablePin, int PWMPinA, int PWMPinB, float speedSetting)
{
  pin_enable = Megamoto_EnablePin;
  pin_PWM_A = PWMPinA;
  pin_PWM_B = PWMPinB;
  speed_actuate = speedSetting;
  speed_stop = 0;
  rampUpTime = 125; // Motor goes from 0 to full speed in 4 * rampUpTime milliseconds
}

// Destructor
ACTUATOR::~ACTUATOR()
{
}

void ACTUATOR::stopActuator() {
  analogWrite(pin_PWM_A, speed_stop);
  analogWrite(pin_PWM_B, speed_stop);
  digitalWrite(pin_enable, LOW);
}

void ACTUATOR::forwardActuator() {
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_A, speed_stop);
  // Ramp up time to get to full speed
  for (float i = 0.2; i <= 1; i += 0.2) {
    analogWrite(pin_PWM_B, speed_actuate*i);
    delay(125);
  }
}

void ACTUATOR::reverseActuator() {
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_B, speed_stop);
    // Ramp up time to get to full speed
  for (float i = 0.2; i <= 1; i += 0.2) {
    analogWrite(pin_PWM_A, speed_actuate*i);
    delay(125);
  }
}
