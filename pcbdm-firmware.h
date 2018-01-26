#ifndef PCBDM_FIRMWARE_H
#define PCBDM_FIRMWARE_H

// Globally included header files
#include "config.h"
#include "pins.h"

// Globally included external libraries
#include <Arduino.h>

#define ON 1
#define OFF 0

// Global state
extern bool positioningType;
extern volatile long currentPosition_x; // Set by the encoders.cpp
extern volatile long currentPosition_y;
extern volatile long currentPosition_z;

extern volatile long targetPosition_x;
extern volatile long targetPosition_y;
extern volatile long targetPosition_z;

#endif