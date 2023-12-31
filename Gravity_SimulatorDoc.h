
// Gravity_SimulatorDoc.h : interface of the CGravity_SimulatorDoc class
//


#pragma once

#include "Environment.h"
#include "Time.h"

class CGravity_SimulatorDoc : public CDocument
{
protected: // create from serialization only
	CGravity_SimulatorDoc();
	DECLARE_DYNCREATE(CGravity_SimulatorDoc)

// Attributes
public:
	bool initialized(){ return m_bInitialized; }
	void create_blackHole(CVector pos){ m_environment->create_blackHole(pos); }
	void destroy_blackHole(){ m_environment->destroy_blackHole(); }
	void play(){ m_environment->play(); }
	double get_dt() const { return m_environment->get_dt(); }
	CParticle * getParticle(const int id) const { return m_environment->getParticle(id); }
	CParticle * getParticleAt(const CPoint point, const WinSetting window) const { return m_environment->getParticleAt(point, window); }
	void disableSelected() { m_environment->disableSelected(); }
	void setID(CParticle & p){ m_environment->setID(p); }
	void set_dt(){ double dt = m_time.passed(); m_environment->set_dt(dt); }
	void init_particles(){ m_environment->init_particles(); }
	void add_particle(CParticle p){ m_environment->add_particle(p); }
	bool isCollide(CParticle p1, CParticle p2) const { m_environment->isCollide(p1, p2); }
	int collide_detect(const int id){ return m_environment->collide_detect(id); }
	void add_force(CParticle * p1, CParticle * p2){ m_environment->add_force(p1, p2); }
	void move(){ m_environment->move(); }
	void draw() const { m_environment->draw(); }
	void onDraw(CDC * pDC, const WinSetting window) const { m_environment->onDraw(pDC, window); }

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGravity_SimulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CEnvironment * m_environment;
	CGlobalTime m_time;
	bool m_bInitialized;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
