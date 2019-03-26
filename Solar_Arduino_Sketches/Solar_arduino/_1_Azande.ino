/*
 * This tab containts all functions, including those sending data to or from Azande interface.
 */


/* 
 * Sends updated readouts (cmdGiveReading) for all sensors to Azande Studio.
 * 
 * requires: nothing
 * effects: updates sensor readouts in Azande 
 */
void SendSensorReadout()
{ 
  sensor_container.run();

    azande.send(eventShowRotaryPosition,      rotary_encoder.getAngle()); 
    azande.send(eventShowPitchPosition,       imu_sensor.getAngle() ); 
    azande.send(eventShowHeadingPosition,     imu_sensor.getHeading());
    azande.send(eventShowCurrentAAltReading,  current_sensor_A_Alt.getCurrent() );
    azande.send(eventShowCurrentBAzReading,   current_sensor_B_Az.getCurrent() );

}

/* 
 * requires: nothing
 * effects: Turns on error light, sends error message to Azande interface,  and leaves program in long delay. 
 */
void errorOccurred(char errorMessage[]){
  digitalWrite(errorLight_Pin, HIGH);
  while(true){
    azande.send(eventErrorMsg, errorMessage); 
    delay(10);
  }
}

/* 
 * This function executes the backoff algorithm.
 * 
 * requires: int motorSpecified_, Actuator actuator__, bool forwardActuationRequested_
 * effects: makes actuator operate in reverse for time timeForBackUp
 */
void backoffRoutine(int motorSpecified_, Actuator actuator__, bool forwardActuationRequested_) {
  //reverse actuator
  if(forwardActuationRequested_){
      actuator__.reverseActuator();
      delay(timeForBackup);
    } else{
      actuator__.forwardActuator();
      delay(timeForBackup);
    }

  //stop actuator
  actuator__.stopActuator();

  //send error message
  if(motorSpecified_ == SentForAzMotor){   
    errorOccurred(AzErrorMsg);
  } else {  
    errorOccurred(AltErrorMsg);
  }
}

/*
 * This function is called when receiving the command "Set Azimuthal Angle", 
 * (cmdSetAzimuthAngle), from Azande Studio, along with requested angle.
 * 
 * requires: double, destination angle
 * effects: makes azimuthal actuator move until reached desired angle
 */
void GetAndSetAzAngle(double azAngleRequest){
  SetAnyAngle(SentForAzMotor, azAngleRequest, azimuthal_actuator, current_sensor_B_Az, maxCurrentAz);
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
  SetAnyAngle(SentForAltMotor, altAngleRequest, altitudinal_actuator, current_sensor_A_Alt, maxCurrentAlt);
}

/*
 * This is the function called only by functions GetAndSetAltAngle and GetAndSetAltAngle. See specs of those functions.
 */
void SetAnyAngle(int motorSpecified, int targetAngle, Actuator actuator_, Current_Sensor currentSensor_, int maxCurrent){
  sensor_container.run(); // Refresh sensor readings.
  bool backoff = false;
  bool forwardActuationRequested = false;
  double currentAngle = 0;

  int thresholdCurrent;     // Current at which array should not be operating, and back off and shutdown are initiated.
  if(motorSpecified == SentForAzMotor){
    thresholdCurrent = maxCurrentAz; 
    currentAngle = rotary_encoder.getAngle();
  } else if(motorSpecified == SentForAltMotor){
    thresholdCurrent = maxCurrentAlt;
    //currentAngle = imu_sensor.getAngle();
  }
  int timeOverThresholdCurrent = 0; // Set to 0 initially.

  if( targetAngle < currentAngle )
  {
     actuator_.forwardActuator();  // Azimuthal actuator turns CCW, or linear actuator extends, until at target angle.
     forwardActuationRequested = true;
  } else {
     actuator_.reverseActuator();  // Azimuthal actuator turns CW, or linear actuator retracts, until at target angle.
  }

  // ...until at target bearing
  int firstTimeThresholdCurrentSurpassed = 0;
  while( backoff == false && abs(currentAngle - targetAngle) > 1 ){
    //refresh sensor readings
    sensor_container.run();
    SendSensorReadout();
    if(motorSpecified == SentForAzMotor){
      currentAngle = rotary_encoder.getAngle();
    } else { 
      //currentAngle = imu_sensor.getAngle();
    }

    // Ensure backoff algorithm runs if current sensor is above threshold for too long.
    if(currentSensor_.getCurrent() > maxCurrent && firstTimeThresholdCurrentSurpassed == 0){
       firstTimeThresholdCurrentSurpassed = millis();
    } else if (currentSensor_.getCurrent() > maxCurrent && firstTimeThresholdCurrentSurpassed > maxTimePassingThresholdCurrent){
      backoff = true;
    }

    delay(20);
  }

  //stop actuator once at target bearing, or current too high
  actuator_.stopActuator();

  // Delay to allow motor to fully stop.
  delay(timeForFullActuatorStop);

  // initiate backoff algorithm if movement terminated early in order to backoff
  if(backoff == true){
    backoffRoutine(motorSpecified, actuator_, forwardActuationRequested);
  }
}
