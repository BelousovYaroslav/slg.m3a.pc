// BgColoredLabel.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "BgColoredLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBgColoredLabel

CBgColoredLabel::CBgColoredLabel()
{
  m_clr = RGB( 0, 0, 0);
}

CBgColoredLabel::~CBgColoredLabel()
{
}


BEGIN_MESSAGE_MAP(CBgColoredLabel, CStatic)
	//{{AFX_MSG_MAP(CBgColoredLabel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBgColoredLabel message handlers

void CBgColoredLabel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  CRect rc;
  GetClientRect( &rc);
  CBrush cBrush;
  cBrush.CreateSolidBrush( m_clr);
  dc.FillRect( &rc, &cBrush);
	// Do not call CStatic::OnPaint() for painting messages
}
