// Configuration
#define ZAXISSPEED 2000
#define XAXISSPEED 2000
#define HOMINGSPEED 8000

#define ENCODER_X_INVERT false
#define ENCODER_Y_INVERT false

#define steps_um_x 1.94
#define steps_um_y 1.94
#define steps_um_z 1

// Controller factors
#define CONTROLLER_X_P 8
#define CONTROLLER_X_D 1

#define CONTROLLER_Y_P 8
#define CONTROLLER_Y_D 1

#define CONTROLLER_Z_P 8
#define CONTROLLER_Z_D 1

// The maximum static error for the axis to be considered stable
#define STABLE_MAX_ERROR 1

// The maximum difference in error between
// control loop ticks for the axis to be considered stable
#define STABLE_MAX_ERROR_DIFFERENCE 1