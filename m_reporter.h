// m_reporter.h

#include "m_task.h"
#include "m_servo_controller.h"

#ifndef _M_REPORTER_h
#define _M_REPORTER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MReporter : public MTask {
private:
	MServoController* c;
public:
	MReporter(unsigned long period, MServoController* c);
	virtual void init();
	virtual void update(unsigned long dt);
};

#endif

