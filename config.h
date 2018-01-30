// Configuration
#define ZAXISSPEED 2000
#define HOMINGSPEED 8000

#define ENCODER_X_INVERT false
#define ENCODER_Y_INVERT false

#define MOTOR_X_INVERT true
#define MOTOR_Y_INVERT false
#define MOTOR_Z_INVERT false

#define steps_um_x 1.94
#define steps_um_y 1.94
#define steps_um_z 1

// Controller factors
#define CONTROLLER_P_X 10 // This is also limited by the maximum acceleration
#define CONTROLLER_P_Y 10 // This is also limited by the maximum acceleration

#define STABLE_MAX_ERROR 2 // The maximum error for the axis to be considered stable
#define UNSTABLE_MIN_ERROR 5 // The minimum error for the axis to be considered unstable
#define STABLECHECKS 8 // Number of times the error needs to be zero to stop the axis from moving