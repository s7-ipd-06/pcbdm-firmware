// Motors
#define _DIR_X     6
#define _PULSE_X   5
#define _PULSE_X_N PE3

#define _DIR_Y     9
#define _PULSE_Y   8
#define _PULSE_Y_N PE6

#define _DIR_Z     3
#define _PULSE_Z   2
#define _PULSE_Z_N PE0

#define _SS_X      7
#define _SS_Y      10
#define _SS_Z      4

/* SPI
MOSI                 51
MISO                 50
CLK                  52
*/

#define _SPINDLE      49

#define _ENC_X_A      18
#define _ENC_X_B      19
#define _ENC_Y_A      20
#define _ENC_Y_B      21

// End switches     Connect these without 5v, just pullup the signal with a pullup resistor and connect ground.
#define _ES_MIN_X     16
#define _ES_MAX_X     17
#define _ES_MIN_Y     14
#define _ES_MAX_Y     15
#define _ES_MIN_Z     12
#define _ES_MAX_Z     13
