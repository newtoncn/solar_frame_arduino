/*
 * motors.cpp - Implements motors.h, library for 
 * altitudinal and azimuthal actuators' code.
 */
 
#include "motors.h"
#include <Arduino.h>

// Constructor
Actuator::Actuator(int Megamoto_EnablePin, int PWMPinA, int PWMPinB, int relayPin, int relaySetting)
{
  pin_enable = Megamoto_EnablePin;
  pin_PWM_A = PWMPinA;
  pin_PWM_B = PWMPinB;
  relayPin_ = relayPin;
  relaySetting_ = relaySetting;
  speed_actuate = 255.0;          // Values from 0-255, range of motor speed from 0-100% 
  speed_stop = 0;
  rampUpTime = 125;               // Motor goes from 0 to full speed in 4 * rampUpTime [milliseconds]
}

// Destructor
Actuator::~Actuator()
{
}

/*
 * Stop actuator from actuating.
 * 
 * requires: nothing.
 * effects:  sets speed of actuator to 0. 
 */
void Actuator::stopActuator() {
  digitalWrite(relayPin_, relaySetting_);
  analogWrite(pin_PWM_A, speed_stop);
  analogWrite(pin_PWM_B, speed_stop);
  digitalWrite(pin_enable, LOW);
}

/*
 * Makes actuator actuate forward.
 * 
 * requires: nothing.
 * effects:  sets speed of actuator to speed selected by 
 * initialized motor speed variable in Solar_arduino tab.
 */
void Actuator::reverseActuator() {
  digitalWrite(relayPin_, relaySetting_);
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_A, speed_stop);
  // Ramp up time to get to full speed
  for (float i = 0.2; i <= 1; i += 0.2) {
    analogWrite(pin_PWM_B, speed_actuate*i);
    delay(125);
  }
}

/*
 * Makes actuator actuate backward.
 * 
 * requires: nothing.
 * effects:  sets speed of actuator to speed selected by 
 * initialized motor speed variable in Solar_arduino tab.
 */
void Actuator::forwardActuator() {
  digitalWrite(relayPin_, relaySetting_);
  digitalWrite(pin_enable, HIGH);
  analogWrite(pin_PWM_B, speed_stop);
    // Ramp up time to get to full speed
  for (float i = 0.2; i <= 1; i += 0.2) {
    analogWrite(pin_PWM_A, speed_actuate*i);
    delay(125);
  }
}

/*
 * Sets actuator speed.
 * 
 * requires: int speedOrder which is desired speed
 * effects:  int speed_actuate which is variable containing speed of actuator
 */
void Actuator::speedSet(int speedOrder){
  speed_actuate = speedOrder;
}

/*
 * Returns actuator speed.
 * 
 * requires: nothing
 * returns:  float copy_of_speed_actuate which is value of speed of actuator
 */
float Actuator::getSpeed(){
  float copy_of_speed_actuate = speed_actuate;
  return copy_of_speed_actuate;
}

/*
 * Returns actuator speed as percentage.
 * 
 * requires: nothing
 * returns:  float speed as percentage
 */
float Actuator::getSpeedAsPercentage(){
  float conversion_factor_255_to_percent = 100.0/255.0;
  float speed_actuate_as_percentage = speed_actuate*conversion_factor_255_to_percent;
  return speed_actuate_as_percentage;
}

/*
 * Returns actuator relaySetting_ as identifier.
 * 
 * requires: nothing
 * returns:  int relaySetting_ which is value of relay setting of actuator.
 */
int Actuator::getRelaySetting(){
  int copy_of_relaySetting_ = relaySetting_;
  return copy_of_relaySetting_;
}

