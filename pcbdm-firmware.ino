#include <SPI.h>
#include <AMIS30543.h>
#include <PinChangeInterrupt.h>

#define steps_um 1

// Motors
#define DIRPIN_X     6
#define PULSEPIN_X   5

#define DIRPIN_Y     9
#define PULSEPIN_Y   8

#define DIRPIN_Z     3
#define PULSEPIN_Z   2

// SPI chip select
#define SS_X         7
#define SS_Y         10
#define SS_Z         4

#define RELAY        49
/* SPI
MOSI                 51
MISO                 50
CLK                  52
*/

AMIS30543 stepper_X;
AMIS30543 stepper_Y;
AMIS30543 stepper_Z;
//####################
//#####ENCODERS######
#define ENC_X_A      18
#define ENC_X_B      19

#define ENC_Y_A      20
#define ENC_Y_B      21

//#####ENDSWITCHES#####     Connect these without 5v, just pullup the signal with a pullup resistor and connect ground.
#define ES_ZERO_X    16
#define ES_MAX_X     17

#define ES_ZERO_Y    14
#define ES_MAX_Y     15

#define ES_ZERO_Z    12
#define ES_MAX_Z     13
//#####################

void setup(){
  setTimers(); //Timers tab
  Serial.begin(115200);
  initDrivers();
  initSwitches();
  initEncoders();
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
   pinMode(A4, OUTPUT);
   //pinMode(2,INPUT);
   Serial.println("Ready");
}//end setup


#define BUFFERSIZE 100
char buff[BUFFERSIZE];
unsigned int buffI;

boolean endStr = false;

void loop(){
  controlLoop();
  //showinfo();
  //showinfo();
  //showAllow();
  if(Serial.available()) {
    char chr = Serial.read();
    
    if(endStr) { // Reading the character after the \n which is a length checksum
      byte checksum = chr;
      byte msgLength = buffI-1;

      if(checksum == msgLength) {
        processCommand();
        //Serial.println("ok: checksum correct");
      } else {
        Serial.println("err: checksum error");
      }
      
      buffI = 0;
      endStr = false;
    } else {
      if(buffI < BUFFERSIZE) buff[buffI++] = chr; // Put character in buffer
      if(chr == '\n') {
        buff[buffI] = 0; // Null terminate string
        endStr = true;
      }
    }
   }
}
