#include "pcbdm-firmware.h" // Contains the global state that is mutated by functions in this file

// Private function declarations
unsigned long speedToInterval(long currentSpeed);
void checkSwitches();
void positionControl();

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

// Position in encoder steps for the XY axis and stepper motor steps for the Z axis
volatile long currentPosition_x = 0;
volatile long currentPosition_y = 0;
volatile long currentPosition_z = 0;
volatile long targetPosition_x = -10000;
volatile long targetPosition_y = -10000;
volatile long targetPosition_z = 0;

long acceleration_x = 5;
long acceleration_y = 5;
long acceleration_z = 5;

// Speed, can go negative
volatile long currentSpeed_x = 0;
volatile long currentSpeed_y = 0;
volatile long currentSpeed_z = 0;
volatile long targetSpeed_x = 0;
volatile long targetSpeed_y = 0;
volatile long targetSpeed_z = 0;
volatile bool currentDirection_x = false;
volatile bool currentDirection_y = false;
volatile bool currentDirection_z = false;

// Pulse intervals, a max value of 0xFFFFFF/4294967295 = ~24 hours between a step which is used as speed 0
volatile unsigned long cmp_x = 0xFFFFFF; // * 10 * 2 us = time between pulse/step
volatile unsigned long cmp_y = 0xFFFFFF;
volatile unsigned long cmp_z = 0xFFFFFF;

// Pulse counters
volatile unsigned long pc_x;
volatile unsigned long pc_y;
volatile unsigned long pc_z;

// Control loop that controls the speed (accelerates/decelerates) by altering the overflow values
// Timer 1: 1 KHz
ISR(TIMER1_COMPA_vect) {
  // Check end switches and adjust speeds & position accordingly
  //checkSwitches();

  // Position control by altering the speeds
  positionControl();

  // Speed control by altering the pulse counter overflow values
  if(currentSpeed_x != targetSpeed_x) {
    bool direction = targetSpeed_x > currentSpeed_x;
    long deltaSpeed = abs(targetSpeed_x-currentSpeed_x);
    
    // Apply maximum speed difference (acceleration)
    deltaSpeed = min(deltaSpeed, acceleration_x);

    // Add the change in speed to the current speed
    currentSpeed_x += (direction ? deltaSpeed : -deltaSpeed);


    cmp_x = speedToInterval(currentSpeed_x);

    // Change direction if needed
    if(currentDirection_x != direction) {
      currentDirection_x = direction;
      digitalWrite(_DIR_X, direction);
    }
  }

  if(currentSpeed_y != targetSpeed_y) {
    bool direction = targetSpeed_y > currentSpeed_y;
    long deltaSpeed = abs(targetSpeed_y-currentSpeed_y);
    
    // Apply maximum speed difference (acceleration)
    deltaSpeed = min(deltaSpeed, acceleration_y);

    // Add the change in speed to the current speed
    currentSpeed_y += (direction ? deltaSpeed : -deltaSpeed);

    cmp_y = speedToInterval(currentSpeed_y);

    // Change direction if needed
    if(currentDirection_y != direction) {
      currentDirection_y = direction;
      digitalWrite(_DIR_Y, direction);
    }
  }

  if(currentSpeed_z != targetSpeed_z) {
    bool direction = targetSpeed_z > currentSpeed_z;
    long deltaSpeed = abs(targetSpeed_z-currentSpeed_z);
    
    // Apply maximum speed difference (acceleration)
    deltaSpeed = min(deltaSpeed, acceleration_z);

    // Add the change in speed to the current speed
    currentSpeed_z += (direction ? deltaSpeed : -deltaSpeed);

    cmp_z = speedToInterval(currentSpeed_z);

    // Change direction if needed
    if(currentDirection_z != direction) {
      currentDirection_z = direction;
      digitalWrite(_DIR_Z, direction);
    }
  }
}

void checkSwitches() {
  boolean es_x_min = !digitalRead(_ES_ZERO_X);  //Neg
  boolean es_x_max = !digitalRead(_ES_MAX_X);   //Neg
  boolean es_y_min = !digitalRead(_ES_ZERO_Y);  //Neg
  boolean es_y_max = !digitalRead(_ES_MAX_Y);   //Neg
  boolean es_z_min = !digitalRead(_ES_ZERO_Z); //Neg
  boolean es_z_max = digitalRead(_ES_MAX_Z);

  if(es_x_min) {
    currentPosition_x = 0;
    targetSpeed_x = max(0, targetSpeed_x);  // Stop immediately
    currentSpeed_x = max(0, currentSpeed_x);
    //Serial.println("End switch X hit");
  }

  if(es_y_min) {
    currentPosition_y = 0;
    targetSpeed_y = max(0, targetSpeed_y);  // Stop immediately
    currentSpeed_y = max(0, currentSpeed_y);
    //Serial.println("End switch Y hit");
  }

  if(es_z_min) {
    currentPosition_z = 0;
    targetSpeed_z = max(0, targetSpeed_z);  // Stop immediately
    currentSpeed_z = max(0, currentSpeed_z);
    //Serial.println("End switch Z hit");
  }
}

void positionControl() {  
  // X-axis P-controller
  long error_x = targetPosition_x - currentPosition_x;
  targetSpeed_x = error_x * CONTROLLER_P_X;

  // Y-axis P-controller
  long error_y = targetPosition_y - currentPosition_y;
  targetSpeed_y = error_y * CONTROLLER_P_Y;

  // Z-axis no controller, max speed used
  long error_z = targetPosition_z - currentPosition_z;
  if(error_z == 0) {
    targetSpeed_z = 0;
  } else {
    targetSpeed_z = error_z > 0 ? ZAXISSPEED : -ZAXISSPEED;
  }
}

// Pulse motors on pulse counter overflow
// Timer 2: 20 KHz / 800 clock cycles
ISR(TIMER2_COMPA_vect) {
  if(pc_x >= cmp_x) {
    pc_x = 0;
    
    PORTE ^= 1 << _PULSE_X_N;
  }
  
  if(pc_y >= cmp_y) {
    pc_y = 0;
    
    PORTE ^= 1 << _PULSE_Y_N;
  }

  if(pc_z >= cmp_z) {
    pc_z = 0;
    
    PORTE ^= 1 << _PULSE_Z_N;

    // Count pulses to determine current position of the Z axis
    currentPosition_z += currentDirection_z ? 1 : -1;
  }
}

// Increase pulse counters
// Timer 3: 100 KHz / 10us / 160 clock cycles
ISR(TIMER3_COMPA_vect) {
  pc_x++;
  pc_y++;
  pc_z++;
}

unsigned long speedToInterval(long currentSpeed) {
  if(currentSpeed == 0)
    return 0xFFFFFF;
  
  const float base = 100000/2;
  int d = base/currentSpeed;
  
  d = max(d, 10); // Minimum interval, max speed
  d = min(d, 50000); // Maximum interval, min speed

  return d;
}
