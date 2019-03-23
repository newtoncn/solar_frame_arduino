/*
 * All functions getting cmds from Azande and sending commands to motor.
 */
// This function is called when receiving the command "Give sensor read for:", (cmdGiveReading), from Azande Studio
void GiveAltCmd(long enumValue)
{ 
//  sensor_container.run();
  switch (enumValue)
  {
    case 0: // Extend
      Extend();
      break;

    case 1: // Retract
      Retract();
      break;

    case 2: // Stop
      Stop();
      break;

    default:
      break;
  }
}

// Any of following functions may be run each time GiveReading function is run
void Extend()
{
    altidutinal_actuator.forwardActuator();
}

void Retract()
{
    altidutinal_actuator.reverseActuator();
}

void Stop()
{
    altidutinal_actuator.stopActuator();
}
