// DlgGraphSetup.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DlgGraphSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup dialog


CDlgGraphSetup::CDlgGraphSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGraphSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGraphSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgGraphSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGraphSetup)
	DDX_Control(pDX, IDC_LBL_LINE_COLOR, m_ctlColorLabel);
	DDX_Control(pDX, IDC_COMMONDIALOG, m_ctlCommonDialog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSetup)
	ON_BN_CLICKED(IDC_LBL_LINE_COLOR, OnLblLineColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup message handlers

void CDlgGraphSetup::OnLblLineColor() 
{
  m_ctlCommonDialog.ShowColor();
  unsigned long col = m_ctlCommonDialog.GetColor();
  unsigned short nRed   = ( unsigned short) ( col & 0xFF);
  unsigned short nGreen = ( unsigned short) (( col & 0xFF00) >> 8);
  unsigned short nBlue  = ( unsigned short) (( col & 0xFF0000) >> 16);	
  
  m_ctlColorLabel.SetBgColor( RGB( nRed, nGreen, nBlue));
  m_ctlColorLabel.Invalidate();
}

void CDlgGraphSetup::SetInitialColor(COLORREF clr)
{
  m_ctlColorLabel.SetBgColor( clr);
}

void CDlgGraphSetup::Init( int nGraph)
{
  m_nGraph = nGraph;

  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph, 0));
  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph, 2));
}

void CDlgGraphSetup::OnOK() 
{
  BOOL bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->GetCheck();
  theApp.GetSettings()->SetGraphSettings( m_nGraph, 0, bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MIN))->GetWindowText( str);
    theApp.GetSettings()->SetGraphSettings( m_nGraph, 1, atof( str));
  }

  bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->GetCheck();
  theApp.GetSettings()->SetGraphSettings( m_nGraph, 2, bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MAX))->GetWindowText( str);
    theApp.GetSettings()->SetGraphSettings( m_nGraph, 3, atof( str));
  }

	CDialog::OnOK();
}
