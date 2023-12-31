
// Gravity_Simulator.h : main header file for the Gravity_Simulator application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

// CGravity_SimulatorApp:
// See Gravity_Simulator.cpp for the implementation of this class
//

class CGravity_SimulatorApp : public CWinApp
{
public:
	CGravity_SimulatorApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CGravity_SimulatorApp theApp;
