/*
 * Functions sending data to Azande GUI, including sensor data.
 */
 
 /*
 * Sends data to Azande GUI
 * 
 * requires: nothing
 * effects:  readable output on Azande GUI
 */
 void SendDataToGUI()
 {
  azande.send(eventShowAzSpeed,   (int) azimuthal_actuator.getSpeedAsPercentage() );     
  azande.send(eventShowAltSpeed,  (int) altidutinal_actuator.getSpeedAsPercentage() );
  current_sensor.readCurrent();
  azande.send(eventSendCurrent,  current_sensor.getCurrent() );
  SendMsgToGUI("Waiting4Cmd", current_status);
 }

 /* 
 * requires:  String statusMessage, length maxiumum 64
 * effects:   Sends status message to Azande interface. 
 */
void SendMsgToGUI(String message, int message_type){
  // convert String to char[], as required by Azande
  int message_len = message.length() + 1; 
  char messageChar[message_len];
  message.toCharArray(messageChar, message_len);

  if(message_type == current_status){
    azande.send(eventSendStatusToGUI, messageChar);    
  } else if (message_type == last_cmd_sent){
    azande.send(eventSendLastCmdToGUI, messageChar);  
  }
}
