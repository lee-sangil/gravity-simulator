#include "Environment.h"

// �� ���� Environment ��ü
CEnvironment * CEnvironment::m_e;

CEnvironment::CEnvironment(){ 
}

// Environment ��ü ��������
CEnvironment * CEnvironment::getEnvironment(){
	if (m_e == 0){
		m_e = new CEnvironment();
	}
	return m_e;
}

// ��Ȧ ����
void CEnvironment::create_blackHole(const CVector pos){
	m_bHoleExist = TRUE;
	m_posHole = pos;
}

// ��Ȧ �Ҹ�
void CEnvironment::destroy_blackHole(){
	m_bHoleExist = FALSE;
}

//
void CEnvironment::play(){
}

// �ùķ��̼� �ð� ���� ��������
double CEnvironment::get_dt() const{
	return m_dt;
}

// ID�� �ش��ϴ� ��ü ��������
CParticle * CEnvironment::getParticle(const int id) const {
	for (UINT i = 0; i < m_nParticle; i++)
		if (id == m_aParticle[i].getID())
			return &(m_aParticle[i]);
	return NULL;
}

// Point P�� ��ġ�ϴ� �ش��ϴ� ��ü ��������
CParticle * CEnvironment::getParticleAt(const CPoint p, const WinSetting window) const {
	for (UINT i = 0; i < m_nParticle; i++){
		CVector center = m_aParticle[i].get_pos();
		DOUBLE radius = m_aParticle[i].get_radius();

		// �������� �ʹ� ������, �������� �д�
		if (radius / window.Scale < 5) radius = 5 * window.Scale;

		CVector point((p.x - window.Center.x) * window.Scale, (p.y - window.Center.y) * window.Scale);

		if ((point - center).norm() <= radius) return &(m_aParticle[i]);
	}
	return NULL;
}

// ��ü���� Selected ������ ��� False�� �ٲ۴�
void CEnvironment::disableSelected(){
	for (UINT i = 0; i < m_nParticle; i++)
		m_aParticle[i].disableSelected();
}

// ���ο� ID ��ȣ �Ҵ�ޱ�
void CEnvironment::setID(CParticle & p){
	if (m_nParticle == 0){
		p.setID(0);
	}
	else{
		int id = m_aParticle[m_nParticle - 1].getID() + 1;
		p.setID(id);
	}
}

// �ùķ��̼� �ð� ���� �����ϱ�
void CEnvironment::set_dt(double dt){
	m_dt = dt;
}

// ��ü �迭 �����ϱ�
void CEnvironment::init_particles(){
	m_nParticle = DEFAULT_NUMBER;
	m_bHoleExist = FALSE;

	// ��ü �迭 ����
	m_aParticle = new CParticle[m_nParticle];
	for (UINT i = 0; i < m_nParticle; i++)
		m_aParticle[i].setID(i);
}

// ��ü P �߰��ϱ�
void CEnvironment::add_particle(CParticle p){
	// �� �迭 ����
	CParticle * new_aParticle = new CParticle[m_nParticle+1];
	
	// ���� �迭�� �� �迭�� ����
	for (unsigned int i = 0; i < m_nParticle; i++){
		new_aParticle[i] = m_aParticle[i];
		new_aParticle[i].setID(m_aParticle[i].getID());
	}

	// ���ο� ��ü �� �迭�� ����
	new_aParticle[m_nParticle] = p;
	setID(new_aParticle[m_nParticle]);
	m_nParticle++;

	// ���� �迭 ���� �� �� �迭�� �⺻ �迭�� ����
	delete[] m_aParticle;
	m_aParticle = new_aParticle;
}

// ��ü P1�� P2�� �浹�ϴ��� ���� ��ȯ
bool CEnvironment::isCollide(CParticle p1, CParticle p2) const {
	CVector dist = p1.get_pos() - p2.get_pos();
	double rad = p1.get_radius() + p2.get_radius();

	// �� ��ü ������ �Ÿ��� �� ��ü ������ ������ �պ��� ���� ���
	if (dist.norm() < rad) return true;
	return false;
}

// �浹 �߻� ��, ���õ� ��ü�� ID ��ȯ
int CEnvironment::collide_detect(const int id){
	int dead_particle[COLLISION_BUFFER] = { -1, };
	unsigned int new_number = m_nParticle;
	int rtn_id = id;

	for (unsigned int i = 0, k = 0; i < m_nParticle; i++){
		for (unsigned int j = i + 1; j < m_nParticle && k < COLLISION_BUFFER; j++){
			if (isCollide(m_aParticle[i], m_aParticle[j])){
				m_aParticle[i].collide_with(m_aParticle[j]);

				// ���� ���õ� ��ü�� �����Ǿ����� �Ǵ�
				if (id == m_aParticle[i].getID() || id == m_aParticle[j].getID())
					rtn_id = m_aParticle[i].getID();

				// ���� �迭�� �ε��� �߰�
				dead_particle[k++] = j;
				new_number--;
			}
		}
	}
	// �浹�� ���� ���, ���� ID ��ȯ
	if (new_number == m_nParticle) return rtn_id;

	// �� �迭 ����
	CParticle * new_particle = new CParticle[new_number];

	for (unsigned int i = 0, j = 0, k = 0; i < m_nParticle; i++){
		if (i != dead_particle[k]){
			// ���� �迭�� �� �迭�� ����
			new_particle[j] = m_aParticle[i];
			new_particle[j++].setID(m_aParticle[i].getID());
		}
		else k++;
	}

	// ���� �迭 ���� �� �� �迭�� �⺻ �迭�� ����
	delete[] m_aParticle;
	m_aParticle = new_particle;
	m_nParticle = new_number;

	return rtn_id;
}

// ��ü P1�� ��ü P2 ���̿� �ۿ��ϴ� ���� ������ ����
void CEnvironment::add_force(CParticle * p1, CParticle * p2){
	// �� ���
	CVector dist = p1->get_pos() - p2->get_pos();
	double radius = p1->get_radius() + p2->get_radius();
	CVector force;

	if (dist.norm() > radius)
		force = GRAVITY_CONST * p1->get_mass() * p2->get_mass() / pow(dist.norm(), 3) * dist;
	else
		force = GRAVITY_CONST * p1->get_mass() * p2->get_mass() / pow(radius, 3) * dist;

	// ������ ��ü�� �� �߰�
	p1->add_force(-force);
	p2->add_force(force);
}

// ��ü �����̱�
void CEnvironment::move(){
	// ��Ȧ�� �����ϴ� ���, ��Ȧ�� �� �߰�
	if (m_bHoleExist){
		for (unsigned int i = 0; i < m_nParticle; i++){
			CVector dist = m_posHole - m_aParticle[i].get_pos();
			double dist_norm = max(pow(dist.norm(), 3), MINIMUM_DIST_NORM);
			CVector force = GRAVITY_CONST * 50 * m_aParticle[i].get_mass() / dist_norm * dist;

			m_aParticle[i].add_force(force);
		}
	}

	// ������ ��ü�� �� ���
	for (unsigned int i = 0; i < m_nParticle; i++)
		for (unsigned int j = i + 1; j < m_nParticle; j++)
			add_force(&m_aParticle[i], &m_aParticle[j]);

	// ��ü �����̱�
	for (unsigned int i = 0; i < m_nParticle; i++){
		m_aParticle[i].move(m_dt);

		// �� �ʱ�ȭ
		m_aParticle[i].reset_force();
	}
}

//
void CEnvironment::draw() const {

}

// CDC�� ��ü �׸���
void CEnvironment::onDraw(CDC * pDC, const WinSetting window) const {
	for (unsigned int i = 0; i < m_nParticle; i++)
		m_aParticle[i].onDraw(pDC, window);
}

//
CEnvironment::~CEnvironment(){
	delete m_e;
	delete[] m_aParticle;
}