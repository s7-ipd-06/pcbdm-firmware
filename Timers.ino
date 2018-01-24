void setTimers(){
  cli();//stop interrupts
  //Don't use timer0, It is reserves for delay() and millis() and interferes with the serial read.

  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  
  OCR1A = 15;// 
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
//set timer2 interrupt at 20kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 99;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  //set timer3 at 100kz
  TCCR3A = 0;// set entire TCCR1A register to 0
  TCCR3B = 0;// same for TCCR1B
  TCNT3  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR3A = 19;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR3B |= (1 << WGM32);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR3B |= (1 << CS31) ;//| (1 << CS30);  
  // enable timer compare interrupt
  TIMSK3 |= (1 << OCIE3A);
  
  sei();//allow interrupts 
}

volatile long cmp_X = 50000;
volatile long cmp_Y = 50000;
volatile long cmp_Z = 50000;

volatile long pc_X;
volatile boolean state_X=false;
volatile boolean stopped_X = true;

volatile long pc_Y;
volatile boolean state_Y=false;
volatile boolean stopped_Y = true;

volatile long pc_Z;
volatile boolean state_Z=false;
volatile boolean stopped_Z = true;
//volatile long countx;
//volatile long county;


ISR(TIMER3_COMPA_vect) { //100K 
  pc_X++;
  pc_Y++;
  pc_Z++;
}

volatile long oldspeed_X = 0;
volatile long desspeed_X = 0;

volatile long oldspeed_Y = 0;
volatile long desspeed_Y = 0;

volatile long oldspeed_Z = 0;
volatile long desspeed_Z = 0;

void setSpeedX(int freq) {
  if(LOCKED){
    freq =0;
  }
  desspeed_X = freq;
}
void setSpeedY(int freq) {
  if(LOCKED){
    freq =0;
  }
  desspeed_Y = freq;
}
void setSpeedZ(int freq) {
  desspeed_Z = freq;
}
long getSpeedX() {
  return desspeed_X;
}
long getSpeedY() {
  return desspeed_Y;
}
long getSpeedZ() {
  return desspeed_Z;
}
const int accel = 20;
long GETES_ZERO_X_STATE() {
  return desspeed_X;
}
volatile int debug_state = 0;
ISR(TIMER1_COMPA_vect) {
  endSwitchRead();
  controlLoop();
  if(!stable){
    stableCheck();
  }
  
  endSwitchRead();
  if(desspeed_X == oldspeed_X) {
    //No update required
  } else {
    if(desspeed_X > 0){  //forward
      if(ES_MAX_X_STATE){  //if endswitch MAX is hit, do not allow forward motion
        forcespeedX(0);
        oldspeed_X=0;
      } else {    //Forward motion is allowed
        if(desspeed_X>oldspeed_X+accel){  //check if the speed change will exceed the max acceleration
          if(oldspeed_X<0){  //Instant stop is allowed, so if the current motion is backward, stop and accel forward.
            forcespeedX(accel);
            oldspeed_X=accel;
          }else{  //accell up.
            forcespeedX(oldspeed_X+accel);
            oldspeed_X+=accel;
          }
        }else{
           forcespeedX(desspeed_X);
           oldspeed_X = desspeed_X;
        }
      }
      }else{    //backward
        if(ES_ZERO_X_STATE){
          forcespeedX(0);
          oldspeed_X=0;
        }else{
          if(desspeed_X<oldspeed_X-accel&&desspeed_X!=0){
            if(oldspeed_X>0){
              forcespeedX(-accel);
              oldspeed_X=-accel;
            }else{
              forcespeedX(oldspeed_X-accel);
              oldspeed_X-=accel;
            }
          }else{
             forcespeedX(desspeed_X);
             oldspeed_X = desspeed_X;
          }
        }
      }
    
  }// END X
  
  
  if(desspeed_Y == oldspeed_Y){
    debug_state = 6;
  }else{
    if(desspeed_Y > 0){
      if(ES_MAX_Y_STATE){
        debug_state=1;
        forcespeedY(0);
        oldspeed_Y=0;
      }else{
        if(desspeed_Y>oldspeed_Y+accel){
          if(oldspeed_Y<0){
            debug_state=2;
            forcespeedY(accel);
            oldspeed_Y=accel;
          }else{
            forcespeedY(oldspeed_Y+accel);
            oldspeed_Y+=accel;
          }
        }else{
          debug_state=0;
           forcespeedY(desspeed_Y);
           oldspeed_Y = desspeed_Y;
        }
      }
    }else{
      if(ES_ZERO_Y_STATE){
        forcespeedY(0);
        oldspeed_Y=0;      
      }else{
        if(desspeed_Y<oldspeed_Y-accel&&desspeed_Y!=0){
          if(oldspeed_Y>0){
            forcespeedY(-accel);
            oldspeed_Y=-accel;
          }else{
            forcespeedY(oldspeed_Y-accel);
            oldspeed_Y-=accel;
          }
        }else{
          forcespeedY(desspeed_Y);
          oldspeed_Y = desspeed_Y;
        }
      }
    }
  }
  
  
  if(desspeed_Z == oldspeed_Z){
    //No update required
  }else{
    if(desspeed_Z > 0){
      if(ES_MAX_Z_STATE){
        forcespeedZ(0);
        oldspeed_Z=0;
      }else{
        if(desspeed_Z>oldspeed_Z+accel){
          if(oldspeed_Z<0){
            forcespeedZ(accel);
            oldspeed_Z=accel;
          }else{
            forcespeedZ(oldspeed_Z+accel);
            oldspeed_Z+=accel;
          }
        }else{
           forcespeedZ(desspeed_Z);
           oldspeed_Z = desspeed_Z;
        }
      }
    }else{
      if(ES_ZERO_Z_STATE){
        forcespeedZ(0);
        oldspeed_Z=0;      
      }else{
        if(desspeed_Z<oldspeed_Z-accel&&desspeed_Z!=0){
          if(oldspeed_Z>0){
            forcespeedZ(-accel);
            oldspeed_Z=-accel;
          }else{
            forcespeedZ(oldspeed_Z-accel);
            oldspeed_Z-=accel;
          }
        }else{
          forcespeedZ(desspeed_Z);
          oldspeed_Z = desspeed_Z;
        }
      }
    }
  }
  
  
  /*
  Serial.print("X: ");
  Serial.print(countx);
  Serial.print("\t");
  Serial.println(pcx);
  
  Serial.print("Y: ");
  Serial.print(county);
  Serial.print("\t");
  Serial.println(pcy);
  countx = 0;
  county = 0;
  */
}

void showinfo(){  
  Serial.print(desspeed_Z);
  Serial.print("\t");
  Serial.print(stopped_Z);
  Serial.print("\t");
  Serial.print(ES_ZERO_Z_STATE);
  Serial.print("\t");
  Serial.print(pc_Z);
  Serial.print("\t");
  Serial.println(cmp_Z);
}

void showAllow(){
  Serial.print(ES_MAX_X_STATE?"\tForward Denied":"Forward Allowed"); 
  Serial.print(ES_ZERO_X_STATE?"\tBackward Denied":"Backward Allowed"); 
  Serial.print(ES_MAX_Y_STATE?"\tForward Denied":"Forward Allowed"); 
  Serial.print(ES_ZERO_Y_STATE?"\tBackward Denied":"Backward Allowed");
  Serial.print(ES_MAX_Z_STATE?"\tDOWN Denied":"DOWN Allowed"); 
  Serial.println(ES_ZERO_Z_STATE?"\tUP Denied":"UP Allowed");
}

volatile boolean DIR_Z = HIGH;
ISR(TIMER2_COMPA_vect){ //20K PULSER  
  if(stopped_X || LOCKED) {
    pc_X=0;
  } else {
    if(pc_X>=cmp_X) {
      pc_X-=cmp_X;
      
      state_X = !state_X;
      digitalWrite(PULSEPIN_X,state_X);
    }
  } 
  if(stopped_Y || LOCKED) {
    pc_Y=0;
    
  } else {
    if(pc_Y>=cmp_Y) {
      pc_Y-=cmp_Y;
      
      state_Y = !state_Y;
      digitalWrite(PULSEPIN_Y,state_Y);
    }
  }
  if(stopped_Z) {
    pc_Z=0;
  } else {
    if(pc_Z>=cmp_Z) {
      pc_Z-=cmp_Z;
      
      state_Z = !state_Z;
      if(state_Z){
        encZ(DIR_Z);
      }
      digitalWrite(PULSEPIN_Z,state_Z);
    }
  } 
}

const float base = 100000/2;
void forcespeedX(int freq) {
  oldspeed_X = freq;
  pc_X = 0;
  if(freq == 0) {
     stopped_X = true; 
  } else {
    stopped_X = false;
  }
  if(freq<0) {
    freq = -freq;
    digitalWrite(DIRPIN_X,HIGH);
    
  } else {
    digitalWrite(DIRPIN_X,LOW);
  }
  
  float f = (float)freq;
  int d = base/freq;
  if(d<5) {
    d=5;
  }
  if(d > 500000) {
    d = 50000;
  }
  cmp_X = d;
 // Serial.print("New X comp value: ");
 // Serial.println(d);
}
void forcespeedY(int freq) {
  oldspeed_Y = freq;
  pc_Y = 0;
  if(freq == 0) {
     stopped_Y = true; 
  } else {
    stopped_Y = false;
  }
  if(freq<0) {
    freq = -freq;
    digitalWrite(DIRPIN_Y,HIGH);
  } else {
    digitalWrite(DIRPIN_Y,LOW);
  }
  
  float f = (float)freq;
  int d = base/freq;
  if(d<5) {
    d=5;
  }
  if(d > 500000) {
    d = 50000;
  }
  cmp_Y = d;
  //Serial.print("New Y comp value: ");
  //Serial.println(d);
}

void forcespeedZ(int freq) {
  oldspeed_Z = freq;
  pc_Z = 0;
  if(freq == 0) {
     stopped_Z = true; 
  } else {
    stopped_Z = false;
  }
  if(freq<0) {
    freq = -freq;
    digitalWrite(DIRPIN_Z,HIGH);
    DIR_Z = HIGH;
  } else {
    digitalWrite(DIRPIN_Z,LOW);
    DIR_Z = LOW;
  }
  
  float f = (float)freq;
  int d = base/freq;
  if(d<5) {
    d=5;
  }
  if(d > 500000) {
    d = 50000;
  }
  cmp_Z = d;
  //Serial.print("New Y comp value: ");
  //Serial.println(d);
}
