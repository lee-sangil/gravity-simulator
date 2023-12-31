#include "Time.h"

CGlobalTime::CGlobalTime(){
	QueryPerformanceFrequency(&m_Frequency);
	m_bInitialized = false;
}
void CGlobalTime::begin(){
	QueryPerformanceCounter(&m_BeginTime);
	m_bInitialized = true;
}
double CGlobalTime::passed(){
	double dt = 0;

	if (m_bInitialized == true) {
		QueryPerformanceCounter(&m_EndTime);
		m_elapsed = (m_EndTime.QuadPart - m_BeginTime.QuadPart);
		dt = (double)m_elapsed / (double)m_Frequency.QuadPart;
		QueryPerformanceCounter(&m_BeginTime);
	}

	return dt * TIME_SCALE;
}