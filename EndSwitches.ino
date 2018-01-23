/*
Copyright 2011 Lex.V.Talionis at gmail
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*/
#include <PinChangeInt.h>
//#include <PinChangeIntConfig.h>
 
#define P1 A8  // the pin we are interested in
#define P2 A9  // the pin we are interested in
#define P3 A10  // the pin we are interested in
#define P4 A11  // the pin we are interested in
#define P5 A12  // the pin we are interested in
#define P6 A13  // the pin we are interested in

#define ES_ZERO_X    A8
#define ES_MAX_X     A9

#define ES_ZERO_Y    A10
#define ES_MAX_Y     A11

#define ES_ZERO_Z    A12
#define ES_MAX_Z     A13
volatile byte burp=0;    // a counter to see how many times the pin has changed
byte cmd=0;     // a place to put our serial data
 
void setup() {
  Serial.begin(9600);
  Serial.print("PinChangeInt test on pin ");
  Serial.println();
  pinMode(ES_ZERO_X, INPUT);     //set the pin to input
  digitalWrite(ES_ZERO_X, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_ZERO_X, esZeroXToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  //PCintPort::attachInterrupt(ES_ZERO_X, esZeroXToggleR,RISING);
  pinMode(ES_MAX_X, INPUT);     //set the pin to input
  digitalWrite(ES_MAX_X, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_MAX_X, esMaxXToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(ES_MAX_X, esMaxXToggleR,RISING);
  pinMode(ES_ZERO_Y, INPUT);     //set the pin to input
  digitalWrite(ES_ZERO_Y, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_ZERO_Y, esZeroYToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(ES_ZERO_Y, esZeroYToggleR,RISING);
  pinMode(ES_MAX_Y, INPUT);     //set the pin to input
  digitalWrite(ES_MAX_Y, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_MAX_Y, esMaxYToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(ES_MAX_Y, esMaxYToggleR,RISING);
  pinMode(ES_ZERO_Z, INPUT);     //set the pin to input
  digitalWrite(ES_ZERO_Z, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_ZERO_Z, esZeroZToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(ES_ZERO_Z, esZeroZToggleR,RISING);
  pinMode(ES_MAX_Z, INPUT);     //set the pin to input
  digitalWrite(ES_MAX_Z, HIGH); //use the internal pullup resistor
  PCintPort::attachInterrupt(ES_MAX_Z, esMaxZToggleF,FALLING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(ES_MAX_Z, esMaxZToggleR,RISING);
// (RISING, FALLING and CHANGE all work with this library)
// and execute the function burpcount when that pin changes
  }
 
void loop() {
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  delay(1000);
}
volatile int c=0;
void burpcount()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}
void burpcount2()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}void burpcount3()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}void burpcount4()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}void burpcount5()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}
void burpcount6()
{
  c++;
  boolean E1 = digitalRead(P1);
  boolean E2 = digitalRead(P2);
  boolean E3 = digitalRead(P3);
  boolean E4 = digitalRead(P4);
  boolean E5 = digitalRead(P5);
  boolean E6 = digitalRead(P6);
  Serial.print(E1?"1|":"0|");
  Serial.print(E2?"1|":"0|");
  Serial.print(E3?"1|":"0|");
  Serial.print(E4?"1|":"0|");
  Serial.print(E5?"1|":"0|");
  Serial.println(E6?"1":"0");
  Serial.println(c);
}
void esZeroXToggle(){
  Serial.print("XZERO: ");
  Serial.println(digitalRead(ES_ZERO_X));
  boolean ES_ZERO_X_STATE = !digitalRead(ES_ZERO_X);
  if(ES_ZERO_X_STATE){
    Serial.println("esZeroXActivated();");
  }

}

void esZeroXToggleR(){
  Serial.println("Rising XZERO");
} 
void esZeroXToggleF(){
  Serial.println("FAlling XZERO");
} 
void esMaxXToggleR(){
  Serial.println("Rising XMAX");
} 
void esMaxXToggleF(){
  Serial.println("FAlling XMAX");
} 

/////
void esZeroYToggleR(){
  Serial.println("Rising YZERO");
} 
void esZeroYToggleF(){
  Serial.println("FAlling YZERO");
} 
void esMaxYToggleR(){
  Serial.println("Rising YMAX");
} 
void esMaxYToggleF(){
  Serial.println("FAlling YMAX");
} 
////
void esZeroZToggleR(){
  Serial.println("Rising ZZERO");
} 
void esZeroZToggleF(){
  Serial.println("FAlling ZZERO");
} 
void esMaxZToggleR(){
  Serial.println("Rising ZMAX");
} 
void esMaxZToggleF(){
  Serial.println("FAlling ZMAX");
} 

void esMaxXToggle(){
   Serial.print("XMAX: ");
  
  
  boolean ES_MAX_X_STATE = !digitalRead(ES_MAX_X);
  Serial.println(ES_MAX_X_STATE);
  if(ES_MAX_X_STATE){
    Serial.println("esMaxXActivated();");
  }
}

void esZeroYToggle(){
   Serial.print("ES_ZERO_Y: ");
  Serial.println(digitalRead(ES_ZERO_Y));
  boolean ES_ZERO_Y_STATE = !digitalRead(ES_ZERO_Y);
  if(ES_ZERO_Y_STATE){
   Serial.println("esZeroYActivated(); ");
  }
}
void esMaxYToggle(){
   Serial.print("ES_MAX_Y: ");
  Serial.println(digitalRead(ES_MAX_Y));
  boolean ES_MAX_Y_STATE = !digitalRead(ES_MAX_Y);
  if(ES_MAX_Y_STATE){
   Serial.println("esMaxYActivated(); ");
  }
}

void esZeroZToggle(){
   Serial.print("ES_ZERO_Z: ");
  Serial.println(digitalRead(ES_ZERO_Z));
  boolean ES_ZERO_Z_STATE = !digitalRead(ES_ZERO_Z);
  if(ES_ZERO_Z_STATE){
     Serial.println("esZeroZActivated(); ");
  }
}
void esMaxZToggle(){
   Serial.print("ES_MAX_Z: ");
  Serial.println(digitalRead(ES_MAX_Z));
  boolean ES_MAX_Z_STATE = !digitalRead(ES_MAX_Z);
  if(ES_MAX_Z_STATE){
     Serial.println("esMaxZActivated(); ");
  }
}
