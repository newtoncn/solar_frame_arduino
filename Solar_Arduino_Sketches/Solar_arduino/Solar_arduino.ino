#include <Azande.h>
#include "sensors.h"
#include "motors.h"

// Azande Feature Macros for Sensor Readings
define_double_event(   eventShowRotaryPosition,  "Azimuth Position",                    0, "degrees", , , );
define_double_event(   eventShowPitchPosition,   "Pitch Position",                      0, "degrees", , , );
define_double_event(   eventShowHeadingPosition, "Heading Position",                    0, "degrees", , , );
define_double_event(   eventShowCurrentAReading, "Current Position A",                  0, "amps"   , , , );
define_double_event(   eventShowCurrentBReading, "Current Position B",                  0, "amps"   , , , );
define_enum_command(   cmdGiveReading,           "Give sensor read for:", GiveReading,  0,  define_enum_item(0, "Azimuthal")\
                                                                                            define_enum_item(1, "Pitch")\
                                                                                            define_enum_item(2, "Heading")\
                                                                                            define_enum_item(3, "Current A")\
                                                                                            define_enum_item(4, "Current B")\
                                                                                            define_enum_item(5, "All sensors"))
define_double_command( cmdSetAzimuthAngle,       "Set Azimuthal Angle",     SetAzAngle,  1, "degrees", 0.0, 90.0);
define_double_command( cmdSetAltitudinalAngle,   "Set Altitudinalal Angle", SetAltAngle, 1, "degrees", 0.0, 90.0);

// The Azande object variable. 'Serial' is default used 'Stream'.
Azande azande;

// Pin Declarations
int re_pin0 = 3;
int re_pin1 = 4;
int re_pin2 = 5;
int re_pin3 = 6;

int Megamoto_EnablePin = 8;
int PWMPinA = 9;
int PWMPinB = 10;

int currentSensorAPin = A5;
int currentSensorBPin = A0;

// Sensors
Sensor_Container sensor_container = Sensor_Container();

Rotary_Encoder rotary_encoder = Rotary_Encoder(re_pin0,re_pin1,re_pin2,re_pin3);
//IMU_Sensor imu_sensor = IMU_Sensor();
Current_Sensor current_sensor_A = Current_Sensor(currentSensorAPin);
Current_Sensor current_sensor_B = Current_Sensor(currentSensorBPin);

// Motors
// Values from 0-255, range of motor speed from 0-100%
float azimuthal_speed = 255; 
float altitudinal_speed = 255; 
Actuator altidutinal_actuator = Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, altitudinal_speed);
Actuator azimuthal_actuator = Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, azimuthal_speed);


void setup() {
    Serial.begin(9600); //May need to adjust Serial values for Azande Interface and IMU sensor
    // Set pin modes
    pinMode(re_pin0, INPUT);
    pinMode(re_pin1, INPUT);
    pinMode(re_pin2, INPUT);
    pinMode(re_pin3, INPUT);
    pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
    pinMode(PWMPinA, OUTPUT);               // Set motor outputs
    pinMode(PWMPinB, OUTPUT);               

    while (!Serial) {
        delay(1); // will pause Zero, Leonardo, etc until serial console opens
    }

    Serial.println("Serial Connected");

  azande.begin();
  // Add features to be visible on Azande Studio.
  azande.add(cmdGiveReading);
  azande.add(cmdSetAzimuthAngle);
  azande.add(cmdSetAltitudinalAngle);
  azande.add(eventShowRotaryPosition);
  azande.add(eventShowPitchPosition);
  azande.add(eventShowHeadingPosition);
  azande.add(eventShowCurrentAReading);
  azande.add(eventShowCurrentBReading);

  // Add sensors to sensor_container so can get all readings simultaneously 
  sensor_container.addSensor(rotary_encoder);
//  sensor_container.addSensor(imu_sensor);
}

void loop() {
  delay(50);
  azande.readStream(); // Let Azande handle the new data.
}

// This function is called when receiving the command "Set Azimuthal Angle", (cmdSetAzimuthAngle), from Azande Studio
void SetAzAngle(double azAngleRequest){
  sensor_container.run();
  
  /*  
   *  With current bearing 'a' and target bearing 'b'
   *  If a<b, and b−a≤180, turn counterclockwise.
   *  If a>b, and a−b>180, turn counterclockwise.
   *  If a<b, and b−a>180, turn clockwise.
   *  If a>b, and a−b≤180, turn clockwise.
   */
   
  double a = rotary_encoder.getCurrentAngle();
  double b = azAngleRequest;

  if( ( (a < b) && ((b - a) <=  180) ) | (a > b && ( (a -  b) >  180) ) )
  {
     azimuthal_actuator.forwardActuator();  // turn CW...
  } else {
     azimuthal_actuator.reverseActuator();  // turn CCW...
  }

  // ...until at target bearing
  while( abs(rotary_encoder.getCurrentAngle() - azAngleRequest) > 1 ){
    sensor_container.run();
    GiveAzPosition();
    delay(100);
  }
  
  azimuthal_actuator.stopActuator();

}

/* This function is called when receiving the command "Set Altitudinal Angle", (cmdSetAltituidnalAngle), from Azande Studio
 * Assumes 0 degrees is tubes are flat front plane: ___ , 90 degrees is all tubes vertical:  |
 */
void SetAltAngle(double altAngleRequest){
  sensor_container.run();
  
  if( altAngleRequest < 59 ) // 59 is placeholder for imu_sensor.getPitch()
  {
     azimuthal_actuator.forwardActuator();  // Linear actuator extends until at target bearing
  } else {
     azimuthal_actuator.reverseActuator();  // Linear actuator retracts until at target bearing
  }

  while( abs(0.59 - altAngleRequest) > 1 ){  // 0.59 is placeholder for imu_sensor.getPitch()
    sensor_container.run();
    GivePitchPosition();
    delay(100);
  }
  
  azimuthal_actuator.stopActuator();
  
}

// This function is called when receiving the command "Give sensor read for:", (cmdGiveReading), from Azande Studio
void GiveReading(long enumValue)
{ 
  sensor_container.run();
  switch (enumValue)
  {
    case 0: // readAz
      GiveAzPosition();
      break;

    case 1: // readPitch
      GivePitchPosition();
      break;

    case 2: // readHeading
      GiveHeadingPosition();
      break;

    case 3 : // readCurrentA
      GiveCurrentAReading();
      break;

    case 4 : // readCurrentB
      GiveCurrentBReading();
      break;

    case 5 : // readAll
      GiveAllReadings();
      break;

    default:
      break;
  }
}

// Any of following functions may be run each time GiveReading function is run
void GiveAzPosition()
{
    azande.send(eventShowRotaryPosition, rotary_encoder.getCurrentAngle()); 
}

void GivePitchPosition()
{
    azande.send(eventShowPitchPosition, 59.0);  //59 in place for imu_sensor.getPitch() until imu working
}

void GiveHeadingPosition()
{
    azande.send(eventShowHeadingPosition, 59.0);  //59 in place for imu_sensor.getHeading() until imu working
}

void GiveCurrentAReading()
{
    azande.send(eventShowCurrentAReading, current_sensor_A.getCurrent() );
}

void GiveCurrentBReading()
{
    azande.send(eventShowCurrentBReading, current_sensor_A.getCurrent() );
}

void GiveAllReadings()
{
    for (int i = 0; i <= 4; i++) {
    GiveReading(i);
  }
}
