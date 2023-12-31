#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cmath>

class CVector{
private:
	double m_x;
	double m_y;
public:
	CVector();
	CVector(double, double);
	void set(double, double);
	double get_x() const;
	double get_y() const;

	CVector operator+() const;
	CVector operator-() const;

	friend CVector operator+(const CVector&, double);
	friend CVector operator-(const CVector&, double);
	friend CVector operator*(const CVector&, double);
	friend CVector operator/(const CVector&, double);

	friend CVector operator+(double, const CVector&);
	friend CVector operator-(double, const CVector&);
	friend CVector operator*(double, const CVector&);
	friend CVector operator/(double, const CVector&);

	CVector& operator=(const CVector&);
	CVector operator+(const CVector&) const;
	CVector operator-(const CVector&) const;
	CVector operator*(const CVector&) const;
	CVector operator/(const CVector&) const;
	CVector& operator+=(const CVector&);
	CVector& operator-=(const CVector&);
	CVector& operator*=(const CVector&);
	CVector& operator/=(const CVector&);

	bool operator==(const CVector&);

	double norm() const;
	void draw() const;
};

#endif