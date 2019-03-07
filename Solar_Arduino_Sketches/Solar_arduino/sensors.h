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

  virtual void run();

  void startTracking();

  double getCurrentAngle();

  private:
  int pin0_,pin1_,pin2_,pin3_,prev_gray_,current_gray_,counter_,num_,val0_,val1_,val2_,val3_;

  unsigned int GrayToBinary32(unsigned int num);
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
  Sensor_Container();

  void addSensor(Sensor& sensor);

  void run();

  private:
  int sensor_num_;
  Sensor* sensors_[NUM_SENSORS];
};

#endif
