/*
 * All functions related to movement.
 */

 /*
 * This function is called when receiving the command "Set Azimuthal Angle", (cmdSetAzimuthAngle), from Azande Studio, along with requested angle.
 * 
 * requires: double, destination angle
 * effects: calls SetAngle to make azimuthal actuator move until reached desired angle
 */
void setAltAngle(double altAngleRequest){
  setAngle(altitudinal_motor_identifier, altAngleRequest);
}

 /* 
 * This function is called when receiving the command "Set Altitudinal Angle", (cmdSetAltituidnalAngle), from Azande Studio, along with requested angle.
 * Assumes 0 degrees is tubes are flat front plane: ___ and 90 degrees is all tubes vertical:  |
 * 
 * requires: double, destination angle
 * effects: calls SetAngle to make altitudinal actuator move until reached desired angle
 */
void setAzAngle(double azAngleRequest){
  setAngle(azimuthal_motor_identifier, azAngleRequest);
}

 /* 
 * This function implements specs of setAltAngle or setAzAngle. 
 * The 2 functions need to exist, instead of just this one, in order to have 2 Azande readouts in GUI.
 * 
 * requires: double, destination angle, int actuator identifier
 * effects: makes either altitudinal or azimuthal actuator move until reached desired angle
 */
void setAngle(int motor_identifier, double angleRequest){
  // Initialize variables
  int motor_id = motor_identifier; // used to id which motor to actuate
  int maxCurrent; //  max current allowed
  int maxTimePassingThresholdCurrent = 3000; // 3 sec
  int timeForFullActuatorStop = 1000; // 1 sec
  bool forwardCWActuationRequested = false;   // used to ascertain requested direction. default value is false
  bool backoff = false; // use to determine whether to initiate backoff sequence
  String moveMsg = "Not moving"; //  Status moving message
  String altMoveMsg = "Actuating Altitud";
  String azMoveMsg = "Actuating Azimuth";
  String errMsg = "No error";  // Error message
  String altErrMessage = "Alt current too high";
  String azErrMessage = "Az current too high";

  // Initialize variables according to which motor is in use
  if(motor_id == azimuthal_motor_identifier){
    maxCurrent = azMaxCurrent;
    moveMsg = azMoveMsg;
    errMsg = azErrMessage;
  } else { 
    maxCurrent = altMaxCurrent;
    moveMsg = altMoveMsg;
    errMsg = altErrMessage;
  }

  // Start movement in correct direction
  sensor_container.run();
  if( angleRequest < getAngle(motor_id) ){
     moveForwardActuator(motor_id);  // Azimuthal actuator turns CCW, or linear actuator extends, until at target angle.
     forwardCWActuationRequested = true;
  } else {
     moveReverseActuator(motor_id);  // Azimuthal actuator turns CW, or linear actuator retracts, until at target angle.
  }

  // ...until at target bearing  
  int firstTimeThresholdCurrentSurpassed;
  int timeSinceThresholdCurrentFirstSurpassed = 0;
  while( backoff == false && abs(getAngle(motor_id) - angleRequest) > 1 ){
    sensor_container.run();
    sendSensorReadout();
    sendErrorMsg(errMsg);
    sendStatus(moveMsg);

    // Ensure backoff algorithm runs if current sensor is above threshold for too long.
    if(getCurrent(motor_id) > maxCurrent && firstTimeThresholdCurrentSurpassed == 0){
      firstTimeThresholdCurrentSurpassed = millis();
    } else if (getCurrent(motor_id) > maxCurrent && firstTimeThresholdCurrentSurpassed > 0){
      timeSinceThresholdCurrentFirstSurpassed = millis() - firstTimeThresholdCurrentSurpassed;
    } else if (getCurrent(motor_id) > maxCurrent && timeSinceThresholdCurrentFirstSurpassed  > maxTimePassingThresholdCurrent){
      backoff = true;
    } else if (getCurrent(motor_id) <= maxCurrent){
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
    backoffRoutine(forwardCWActuationRequested, errMsg);
  }
  
  azimuthal_actuator.stopActuator();
}

/* 
 * This function starts an actuator's forward or CW movement.
 * 
 * requires: int motor_id_
 * effects: makes actuator operate in forward or CW direction
 */
void moveForwardActuator(int motor_id_){
    if(motor_id_ == azimuthal_motor_identifier){
    azimuthal_actuator.forwardActuator();
  } else { 
    altitudinal_actuator.forwardActuator();
  }
}

/* 
 * This function starts an actuator's reverse or CCW movement.
 * 
 * requires: int motor_id_
 * effects: makes actuator operate in reverse or CCW.
 */
void moveReverseActuator(int motor_id_){
    if(motor_id_ == azimuthal_motor_identifier){
    azimuthal_actuator.reverseActuator();
  } else { 
    altitudinal_actuator.reverseActuator();
  }
}

/* 
 * This function executes the backoff algorithm.
 * 
 * requires: int motorSpecified_, Actuator actuator__, bool forwardActuationRequested_
 * effects: makes actuator operate in reverse for time timeForBackUp
 */
void backoffRoutine(bool forwardActuationRequested_, String errMsg_) {
  int timeForBackup = 5000; // 5 seconds
  
  if(forwardActuationRequested_){
      azimuthal_actuator.reverseActuator();
      delay(timeForBackup);
    } else{
      azimuthal_actuator.forwardActuator();
      delay(timeForBackup);
    }

  azimuthal_actuator.stopActuator();  // stop actuator
  sendErrorMsg(errMsg_);//send error message
}

/* 
 * This function returns either pitch or azimuthal angle, depending on which motor is functioning.
 * 
 * requires: int motor_id_
 * returns: pitch or azimuthal angle reading.
 */
double getAngle(int motor_id_){
  if(motor_id_ == azimuthal_motor_identifier){
    return rotary_encoder.getCurrentAngle();
  } else { 
    return imu_sensor.getPitch();
  }
}

/* 
 * This function returns the current reading for a motor.
 * 
 * requires: int motor_id_
 * returns: current reading for a motor.
 */
double getCurrent(int motor_id_){
  if(motor_id_ == azimuthal_motor_identifier){
    return current_sensor_A.getCurrent();
  } else { 
    return current_sensor_B.getCurrent();
  }
}
