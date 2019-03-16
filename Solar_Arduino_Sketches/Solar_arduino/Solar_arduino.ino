#include <Azande.h>
#include "sensors.h"
#include "motors.h"

// Pin Declarations
int re_pin0 = 2;
int re_pin1 = 3;
int re_pin2 = 4;
int re_pin3 = 5;

// Motors
int Megamoto_EnablePin = 8;
int PWMPinA = 9;
int PWMPinB = 10;

int currentSensorAPin = A5;
int currentSensorBPin = A0;

// Sensors
Rotary_Encoder rotary_encoder = Rotary_Encoder(re_pin0,re_pin1,re_pin2,re_pin3);

Sensor_Container sensor_container = Sensor_Container();
//Current_Sensor current_sensor_A = Current_Sensor(currentSensorAPin);  //DOES NOT WORK
//Current_Sensor current_sensor_B = Current_Sensor(currentSensorBPin);  //DOES NOT WORK

// Motors
LINEAR_ACTUATOR altidutinal_actuator = LINEAR_ACTUATOR(Megamoto_EnablePin, PWMPinA, PWMPinB);

//Azande
Azande azande(Serial);    // The Azande object variable. Using 'Serial' as 'Stream'

define_int_event(rotaryPosition, "Azimuth Position", 0 , "degrees" , , , );

void setup() {
  // Set pin modes
  pinMode(re_pin0, INPUT);
  pinMode(re_pin1, INPUT);
  pinMode(re_pin2, INPUT);
  pinMode(re_pin3, INPUT);
  pinMode(Megamoto_EnablePin, OUTPUT);    // Enable Megamoto
  pinMode(PWMPinA, OUTPUT);
  pinMode(PWMPinB, OUTPUT);               // Set motor outputs

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
