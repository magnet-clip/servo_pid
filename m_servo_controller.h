// m_servo_controller.h

#include "m_task.h"
#include "m_dc_motor.h"
#include "m_pid.h"
#include "m_pot_reader.h"
#include "m_ewma.h"

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
	
	MEwma<10> desiredSmoother;
	MEwma<10> actualSmoother;

	
	float desiredAngle, actualAngle;
	float lastPid, lastPidAdj;
	int pwmSpeed;
		
public:
	//(UPDATE_PID, MAX_RPM, &motor, &pid, &angleReader, &positionReader);
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

		unsigned int actualAngleInAdcValue = actualReader->get();
		actualAngle = actualAngleInAdcValue;  // actualSmoother.smooth(actualAngleInAdcValue);
		
		signed int delta = (signed int)desiredAngle - (signed int)actualAngle;
		//Serial.println(delta);
		if ((delta < 20) && (delta > -20)) {
			//Serial.println("1");
			lastPid = 0;
			lastPidAdj = 0;
			pwmSpeed = 0;
			motor->setSpeed(0);

		} else {
			//Serial.println("2");
			lastPid = pid->calculate(desiredAngle, actualAngle); // some value to be converted to PWM

			if (lastPid < 0) {
				motor->reardrive();
				lastPidAdj = -lastPid;
			} else {
				motor->forward();
				lastPidAdj = lastPid;
			}
		
			lastPidAdj = constrain(lastPidAdj, 0, 1023);
		
			pwmSpeed = lastPidAdj/4.0;
			motor->setSpeed(pwmSpeed);
			
		}
		
	}
};

#endif

