#include <PinChangeInt.h>


// Interrupt information
// 0 on pin 2
// 1 on pin 3

#define encoder1A 20
#define encoder1B 21 // Only use one interrupt in this example
#define encoder2A 18
#define encoder2B 19 // Only use one interrupt in this example
//#define END1 20 // Only use one interrupt in this example
volatile long count1;
volatile long count2;
void setup()
{
  Serial.begin(115200);
  count1=0;
  count2=0;
  pinMode(encoder1A, INPUT);
  pinMode(encoder1B, INPUT);
  pinMode(encoder2A, INPUT);
  pinMode(encoder2B, INPUT);
  //pinMode(END1, INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder1A), HE1P, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder1B), HE1N, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2A), HE2P, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2B), HE2N, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(END1), T, CHANGE);
}

void loop()
{
  Serial.print(count1);
  Serial.print("\t");
  Serial.println(count2);
  delay(10);
}
void T(){
   Serial.println("END"); 
}
void HE1P()
{
  if(digitalRead(encoder1A) == digitalRead(encoder1B))
  {
    count1++;
  }
  else
  {
    count1--;
  }
  
}
void HE1N()
{
  if(digitalRead(encoder1A) == digitalRead(encoder1B))
  {
    count1--;
  }
  else
  {
    count1++;
  }
  
}
void HE2P()
{
  if(digitalRead(encoder2A) == digitalRead(encoder2B))
  {
    count2++;
  }
  else
  {
    count2--;
  }
  
}
void HE2N()
{
  if(digitalRead(encoder2A) == digitalRead(encoder2B))
  {
    count2--;
  }
  else
  {
    count2++;
  }
  
}
