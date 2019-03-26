/*
  motors.h - Library for sensors code.
*/

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

#ifndef SENSORS_H
#define SENSORS_H

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

#define GEAR_RATIO 60.0

/*
 * Superclass that all of our sensors should adhere to.
 * When we call run() sensor class should gather data.
 */
class Sensor{
  public:
  Sensor(){}
  
  virtual void run()=0; // "run()" is premade function, so cannot be implemented during class implementation. "virtual void" allows this lack of implementation.
};

/*
 * Current Sensor Class. Gets and returns converted current sensor readings.
 */
class Current_Sensor : public Sensor {
  public: 
    Current_Sensor(int pinNum) : Sensor(){
      pinNum_ = pinNum;
      }
    virtual void run();
    double getCurrent();
    
  private:
    int pinNum_;
    float convertVoltsToAmps = 0.075;     // Current sense output: I*0.075 = Vc, where Vc ranges 0 to 2.99V.
    float convertanalogReadToVolts = 5.0/1023.0; // analogRead reads 0 to 5V, as from 0 to 1023
    float analogReading_;
};

/*
 * IMU Sensor Class. Gets and sends pitch and heading readings.
 */
class IMU_Sensor : public Sensor{
  public:
  IMU_Sensor() : Sensor(){
    lsm = Adafruit_LSM9DS1();
   }

  virtual void run();
  double getAngle();
  double getHeading();
  void setupIMU();

  private:
  double a_accel_x_, a_accel_y_, a_accel_z_, m_gauss_x_, m_gauss_y_;
  Adafruit_LSM9DS1 lsm;
};

/*
 * Rotary Encoder Sensor class. Gets and sends azimuthal sensor readings.
 */
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
    double getAngle();
    
  private:
    int pin0_,pin1_,pin2_,pin3_,prev_gray_,current_gray_,counter_,num_,val0_,val1_,val2_,val3_;
    unsigned int GrayToBinary32(unsigned int num);
};

/*
 * Container to hold all of our sensors.
 * When we call run() this container will run through each sensor and call the run function on them.
 */
class Sensor_Container{
  public:
  /*
   * Initialize with number of sensors that will be added
   */
    Sensor_Container();
    void addSensor(Sensor& sensor);
    virtual void run();
    
  private:
    int sensor_num_;
    Sensor* sensors_[10];    // An array of pointers to sensor objects
};

#endif
