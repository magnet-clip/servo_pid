// m_servo_controller.h

#include "m_task.h"
#include "m_dc_motor.h"
#include "m_pid.h"
#include "m_pot_reader.h"
#include "m_ewma.h"
#include "no_smoother.h"
#include "m_settings.h"

#ifndef _M_SERVO_CONTROLLER_h
#define _M_SERVO_CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MServoController: public MTask {
private:
	MDcMotor* motor;
	MPid* pid;
	MPotReader* desiredReader;
	MPotReader* actualReader;
	
	NoSmooter desiredSmoother;
	NoSmooter actualSmoother;

	float desiredAngle, actualAngle;
	float lastPid, lastPidAdj;
	int pwmSpeed;
		
public:
	MServoController(unsigned long period, MDcMotor* motor, MPid* pid, MPotReader* desiredAngleReader, MPotReader* actualAngleReader) : MTask(period) {
		this->motor = motor;
		this->pid = pid;
		this->desiredReader = desiredAngleReader;
		this->actualReader = actualAngleReader;
	}

	void init() {
	}
	
	float getActualPosition() { return this->actualAngle; }
	float getDesiredPosition() { return this->desiredAngle; }
	float getLastPid() { return this->lastPid; }
	float getLastPidAdj() { return this->lastPidAdj;}
	int getLastPwm() { return this->pwmSpeed; }
		
protected:
	void update(unsigned long dt);
};

#endif

