
// Gravity_SimulatorDoc.cpp : implementation of the CGravity_SimulatorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gravity_Simulator.h"
#endif

#include "Gravity_SimulatorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGravity_SimulatorDoc

IMPLEMENT_DYNCREATE(CGravity_SimulatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CGravity_SimulatorDoc, CDocument)
END_MESSAGE_MAP()


// CGravity_SimulatorDoc construction/destruction

CGravity_SimulatorDoc::CGravity_SimulatorDoc()
{
	// TODO: add one-time construction code here

	// AfxMessageBox(_T("Initialize Global Time Object"));
	m_time.begin();

	// AfxMessageBox(_T("Create and Get Environment Object"));
	m_environment = CEnvironment::getEnvironment();

	// AfxMessageBox(_T("Initialize Particles"));
	m_environment->init_particles();
	m_bInitialized = TRUE;
}

CGravity_SimulatorDoc::~CGravity_SimulatorDoc()
{
	m_environment->~CEnvironment();
}

BOOL CGravity_SimulatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CGravity_SimulatorDoc serialization

void CGravity_SimulatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGravity_SimulatorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGravity_SimulatorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGravity_SimulatorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGravity_SimulatorDoc diagnostics

#ifdef _DEBUG
void CGravity_SimulatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGravity_SimulatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGravity_SimulatorDoc commands
