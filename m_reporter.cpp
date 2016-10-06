// 
// 
// 

#include "m_reporter.h"


 MReporter::MReporter(unsigned long period, MServoController* c) : MTask(period)
{
	this->c = c;
}

void MReporter::init()
{
	Serial.begin(115200);
}

void MReporter::update(unsigned long dt)
{
	Serial.print("Not smoothed actual/desired position: ");
	Serial.print(c->getActualPosition());
	Serial.print("/");
	Serial.println(c->getDesiredPosition());
	
	Serial.print("PID real/adjusted: ");
	Serial.print(c->getLastPid());
	Serial.print(" ");
	Serial.println(c->getLastPidAdj());
	
	Serial.print("PWM: ");
	Serial.println(c->getLastPwm());
	
	
	Serial.println("--------------");
}
