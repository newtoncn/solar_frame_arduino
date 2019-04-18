/*
  sensors.h - Library for sensors code.
*/


#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Azande.h>"

/*
 * Superclass that all of our sensors should adhere to.
 * When we call run() sensor class should gather data.
 */
class Sensor{
  public:
  Sensor(){}
  
//  virtual void run()=0; // "run()" is premade function, so cannot be implemented during class implementation. "virtual void" allows this lack of implementation.
};


/*
 * Current Sensor Class. Gets and returns converted current sensor readings.
 */
class Current_Sensor : public Sensor {
  public: 
    Current_Sensor(int pinNum) : Sensor(){
      pinNum_ = pinNum;
      }
    double getCurrent();
    void   readCurrent();
    
  private:
    int pinNum_;
    float convertVoltsToAmps = 0.075;     // Current sense output: I*0.075 = Vc, where Vc ranges 0 to 2.99V.
    float analogReading_;
};

#endif
