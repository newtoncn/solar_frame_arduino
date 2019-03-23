/*
 * Test code for making altitudinal motor extend, retract, and stop, using Azande.
 */

#include <Azande.h>
#include "motors.h"

// The Azande object variable. 'Serial' is default used 'Stream'.
Azande azande;

// Azande Feature Macros for Motor test
define_enum_command(   cmdSetAltitudinalAngle,      "Send Alt Motor Cmd", GiveAltCmd,  0,  define_enum_item(0, "Extend")\
                                                                                           define_enum_item(1, "Retract")\
                                                                                           define_enum_item(2, "Stop"))
// Pin Declarations, as per pinout sheet
int Megamoto_EnablePin = 8;
int PWMPinA = 9;
int PWMPinB = 10;
int relayPin = 2;

// Motor objects and related variables/parameters
float altitudinal_speed = 255.0; // Values from 0-255, range of motor speed from 0-100% 
int altitudinal_relay_setting = HIGH; 
Actuator altidutinal_actuator = Actuator(Megamoto_EnablePin, PWMPinA, PWMPinB, altitudinal_speed, relayPin, altitudinal_relay_setting);

void setup() {
    
  Serial.begin(9600);
    
  // Set pin modes
  pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
  pinMode(PWMPinA, OUTPUT);               // Set motor outputs
  pinMode(PWMPinB, OUTPUT);               

  while (!Serial) {
      delay(1); // Will pause Arduino until serial console opens.
  }

  azande.begin();
  
  // Add features to be visible on Azande Studio
  azande.add(cmdSetAltitudinalAngle);
}


void loop() {  
  delay(20); 
  azande.readStream(); // Let Azande handle the new data
}


