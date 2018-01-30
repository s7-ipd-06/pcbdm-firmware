#ifndef COMMANDS_H
#define COMMANDS_H

// Public functions
void processCommand(char * buff);

// Private functions
void setSpindle(bool state);
void homeAxis();
void waitForDoor();

#endif