// m_settings.h

#ifndef _M_SETTINGS_h
#define _M_SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// Task periodicity
#define READ_PERIOD 2
#define UPDATE_PID 10
#define REPORT_PERIOD 500

// PID params
#define KP 1
#define KI 0.01
#define KD 0.25

#define MAX_I 16384

// difference between desired and actual position in ADC points when they are considered equal
#define ACCURACY 20

// minimum level of PWM when motor continues to turn (when PWM is less friction does not let it turn)
// this value is calibrated for 19 volts input voltage
#define MIN_PWM 10


#define FWD_COEFF 0.4
#define RWD_COEFF 2

#define MIN_DESIRED 60
#define MAX_DESIRED 940



#endif

