/*
 * All functions sending data to, or from, Azande interface.
 */

/* 
 * Sends updated readouts (cmdGiveReading) for all sensors to Azande Studio.
 * 
 * requires: nothing
 * effects: updates sensor readouts in Azande 
 */
void sendSensorReadout(){ 
 sensor_container.run();
 azande.send(eventShowRotaryPosition, rotary_encoder.getCurrentAngle()); 
 azande.send(eventShowPitchPosition, imu_sensor.getPitch() );   
 azande.send(eventShowHeadingPosition, imu_sensor.getHeading());
 azande.send(eventShowCurrentA_Alt_Reading, current_sensor_A.getCurrent() );
 azande.send(eventShowCurrentB_Az_Reading, current_sensor_B.getCurrent() );
}

/* 
 * requires: String statusMessage, length maxiumum 64
 * effects: Sends status message to Azande interface. 
 */
void sendStatus(String statusMessage){
  // convert String to char[], as required by Azande
  int statusMessage_len = statusMessage.length(); 
  char statusMessageChar[statusMessage_len];
  statusMessage.toCharArray(statusMessageChar, statusMessage_len);
  
  azande.send(eventStatusMsg, statusMessageChar);
}

/* 
 * requires: String errorMessage, length maxiumum 64
 * effects: Turns on error light, sends error message to Azande interface,  and leaves program in long delay. 
 */
void sendErrorMsg(String errorMessage){
  digitalWrite(errorLight_Pin, HIGH);

  // convert String to char[], as required by Azande
  int errorMessage_len = errorMessage.length(); 
  char errorMessageChar[errorMessage_len];
  errorMessage.toCharArray(errorMessageChar, errorMessage_len);

  while(true){
    sendSensorReadout();
    azande.send(eventStatusMsg, errorMessageChar);
    delay(10);
  }
}
