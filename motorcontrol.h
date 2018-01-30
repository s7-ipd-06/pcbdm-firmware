#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

// Public functions
void initTimers();

// Private function declarations
unsigned long speedToInterval(long currentSpeed);
void controlLoop();
void checkSwitches();
void positionControl();

#endif