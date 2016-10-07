// no_smoother.h

#include "m_smoother.h"

#ifndef _NO_SMOOTHER_h
#define _NO_SMOOTHER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
class NoSmooter : public MSmoother {
public:
	virtual float smooth(float nextValue) {
		return nextValue;
	}
};

#endif

