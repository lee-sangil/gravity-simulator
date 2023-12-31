#include "Environment.h"

// 한 개의 Environment 객체
CEnvironment * CEnvironment::m_e;

CEnvironment::CEnvironment(){ 
}

// Environment 객체 가져오기
CEnvironment * CEnvironment::getEnvironment(){
	if (m_e == 0){
		m_e = new CEnvironment();
	}
	return m_e;
}

// 블랙홀 생성
void CEnvironment::create_blackHole(const CVector pos){
	m_bHoleExist = TRUE;
	m_posHole = pos;
}

// 블랙홀 소멸
void CEnvironment::destroy_blackHole(){
	m_bHoleExist = FALSE;
}

//
void CEnvironment::play(){
}

// 시뮬레이션 시간 간격 가져오기
double CEnvironment::get_dt() const{
	return m_dt;
}

// ID에 해당하는 물체 가져오기
CParticle * CEnvironment::getParticle(const int id) const {
	for (UINT i = 0; i < m_nParticle; i++)
		if (id == m_aParticle[i].getID())
			return &(m_aParticle[i]);
	return NULL;
}

// Point P에 위치하는 해당하는 물체 가져오기
CParticle * CEnvironment::getParticleAt(const CPoint p, const WinSetting window) const {
	for (UINT i = 0; i < m_nParticle; i++){
		CVector center = m_aParticle[i].get_pos();
		DOUBLE radius = m_aParticle[i].get_radius();

		// 반지름이 너무 작으면, 최저값을 둔다
		if (radius / window.Scale < 5) radius = 5 * window.Scale;

		CVector point((p.x - window.Center.x) * window.Scale, (p.y - window.Center.y) * window.Scale);

		if ((point - center).norm() <= radius) return &(m_aParticle[i]);
	}
	return NULL;
}

// 물체들의 Selected 변수를 모두 False로 바꾼다
void CEnvironment::disableSelected(){
	for (UINT i = 0; i < m_nParticle; i++)
		m_aParticle[i].disableSelected();
}

// 새로운 ID 번호 할당받기
void CEnvironment::setID(CParticle & p){
	if (m_nParticle == 0){
		p.setID(0);
	}
	else{
		int id = m_aParticle[m_nParticle - 1].getID() + 1;
		p.setID(id);
	}
}

// 시뮬레이션 시간 간격 설정하기
void CEnvironment::set_dt(double dt){
	m_dt = dt;
}

// 물체 배열 생성하기
void CEnvironment::init_particles(){
	m_nParticle = DEFAULT_NUMBER;
	m_bHoleExist = FALSE;

	// 물체 배열 생성
	m_aParticle = new CParticle[m_nParticle];
	for (UINT i = 0; i < m_nParticle; i++)
		m_aParticle[i].setID(i);
}

// 물체 P 추가하기
void CEnvironment::add_particle(CParticle p){
	// 새 배열 선언
	CParticle * new_aParticle = new CParticle[m_nParticle+1];
	
	// 기존 배열을 새 배열에 복사
	for (unsigned int i = 0; i < m_nParticle; i++){
		new_aParticle[i] = m_aParticle[i];
		new_aParticle[i].setID(m_aParticle[i].getID());
	}

	// 새로운 물체 새 배열에 저장
	new_aParticle[m_nParticle] = p;
	setID(new_aParticle[m_nParticle]);
	m_nParticle++;

	// 기존 배열 삭제 및 새 배열을 기본 배열로 설정
	delete[] m_aParticle;
	m_aParticle = new_aParticle;
}

// 물체 P1과 P2가 충돌하는지 여부 반환
bool CEnvironment::isCollide(CParticle p1, CParticle p2) const {
	CVector dist = p1.get_pos() - p2.get_pos();
	double rad = p1.get_radius() + p2.get_radius();

	// 두 물체 사이의 거리가 두 물체 사이의 반지름 합보다 작은 경우
	if (dist.norm() < rad) return true;
	return false;
}

// 충돌 발생 후, 선택된 물체의 ID 반환
int CEnvironment::collide_detect(const int id){
	int dead_particle[COLLISION_BUFFER] = { -1, };
	unsigned int new_number = m_nParticle;
	int rtn_id = id;

	for (unsigned int i = 0, k = 0; i < m_nParticle; i++){
		for (unsigned int j = i + 1; j < m_nParticle && k < COLLISION_BUFFER; j++){
			if (isCollide(m_aParticle[i], m_aParticle[j])){
				m_aParticle[i].collide_with(m_aParticle[j]);

				// 기존 선택된 물체가 삭제되었는지 판단
				if (id == m_aParticle[i].getID() || id == m_aParticle[j].getID())
					rtn_id = m_aParticle[i].getID();

				// 삭제 배열에 인덱스 추가
				dead_particle[k++] = j;
				new_number--;
			}
		}
	}
	// 충돌이 없을 경우, 기존 ID 반환
	if (new_number == m_nParticle) return rtn_id;

	// 새 배열 선언
	CParticle * new_particle = new CParticle[new_number];

	for (unsigned int i = 0, j = 0, k = 0; i < m_nParticle; i++){
		if (i != dead_particle[k]){
			// 기존 배열을 새 배열에 복사
			new_particle[j] = m_aParticle[i];
			new_particle[j++].setID(m_aParticle[i].getID());
		}
		else k++;
	}

	// 기존 배열 삭제 및 새 배열을 기본 배열로 설정
	delete[] m_aParticle;
	m_aParticle = new_particle;
	m_nParticle = new_number;

	return rtn_id;
}

// 물체 P1과 물체 P2 사이에 작용하는 힘을 각각에 적용
void CEnvironment::add_force(CParticle * p1, CParticle * p2){
	// 힘 계산
	CVector dist = p1->get_pos() - p2->get_pos();
	double radius = p1->get_radius() + p2->get_radius();
	CVector force;

	if (dist.norm() > radius)
		force = GRAVITY_CONST * p1->get_mass() * p2->get_mass() / pow(dist.norm(), 3) * dist;
	else
		force = GRAVITY_CONST * p1->get_mass() * p2->get_mass() / pow(radius, 3) * dist;

	// 각각의 물체에 힘 추가
	p1->add_force(-force);
	p2->add_force(force);
}

// 물체 움직이기
void CEnvironment::move(){
	// 블랙홀이 존재하는 경우, 블랙홀의 힘 추가
	if (m_bHoleExist){
		for (unsigned int i = 0; i < m_nParticle; i++){
			CVector dist = m_posHole - m_aParticle[i].get_pos();
			double dist_norm = max(pow(dist.norm(), 3), MINIMUM_DIST_NORM);
			CVector force = GRAVITY_CONST * 50 * m_aParticle[i].get_mass() / dist_norm * dist;

			m_aParticle[i].add_force(force);
		}
	}

	// 각각의 물체에 힘 계산
	for (unsigned int i = 0; i < m_nParticle; i++)
		for (unsigned int j = i + 1; j < m_nParticle; j++)
			add_force(&m_aParticle[i], &m_aParticle[j]);

	// 물체 움직이기
	for (unsigned int i = 0; i < m_nParticle; i++){
		m_aParticle[i].move(m_dt);

		// 힘 초기화
		m_aParticle[i].reset_force();
	}
}

//
void CEnvironment::draw() const {

}

// CDC에 물체 그리기
void CEnvironment::onDraw(CDC * pDC, const WinSetting window) const {
	for (unsigned int i = 0; i < m_nParticle; i++)
		m_aParticle[i].onDraw(pDC, window);
}

//
CEnvironment::~CEnvironment(){
	delete m_e;
	delete[] m_aParticle;
}