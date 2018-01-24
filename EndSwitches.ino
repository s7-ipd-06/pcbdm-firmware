void initSwitches() {
  //X
  pinMode(ES_ZERO_X, INPUT);     
  digitalWrite(ES_ZERO_X, HIGH); 
  pinMode(ES_MAX_X, INPUT);     
  digitalWrite(ES_MAX_X, HIGH); 
  
  //Y
  pinMode(ES_ZERO_Y, INPUT);     
  digitalWrite(ES_ZERO_Y, HIGH); 
  pinMode(ES_MAX_Y, INPUT);     
  digitalWrite(ES_MAX_Y, HIGH); 
  
  //Z
  pinMode(ES_ZERO_Z, INPUT);     
  digitalWrite(ES_ZERO_Z, HIGH); 
  pinMode(ES_MAX_Z, INPUT);     
  digitalWrite(ES_MAX_Z, HIGH); 
  
  boolean ES_ZERO_X_STATE = !digitalRead(ES_ZERO_X);  //Neg
  boolean ES_MAX_X_STATE = !digitalRead(ES_MAX_X);   //Neg
  boolean ES_ZERO_Y_STATE = !digitalRead(ES_ZERO_Y);  //Neg
  boolean ES_MAX_Y_STATE = !digitalRead(ES_MAX_Y);   //Neg
  boolean ES_ZERO_Z_STATE = !digitalRead(ES_ZERO_Z); //Neg
  boolean ES_MAX_Z_STATE = digitalRead(ES_MAX_Z);
  esZeroXToggle();
  esMaxXToggle();
  esZeroYToggle();
  esMaxYToggle();
  esZeroZToggle();
  esMaxZToggle();
}


void endSwitchRead(){
  boolean E1 = !digitalRead(ES_ZERO_X);  //Neg
  boolean E2 = !digitalRead(ES_MAX_X);   //Neg
  boolean E3 = !digitalRead(ES_ZERO_Y);  //Neg
  boolean E4 = !digitalRead(ES_MAX_Y);   //Neg
  boolean E5 = !digitalRead(ES_ZERO_Z); //Neg
  boolean E6 = digitalRead(ES_MAX_Z);
  
  if(E1!=ES_ZERO_X_STATE){
    ES_ZERO_X_STATE = E1;
    esZeroXToggle();
  }
  if(E2!=ES_MAX_X_STATE){
    ES_MAX_X_STATE = E2;
    esMaxXToggle();
  }
  if(E3!=ES_ZERO_Y_STATE){
    ES_ZERO_Y_STATE = E3;
    esZeroYToggle();
  }
  if(E4!=ES_MAX_Y_STATE){
    ES_MAX_Y_STATE = E4;
    esMaxYToggle();
  }
  if(E5!=ES_ZERO_Z_STATE){
    ES_ZERO_Z_STATE = E5;
    esZeroZToggle();
  }
  if(E6!=ES_MAX_Z_STATE){
    ES_MAX_Z_STATE = E6;
    esMaxZToggle();
  }
}

void esZeroXToggle(){
  if(ES_ZERO_X_STATE){
    esZeroXActivated();
  }
}
void esMaxXToggle(){
  if(ES_MAX_X_STATE){
    esMaxXActivated();
  }
}

void esZeroYToggle(){
  if(ES_ZERO_Y_STATE){
   esZeroYActivated(); 
  }
}
void esMaxYToggle(){
  if(ES_MAX_Y_STATE){
   esMaxYActivated(); 
  }
}

void esZeroZToggle(){
  if(ES_ZERO_Z_STATE){
     esZeroZActivated(); 
  }
}
void esMaxZToggle(){
  if(ES_MAX_Z_STATE){
     esMaxZActivated(); 
  }
}
