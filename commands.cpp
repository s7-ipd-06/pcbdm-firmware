#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

#include "commands.h"

/**
 * Look for character /code/ in the buffer and read the float that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @input code the character to look for.
 * @input val the return value if /code/ is not found.
 **/
float parseNumber(char * buff, char code, float val) {
  char *ptr=buff;  // start at the beginning of buffer
  while((long)ptr > 1 && *ptr) {  // walk to the end
    if(*ptr==code) {  // if you find code on your walk,
      return atof(ptr+1);  // convert the digits that follow into a float and return it
    }
    ptr = strchr(ptr,' ') + 1;  // take a step from here to the letter after the next space
  }
  return val;  // end reached, nothing found, return default val.
}

void processCommand(char * buff) {
  int cmdG = parseNumber(buff, 'G', -1);
  
  int unknownCounter = 0;

  switch(cmdG) {
    case 0:{
      waitForDoor();

      long distanceX = (long)parseNumber(buff, 'X', 0);
      long distanceY = (long)parseNumber(buff, 'Y', 0);
      long distanceZ = (long)parseNumber(buff, 'Z', 0);

      distanceX *= steps_um_x;
      distanceY *= steps_um_y;
      distanceZ *= steps_um_z;
      
      if(strchr(buff, 'X') != NULL) {
        targetPosition_x = (positioningType == 0 ? distanceX : currentPosition_x+distanceX);
      }
      if(strchr(buff, 'Y') != NULL) {
        targetPosition_y = (positioningType == 0 ? distanceY : currentPosition_y+distanceX);
      }
      if(strchr(buff, 'Z') != NULL) {
        targetPosition_z = (positioningType == 0 ? distanceX : currentPosition_z+distanceZ);
      }

      reportedStable = false;
      
      break;}
    case 28: //Home axis
      homeAxis();
      Serial.println("ok");
      break;
    case 90: // Set positioning type to absolute
      Serial.println("ok");
      positioningType = 0;
      break;
    case 91: // Set positioning type to relative
      Serial.println("ok");
      positioningType = 1;
      break;
    default:
      unknownCounter++;
      break;
  }

  int cmdM = parseNumber(buff, 'M', -1);
  switch(cmdM) {
    case 3: // Spindle on
      waitForDoor();
      setSpindle(ON);
      break;
    case 5: // Spindle off
      setSpindle(OFF);
      break;
    case 114: // Get current positions
      Serial.print("ok X:");
      Serial.print(currentPosition_x);
      Serial.print(" Y:");
      Serial.print(currentPosition_y);
      Serial.print(" Z:");
      Serial.println(currentPosition_z);
      break;
    case 203: // Set maximum feedrate
      // TODO
      Serial.println("ok: Not implemented");
      break;
    default:
      unknownCounter++;
      break;
  }

  // Custom commands
  int cmdC = parseNumber(buff, 'C', -1);
  switch(cmdC) {
    case 0: // C0
      // Nothing
      Serial.println("ok: Command does not exist");
      break;
    default:
      unknownCounter++;
      break;
  }

  if(unknownCounter == 3) {
    Serial.println("ok: Command does not exist");
  }
}

void waitForDoor() {
  while(digitalRead(_DOOR_SWITCH) == 0) {
    // Wait to make sure the door is closed
  }
}

void setSpindle(bool state) {
  digitalWrite(_SPINDLE, state);
  Serial.println("ok");
}

void homeAxis() {
  targetPosition_x = -1000000;
  targetPosition_y = -1000000;
  targetPosition_z = -1000000;
}