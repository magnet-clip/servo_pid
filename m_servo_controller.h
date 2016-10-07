// m_servo_controller.h

#include "m_task.h"
#include "m_dc_motor.h"
#include "m_pid.h"
#include "m_pot_reader.h"
#include "m_ewma.h"
#include "no_smoother.h"

#ifndef _M_SERVO_CONTROLLER_h
#define _M_SERVO_CONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// difference between desired and actual position in ADC points when they are considered equal
#define ACCURACY 10

// minimum level of PWM when motor continues to turn (when PWM is less friction does not let it turn)
// this value is calibrated for 19 volts input voltage
#define MIN_PWM 15 

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
		if ((delta < ACCURACY) && (delta > -ACCURACY)) {
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
			} else {
				motor->reardrive();
				lastPidAdj = lastPid;
			}
		
			lastPidAdj = constrain(lastPidAdj, 0, 1023);
		
			pwmSpeed = lastPidAdj/4.0;
			
			if (pwmSpeed < MIN_PWM) pwmSpeed = 0;
			motor->setSpeed(pwmSpeed);
			
		}
		
	}
};

#endif

