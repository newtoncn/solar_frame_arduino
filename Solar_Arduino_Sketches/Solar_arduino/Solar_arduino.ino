/*
 * Arduino Uno microcontroller code, interfacing with Azande software, connected to a 
 * laptop via serial or Bluetooth. It runs 2 motors, altitudinal and azimuthal. It has
 * 2 sensors, a rotary encoder providing azimuthal position, and an inertial measurement 
 * unit providing pitch and heading positions. 
 */

#include <Azande.h>
#include "sensors.h"
#include "motors.h"

// The Azande object variable. 'Serial' is default used 'Stream'.
Azande azande;

// Azande Feature Macros for Sensor Readings
define_double_event(    eventShowRotaryPosition,      "Azimuth Position",                           0, "°", , , );
define_double_event(    eventShowPitchPosition,       "Pitch Position",                             0, "°", , , );
define_double_event(    eventShowHeadingPosition,     "Heading Position",                           0, "°", , , );
define_double_event(    eventShowCurrentA_Alt_Reading,  "Current Altitudinal (A)",                    0, "A"   , , , );
define_double_event(    eventShowCurrentB_Az_Reading,   "Current Azimuthal (B)",                      0, "A"   , , , );
define_double_command(  cmdSetAzimuthAngle,           "Set Azimuthal Angle",     SetAzAngle,  1, "0°-359°", 0.0, 359.0);
define_double_command(  cmdSetAltitudinalAngle,       "Set Altitudinalal Angle", SetAltAngle, 1, "15°-90°", 15.0, 90.0);
define_text_event(      eventErrorMsg,                "Error Message",                              2,  64);
define_text_event(      eventStatusMsg,               "Current Status",                              2,  64);

// Pin Declarations, as per pinout sheet
int re_pin0 = 3;  // Rotary encoder
int re_pin1 = 4;
int re_pin2 = 5;
int re_pin3 = 6;
int Megamoto_EnablePin = 8;
int PWMPinA = 9;
int PWMPinB = 10;
int relayPin = 2;
int currentSensorAPin = A5;
int currentSensorBPin = A0;
int errorLight_Pin = 1;

// Sensor objects
Sensor_Container sensor_container = Sensor_Container();

Rotary_Encoder rotary_encoder = Rotary_Encoder(re_pin0,re_pin1,re_pin2,re_pin3);
IMU_Sensor imu_sensor = IMU_Sensor();
Current_Sensor current_sensor_A = Current_Sensor(currentSensorAPin);
Current_Sensor current_sensor_B = Current_Sensor(currentSensorBPin);

// Motor objects and related variables/parameters
float azimuthal_speed = 155.0;  // Values from 0-255, range of motor speed from 0-100% 
float altitudinal_speed = 255.0;
int azimuthal_relay_setting = LOW; 
int altitudinal_relay_setting = HIGH; 
Actuator azimuthal_actuator =   Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, azimuthal_speed,   relayPin, azimuthal_relay_setting);
Actuator altidutinal_actuator = Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, altitudinal_speed, relayPin, altitudinal_relay_setting);


void setup() {
    
  Serial.begin(9600);
    
  // Set pin modes
  pinMode(re_pin0, INPUT);
  pinMode(re_pin1, INPUT);
  pinMode(re_pin2, INPUT);
  pinMode(re_pin3, INPUT);
  pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
  pinMode(PWMPinA, OUTPUT);               // Set motor outputs
  pinMode(PWMPinB, OUTPUT);               

  while (!Serial) {
      delay(1); // Will pause Arduino until serial console opens.
  }

  azande.begin();
  
  // Add features to be visible on Azande Studio
  azande.add(cmdSetAzimuthAngle);
  azande.add(cmdSetAltitudinalAngle);
  azande.add(eventShowRotaryPosition);
  azande.add(eventShowPitchPosition);
  azande.add(eventShowHeadingPosition);
  azande.add(eventShowCurrentA_Alt_Reading);
  azande.add(eventShowCurrentB_Az_Reading);
  azande.add(eventErrorMsg);
  azande.add(eventStatusMsg);

  //Setup IMU_sensor object(s) before running any reading functions
  imu_sensor.setupIMU();
 
  // Add sensors to sensor_container, which allows for simultaneous update of all variables with sensor readings
  sensor_container.addSensor(rotary_encoder);
  sensor_container.addSensor(imu_sensor);

}


void loop() {
  
  SendSensorReadout();
  
  // Initialize first messages to user
  azande.send(eventErrorMsg, "No errors");
  azande.send(eventStatusMsg, "Awaiting Command");
  delay(20);
  
  azande.readStream(); // Let Azande handle the new data
  
}


