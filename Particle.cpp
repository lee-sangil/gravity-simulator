#include "Particle.h"
#include "Time.h"

unsigned int seed = (unsigned int)std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::chi_squared_distribution<double> chi_dist(5);
std::normal_distribution<double> normal_dist(0, 5);

COLORREF colorObj(double mass, bool bSelected){
	double _mass = log(1 + mass / 2);
	int r, g, b;

	if (_mass > BIG_MASS){
		r = (int) max(250 - 2 * (_mass - SMALL_MASS), 1);
		g = (int) max(250 - 50 * (BIG_MASS - SMALL_MASS) - 50 * (_mass - BIG_MASS), 1);
		b = (int) max(250 - 100 * (BIG_MASS - SMALL_MASS) - 50 * (_mass - BIG_MASS), 1);
	}
	else if (_mass > SMALL_MASS){
		r = (int) max(250 - 2 * (_mass - SMALL_MASS), 1);
		g = (int) max(250 - 50 * (_mass - SMALL_MASS), 1);
		b = (int) max(250 - 100 * (_mass - SMALL_MASS), 1);
	}
	else{
		r = (int) (150 + 50 * _mass);
		g = (int) (150 + 50 * _mass);
		b = (int) (150 + 50 * _mass);
	}

	if (bSelected){
		r = (r > 100) ? r - 100 : 1;
		g = (g > 100) ? g - 100 : 1;
		b = (b > 100) ? b - 100 : 1;
	}

	return RGB(r, g, b);
}

CParticle::CParticle(){
	m_mass = chi_dist(generator);
	m_pos.set(POS_SCALE*normal_dist(generator), POS_SCALE*normal_dist(generator));
	m_vel.set(VEL_SCALE*normal_dist(generator), VEL_SCALE*normal_dist(generator));
}
CParticle::CParticle(double mass){
	m_mass = chi_dist(generator) * mass;
	m_pos.set(POS_SCALE*normal_dist(generator), POS_SCALE*normal_dist(generator));
	m_vel.set(VEL_SCALE*normal_dist(generator), VEL_SCALE*normal_dist(generator));
}
CParticle::CParticle(double mass, CVector pos){
	m_mass = chi_dist(generator) * mass;
	m_pos = pos;
	m_vel.set(VEL_SCALE*normal_dist(generator), VEL_SCALE*normal_dist(generator));
}
CParticle::CParticle(double mass, CVector pos, CVector vel){
	m_mass = chi_dist(generator) * mass;
	m_pos = pos;
	m_vel = vel;
}
CParticle::CParticle(CVector pos, CVector vel){
	m_mass = chi_dist(generator);
	m_pos = pos;
	m_vel = vel;
}

int CParticle::getID() const {
	return m_ID;
}
CVector CParticle::get_pos() const {
	return m_pos;
}
CVector CParticle::get_vel() const {
	return m_vel;
}
double CParticle::get_mass() const{
	return m_mass;
}
double CParticle::get_radius() const {
	return pow(m_mass, 0.5) * RADIUS_SCALE;
}

void CParticle::selected(){
	m_bSelected = TRUE;
}
void CParticle::disableSelected(){
	m_bSelected = FALSE;
}
void CParticle::setID(int id){
	m_ID = id;
}
void CParticle::reset_force(){
	m_force.set(0, 0);
}
void CParticle::add_force(CVector force){
	m_force += force;
}
void CParticle::move(double dt){
	m_vel += m_force / m_mass * dt;
	m_pos += m_vel * dt;
}
void CParticle::draw() const {
	
}
void CParticle::onDraw(CDC * pDC, const WinSetting window) const {
	CPoint center = window.Center;
	double viewScale = window.Scale;

	int cx = _round(center.x);
	int cy = _round(center.y);
	double rad = get_radius();

	CPen pen;
	CBrush brush;
	
	pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	brush.CreateSolidBrush(colorObj(m_mass, m_bSelected));

	CPen * pOldPen = pDC->SelectObject(&pen);
	CBrush * pOldBrush = pDC->SelectObject(&brush);
	
	pDC->Ellipse((int)floor((m_pos.get_x() - rad) / viewScale) + cx, (int)floor((m_pos.get_y() - rad) / viewScale) + cy,
				 (int) ceil((m_pos.get_x() + rad) / viewScale) + cx, (int) ceil((m_pos.get_y() + rad) / viewScale) + cy);

	//// Debugging
	//CString str;
	//str.Format(_T("%d"), m_ID);
	//pDC->TextOutW((int)floor(m_pos.get_x() / viewScale) + cx, (int)floor(m_pos.get_x() / viewScale) + cy - 2, str);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	
	DeleteObject(pen);
	DeleteObject(brush);
}
void CParticle::collide_with(CParticle p){
	double new_mass = m_mass + p.get_mass();
	CVector new_pos = (m_pos * m_mass + p.get_pos() * p.get_mass()) / new_mass;
	CVector new_vel = (m_vel * m_mass + p.get_vel() * p.get_mass()) / new_mass;

	m_mass = new_mass;
	m_pos = new_pos;
	m_vel = new_vel;
}

CParticle::~CParticle(){
}