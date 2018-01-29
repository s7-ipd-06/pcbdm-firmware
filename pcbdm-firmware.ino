#include "pcbdm-firmware.h"

// Include declarations of functions that are used in this file
#include "commands.h"
#include "encoders.h"
#include "motorcontrol.h"

// External libraries
#include <SPI.h>
#include <AMIS30543.h>

// Global state initialization
bool positioningType = 0; // 0 = absolute, 1 = relative

AMIS30543 stepper_X;
AMIS30543 stepper_Y;
AMIS30543 stepper_Z;

void initDrivers() {
  SPI.begin();
  
  stepper_X.init(_SS_X);
  stepper_Y.init(_SS_Y);
  stepper_Z.init(_SS_Z);
  delay(1);
  
  stepper_X.resetSettings();
  stepper_X.setCurrentMilliamps(1260);
  stepper_X.setStepMode(8);
  stepper_X.enableDriver();
  
  stepper_Y.resetSettings();
  stepper_Y.setCurrentMilliamps(1260);
  stepper_Y.setStepMode(8);
  stepper_Y.enableDriver();
  
  stepper_Z.resetSettings();
  stepper_Z.setCurrentMilliamps(300);
  stepper_Z.setStepMode(8);
  stepper_Z.enableDriver();
}

void setup() {
  Serial.begin(115200);

  // Initialize pins
  pinMode(_SPINDLE, OUTPUT);
  digitalWrite(_SPINDLE, LOW);

  pinMode(_ES_MIN_X, INPUT_PULLUP);
  pinMode(_ES_MAX_X, INPUT_PULLUP);
  pinMode(_ES_MIN_Y, INPUT_PULLUP);
  pinMode(_ES_MAX_Y, INPUT_PULLUP);
  pinMode(_ES_MIN_Z, INPUT_PULLUP);
  pinMode(_ES_MAX_Z, INPUT_PULLUP);
  
  delay(5000);
  
  initTimers();
  initEncoders();
  initDrivers();
  
  Serial.println("Ready");
}

#define BUFFERSIZE 100
char buff[BUFFERSIZE];
unsigned int buffI;

boolean endStr = false;

void loop() {
  Serial.print("X:");
  Serial.print(currentPosition_x);
  Serial.print(" Y:");
  Serial.println(currentPosition_y);
  delay(500);
}
