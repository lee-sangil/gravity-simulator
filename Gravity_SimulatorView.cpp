
// Gravity_SimulatorView.cpp : implementation of the CGravity_SimulatorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gravity_Simulator.h"
#endif

#include "Gravity_SimulatorDoc.h"
#include "Gravity_SimulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGravity_SimulatorView

IMPLEMENT_DYNCREATE(CGravity_SimulatorView, CView)

BEGIN_MESSAGE_MAP(CGravity_SimulatorView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_FILE_NEW, &CGravity_SimulatorView::OnFileNew)
END_MESSAGE_MAP()

// CGravity_SimulatorView construction/destruction

CGravity_SimulatorView::CGravity_SimulatorView()
{
	// TODO: add construction code here
	init_frame();
}

CGravity_SimulatorView::~CGravity_SimulatorView()
{
}

// CPoint�� double�� ���ϱ� ������
CPoint operator*(const CPoint p, const double f){
	CPoint result((int)(p.x * f), (int)(p.y * f));
	return result;
}

// CPoint�� double�� ������ ������
CPoint operator/(const CPoint p, const double f){
	CPoint result((int)(p.x / f), (int)(p.y / f));
	return result;
}

// CPoint���� CVector�� ��ȯ�ϴ� �Լ�
CVector CGravity_SimulatorView::frame2window(const CPoint p) const {
	CVector result((p.x - m_window.Center.x) * m_window.Scale, (p.y - m_window.Center.y) * m_window.Scale);
	return result;
}

// CVector���� CPoint�� ��ȯ�ϴ� �Լ�
CPoint CGravity_SimulatorView::window2frame(const CVector v) const {
	CPoint result((int) (v.get_x() / m_window.Scale + m_window.Center.x), (int) (v.get_y() / m_window.Scale + m_window.Center.y));
	return result;
}

// �������� �ʱ�ȭ�ϴ� �Լ�
void CGravity_SimulatorView::init_frame() {
	m_window.Center.SetPoint(SIZE_HORIZONTAL / 2, SIZE_VERTICAL / 2);
	m_window.Scale = 1;
}

// ���콺�� �������̽� ���� ��ȣ �ۿ� ������Ʈ
void CGravity_SimulatorView::MouseInteractionUpdate(){
	
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// �� ��ġ�� ��ü�� �ִ��� �Ǵ��Ѵ�
	CParticle * temp_iParticle = pDoc->getParticleAt(m_point, m_window);
	if (temp_iParticle != NULL){
		m_bMouseOverParticle = TRUE;
		temp_iParticle->selected();
		SetCursor(LoadCursor(NULL, IDC_HAND));
	}
	else{
		m_bMouseOverParticle = FALSE;
		pDoc->disableSelected();
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}
}


BOOL CGravity_SimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGravity_SimulatorView drawing

void CGravity_SimulatorView::OnDraw(CDC* pDC)
{
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	// �ʱ�ȭ
	m_bmp.LoadBitmapW(IDB_BITMAP1);
	m_bmp.GetBitmap(&m_bmpInfo);

	m_memDC.CreateCompatibleDC(pDC);
	m_pOldBmp = m_memDC.SelectObject(&m_bmp);

	// �ùķ��̼� ������Ʈ
	//
	// ��ü �̵�
	pDoc->move();
	// �ùķ��̼� �ð� ���� ����
	pDoc->set_dt();

	// �浹 ��, ������ ���õǾ��� ��ü �缱��
	m_selectID = pDoc->collide_detect(m_selectID);
	m_iParticle = pDoc->getParticle(m_selectID);


	// ������ ������Ʈ
	//

	// ���콺�� �������̽� ���� ��ȣ �ۿ� ������Ʈ
	MouseInteractionUpdate();

	// ���õ� ��ü�� �߽����� �̵���Ű�� ����
	static double slope = 0;
	if (m_bSelectChanged == TRUE) {
		slope = 0;
		m_bSelectChanged = FALSE;
	}
	if (m_bParticleSelected == TRUE){
		CPoint frame_center(SIZE_HORIZONTAL / 2, SIZE_VERTICAL * 2 / 5);
		CPoint new_center = window2frame(m_iParticle->get_pos());
		m_window.Center += (frame_center - new_center) * slope;

		if (slope < 1) slope += 0.01;
		else slope = 1;
	}
	else slope = 0;

	// DC ������Ʈ
	//
	// ��� DC ������Ʈ
	if (pDoc->initialized()) pDoc->onDraw(&m_memDC, m_window);

	// �巡�� ��, �Ķ��� �׸���
	if (m_bLButtonDown == TRUE){
		CPen pen;
		CPen * pOldObj;
		pen.CreatePen(PS_SOLID, 2, RGB(20, 40, 80));
		pOldObj = m_memDC.SelectObject(&pen);

		m_memDC.MoveTo(m_posNewParticle);
		m_memDC.LineTo(m_vecNewParticle);
		m_memDC.SelectObject(pOldObj);
		DeleteObject(&pen);
	}

	//// Debugging
	//if (m_iParticle != NULL){
	//	//m_str.Format(_T("Selected Particle: mass: %.2f, pos_x: %.2f, pos_y: %.2f"), m_iParticle->get_mass(), m_iParticle->get_pos().get_x(), m_iParticle->get_pos().get_y());
	//	m_str.Format(_T("MouseOver: %d, Selected: %d, Particle ID: %d, center: [%d, %d]"), m_bMouseOverParticle == TRUE, m_bParticleSelected == TRUE, m_selectID, m_window.Center.x, m_window.Center.y);
	//	//m_str.Format(_T("POINT: [%d, %d], CENTER: [%d, %d], ZOOM: %.4f, FPS: %.4f"), m_point.x, m_point.y, m_window.Center.x, m_window.Center.y, m_window.Scale, pDoc->get_dt());
	//	m_memDC.TextOutW(200, 0, m_str);
	//}

	//CBrush null_brush;
	//CBrush * pOldBrush;

	//null_brush.CreateStockObject(NULL_BRUSH);
	//pOldBrush = m_memDC.SelectObject(&null_brush);
	//m_memDC.Ellipse(m_window.Center.x - 5, m_window.Center.y - 5, m_window.Center.x + 5, m_window.Center.y + 5);
	//m_memDC.SelectObject(pOldBrush);

	// CDC ������Ʈ
	pDC->BitBlt(0, 0, m_bmpInfo.bmWidth, m_bmpInfo.bmHeight, &m_memDC, 0, 0, SRCCOPY);

	m_memDC.SelectObject(m_pOldBmp);
	m_memDC.DeleteDC();
	m_bmp.DeleteObject();
}


// CGravity_SimulatorView diagnostics


#ifdef _DEBUG
void CGravity_SimulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CGravity_SimulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGravity_SimulatorDoc* CGravity_SimulatorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGravity_SimulatorDoc)));
	return (CGravity_SimulatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGravity_SimulatorView message handlers


int CGravity_SimulatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(IDT_DRAW, 25, NULL);
	return 0;
}


void CGravity_SimulatorView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(IDT_DRAW);
}


void CGravity_SimulatorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CWinApp * h = AfxGetApp();

	switch (nIDEvent){
	case IDT_DRAW:
		Invalidate(FALSE);
		//h->OnIdle(1);
		break;
	default:
		Invalidate(FALSE);
		break;
	}

	CView::OnTimer(nIDEvent);
}


void CGravity_SimulatorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ���콺�� ��ü ���� �÷������� ��
	if (m_bMouseOverParticle == TRUE){
		// �� ��ü�� �����Ѵ�
		CParticle * temp_Particle = pDoc->getParticleAt(m_point, m_window);

		if (temp_Particle != NULL){
			// ������ �Ͱ� �ٸ� ��ü�� ������ ���, �ٲ���ٰ� �˸���
			if (temp_Particle != m_iParticle)
				m_bSelectChanged = TRUE;

			// ���õ� ���, ID�� �����Ѵ�
			m_bParticleSelected = TRUE;
			m_iParticle = temp_Particle;
			m_selectID = temp_Particle->getID();
		}
	}
	// ���콺�� �ܺο� ���� ���, ���ο� ��ü�� ���� �غ� �Ѵ�
	else {
		CClientDC dc(this);
		dc.MoveTo(point);
		m_posNewParticle = point;
		m_vecNewParticle = point;

		// �巡�� ��, ���� ���� �غ� �Ѵ�
		m_bLButtonDown = TRUE;
	}

	CView::OnLButtonDown(nFlags, point);
}


void CGravity_SimulatorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ���콺�� ��ü �ܺο� �ִ� ���
	if (m_bMouseOverParticle == FALSE){
		// ��ü�� ���õǾ� �ְ�, �ܺο� �ܼ� Ŭ�� => ��ü ���� ����
		if (m_posNewParticle == point && m_bParticleSelected == TRUE){
			m_iParticle = NULL;
			m_selectID = -1;
			m_bParticleSelected = FALSE;
		}
		else if (m_bLButtonDown == TRUE) {
			// ���ο� ��ü ��ġ ���
			CVector pos = frame2window(m_posNewParticle);

			// ���ο� ��ü �ӵ� ���
			CPoint add_particle_vel = m_posNewParticle - point;
			CVector vel(add_particle_vel.x, add_particle_vel.y);
			if (m_bParticleSelected == TRUE)
				vel = vel * NEW_VEL_SCALE * m_window.Scale + m_iParticle->get_vel();
			else
				vel = vel * NEW_VEL_SCALE * m_window.Scale;

			// ���ο� ��ü ���� �� �߰�
			CParticle p(m_window.Scale, pos, vel);

			if (m_bParticleSelected == TRUE && m_iParticle != NULL){
				// ���� ���õ� ��ü�� ID �ӽ� ����
				int ID = m_iParticle->getID();

				// ��ü �߰�
				pDoc->add_particle(p);

				// �����ĵ� ��ü �缱��
				m_selectID = ID;
				m_iParticle = pDoc->getParticle(m_selectID);
			}
			else{
				pDoc->add_particle(p);
			}
		}
	}

	m_bLButtonDown = FALSE;
	CView::OnLButtonUp(nFlags, point);
}


void CGravity_SimulatorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ���콺 ������ ��ġ ����
	m_point = point;

	// ���콺�� �������̽� ���� ��ȣ �ۿ� ������Ʈ
	MouseInteractionUpdate();

	// ���� ��ư �巡�� => ��ü �ӵ� �׸���
	if (m_bLButtonDown == TRUE){
		m_vecNewParticle = m_point;
	}

	// ������ ��ư �巡�� => ��Ȧ ��ġ �̵�
	if (m_bRButtonDown == TRUE){
		CVector pos = frame2window(m_point);
		pDoc->create_blackHole(pos);
	}

	// ��� ��ư �巡�� => ������ �̵�
	if (m_bMButtonDown == TRUE){
		m_window.Center += m_point - m_prevMButtonPos;
		m_prevMButtonPos = m_point;
	}

	CView::OnMouseMove(nFlags, point);
}


void CGravity_SimulatorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ȧ ��ġ ���
	CVector pos = frame2window(point);
	pDoc->create_blackHole(pos);

	m_bRButtonDown = TRUE;
	CView::OnRButtonDown(nFlags, point);
}


void CGravity_SimulatorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ȧ �Ҹ�
	pDoc->destroy_blackHole();
	
	m_bRButtonDown = FALSE;
	CView::OnRButtonUp(nFlags, point);
}


void CGravity_SimulatorView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_prevMButtonPos = point;
	
	m_bMButtonDown = TRUE;
	CView::OnMButtonDown(nFlags, point);
}


void CGravity_SimulatorView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMButtonDown = FALSE;
	CView::OnMButtonUp(nFlags, point);
}


BOOL CGravity_SimulatorView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	double prev_viewScale = m_window.Scale;
	double new_viewScale;

	// ������ ������ ����
	if (zDelta > 0)
		new_viewScale = m_window.Scale / (1 + 0.1 * zDelta / 120);
	else
		new_viewScale = m_window.Scale * (1 + 0.1 * -zDelta / 120);

	if (new_viewScale < 0.2) new_viewScale = 0.2;

	// �߽� ��ġ ���� �� �߽� ��ġ�� �������� ������ �̵�
	if (m_bParticleSelected == TRUE){
		CPoint point = window2frame(m_iParticle->get_pos());
		m_window.Center -= (point - m_window.Center) * (prev_viewScale - new_viewScale) / new_viewScale;
	}
	else{
		m_window.Center -= (m_point - m_window.Center) * (prev_viewScale - new_viewScale) / new_viewScale;
	}
	m_window.Scale = new_viewScale;

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGravity_SimulatorView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = SIZE_HORIZONTAL;
	lpMMI->ptMinTrackSize.y = SIZE_VERTICAL;
	lpMMI->ptMaxTrackSize.x = SIZE_HORIZONTAL;
	lpMMI->ptMaxTrackSize.y = SIZE_VERTICAL;

	CView::OnGetMinMaxInfo(lpMMI);
}


void CGravity_SimulatorView::OnFileNew()
{
	// TODO: Add your command handler code here
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ������ �ʱ�ȭ
	init_frame();
	pDoc->init_particles();

	// ������ ���� ���� �ʱ�ȭ
	m_selectID = -1;
	m_iParticle = NULL;

	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;
	m_bMButtonDown = FALSE;
	m_bMouseOverParticle = FALSE;
	m_bParticleSelected = FALSE;
	m_bSelectChanged = FALSE;
}
