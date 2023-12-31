#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <cmath>
#include "stdafx.h"
#include "Header.h"
#include "Particle.h"

class CEnvironment{
private:
	static CEnvironment * m_e;
	
	CParticle * m_aParticle;
	unsigned int m_nParticle;

	bool m_bHoleExist;
	CVector m_posHole;

	double m_dt;

private:
	CEnvironment();	// Singleton Pattern
public:
	static CEnvironment * getEnvironment();
	void create_blackHole(const CVector);
	void destroy_blackHole();
	void play();
	double get_dt() const;
	CParticle * getParticle(const int) const;
	CParticle * getParticleAt(const CPoint, const WinSetting) const;
	void disableSelected();

	void setID(CParticle &);
	void set_dt(double dt);

	void init_particles();
	void add_particle(CParticle);
	bool isCollide(CParticle, CParticle) const;
	int collide_detect(const int);
	void add_force(CParticle *, CParticle *);
	void move();
	void draw() const;
	void onDraw(CDC *, const WinSetting) const;
	~CEnvironment();
};

#endif