#ifndef HEADER_H
#define HEADER_H

#include "stdafx.h"
// vector.cpp

// particle.cpp
#define VEL_SCALE						0.2
#define RADIUS_SCALE                    2

// environment.cpp
#define MAX_NUMBER                      500
#define POS_SCALE                       20
#define TIME_SCALE                      100
#define GRAVITY_CONST                   5
#define MINIMUM_DIST_NORM               20
#define DEFAULT_NUMBER                  0
#define COLLISION_BUFFER                10

// view.cpp
#define NEW_VEL_SCALE					0.01

#define SIZE_VERTICAL                   500
#define SIZE_HORIZONTAL                 800

struct WinSetting
{
	CPoint Center;
	DOUBLE Scale;
};

#endif