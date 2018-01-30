#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

#include "motorcontrol.h"

// Position in encoder steps for the XY axis and stepper motor steps for the Z axis
volatile long currentPosition_x = 0;
volatile long currentPosition_y = 0;
volatile long currentPosition_z = 0;
volatile long targetPosition_x = -9999999;
volatile long targetPosition_y = 0;
volatile long targetPosition_z = 0;

long acceleration_x = 50;
long acceleration_y = 50;
long acceleration_z = 50;
long deceleration_x = 100;
long deceleration_y = 100;
long deceleration_z = 100;

// Speed, can go negative
volatile long currentSpeed_x = 0;
volatile long currentSpeed_y = 0;
volatile long currentSpeed_z = 0;
volatile long targetSpeed_x = 0;
volatile long targetSpeed_y = 0;
volatile long targetSpeed_z = 0;

// Initial direction values
volatile bool currentDirection_x = true;
volatile bool currentDirection_y = true;
volatile bool currentDirection_z = true;

volatile bool reportedStable = true;

// Pulse intervals, a max value of 0xFFFFFF/4294967295 = ~24 hours between a step which is used as speed 0
volatile unsigned long cmp_x = 0xFFFFFF; // * 10 * 2 us = time between pulse/step
volatile unsigned long cmp_y = 0xFFFFFF;
volatile unsigned long cmp_z = 0xFFFFFF;

// Pulse counters
volatile unsigned long pc_x;
volatile unsigned long pc_y;
volatile unsigned long pc_z;

void initController() {
  digitalWrite(_DIR_X, currentDirection_x);
  digitalWrite(_DIR_Y, currentDirection_y);
  digitalWrite(_DIR_Z, currentDirection_z);
}

// Control loop that controls the speed (accelerates/decelerates) by altering the overflow values
// Timer 1: 1 KHz / 1ms / 16000 available clock cycles
ISR(TIMER1_COMPA_vect) {
  controlLoop(); 
}

void controlLoop() {
  // Check end switches and adjust target- speeds & position accordingly
  checkSwitches();

  // Determine target speed based on target position
  positionControl();

  // Change speed, accelerate/decelerate to reach target speed
  if(currentSpeed_x != targetSpeed_x) {
    bool speedChangeDirection = targetSpeed_x > currentSpeed_x;
    bool accelerating = speedChangeDirection ? currentSpeed_x >= 0 : currentPosition_x <= 0;

    long deltaSpeed = targetSpeed_x - currentSpeed_x;

    long rateOfChange = accelerating ? acceleration_x : deceleration_x;
    deltaSpeed = deltaSpeed > rateOfChange ? rateOfChange : deltaSpeed;
    deltaSpeed = deltaSpeed < -rateOfChange ? -rateOfChange : deltaSpeed;

    Serial.print("\t"); Serial.print(deltaSpeed);

    currentSpeed_x += deltaSpeed; // Add the change in speed to the current speed

    cmp_x = speedToInterval(currentSpeed_x);

    currentDirection_x = currentSpeed_x > 0;
    
    digitalWrite(_DIR_X, currentDirection_x);
  }
}

long previousError_x = 0;
long previousError_y = 0;
long previousError_z = 0;

void positionControl() {  
  // X-axis controller
  long error_x = targetPosition_x - currentPosition_x;
  long deltaError_x = error_x - previousError_x;
  bool stable_x = abs(error_x) <= STABLE_MAX_ERROR && abs(deltaError_x) <= STABLE_MAX_ERROR_DIFFERENCE;
  targetSpeed_x = error_x * CONTROLLER_X_P + deltaError_x * CONTROLLER_X_D;

  // Y-axis controller
  long error_y = targetPosition_y - currentPosition_y;
  long deltaError_y = error_y - previousError_y;
  bool stable_y = abs(error_y) <= STABLE_MAX_ERROR && abs(deltaError_y) <= STABLE_MAX_ERROR_DIFFERENCE;
  targetSpeed_y = error_y * CONTROLLER_Y_P + deltaError_y * CONTROLLER_Y_D;

  // Z-axis controller
  long error_z = targetPosition_z - currentPosition_z;
  long deltaError_z = error_z - previousError_z;
  bool stable_z = abs(error_z) <= STABLE_MAX_ERROR && abs(deltaError_z) <= STABLE_MAX_ERROR_DIFFERENCE;
  targetSpeed_z = error_z * CONTROLLER_Z_P + deltaError_z * CONTROLLER_Z_D;

  // Print "ok" to serial if all axis are stable
  if(!reportedStable) {
    if(stable_x && stable_y && stable_z) {
      Serial.println("ok");
      reportedStable = true;
    }
  }
}

// Pulse motors on pulse counter overflow
// Timer 2: 20 KHz / 50µs / 800 available clock cycles
ISR(TIMER2_COMPA_vect) {
  if(pc_x >= cmp_x) {
    pc_x = 0;
    
    // Set NXT/STEP signal high
    _PULSE_X_R |= (1 << _PULSE_X_N);

    // Count pulses to determine current position of the Z axis
    currentPosition_x += currentDirection_x ? 1 : -1;
  }
  
  if(pc_y >= cmp_y) {
    pc_y = 0;
    
    _PULSE_Y_R |= (1 << _PULSE_Y_N);

    // Count pulses to determine current position of the Z axis
    currentPosition_y += currentDirection_y ? 1 : -1;
  }

  if(pc_z >= cmp_z) {
    pc_z = 0;
    
    _PULSE_Z_R |= (1 << _PULSE_Z_N);

    // Count pulses to determine current position of the Z axis
    currentPosition_z += currentDirection_z ? 1 : -1;
  }
}

// Increase pulse counters
// Timer 3: 100 KHz / 10us / 160 available clock cycles
ISR(TIMER3_COMPA_vect) {
  pc_x++;
  pc_y++;
  pc_z++;

  // Set the NXT/STEP signals low after 20 µs, the minimum high pulse width is 2 µs.
  if(pc_x == 2) _PULSE_X_R |= (1 << _PULSE_X_N);
  if(pc_y == 2) _PULSE_Y_R |= (1 << _PULSE_Y_N);
  if(pc_z == 2) _PULSE_Z_R |= (1 << _PULSE_Z_N);
}

unsigned long speedToInterval(long currentSpeed) {
  if(currentSpeed == 0) return 0xFFFFFF;
  
  const float base = 100000/2;
  int d = base/currentSpeed;
  
  d = max(d, 1); // Minimum interval, max speed
  d = min(d, 50000); // Maximum interval, min speed

  return d;
}


void initTimers() {
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

void checkSwitches() {
  bool es_x_min = !digitalRead(_ES_MIN_X);
  bool es_x_max = !digitalRead(_ES_MAX_X);
  bool es_y_min = !digitalRead(_ES_MIN_Y);
  bool es_y_max = !digitalRead(_ES_MAX_Y);
  bool es_z_min = !digitalRead(_ES_MIN_Z);
  bool es_z_max = digitalRead(_ES_MAX_Z);

  if(es_x_min && targetSpeed_x < 0) { // If switch hit and moving into switch direction
    currentPosition_x = 0;
    targetPosition_x = 0;
    currentSpeed_x = 0;
    Serial.println("Min switch X hit");
  }

  if(es_x_max && targetSpeed_x > 0) { // If switch hit and moving into switch direction
    targetPosition_x = currentPosition_x;
    Serial.println("Max switch X hit");
  }

  if(es_y_min && targetSpeed_y < 0) { // If switch hit and moving into switch direction
    currentPosition_y = 0;
    targetPosition_y = 0;
    Serial.println("Min switch Y hit");
  }

  if(es_y_max && targetSpeed_y > 0) { // If switch hit and moving into switch direction
    targetPosition_y = currentPosition_x;
    Serial.println("Max switch Y hit");
  }

  if(es_z_min && targetSpeed_z < 0) { // If switch hit and moving into switch direction
    currentPosition_z = 0;
    targetPosition_z = 0;
    Serial.println("Min switch Z hit");
  }

  if(es_z_max && targetSpeed_z > 0) { // If switch hit and moving into switch direction
    targetPosition_z = currentPosition_x;
    Serial.println("Max switch Z hit");
  }
}