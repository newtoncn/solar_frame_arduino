const int loop_delay = 20; // milliseconds
const int time_allowed_at_max_current = 1200;// milliseconds
const int sizeOfCurrentArray = time_allowed_at_max_current / loop_delay; // convert loop delay to approximately queue array size
const float maxAltCurrent = 4.5;  // [amps] max amperage allowed for altitudinal
const float maxAzCurrent = 12;  // [amps] max amperage allowed for azimuthal
float currentArray[sizeOfCurrentArray]; // array used to measure value


void AddValueToCurrentArray(float currentValue){
  for(int i = (sizeOfCurrentArray - 1); 0 <= (i - 1); i--){
    currentArray[i] = currentArray[i-1];
  }
  currentArray[0] = currentValue;
}

float ReturnCurrentAverageOverTime(){
  float average = 0;
  for(int i = 0; i <= sizeOfCurrentArray -1; i++){
    average += currentArray[i];
  }
  return average/((float) sizeOfCurrentArray);
}

void EnsureCurrentSafety(){
  int maxCurrent = 0;
  if( altOrAz == alt){
    maxCurrent = maxAltCurrent;
  } else if( altOrAz == az){
    maxCurrent = maxAzCurrent;
  }

  if(ReturnCurrentAverageOverTime() > maxCurrent){
    azimuthal_actuator.stopActuator();
    altitudinal_actuator.stopActuator();
    SendMsgToGUI("Current Exceeded", current_status);
    SendMsgToGUI("Stopped: Current Unsafe", last_cmd_sent);
  }
}

void InitializeCurrentArray(){
  for(int i = 0; i < sizeOfCurrentArray -1; i++){
    currentArray[i] = 0.0;
  }
}
