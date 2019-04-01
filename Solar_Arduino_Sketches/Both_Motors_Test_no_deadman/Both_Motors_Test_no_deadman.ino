/*
 * Test code for making altitudinal motor extend, retract, and stop, using Azande.
 */

#include <Azande.h>
#include "motors.h"
#include "sensors.h"

// The Azande object variable. 'Serial' is default used 'Stream'.
Azande azande;

// Azande Feature Macros for Motor test
define_enum_command(   cmdSetAltitudinalAngle,"Send Alt Motor Cmd", GiveAltCmd,  0, define_enum_item(0, "Extend or CW")\
                                                                                    define_enum_item(1, "Retract or CCW")\
                                                                                    define_enum_item(2, "Stop"))
define_double_command( cmdSetAltSpeed,        "Set Alt Speed:",      setAltSpeed, 0, "0-100 [%]", 0, 100);
define_double_event(   eventShowAltSpeed,     "Alt Speed:",                       0, "[%]", , , );
define_enum_command(   cmdSetAzimuthalAngle,  "Send Az Motor Cmd:",  GiveAzCmd,   1, define_enum_item(0, "Clockwise")\
                                                                                    define_enum_item(1, "Counterclockwise")\
                                                                                    define_enum_item(2, "Stop"))
define_double_command( cmdSetAzSpeed,         "Set Az Speed:",       setAzSpeed,  1, "0-100 [%]", 0, 100);
define_double_event(   eventShowAzSpeed,      "Az Speed:",                        1, "[%]", , , );
define_text_event(     eventSendStatusToGUI,  "Status:",                          3, 64);
define_text_event(     eventSendLastCmdToGUI, "Last Cmd Sent:",                   3, 64);
const int last_cmd_sent = 0;
const int current_status = 1;
define_double_event(   eventSendCurrent,      "Current:",                         2, "Range: 0-20", , , );

// Pin Declarations, as per pinout sheet
const int Megamoto_EnablePin = 8;
const int PWMPinA = 9;
const int PWMPinB = 10;
const int relayPin = 2;

// Motor objects and related variables/parameters
const int altitudinal_relay_setting = HIGH; 
const int azimuthal_relay_setting = LOW; 
Actuator altidutinal_actuator = Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, relayPin, altitudinal_relay_setting);
Actuator azimuthal_actuator =   Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, relayPin, azimuthal_relay_setting);

// Sensor objects and variables/parameters
const byte currentSensorPin = A0;
Current_Sensor current_sensor = Current_Sensor(currentSensorPin);

void setup() {
    
  Serial.begin(9600);
    
  // Set pin modes
  pinMode(relayPin, OUTPUT); 
  pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
  pinMode(PWMPinA, OUTPUT);               // Set motor outputs
  pinMode(PWMPinB, OUTPUT);               
  pinMode(A0, INPUT);              

  while (!Serial) {
      delay(1); // Will pause Arduino until serial console opens.
  }

  azande.begin();
  
  // Add features to be visible on Azande Studio
  azande.add(cmdSetAltitudinalAngle);
  azande.add(cmdSetAltSpeed);
  azande.add(eventShowAltSpeed);
  azande.add(cmdSetAzimuthalAngle);
  azande.add(cmdSetAzSpeed);
  azande.add(eventShowAzSpeed);
  azande.add(eventSendCurrent);
  azande.add(eventSendStatusToGUI);
  azande.add(eventSendLastCmdToGUI);
}


void loop() {
  SendDataToGUI();    
  delay(20); 
  azande.readStream(); // Let Azande handle the new data
}


