#include "Vector.h"

CVector::CVector():m_x(0), m_y(0){}
CVector::CVector(double x = 0, double y = 0){
	m_x = x;
	m_y = y;
}
void CVector::set(double x, double y){
	m_x = x;
	m_y = y;
}
double CVector::get_x(void) const {return m_x;}
double CVector::get_y(void) const {return m_y;}

CVector CVector::operator+() const{
	CVector rtn = *this;
	return rtn;
}
CVector CVector::operator-() const{
	CVector rtn(-m_x, -m_y);
	return rtn;
}

CVector operator+(const CVector& v, double f){
	CVector rtn(v.get_x() + f, v.get_y() + f);
	return rtn;
}
CVector operator-(const CVector& v, double f){
	CVector rtn(v.get_x() - f, v.get_y() - f);
	return rtn;
}
CVector operator*(const CVector& v, double f){
	CVector rtn(v.get_x() * f, v.get_y() * f);
	return rtn;
}
CVector operator/(const CVector& v, double f){
	CVector rtn(v.get_x() / f, v.get_y() / f);
	return rtn;
}

CVector operator+(double f, const CVector& v){
	CVector rtn(v.get_x() + f, v.get_y() + f);
	return rtn;
}
CVector operator-(double f, const CVector& v){
	CVector rtn(v.get_x() - f, v.get_y() - f);
	return rtn;
}
CVector operator*(double f, const CVector& v){
	CVector rtn(v.get_x() * f, v.get_y() * f);
	return rtn;
}
CVector operator/(double f, const CVector& v){
	CVector rtn(v.get_x() / f, v.get_y() / f);
	return rtn;
}

CVector& CVector::operator=(const CVector& v){
	m_x = v.m_x;
	m_y = v.m_y;
	return *this;
}
CVector CVector::operator+(const CVector& v) const{
	CVector result = *this;
	result += v;
	return result;
}
CVector CVector::operator-(const CVector& v) const{
	CVector result = *this;
	result -= v;
	return result;
}
CVector CVector::operator*(const CVector& v) const{
	CVector result = *this;
	result *= v;
	return result;
}
CVector CVector::operator/(const CVector& v) const{
	CVector result = *this;
	result /= v;
	return result;
}
CVector& CVector::operator+=(const CVector& v){
	m_x += v.m_x;
	m_y += v.m_y;
	return *this;
}
CVector& CVector::operator-=(const CVector& v){
	m_x -= v.m_x;
	m_y -= v.m_y;
	return *this;
}
CVector& CVector::operator*=(const CVector& v){
	m_x *= v.m_x;
	m_y *= v.m_y;
	return *this;
}
CVector& CVector::operator/=(const CVector& v){
	m_x /= v.m_x;
	m_y /= v.m_y;
	return *this;
}
bool CVector::operator==(const CVector& v){
	if (v.m_x == m_x && v.m_y == m_y)
		return true;
	return false;
}
double CVector::norm() const{
	return sqrt(pow(m_x, 2) + pow(m_y, 2));
}
void CVector::draw() const{

}