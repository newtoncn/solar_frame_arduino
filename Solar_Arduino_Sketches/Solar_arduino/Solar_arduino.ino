#include <Azande.h>
#include "sensors.h"
#include "motors.h"

// Pin Declarations
int relay_enable = 2;
int rotary_pin0 = 3;
int rotary_pin1 = 4;
int rotary_pin2 = 5;
int rotary_pin3 = 6;
int megamoto_enable = 8;
int motor_controller_A = 9;
int motor_controller_B = 10;

// Sensors
Rotary_Encoder rotary_encoder = Rotary_Encoder(rotary_pin0,rotary_pin1,rotary_pin2,rotary_pin3);

Sensor_Container sensor_container = Sensor_Container();

// Motors

//Azande
Azande azande(Serial);    // The Azande object variable. Using 'Serial' as 'Stream'

define_int_event(rotaryPosition, "Azimuth Position", 0 , "degrees" , , , );

void setup() {
  //Set pin modes
  pinMode(rotary_pin0, INPUT);
  pinMode(rotary_pin1, INPUT);
  pinMode(rotary_pin2, INPUT);
  pinMode(rotary_pin3, INPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }

  Serial.println("Serial Connected");

  azande.begin();

  azande.add(rotaryPosition);

  sensor_container.addSensor(rotary_encoder);
}

void loop() {
  sensor_container.run();

  azande.send(rotaryPosition,rotary_encoder.getCurrentAngle());
  
  delay(200);
}
