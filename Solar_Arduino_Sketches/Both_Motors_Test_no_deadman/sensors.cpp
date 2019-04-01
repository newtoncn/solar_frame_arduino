/*
 * sensors.cpp - Implements sensors.h, library for 
 * altitudinal and azimuthal actuators' code.
 */

#include "sensors.h"

/*
 * requires: nothing
 * effects:  float send_value, value of current sensor reading
 */
double Current_Sensor::getCurrent() 
{
  float send_value = analogReading_*convertVoltsToAmps; //Current sense output: I = Vc/0.075, where Vc ranges 0 to 2.99V.
  return send_value;
}

/*
 * requires: nothing
 * effects:  analogReading_, updates current sensor reading
 */
void Current_Sensor::readCurrent()
{
  analogReading_ = analogRead(pinNum_);   // analogRead reads int value of 0-1023, represent 0 to 5V.
}
