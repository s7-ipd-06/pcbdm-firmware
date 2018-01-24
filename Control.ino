#define ZAXISSPEED 2000
#define HOMINGSPEED 5000

// End switch states
volatile boolean ES_ZERO_X_STATE = false;
volatile boolean ES_MAX_X_STATE = false;
volatile boolean ES_ZERO_Y_STATE = false;
volatile boolean ES_MAX_Y_STATE = false;
volatile boolean ES_ZERO_Z_STATE = false;
volatile boolean ES_MAX_Z_STATE = false;

// Setpoints
volatile long setPoint_X;
volatile long setPoint_Y;
volatile long setPoint_Z;// 1 for down -1 for up 0 for still

volatile boolean homing = false;
volatile boolean stable = false;

void setUnstable() {
  stable = false; 
}

void controlLoop() {
  if(homing) {
    if(!ES_ZERO_Z_STATE) {
      lock(true);
    } else {
      lock(false);
    }
    
    if(ES_ZERO_X_STATE && ES_ZERO_Y_STATE && ES_ZERO_Z_STATE) {
      Serial.println("ok");
      stable = true;
      homing = false;
      //Serial.println("Homing done");
    } else {
      setSpeedX(-HOMINGSPEED);
      setSpeedY(-HOMINGSPEED);
      setSpeedZ(-ZAXISSPEED);
    }
    return;
  }
  
  long error_X = setPoint_X - getXPos();
  if(error_X>1000) {
    setSpeedX(10000);
  } else{ 
    if(error_X<-1000) {
      setSpeedX(-10000);
    } else {
      int out_X = error_X * 10;
      setSpeedX(out_X);
    }
  }
  
  long error_Y = setPoint_Y - getYPos();
  if(error_Y>1000) {
    setSpeedY(10000);
  } else {
    if(error_Y<-1000){
      setSpeedY(-10000);
    } else {
      int out_Y = error_Y * 10;
      setSpeedY(out_Y);
    } 
  }
  
  long error_Z = setPoint_Z - getZPos();
  if(error_Z>1000) {
    setSpeedZ(10000);
  } else {
    if(error_Z<-1000) {
      setSpeedZ(-10000);
    } else {
      int out_Z = error_Z * 10;
      setSpeedZ(out_Z);
    } 
  }
  /*
  if(setPoint_Z <0){
    setSpeedZ(-1*ZAXISSPEED);
  }else{
     if(setPoint_Z > 0){
      setSpeedZ(ZAXISSPEED);
     }else{
      setSpeedZ(0);
     } 
  }*/
}

volatile int stable_X = 0;
volatile int stable_Y = 0;
volatile int stable_Z = 0;

int threshold = 200;
void stableCheck() {  //this function is called every ~1ms
  if(homing) {
    return;
  }
  if(getXPos() == setPoint_X || LOCKED) {
    stable_X++;
    if(stable_X>threshold){
       stable_X=threshold; 
    }
  } else {
    stable_X = 0;
  }
  if(getYPos() == setPoint_Y || LOCKED) {
    stable_Y++;
    if(stable_Y>threshold){
       stable_Y=threshold; 
    }
  } else {
    stable_Y = 0;
  }
  
  if(getZPos() == setPoint_Z || (setPoint_Z>getZPos()&& ES_MAX_Z_STATE) || (setPoint_Z<getZPos()&& ES_ZERO_Z_STATE)){
    stable_Z++;
    if(stable_Z>threshold){
       stable_Z=threshold; 
    }
  } else {
    stable_Z = 0;
  }
  /*
  Serial.print(stable_X);
  Serial.print("\t");
  Serial.print(stable_Y);
  Serial.print("\t");
  Serial.println(stable_Z);*/
  if(stable_X == threshold && stable_Y==threshold && stable_Z==threshold){
    stable = true;
    Serial.println("ok");
  }
}

boolean isStable(){
  return stable;
}
void homeAxis(){
  homing = true;
  stable = false;
  setPoint_X = 0;
  setPoint_Y = 0;
  setPoint_Z = 0;
}

void setSetPointX(long s){
    setPoint_X = s;
    stable = false;
}
void setSetPointY(long s){
    setPoint_Y = s;
    stable = false;
}
void setSetPointZ(long s){
    setPoint_Z = s;
    stable = false;
}

void drillDown(){
  setPoint_Z = 1;
  stable = false;
}
void drillUp(){
  setPoint_Z = -1;
  stable = false;
}

void esZeroXActivated(){
  if(getSpeedX()<0){
    setSpeedX(0);  
    forcespeedX(0);   
  }
  homeXEncoder();
  Serial.println("Stopping X backward motion");
}
void esMaxXActivated(){
  if(getSpeedX()>0){
    setSpeedX(0);
    forcespeedX(0);     
  }
  Serial.println("Stopping X forward motion");
}
void esZeroYActivated(){
  if(getSpeedY()<0){
    setSpeedY(0);
     forcespeedY(0);     
  }
  homeYEncoder();
  Serial.println("Stopping Y backward motion");
}
void esMaxYActivated(){
  if(getSpeedY()>0){
    setSpeedY(0); 
    forcespeedY(0);   
  }
  Serial.println("Stopping Y forward motion");
}
void esZeroZActivated(){
  if(getSpeedZ()<0){
    setSpeedZ(0);
    forcespeedZ(0);    
  }
  homeZEncoder();
  Serial.println("Stopping Z up motion");
}
void esMaxZActivated(){
  if(getSpeedZ()>0){
    setSpeedZ(0);
    forcespeedZ(0);     
  }
  Serial.println("Stopping Z down motion");
}
