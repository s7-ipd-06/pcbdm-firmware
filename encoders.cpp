#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

// External libraries
#include <PinChangeInterrupt.h>

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
  #if ENC_X_INVERT
    enc_x_a_v = !digitalRead(_ENC_X_A);
  #else
    enc_x_a_v = digitalRead(_ENC_X_A);
  #endif
  
  currentPosition_x += enc_x_a_v == enc_x_b_v ? 1 : -1;
}
void encXB() {
  #if ENC_X_INVERT
    enc_x_a_v = !digitalRead(_ENC_X_A);
  #else
    enc_x_a_v = digitalRead(_ENC_X_A);
  #endif

  currentPosition_x += enc_x_a_v != enc_x_b_v ? 1 : -1;
}
void encYA() {
  #if ENC_Y_INVERT
    enc_y_a_v = !digitalRead(_ENC_y_A);
  #else
    enc_y_a_v = digitalRead(_ENC_y_A);
  #endif

  currentPosition_y += enc_y_a_v == enc_y_b_v ? 1 : -1;
}
void encYB() {
  #if ENC_Y_INVERT
    enc_y_a_v = !digitalRead(_ENC_y_A);
  #else
    enc_y_a_v = digitalRead(_ENC_y_A);
  #endif

  currentPosition_y += enc_y_a_v != enc_y_b_v ? 1 : -1;
}
