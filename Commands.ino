
void setXSetpoint(long point) {
  Serial.print("Setting X-Point: ");
  
  Serial.println(point);
  setSetPointX(point);
  setUnstable();
}

void setYSetpoint(long point) {
  Serial.print("Setting Y-Point ");
  Serial.println(point);
  setSetPointY(point);
  setUnstable();
}

void setZSetpoint(long point) {
  Serial.print("Setting Z-Point ");
  Serial.println(point);
  setSetPointZ(point);
  setUnstable();
  
}
boolean LOCKED = false;
void lock(boolean l){
  LOCKED = l;
  if(LOCKED){
    setSpeedX(0);
    setSpeedY(0);
  }
}

void setSpindleOn() {
  digitalWrite(RELAY, HIGH);
  Serial.println("ok Spindle On");
}

void setSpindleOff() {
  digitalWrite(RELAY, LOW);
  Serial.println("ok Spindle Off");
}

bool positioningType = 0; // 0 = absolute, 1 = relative

void processCommand() {
  int cmdG = parseNumber('G', -1);
  
  switch(cmdG) {
    case 0:{
      //Serial.println(buff);
      long distanceX = (long)parseNumber('X', 0);
      long distanceY = (long)parseNumber('Y', 0);
      long distanceZ = (long)parseNumber('Z', 0);
      
      if(positioningType == 0) { // Absolute
        if(strchr(buff, 'X') != NULL) {
          distanceX *= steps_um;
          setXSetpoint(distanceX);
        }
        if(strchr(buff, 'Y') != NULL) {
          distanceY *= steps_um;
          setYSetpoint(distanceY);
        }
        if(strchr(buff, 'Z') != NULL) {
          distanceZ *= steps_um;
          setZSetpoint(distanceZ);
        }
      } else { // Relative
        if(strchr(buff, 'X') != NULL) {
          distanceX *= steps_um;
          setXSetpoint(getXPos()+distanceX);
        }
        if(strchr(buff, 'Y') != NULL) {
          distanceY *= steps_um;
          setYSetpoint(getYPos()+distanceY);
        }
        if(strchr(buff, 'Z') != NULL) {
          distanceZ *= steps_um;
          setZSetpoint(getZPos()+distanceZ);
        }
      }
      
      break;}
    case 28: //Home axis
      homeAxis();
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

  int cmdM = parseNumber('M', -1);
  switch(cmdM) {
    case 3: // Spindle on
      setSpindleOn();
      break;
    case 5: // Spindle off
      setSpindleOff();
      break;
    case 203: // Set maximum feedrate
      Serial.println("ok");
      break;
  }

  // Custom commands
  int cmdC = parseNumber('C', -1);
  switch(cmdC) {
    case 0: // Get X position
      //Serial.println(buff);
      Serial.println(getXPos());
      Serial.println("ok");
      break;
    case 1: // Get Y position
      //Serial.println(buff);
      Serial.println(getYPos());
      Serial.println("ok");
      break;
    case 2: // Get Z position
      //Serial.println(buff);
      Serial.println(getZPos());
      Serial.println("ok");
      break;
    case 3: // Lock X & Y
      Serial.println("ok Locking X & Y axis");
      lock(true);
      break;
    case 4: // Unlock X & Y
      Serial.println("ok Unlocking X & Y axis");
      lock(false);
      break; 
  }
}

/**
 * Look for character /code/ in the buffer and read the float that immediately follows it.
 * @return the value found.  If nothing is found, /val/ is returned.
 * @input code the character to look for.
 * @input val the return value if /code/ is not found.
 **/
float parseNumber(char code, float val) {
  char *ptr=buff;  // start at the beginning of buffer
  while((long)ptr > 1 && (*ptr) && (long)ptr < (long)buff+buffI) {  // walk to the end
    if(*ptr==code) {  // if you find code on your walk,
      return atof(ptr+1);  // convert the digits that follow into a float and return it
    }
    ptr=strchr(ptr,' ')+1;  // take a step from here to the letter after the next space
  }
  return val;  // end reached, nothing found, return default val.
}
