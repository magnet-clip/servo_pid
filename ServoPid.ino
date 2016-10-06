#include "m_servo_controller.h"
#include "m_reporter.h"
#include "m_ewma.h"
#include "m_smoother.h"
#include "m_servo_controller.h"
#include "m_dc_motor.h"
#include "m_pot_reader.h"
#include "m_pid.h"

// Pin definitions
#define POS_PIN A1
#define ANGLE_PIN A4

#define M1_PWM_PIN 5

#define M1_PIN1 7
#define M1_PIN2 8

#define M1_CURR_PIN A2

// Task periodicity
#define READ_PERIOD 2
#define UPDATE_PID 10
#define REPORT_PERIOD 500

// PID params
#define KP 0.5
// TODO Integral part depends on frequency if PID calls, hence I'd better include dt into PID calculate
#define KI 0.01 
#define KD 0.0

#define DEBUG_PID
#define DEBUG_ROTATION

MPotReader angleReader(ANGLE_PIN, READ_PERIOD);
MPotReader positionReader(POS_PIN, READ_PERIOD);

MDcMotor motor(M1_PIN1, M1_PIN2, M1_PWM_PIN);

MPid pid(KP, KI, KD, 4096);

MServoController controller(UPDATE_PID, &motor, &pid, &angleReader, &positionReader);

MReporter reporter(REPORT_PERIOD, &controller);


void setup() {
	angleReader.init();
	positionReader.init();
	
	motor.init();
	controller.init();
	reporter.init();
		
	#ifndef DEBUG_PID
	reporter.disable();
	#endif
}

void loop() {
	angleReader.execute();
	positionReader.execute();
	controller.execute();
	reporter.execute();
}