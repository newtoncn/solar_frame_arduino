#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

/*
 * Interface that all of our sensors should adhere to.
 * When we call run() sensor class should gather data.
 */
class Sensor{
  void run();
};

/*
 * Container to hold all of our sensors.
 * When we call run() this container will run through each sensor and call the run function on them
 */
class Sensor_Container{
  Sensor_Container();

  void addSensor();

  void run();
};

class CURRENT_SENSOR : Sensor{
  CURRENT_SENSOR(int pinNum);

  void run();

  double getCurrent();
};

class IMU_SENSOR : Sensor{
  IMU_SENSOR();

  void run();

  double getPitch();

  double getHeading();
};

class ROTARY_ENCODER : Sensor{
  ROTARY_ENCODER();

  void run();

  void startTracking();

  double getCurrentAngle();
};

#endif
