#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

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
  
  switch(cmdG) {
    case 0:{
      //Serial.println(buff);
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
      
      break;}
    case 28: //Home axis
      //homeAxis();
      break;
    case 90: // Set positioning type to absolute
      Serial.println("ok");
      positioningType = 0;
      break;
    case 91: // Set positioning type to relative
      Serial.println("ok");
      positioningType = 1;
      break;
  }

  int cmdM = parseNumber(buff, 'M', -1);
  switch(cmdM) {
    case 3: // Spindle on
      setSpindle(ON);
      break;
    case 5: // Spindle off
      setSpindle(OFF);
      break;
    case 203: // Set maximum feedrate
      Serial.println("ok");
      break;
  }

  // Custom commands
  int cmdC = parseNumber(buff, 'C', -1);
  switch(cmdC) {
    case 0: // Get XYZ positions
      Serial.print("X:\t"); Serial.println(currentPosition_x);
      Serial.print("Y:\t"); Serial.println(currentPosition_y);
      Serial.print("Z:\t"); Serial.println(currentPosition_z);
      //Serial.println(getXPos());
      Serial.println("ok");
      break;
    case 1: // Get Y position
      //Serial.println(buff);
      //Serial.println(getYPos());
      Serial.println("ok");
      break;
    case 2: // Get Z position
      //Serial.println(buff);
      //Serial.println(getZPos());
      Serial.println("ok");
      break;
    case 3: // Lock X & Y
      Serial.println("ok Locking X & Y axis");
      //lock(true);
      break;
    case 4: // Unlock X & Y
      Serial.println("ok Unlocking X & Y axis");
      //lock(false);
      break; 
  }
}

// void setXSetpoint(long point) {
//   Serial.print("Setting X-Point: ");
  
//   Serial.println(point);
//   setSetPointX(point);
//   setUnstable();
// }

// void setYSetpoint(long point) {
//   Serial.print("Setting Y-Point ");
//   Serial.println(point);
//   setSetPointY(point);
//   setUnstable();
// }

// void setZSetpoint(long point) {
//   Serial.print("Setting Z-Point ");
//   Serial.println(point);
//   setSetPointZ(point);
//   setUnstable();
  
// }
// boolean LOCKED = false;
// void lock(boolean l){
//   LOCKED = l;
//   if(LOCKED){
//     setSpeedX(0);
//     setSpeedY(0);
//   }
// }

void setSpindle(bool state) {
  digitalWrite(_SPINDLE, state);
  Serial.println("ok");
}

// void setSpindleOff() {
//   digitalWrite(RELAY, LOW);
//   Serial.println("ok Spindle Off");
// }