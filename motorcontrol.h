#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

// Public functions
void initTimers();

// Private function declarations
unsigned long speedToInterval(long currentSpeed);
void checkSwitches();
void positionControl();

#endif