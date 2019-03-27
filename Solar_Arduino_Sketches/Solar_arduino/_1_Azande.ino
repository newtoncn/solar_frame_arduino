/*
 * All functions sending data to or from Azande interface.
 */

/* 
 * Sends updated readouts (cmdGiveReading) for all sensors to Azande Studio.
 * 
 * requires: nothing
 * effects: updates sensor readouts in Azande 
 */
void SendSensorReadout(){ 
 sensor_container.run();
 azande.send(eventShowRotaryPosition, rotary_encoder.getCurrentAngle()); 
 azande.send(eventShowPitchPosition, imu_sensor.getPitch() );   azande.send(eventShowHeadingPosition, imu_sensor.getHeading());
 azande.send(eventShowCurrentA_Alt_Reading, current_sensor_A.getCurrent() );
 azande.send(eventShowCurrentB_Az_Reading, current_sensor_B.getCurrent() );
}

/* 
 * This function executes the backoff algorithm.
 * 
 * requires: int motorSpecified_, Actuator actuator__, bool forwardActuationRequested_
 * effects: makes actuator operate in reverse for time timeForBackUp
 */
void backoffRoutine(bool forwardActuationRequested_) {
  int timeForBackup = 5000; // 5 seconds
  char AzErrorMsg[] = "Azimuthal motor current over threshold";
  
  if(forwardActuationRequested_){
      azimuthal_actuator.reverseActuator();
      delay(timeForBackup);
    } else{
      azimuthal_actuator.forwardActuator();
      delay(timeForBackup);
    }

  azimuthal_actuator.stopActuator();  // stop actuator
  errorOccurred(AzErrorMsg);//send error message
}

/* 
 * requires: nothing
 * effects: Turns on error light, sends error message to Azande interface,  and leaves program in long delay. 
 */
void errorOccurred(char errorMessage[]){
  digitalWrite(errorLight_Pin, HIGH);
  while(true){
    SendSensorReadout();
    azande.send(eventErrorMsg, errorMessage); 
    delay(10);
  }
}

/*
 * This function is called when receiving the command "Set Azimuthal Angle", (cmdSetAzimuthAngle), from Azande Studio, along with requested angle.
 * 
 * requires: double, destination angle
 * effects: makes azimuthal actuator move until reached desired angle
 */
void SetAzAngle(double azAngleRequest){
  int maxCurrentAlt = 15;       // THESE 2 NUMBERS ARE GUESS; MUST TEST TO ASCERTAIN
  int maxCurrentAz = 10;        // THESE 2 NUMBERS ARE GUESS; MUST TEST TO ASCERTAIN
  int maxTimePassingThresholdCurrent = 3000; // 3 sec
  int timeForFullActuatorStop = 1000; // 1 sec
  bool forwardActuationRequested;   // used to ascertain requested direction
  bool backoff = false; // use to initiate backoff sequence, or not
  
  sensor_container.run();
  
  if( azAngleRequest < rotary_encoder.getCurrentAngle() ){
     azimuthal_actuator.forwardActuator();  // Azimuthal actuator turns CCW, or linear actuator extends, until at target angle.
     forwardActuationRequested = true;
  } else {
     azimuthal_actuator.reverseActuator();  // Azimuthal actuator turns CW, or linear actuator retracts, until at target angle.
  }

  // ...until at target bearing  
  int firstTimeThresholdCurrentSurpassed;
  int timeSinceThresholdCurrentFirstSurpassed = 0;
  while( backoff == false && abs(rotary_encoder.getCurrentAngle() - azAngleRequest) > 1 ){
    sensor_container.run();
    SendSensorReadout();
    azande.send(eventErrorMsg, "No errors");
    azande.send(eventStatusMsg, "Moving Azimuth");

    // Ensure backoff algorithm runs if current sensor is above threshold for too long.
    if(current_sensor_A.getCurrent() > maxCurrentAz && firstTimeThresholdCurrentSurpassed == 0){
      firstTimeThresholdCurrentSurpassed = millis();
    } else if (current_sensor_A.getCurrent() > maxCurrentAz && firstTimeThresholdCurrentSurpassed > 0){
      timeSinceThresholdCurrentFirstSurpassed = millis() - firstTimeThresholdCurrentSurpassed;
    } else if (current_sensor_A.getCurrent() > maxCurrentAz && timeSinceThresholdCurrentFirstSurpassed  > maxTimePassingThresholdCurrent){
      backoff = true;
    } else if (current_sensor_A.getCurrent() <= maxCurrentAz){
      timeSinceThresholdCurrentFirstSurpassed = 0;
      firstTimeThresholdCurrentSurpassed=0;
    }
    delay(20);
  }

  //stop actuator once at target bearing, or current too high
  azimuthal_actuator.stopActuator();

  // Delay to allow motor to fully stop.
  delay(timeForFullActuatorStop);

  // initiate backoff algorithm if movement terminated early in order to backoff
  if(backoff == true){
    backoffRoutine(forwardActuationRequested);
  }
  
  azimuthal_actuator.stopActuator();
}

 /* 
 * This function is called when receiving the command "Set Altitudinal Angle", (cmdSetAltituidnalAngle), from Azande Studio, along with requested angle.
 * Assumes 0 degrees is tubes are flat front plane: ___ and 90 degrees is all tubes vertical:  |
 * 
 * requires: double, destination angle
 * effects: makes altitudinal actuator move until reached desired angle
 */
void SetAltAngle(double altAngleRequest){
  

  
}
