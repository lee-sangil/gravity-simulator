
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

// CPoint와 double의 곱하기 연산자
CPoint operator*(const CPoint p, const double f){
	CPoint result((int)(p.x * f), (int)(p.y * f));
	return result;
}

// CPoint와 double의 나누기 연산자
CPoint operator/(const CPoint p, const double f){
	CPoint result((int)(p.x / f), (int)(p.y / f));
	return result;
}

// CPoint에서 CVector로 변환하는 함수
CVector CGravity_SimulatorView::frame2window(const CPoint p) const {
	CVector result((p.x - m_window.Center.x) * m_window.Scale, (p.y - m_window.Center.y) * m_window.Scale);
	return result;
}

// CVector에서 CPoint로 변환하는 함수
CPoint CGravity_SimulatorView::window2frame(const CVector v) const {
	CPoint result((int) (v.get_x() / m_window.Scale + m_window.Center.x), (int) (v.get_y() / m_window.Scale + m_window.Center.y));
	return result;
}

// 프레임을 초기화하는 함수
void CGravity_SimulatorView::init_frame() {
	m_window.Center.SetPoint(SIZE_HORIZONTAL / 2, SIZE_VERTICAL / 2);
	m_window.Scale = 1;
}

// 마우스와 인터페이스 간의 상호 작용 업데이트
void CGravity_SimulatorView::MouseInteractionUpdate(){
	
	CGravity_SimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 그 위치에 물체가 있는지 판단한다
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

	// 초기화
	m_bmp.LoadBitmapW(IDB_BITMAP1);
	m_bmp.GetBitmap(&m_bmpInfo);

	m_memDC.CreateCompatibleDC(pDC);
	m_pOldBmp = m_memDC.SelectObject(&m_bmp);

	// 시뮬레이션 업데이트
	//
	// 물체 이동
	pDoc->move();
	// 시뮬레이션 시간 간격 설정
	pDoc->set_dt();

	// 충돌 후, 기존의 선택되었던 물체 재선택
	m_selectID = pDoc->collide_detect(m_selectID);
	m_iParticle = pDoc->getParticle(m_selectID);


	// 프레임 업데이트
	//

	// 마우스와 인터페이스 간의 상호 작용 업데이트
	MouseInteractionUpdate();

	// 선택된 물체를 중심으로 이동시키는 과정
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

	// DC 업데이트
	//
	// 백업 DC 업데이트
	if (pDoc->initialized()) pDoc->onDraw(&m_memDC, m_window);

	// 드래그 시, 파란선 그리기
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

	// CDC 업데이트
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

	// 마우스가 물체 위에 올려져있을 때
	if (m_bMouseOverParticle == TRUE){
		// 그 물체를 선택한다
		CParticle * temp_Particle = pDoc->getParticleAt(m_point, m_window);

		if (temp_Particle != NULL){
			// 기존의 것과 다른 물체를 선택한 경우, 바뀌었다고 알린다
			if (temp_Particle != m_iParticle)
				m_bSelectChanged = TRUE;

			// 선택된 경우, ID를 설정한다
			m_bParticleSelected = TRUE;
			m_iParticle = temp_Particle;
			m_selectID = temp_Particle->getID();
		}
	}
	// 마우스가 외부에 있을 경우, 새로운 물체를 만들 준비를 한다
	else {
		CClientDC dc(this);
		dc.MoveTo(point);
		m_posNewParticle = point;
		m_vecNewParticle = point;

		// 드래그 시, 선을 그을 준비를 한다
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

	// 마우스가 물체 외부에 있는 경우
	if (m_bMouseOverParticle == FALSE){
		// 물체는 선택되어 있고, 외부에 단순 클릭 => 물체 선택 해제
		if (m_posNewParticle == point && m_bParticleSelected == TRUE){
			m_iParticle = NULL;
			m_selectID = -1;
			m_bParticleSelected = FALSE;
		}
		else if (m_bLButtonDown == TRUE) {
			// 새로운 물체 위치 계산
			CVector pos = frame2window(m_posNewParticle);

			// 새로운 물체 속도 계산
			CPoint add_particle_vel = m_posNewParticle - point;
			CVector vel(add_particle_vel.x, add_particle_vel.y);
			if (m_bParticleSelected == TRUE)
				vel = vel * NEW_VEL_SCALE * m_window.Scale + m_iParticle->get_vel();
			else
				vel = vel * NEW_VEL_SCALE * m_window.Scale;

			// 새로운 물체 정의 및 추가
			CParticle p(m_window.Scale, pos, vel);

			if (m_bParticleSelected == TRUE && m_iParticle != NULL){
				// 기존 선택된 물체의 ID 임시 저장
				int ID = m_iParticle->getID();

				// 물체 추가
				pDoc->add_particle(p);

				// 재정렬된 물체 재선택
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

	// 마우스 포인터 위치 저장
	m_point = point;

	// 마우스와 인터페이스 간의 상호 작용 업데이트
	MouseInteractionUpdate();

	// 왼쪽 버튼 드래그 => 물체 속도 그리기
	if (m_bLButtonDown == TRUE){
		m_vecNewParticle = m_point;
	}

	// 오른쪽 버튼 드래그 => 블랙홀 위치 이동
	if (m_bRButtonDown == TRUE){
		CVector pos = frame2window(m_point);
		pDoc->create_blackHole(pos);
	}

	// 가운데 버튼 드래그 => 프레임 이동
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

	// 블랙홀 위치 계산
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

	// 블랙홀 소멸
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

	// 프레임 스케일 설정
	if (zDelta > 0)
		new_viewScale = m_window.Scale / (1 + 0.1 * zDelta / 120);
	else
		new_viewScale = m_window.Scale * (1 + 0.1 * -zDelta / 120);

	if (new_viewScale < 0.2) new_viewScale = 0.2;

	// 중심 위치 지정 및 중심 위치를 기준으로 프레임 이동
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

	// 프레임 초기화
	init_frame();
	pDoc->init_particles();

	// 프레임 내부 변수 초기화
	m_selectID = -1;
	m_iParticle = NULL;

	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;
	m_bMButtonDown = FALSE;
	m_bMouseOverParticle = FALSE;
	m_bParticleSelected = FALSE;
	m_bSelectChanged = FALSE;
}
