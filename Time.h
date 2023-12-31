#ifndef TIME_H
#define TIME_H

#include "Header.h"

class CGlobalTime{
private:
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_BeginTime;
	LARGE_INTEGER m_EndTime;
	INT64 m_elapsed;

	bool m_bInitialized;

public:
	CGlobalTime();
	void begin();
	double passed();
};

#endif