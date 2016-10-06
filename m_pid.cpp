// 
// 
// 

#include "m_pid.h"

 MPid::MPid(float kp, float ki, float kd, float max_acc)
{
	p = kp;
	i = ki;
	d = kd;
	this->max_acc = max_acc;
	
	acc = 0.0;
	err = 0.0;
}

float MPid::calculate(float desired, float actual)
{
	float error = desired - actual;
	
	acc += error;
	
	float p_term = p * error;
	float i_term = i * constrain(acc, -max_acc, max_acc); 
	float d_term = d * (error - err);
	
	err = error;
	
	float pid = p_term + i_term + d_term;
	return pid;
}
