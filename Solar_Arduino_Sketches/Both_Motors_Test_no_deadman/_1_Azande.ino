/*
 * All functions getting cmds from Azande and sending commands to motor.
 */

// This function takes Azande input and sets altitudinal speed.
void setAltSpeed(double speedOrder){
  altidutinal_actuator.speedSet(speedOrder);
}

// This function takes Azande input and sets azimuthal speed.
void setAzSpeed(double speedOrder){
  azimuthal_actuator.speedSet(speedOrder);
}
 
// This function is called when receiving the command "Give sensor read for:", (cmdGiveReading), from Azande Studio
void GiveAltCmd(long enumValue)
{ 
//  sensor_container.run();
  switch (enumValue)
  {
    case 0: // Extend
      ExtendOrCW(altidutinal_actuator);
      break;

    case 1: // Retract
      RetractOrCCW(altidutinal_actuator);
      break;

    case 2: // Stop
      Stop(altidutinal_actuator);
      break;

    default:
      break;
  }
}

// This function is called when receiving the command "Give sensor read for:", (cmdGiveReading), from Azande Studio
void GiveAzCmd(long enumValue)
{ 
//  sensor_container.run();
  switch (enumValue)
  {
    case 0: // CW
      ExtendOrCW(azimuthal_actuator);
      break;

    case 1: // CCW
      RetractOrCCW(azimuthal_actuator);
      break;

    case 2: // Stop
      Stop(azimuthal_actuator);
      break;

    default:
      break;
  }
}

// Any of following functions may be run each time GiveReading function is run
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
