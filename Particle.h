#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include "stdafx.h"
#include "Vector.h"
#include "Header.h"

#define SMALL_MASS			1
#define BIG_MASS			1.5

class CParticle{
private:
	int m_ID;
	double m_mass;
	CVector m_pos;
	CVector m_vel;
	CVector m_force;
	bool m_bSelected = FALSE;

public:
	CParticle();
	CParticle(double mass);
	CParticle(double mass, CVector pos);
	CParticle(double mass, CVector pos, CVector vel);
	CParticle(CVector pos, CVector vel);

	int getID() const;
	CVector get_pos() const;
	CVector get_vel() const;
	double get_mass() const;
	double get_radius() const;

	void selected();
	void disableSelected();
	void setID(int);
	void reset_force();

	void add_force(CVector force);
	void move(double dt);
	void draw() const;
	void onDraw(CDC * pDC, const WinSetting window) const;
	void collide_with(CParticle p);

	~CParticle();

};

inline int _round(double a){
	return ((a - floor(a)) > 0.5) ? (int)ceil(a) : (int)floor(a);
}

#endif