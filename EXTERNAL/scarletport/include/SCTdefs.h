/*
 * CSRG - UTFSM Computer Systems Research Group
 * ACS-UTFSM Team
*/

#define TARGET_AZIMUTH 4
#define REAL_AZIMUTH 5
#define TARGET_ELEVATION 6
#define REAL_ELEVATION 7
#define SPEED_AZIMUTH 8 
#define SPEED_ELEVATION 9

/* Initial calibration configuration. */
/* Degree/iteration  */
#define INIT_AZIMUTH_SPEED    5
#define INIT_ELEVATION_SPEED   5

#define RCX_WAIT_PERIOD 10

/* Sensor assignments. */
#define EVENT_ELEVATION 1
#define EVENT_AZIMUTH   2

/* Motor assignments. */
#define MOTOR_AZIMUTH   OUT_A
#define MOTOR_ELEVATION OUT_C

/* Initial calibration configuration. */
#define POSITION_SENSOR_LOW        40
#define POSITION_SENSOR_HIGH       50
#define AZIMUTH_MAX		   360
#define ELEVATION_MAX		   90
#define AZIMUTH_MIN		   0
#define ELEVATION_MIN		   0

