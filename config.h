// Configuration
#define ZAXISSPEED 2000
#define HOMINGSPEED 8000

#define ENCODER_X_INVERT TRUE
#define ENCODER_Y_INVERT FALSE

#define steps_um_x 1.94
#define steps_um_y 1.94
#define steps_um_z 1

// Controller factors
#define CONTROLLER_P_X 10 // This is also limited by the maximum acceleration
#define CONTROLLER_P_Y 10 // This is also limited by the maximum acceleration

#define STABLECHECKS 8 // Number of times the error needs to be zero to stop the axis from moving