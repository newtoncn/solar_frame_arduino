#include <Azande.h>
#include "motors.h"
#include "sensors.h"

// Pin Declarations
int re_pin0 = 2;
int re_pin1 = 3;
int re_pin2 = 4;
int re_pin3 = 5;

int Megamoto_EnablePin = 8;       // This was Pin 8 during testing (delete this note after successful integration)
int PWMPinA = 9;                  // This was Pin 11 during testing (delete this note after successful integration)
int PWMPinB = 10;                 // This was Pin 3 during testing (delete this note after successful integration)

// Sensors
Rotary_Encoder rotary_encoder = Rotary_Encoder(re_pin0,re_pin1,re_pin2,re_pin3);

Sensor_Container sensor_container = Sensor_Container();

// Motors
LINEAR_ACTUATOR altidutinal_actuator = LINEAR_ACTUATOR(Megamoto_EnablePin, PWMPinA, PWMPinB);

Azande azande(Serial);    // The Azande object variable. Using 'Serial' as 'Stream'

void setup() {
  Serial.begin(9600);
  // Set pin modes
  pinMode(re_pin0, INPUT);
  pinMode(re_pin1, INPUT);
  pinMode(re_pin2, INPUT);
  pinMode(re_pin3, INPUT);
  pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
  pinMode(PWMPinA, OUTPUT);
  pinMode(PWMPinB, OUTPUT);               // Set motor outputs
  
  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }

  Serial.println("Serial Connected");

  sensor_container.addSensor(rotary_encoder);
}

void loop() {
  sensor_container.run();
  Serial.print("Counter "); Serial.println(rotary_encoder.getCurrentAngle());
  
  delay(200);

}
