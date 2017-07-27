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
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGraphSetup, CDialog)
	//{{AFX_MSG_MAP(CDlgGraphSetup)
	ON_EN_CHANGE(IDC_EDT_R, OnChangeEdtR)
	ON_EN_CHANGE(IDC_EDT_G, OnChangeEdtG)
	ON_EN_CHANGE(IDC_EDT_B, OnChangeEdtB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup message handlers

void CDlgGraphSetup::Init( int nGraph)
{
  CString str;

  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: in");

  m_nGraph = nGraph;

  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: ymin 1");
  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph)->Get_bMinY());

  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: ymin 2");
  str.Format( _T("%.03f"), theApp.GetSettings()->GetGraphSettings( nGraph)->Get_dblMinY());
  CWnd *wn = ( ( CWnd *) GetDlgItem( IDC_EDT_Y_MIN));
  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: wn = 0x%X", wn);
  wn->SetWindowText( str);

  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: ymax 1");
  ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->SetCheck( theApp.GetSettings()->GetGraphSettings( nGraph)->Get_bMaxY());

  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: ymax 2");
  str.Format( _T("%.03f"), theApp.GetSettings()->GetGraphSettings( nGraph)->Get_dblMaxY());
  ( ( CWnd *) GetDlgItem( IDC_EDT_Y_MAX))->SetWindowText( str);


  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: color");
  m_ctlColorLabel.SetBgColor( theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor());
  
  str.Format( _T("%d"), theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor() & 0xFF);
  ( ( CWnd *) GetDlgItem( IDC_EDT_R))->SetWindowText( str);

  str.Format( _T("%d"), ( theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor() & 0xFF00) >> 8);
  ( ( CWnd *) GetDlgItem( IDC_EDT_G))->SetWindowText( str);

  str.Format( _T("%d"), ( theApp.GetSettings()->GetGraphSettings( nGraph)->GetLineColor() & 0xFF0000) >> 16);
  ( ( CWnd *) GetDlgItem( IDC_EDT_B))->SetWindowText( str);


  theApp.GetLogger()->LogTrace( "CDlgGraphSetup::Init: out");
}

void CDlgGraphSetup::OnOK() 
{
  BOOL bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MIN))->GetCheck();
  theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_bMinY( bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MIN))->GetWindowText( str);
    theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_dblMinY( atof( str));
  }
  
  bCheck = ( ( CButton *) GetDlgItem( IDC_CHK_Y_MAX))->GetCheck();
  theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_bMaxY( bCheck);
  if( bCheck) {
    CString str;
    ( ( CEdit *) GetDlgItem( IDC_EDT_Y_MAX))->GetWindowText( str);
    theApp.GetSettings()->GetGraphSettings( m_nGraph)->Set_dblMaxY( atof( str));
  }

  theApp.GetSettings()->GetGraphSettings( m_nGraph)->SetLineColor( m_ctlColorLabel.GetBgColor());

	CDialog::OnOK();
}

void CDlgGraphSetup::OnChangeEdtR() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	ColorChanged();
}

void CDlgGraphSetup::OnChangeEdtG() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	ColorChanged();
	
}

void CDlgGraphSetup::OnChangeEdtB() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	ColorChanged();	
}

void CDlgGraphSetup::ColorChanged()
{
  CString tmp;
  UINT r, g, b;
	
  ( ( CWnd *) GetDlgItem( IDC_EDT_R))->GetWindowText( tmp);
  r = atoi( tmp);

  ( ( CWnd *) GetDlgItem( IDC_EDT_G))->GetWindowText( tmp);
  g = atoi( tmp);

  ( ( CWnd *) GetDlgItem( IDC_EDT_B))->GetWindowText( tmp);
  b = atoi( tmp);
	
  m_ctlColorLabel.SetBgColor( RGB( r, g, b));
  m_ctlColorLabel.Invalidate();
}
