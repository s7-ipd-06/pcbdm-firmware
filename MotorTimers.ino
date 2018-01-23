#include <SPI.h>
#include <AMIS30543.h>

#define DIRPINX 8
#define PULSEPINX 9

#define DIRPINY 23
#define PULSEPINY 22
AMIS30543 stepperx;
AMIS30543 steppery;
void setup(){
  
  
  setTimers(); //Timers tab
  

Serial.begin(9600);
  SPI.begin();
  stepperx.init(SS);
  steppery.init(24);
  delay(1);

  stepperx.resetSettings();
  stepperx.setCurrentMilliamps(1260);
  stepperx.setStepMode(8);
  stepperx.enableDriver();
  
  steppery.resetSettings();
  steppery.setCurrentMilliamps(1260);
  steppery.setStepMode(8);
  steppery.enableDriver();


}//end setup


//int qw = 0;
boolean as = true;
void loop(){
  while(Serial.available()) {
    //qw++;
      String a= Serial.readString();// read the incoming data as string
      if(a.charAt(0) == 'x'){
        as = true;
      }else{
      if(a.charAt(0) == 'y'){
        as = false;
      }else{
        if(a.charAt(0) == 'c'){
        
      }else{
      //int i = a.toInt();
      long i = atol(a.c_str());
      if(i<-1000000 || i>1000000){
         Serial.println("Number not allowed"); 
      }else{
        if(as){
          setspeedX(i);
          Serial.print("Setting X-Speed: ");
          Serial.println(i);
        }else{
          setspeedY(i);
          Serial.print("Setting Y-Speed: ");
          Serial.println(i);
        }
        
        
        //updateFPGA((byte)i);
        //Serial.print("New value: ");
        //Serial.println(i);
      }}}}
  }
  //Serial.print(qw);
  //getXPos();
 
  
}
