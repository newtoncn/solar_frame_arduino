#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

#define NUM_SENSORS 2
#define GEAR_RATIO 60

/*
 * Interface that all of our sensors should adhere to.
 * When we call run() sensor class should gather data.
 */
class Sensor{
  public:

  Sensor(){}
  
  virtual void run()=0;
};

class Current_Sensor : Sensor{
  Current_Sensor(int pinNum) : Sensor(){
    
  }

  virtual void run();

  double getCurrent();
};

class IMU_Sensor : Sensor{
  public:
  IMU_Sensor() : Sensor(){}

  virtual void run();

  double getPitch();

  double getHeading();
};

class Rotary_Encoder : public Sensor {
  public:
  Rotary_Encoder(int pin0, int pin1, int pin2, int pin3) : Sensor(){
      pin0_ = pin0;
      pin1_ = pin1;
      pin2_ = pin2;
      pin3_ = pin3;
      prev_gray_ = -1;
      current_gray_ = 0;
  }

  virtual void run(){
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

  void startTracking(){
      counter_ = 0;
  }

  double getCurrentAngle(){
      return counter_; //counter_ * (360 / 16) * (1.0 / GEAR_RATIO);
  }

  private:
  int pin0_,pin1_,pin2_,pin3_,prev_gray_,current_gray_,counter_,num_,val0_,val1_,val2_,val3_;

  unsigned int GrayToBinary32(unsigned int num){
      num = num ^ (num >> 16);
      num = num ^ (num >> 8);
      num = num ^ (num >> 4);
      num = num ^ (num >> 2);
      num = num ^ (num >> 1);
      return num;
      }
};

/*
 * Container to hold all of our sensors.
 * When we call run() this container will run through each sensor and call the run function on them
 */
class Sensor_Container{
  public:
  /*
   * Initialize with number of sensors that will be added
   */
  Sensor_Container(){
      sensor_num_ = 0;
  }

  void addSensor(Sensor& sensor){
      sensors_[sensor_num_] = &sensor;
      sensor_num_++;
  }

  void run(){
      for (int a = 0; a < (int)sizeof(sensors_)/sizeof(Sensor*); a++) sensors_[a]->run();
  }

  private:
  int sensor_num_;
  Sensor* sensors_[NUM_SENSORS];
};

#endif
