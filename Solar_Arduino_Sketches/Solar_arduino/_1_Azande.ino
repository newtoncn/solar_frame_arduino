/*
 * All functions sending data to or from Azande interface.
 */

int SentForAzMotor = 1;
int SentForAltMotor = 0;

/* 
 * Sends updated readouts (cmdGiveReading) for all sensors to Azande Studio.
 * 
 * requires: nothing
 * effects: updates sensor readouts in Azande 
 */
void SendSensorReadout()
{ 
  sensor_container.run();

    azande.send(eventShowRotaryPosition, rotary_encoder.getAngle()); 
    azande.send(eventShowPitchPosition, imu_sensor.getAngle() ); 
    azande.send(eventShowHeadingPosition, imu_sensor.getHeading());
    azande.send(eventShowCurrentAAltReading, current_sensor_A_Alt.getCurrent() );
    azande.send(eventShowCurrentBAzReading, current_sensor_B_Az.getCurrent() );

}

/* 
 * requires: nothing
 * effects: Turns on error light and leaves program in long delay. 
 */
void errorOccurred(){
  digitalWrite(errorLight_Pin, HIGH);
  delay(99999999999); // infinite delay, until restart
}

/*
 * This function is called when receiving the command "Set Azimuthal Angle", 
 * (cmdSetAzimuthAngle), from Azande Studio, along with requested angle.
 * 
 * requires: double, destination angle
 * effects: makes azimuthal actuator move until reached desired angle
 */
void GetAndSetAzAngle(double azAngleRequest){
  SetAnyAngle(SentForAzMotor, azAngleRequest, azimuthal_actuator, rotary_encoder, current_sensor_B_Az, maxCurrentAz);
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
void GetAndSetAltAngle(double altAngleRequest){
  SetAnyAngle(SentForAltMotor, altAngleRequest, altitudinal_actuator, imu_sensor, current_sensor_A_Alt, maxCurrentAlt);
}

/*
 * This is the function called only by functions GetAndSetAltAngle and GetAndSetAltAngle. See specs of those functions.
 */
void SetAnyAngle(int motorSpecified, int targetAngle, Actuator actuator_, Sensor angle_sensor_, Current_Sensor currentSensor_, int maxCurrent){

  bool backoff = false;
  bool forwardActuationRequested = false;
 
  int thresholdCurrent;     // Current at which array should not be operating, and back off and shutdown are initiated.
  if(motorSpecified == SentForAzMotor){
    thresholdCurrent = maxCurrentAz; 
  } else if(motorSpecified == SentForAltMotor){
    thresholdCurrent = maxCurrentAlt;
  }
  int timeOverThresholdCurrent = 0; // Set to 0 initially.

  sensor_container.run(); // Refresh sensor readings.
  if( targetAngle < angle_sensor_.getAngle() )
  {
     actuator_.forwardActuator();  // Azimuthal actuator turns CCW, or linear actuator extends, until at target angle.
     forwardActuationRequested = true;
  } else {
     actuator_.reverseActuator();  // Azimuthal actuator turns CW, or linear actuator retracts, until at target angle.
  }

  // ...until at target bearing
  int firstTimeThresholdCurrentSurpassed = 0;
  while( abs(angle_sensor_.getAngle() - targetAngle) > 1 && backoff == false ){
    sensor_container.run();
      
    // Keep sending angle & current sensor readout until destination reached.
    if(motorSpecified == SentForAzMotor){   
      azande.send(eventShowRotaryPosition, angle_sensor_.getAngle());
      azande.send(eventShowCurrentBAzReading, currentSensor_.getCurrent());
    } else if(motorSpecified == SentForAltMotor){
      azande.send(eventShowPitchPosition, angle_sensor_.getAngle());
      azande.send(eventShowCurrentAAltReading, currentSensor_.getCurrent());
    }

    // Ensure backoff algorithm runs if current sensor is above threshold for too long.
    if(currentSensor_.getCurrent() > maxCurrent && firstTimeThresholdCurrentSurpassed == 0){
       firstTimeThresholdCurrentSurpassed = millis();
    } else if (currentSensor_.getCurrent() > maxCurrent && firstTimeThresholdCurrentSurpassed > maxTimePassingThresholdCurrent){
      backoff = true;
    }
    
    delay(20);
  }
  
  actuator_.stopActuator();

  // Delay to allow motor to fully stop.
  delay(timeForFullActuatorStop);

  // backoff algorithm
  if(backoff == true){
    if(forwardActuationRequested){
      actuator_.reverseActuator();
      delay(timeForBackup);
    } else{
      actuator_.forwardActuator();
      delay(timeForBackup);
    }
    errorOccurred();
  }

}
