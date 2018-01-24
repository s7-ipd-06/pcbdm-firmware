void initEncoders(){
  pinMode(ENC_X_A, INPUT);
  pinMode(ENC_X_B, INPUT);
  pinMode(ENC_Y_A, INPUT);
  pinMode(ENC_Y_B, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENC_X_A), encXA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_X_B), encXB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_Y_A), encYA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_Y_B), encYB, CHANGE);
}

volatile long pos_X;
volatile long pos_Y;
volatile long pos_Z;

long getXPos() {
  return pos_X;
}
long getYPos() {
  return pos_Y;
}
long getZPos() {
  return pos_Z;
}
volatile bool enc_x_a_v = false;
volatile bool enc_x_b_v = false;
volatile bool enc_y_a_v = false;
volatile bool enc_y_b_v = false;
void encXA() {
  enc_x_a_v = digitalRead(ENC_X_A);
  enc_x_a_v == enc_x_b_v ? pos_X++ : pos_X--;
  if(ES_ZERO_X_STATE) {
    pos_X = 0;
  }
}
void encXB() {
  enc_x_b_v = digitalRead(ENC_X_B);
  enc_x_a_v != enc_x_b_v ? pos_X++ : pos_X--;
  if(ES_ZERO_X_STATE) {
    pos_X = 0;
  }
}
void encYA() {
  enc_y_a_v = digitalRead(ENC_Y_A);
  enc_y_a_v == enc_y_b_v ? pos_Y++ : pos_Y--;
  if(ES_ZERO_Y_STATE) {
    pos_Y = 0;
  }
}
void encYB() {
  enc_y_b_v = digitalRead(ENC_Y_B);
  enc_y_a_v != enc_y_b_v ? pos_Y++ : pos_Y--;
  if(ES_ZERO_Y_STATE) {
    pos_Y = 0;
  }
}
void encZ(boolean dec) {
  if(dec) {
    pos_Z--;
  } else {
    pos_Z++;
  }
  if(ES_ZERO_Z_STATE) {
    pos_Z=0;
  } 
}
void homeXEncoder() {
  pos_X = 0;
}
void homeYEncoder() {
  pos_Y = 0;
}
void homeZEncoder() {
  pos_Z = 0;
}
