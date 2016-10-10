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
	void update(unsigned long dt) {
		unsigned int desiredAngleInAdcValue = desiredReader->get();
		desiredAngle = desiredAngleInAdcValue; //desiredSmoother.smooth(desiredAngleInAdcValue);
		desiredAngle = map(desiredAngleInAdcValue, 0, 1023, MIN_DESIRED, MAX_DESIRED);

		unsigned int actualAngleInAdcValue = actualReader->get();
		actualAngle = actualAngleInAdcValue;  // actualSmoother.smooth(actualAngleInAdcValue);
		
		signed int delta = (signed int)desiredAngle - (signed int)actualAngle;
		if ((delta < ACCURACY) && (delta > -ACCURACY)) {
			// we are in acceptable interval
			lastPid = 0;
			lastPidAdj = 0;
			pwmSpeed = 0;
			motor->setSpeed(0);
		} else {
			lastPid = pid->calculate((float)desiredAngle, (float)actualAngle); // some value to be converted to PWM

			// direction when pid is less or greater than 0 is determined by wiring of the motor
			if (lastPid < 0) {	
				motor->forward();
				lastPidAdj = -lastPid;
				lastPidAdj *= FWD_COEFF;
			} else {
				motor->reardrive();
				lastPidAdj = lastPid;
				lastPidAdj *= RWD_COEFF;
			}
		
			lastPidAdj = constrain(lastPidAdj, 0, 1023);
		
			pwmSpeed = lastPidAdj/4.0; // simpler version of map(lastPidAdj, 0, 1023, 0, 255)
			
			if (pwmSpeed < MIN_PWM) pwmSpeed = 0;
			motor->setSpeed(pwmSpeed);
		}
	}
};

#endif

