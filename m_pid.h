// m_pid.h

#ifndef _M_PID_h
#define _M_PID_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MPid {
	private:
		float p, i, d; // coefficient
		float acc;		// accumulated error
		float lastError;		// last error
		
		float max_acc;

	public:
		MPid(float kp, float ki, float kd, float max_acc);
		float calculate(float desired, float actual);			
};

#endif

