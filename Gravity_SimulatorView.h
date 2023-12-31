
// Gravity_SimulatorView.h : interface of the CGravity_SimulatorView class
//

#pragma once

#include "Header.h"

class CGravity_SimulatorView : public CView
{
protected: // create from serialization only
	CGravity_SimulatorView();
	DECLARE_DYNCREATE(CGravity_SimulatorView)

	WinSetting m_window;

	BITMAP m_bmpInfo;
	CBitmap m_bmp;
	CBitmap * m_pOldBmp = NULL;
	CDC m_memDC;

	bool m_bLButtonDown = FALSE;
	bool m_bRButtonDown = FALSE;
	bool m_bMButtonDown = FALSE;
	bool m_bMouseOverParticle = FALSE;
	bool m_bParticleSelected = FALSE;
	bool m_bSelectChanged = FALSE;

	CPoint m_posNewParticle;
	CPoint m_vecNewParticle;

	CPoint m_prevMButtonPos;
	CPoint m_destMButtonPos;

	CPoint m_point;
	CString m_str;

	int m_selectID = -1;
	CParticle * m_iParticle = NULL;

// Attributes
public:
	CGravity_SimulatorDoc* GetDocument() const;
	CVector frame2window(const CPoint) const;
	CPoint window2frame(const CVector) const;
	void MouseInteractionUpdate();
	void init_frame();

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CGravity_SimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFileNew();
};

#ifndef _DEBUG  // debug version in Gravity_SimulatorView.cpp
inline CGravity_SimulatorDoc* CGravity_SimulatorView::GetDocument() const
   { return reinterpret_cast<CGravity_SimulatorDoc*>(m_pDocument); }
#endif