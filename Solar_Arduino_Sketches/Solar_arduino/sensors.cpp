/*
 * sensors.cpp - Implements sensors.h, library for 
 * altitudinal and azimuthal actuators' code.
 */

#include "sensors.h"

//  Rotary_Encoder class

/*
 * requires: num, sensor reading in Gray code
 * effects:  converts and returns rotary encoder sensor reading from Gray to Binary32
 */
unsigned int Rotary_Encoder::GrayToBinary32(unsigned int num)
{
  num = num ^ (num >> 16);
  num = num ^ (num >> 8);
  num = num ^ (num >> 4);
  num = num ^ (num >> 2);
  num = num ^ (num >> 1);
  return num;
}

/*
 * requires: nothing.
 * effects:  num_, updates rotary encoder sensor reading
 */
void Rotary_Encoder::run()
{
  val0_ = digitalRead(pin0_);
  val1_ = digitalRead(pin1_);
  val2_ = digitalRead(pin2_);
  val3_ = digitalRead(pin3_);

  num_ = val0_ + val1_ * 2 + val2_ * 4 + val3_ * 8;

  if (prev_gray_ == -1) {
  prev_gray_ = GrayToBinary32(num_);
  } else {
    current_gray_ = GrayToBinary32(num_);
  }

  if (prev_gray_ > current_gray_) {
    counter_++;
  } else if (prev_gray_ < current_gray_) {
    counter_--;
  }

  prev_gray_ = current_gray_;
}

/*
 * Resets rotary encoder counter to 0.
 * 
 * requires: nothing
 * effects:  counter_
 */
void Rotary_Encoder::startTracking()
{
  counter_ = 0;
}

/*
 * Converts rotary encoder counter to angle in degrees.
 * 
 * requires: nothing
 * effects:  counter_
 */
double Rotary_Encoder::getCurrentAngle()
{
  return counter_ * (360.0 / 16.0) * (1.0 / GEAR_RATIO);
}

// Current_Sensor class

/*
 * requires: nothing
 * effects:  analogReading_, converts and returns current sensor reading
 */
double Current_Sensor::getCurrent() 
{
  return analogReading_*convertanalogReadToVolts*convertVoltsToAmps; // //Current sense output: I = Vc/0.075, where Vc ranges 0 to 2.99V.
}

/*
 * requires: nothing
 * effects:  analogReading_, updates current sensor reading
 */
void Current_Sensor::run()
{
  analogReading_ = analogRead(pinNum_);   // analogRead reads int value of 0-1023, represent 0 to 5V.
}

// IMU_Sensor class

/*
 * requires: nothing
 * effects:  a_accel_x_,  a_accel_y_, a_accel_z_, m_guass_x_, m_guass_y_  
 * updates IMU sensor readings
 */
void IMU_Sensor::run()
{
  //lsm.read();  /* ask it to read in the data */

  /* Get a new sensor event */
  sensors_event_t a, m, g, temp;

  //lsm.getEvent(&a, &m, &g, &temp);

  a_accel_x_ = a.acceleration.x;
  a_accel_y_ = a.acceleration.y;
  a_accel_z_ = a.acceleration.z;

  m_guass_x_ = m.magnetic.x; 
  m_guass_y_ = m.magnetic.y;
}

/*
 * requires: nothing
 * effects:  nothing, converts and returns IMU sensor pitch readings
 */
double IMU_Sensor::getPitch()
{
  return 2.0; //atan2(-a_accel_x_, sqrt(a_accel_y_*a_accel_y_ + a_accel_z_*(a_accel_z_))) * 180/M_PI;
}

/*
 * requires: nothing
 * effects:  nothing, converts and returns IMU sensor heading readings
 */
double IMU_Sensor::getHeading()
{
  return 2.0; //atan2(m_guass_y_, m_guass_x_) * 180/M_PI;
}

// Sensor_Container class

/*
 * Construct for Sensor_Container
 */
Sensor_Container::Sensor_Container() 
{
  sensor_num_ = 1;
}

/*
 * requires: Sensor object
 * effects:  sensors[], adds Sensor object to array
 */
void Sensor_Container::addSensor(Sensor& sensor) 
{
  sensors_[sensor_num_ - 1] = &sensor;
  sensor_num_++;
}

/*
 * requires: nothing
 * effects:  reading variables all Sensor objects in array, updates sensor readings
 */
void Sensor_Container::run() 
{
  for (int a = 0; a < sensor_num_; a++){
      if(sensors_[a] != NULL){
          sensors_[a]->run();
      }
  }
}
