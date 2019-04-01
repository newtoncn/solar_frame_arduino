/*
 * All functions getting cmds from Azande and sending commands to motor.
 */

 /*
 * Takes Azande input and sets altitudinal speed.
 * 
 * requires: double speedOrder, input from Azande GUI
 * effects:  float speed_actuate variable of altidutinal_actuator
 */
void setAltSpeed(double speedOrder)
{
  float conversion_factor_percent_to_255 = 255.0/100.0;
  float speedOrderIn255 = speedOrder*conversion_factor_percent_to_255;
  altidutinal_actuator.speedSet(speedOrderIn255);
}

 /*
 * Takes Azande input and sets azimuthal speed.
 * 
 * requires: double speedOrder, input from Azande GUI
 * effects:  float speed_actuate variable of azimuthal_actuator
 */
void setAzSpeed(double speedOrder)
{
  float conversion_factor_percent_to_255 = 255.0/100.0;
  float speedOrderIn255 = speedOrder*conversion_factor_percent_to_255;
  azimuthal_actuator.speedSet(speedOrderIn255);
}
 
 /*
 * This function called when receiving command from "Send Alt Motor Cmd", 
 * (cmdSetAltitudinalAngle), from Azande GUI
 * 
 * requires: long enumValue, input from Azande GUI
 * effects:  initiates extension, retraction, or stoppage of altitudinal actuator
 */
void GiveAltCmd(long enumValue)
{
   TakeAzOrAltCmdAndActuate(enumValue, altidutinal_actuator);
}

 /*
 * This function called when receiving command from "Send Az Motor Cmd"", 
 * (cmdSetAzimuthalAngle), from Azande GUI
 * 
 * requires: long enumValue, input from Azande GUI
 * effects:  initiates clockwise, counterclockwise, or stoppage of azimuthal actuator
 */
void GiveAzCmd(long enumValue)
{ 
  TakeAzOrAltCmdAndActuate(enumValue, azimuthal_actuator);
}

// Implements GiveAltCmd and GiveAzCmd functions
void TakeAzOrAltCmdAndActuate(long enumValue_, Actuator actuator_)
{


  // Identifies which motor being actuated, so that correct messages are sent to Azande GUI.
  bool AzActuator;
  
  
  switch (enumValue_)
  {
    case 0: // Extend or CW
      ExtendOrCW(actuator_);
      if(actuator_.getRelaySetting() == azimuthal_relay_setting){ SendMsgToGUI("Az Act Clockwise", last_cmd_sent); }
      else { SendMsgToGUI("Extend Alt Act", last_cmd_sent); }
      break;

    case 1: // Retract or CCW
      RetractOrCCW(actuator_);
      if(actuator_.getRelaySetting() == azimuthal_relay_setting){ SendMsgToGUI("Az Act CountClock", last_cmd_sent); }
      else { SendMsgToGUI("Retract Alt Act", last_cmd_sent); }
      break;

    case 2: // Stop
      Stop(actuator_);
      SendMsgToGUI("Stop", last_cmd_sent);
      break;

    default:
      break;
  }
}

/* 
 * Any of following functions may be run each time TakeAzOrAltCmdAndActuate 
 * function is run. Implements that function. 
 */
void ExtendOrCW(Actuator act)
{
    act.forwardActuator();
}

void RetractOrCCW(Actuator act)
{
    act.reverseActuator();
}

void Stop(Actuator act)
{
    act.stopActuator();
}
