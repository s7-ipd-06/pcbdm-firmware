#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

// External libraries
#include <PinChangeInterrupt.h>

// Private function declarations
void encXA();
void encXB();
void encYA();
void encYB();

void initEncoders() {
  // pinMode(ENC_X_A, INPUT);
  // pinMode(ENC_X_B, INPUT);
  // pinMode(ENC_Y_A, INPUT);
  // pinMode(ENC_Y_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(_ENC_X_A), encXA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_ENC_X_B), encXB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_ENC_Y_A), encYA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(_ENC_Y_B), encYB, CHANGE);
}

volatile bool enc_x_a_v = false;
volatile bool enc_x_b_v = false;
volatile bool enc_y_a_v = false;
volatile bool enc_y_b_v = false;
void encXA() {
  enc_x_a_v = !digitalRead(_ENC_X_A);
  enc_x_a_v == enc_x_b_v ? currentPosition_x++ : currentPosition_x--;
}
void encXB() {
  enc_x_b_v = !digitalRead(_ENC_X_B);
  enc_x_a_v != enc_x_b_v ? currentPosition_x++ : currentPosition_x--;
}
void encYA() {
  enc_y_a_v = digitalRead(_ENC_Y_A);
  enc_y_a_v == enc_y_b_v ? currentPosition_y++ : currentPosition_y--;
}
void encYB() {
  enc_y_b_v = digitalRead(_ENC_Y_B);
  enc_y_a_v != enc_y_b_v ? currentPosition_y++ : currentPosition_y--;
}
// void encZ(boolean dec) {
//   if(dec) {
//     pos_Z--;
//   } else {
//     pos_Z++;
//   }
//   if(ES_ZERO_Z_STATE) {
//     pos_Z=0;
//   } 
// }
// void homeXEncoder() {
//   pos_X = 0;
// }
// void homeYEncoder() {
//   pos_Y = 0;
// }
// void homeZEncoder() {
//   pos_Z = 0;
// }
