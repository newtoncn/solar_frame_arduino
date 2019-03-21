/*
 * All functions sending data to or from Azande interface.
 */

/*
 * This function is called when receiving the command "Set Azimuthal Angle", 
 * (cmdSetAzimuthAngle), from Azande Studio, along with requested angle.
 * 
 * requires: double, destination angle
 * effects: makes azimuthal actuator move until reached desired angle
 */
void SetAzAngle(double azAngleRequest){
  
  sensor_container.run();
  double a = rotary_encoder.getCurrentAngle();
  double b = azAngleRequest;

  /*  
   *  With current bearing 'a' and target bearing 'b'
   *  If a<b, and b−a≤180, turn counterclockwise.
   *  If a>b, and a−b>180, turn counterclockwise.
   *  If a<b, and b−a>180, turn clockwise.
   *  If a>b, and a−b≤180, turn clockwise.
   */
  if( ( (a < b) && ((b - a) <=  180) ) | (a > b && ( (a -  b) >  180) ) )
  {
     azimuthal_actuator.forwardActuator();  // turn CW...
  } else {
     azimuthal_actuator.reverseActuator();  // turn CCW...
  }

  // ...until at target bearing
  while( abs(rotary_encoder.getCurrentAngle() - azAngleRequest) > 1 ){
    sensor_container.run();
    azande.send(eventShowRotaryPosition, rotary_encoder.getCurrentAngle());
    delay(20);
  }
  
  azimuthal_actuator.stopActuator();

}

 /* 
 * This function is called when receiving the command "Set Altitudinal Angle", 
 * (cmdSetAltituidnalAngle), from Azande Studio, along with requested angle.
 * Assumes 0 degrees is tubes are flat front plane: ___ 
 * and 90 degrees is all tubes vertical:  |
 * 
 * requires: double, destination angle
 * effects: makes altitudinal actuator move until reached desired angle
 */
void SetAltAngle(double altAngleRequest){
  
  sensor_container.run();
  
  if( altAngleRequest < imu_sensor.getPitch() )
  {
     azimuthal_actuator.forwardActuator();  // Linear actuator extends until at target bearing.
  } else {
     azimuthal_actuator.reverseActuator();  // Linear actuator retracts until at target bearing.
  }

  while( abs(imu_sensor.getPitch() - altAngleRequest) > 1 ){ 
    sensor_container.run();
    azande.send(eventShowPitchPosition, imu_sensor.getPitch() );  // Keep sending pitch sensor readout until destination reached.
    delay(20);
  }
  
  azimuthal_actuator.stopActuator();
  
}

/* 
 * Sends updated readouts (cmdGiveReading) for all sensors to Azande Studio.
 * 
 * requires: nothing
 * effects: updates sensor readouts in Azande 
 */
void SendSensorReadout()
{ 
  sensor_container.run();

    azande.send(eventShowRotaryPosition, rotary_encoder.getCurrentAngle()); 
    azande.send(eventShowPitchPosition, imu_sensor.getPitch() ); 
    azande.send(eventShowHeadingPosition, imu_sensor.getHeading());
    azande.send(eventShowCurrentAReading, current_sensor_A.getCurrent() );
    azande.send(eventShowCurrentBReading, current_sensor_A.getCurrent() );

}
