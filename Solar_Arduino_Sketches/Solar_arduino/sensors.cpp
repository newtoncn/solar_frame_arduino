#include "sensors.h"

//  Class Rotary_Encoder 
unsigned int Rotary_Encoder::GrayToBinary32(unsigned int num)
{
  num = num ^ (num >> 16);
  num = num ^ (num >> 8);
  num = num ^ (num >> 4);
  num = num ^ (num >> 2);
  num = num ^ (num >> 1);
  return num;
}

void Rotary_Encoder::run()
{
  val0_ = digitalRead(pin0_);
  val1_ = digitalRead(pin1_);
  val2_ = digitalRead(pin2_);
  val3_ = digitalRead(pin3_);

  num_ = val0_ + val1_ * 2 + val2_ * 4 + val3_ * 8;

  if (prev_gray_ == -1) prev_gray_ = GrayToBinary32(num_);
  else {
    current_gray_ = GrayToBinary32(num_);
    if (prev_gray_ > current_gray_) counter_++;
    else if (prev_gray_ < current_gray_) counter_--;
  }

  prev_gray_ = current_gray_;
}

void Rotary_Encoder::startTracking()
{
  counter_ = 0;
}

double Rotary_Encoder::getCurrentAngle()
{
  return counter_; //counter_ * (360 / 16) * (1.0 / GEAR_RATIO);
}


// Class Current_Sensor
double Current_Sensor::getCurrent() 
{
  int integerZeroTo1023 = analogRead(pinNum_);   // analogRead reads int value of 0-1023, represent 0 to 5V.
  double currentValue = integerZeroTo1023*convertIntToVolts*convertVoltsToAmps; // //Current sense output: I = Vc/0.075, where Vc ranges 0 to 2.99V.
  return currentValue;     
}

void Current_Sensor::run()
{
  
}


//  Class Sensor_Container
Sensor_Container::Sensor_Container() {
  sensor_num_ = 0;
}

void Sensor_Container::addSensor(Sensor& sensor) {
  sensors_[sensor_num_] = &sensor;
  sensor_num_++;
}

void Sensor_Container::run() {
  for (int a = 0; a < (int)sizeof(sensors_)/sizeof(Sensor*); a++) sensors_[a]->run();
}
