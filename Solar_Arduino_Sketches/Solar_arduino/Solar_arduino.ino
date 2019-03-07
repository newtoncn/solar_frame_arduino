#include <Azande.h>
#include "sensors.h"
#include "motors.h"

// Pin Declarations
int re_pin0 = 2;
int re_pin1 = 3;
int re_pin2 = 4;
int re_pin3 = 5;


// Sensors
Rotary_Encoder rotary_encoder = Rotary_Encoder(re_pin0,re_pin1,re_pin2,re_pin3);

Sensor_Container sensor_container = Sensor_Container();

// Motors

Azande azande(Serial);    // The Azande object variable. Using 'Serial' as 'Stream'

void setup() {
  //Set pin modes
  pinMode(re_pin0, INPUT);
  pinMode(re_pin1, INPUT);
  pinMode(re_pin2, INPUT);
  pinMode(re_pin3, INPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);

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
