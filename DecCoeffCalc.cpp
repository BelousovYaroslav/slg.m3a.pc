// DecCoeffCalc.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "DecCoeffCalc.h"
#include "DecCoeffCalcParams.h"
#include "MainFrm.h"
#include "MainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDecCoeffCalcParams gl_pDecCoeffCalcParams;

extern bool PutByteInCircleBuffer(BYTE bt);

#define MY_REFRESH_TIMER 100

/////////////////////////////////////////////////////////////////////////////
// CDecCoeffCalc dialog


CDecCoeffCalc::CDecCoeffCalc(CWnd* pParent /*=NULL*/)
	: CDialog(CDecCoeffCalc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDecCoeffCalc)
	m_strDecCoeff = _T("");
	//}}AFX_DATA_INIT
}


void CDecCoeffCalc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDecCoeffCalc)
	DDX_Control(pDX, IDC_CWG_DN, m_ctlGraphDn);
	DDX_Control(pDX, IDC_CWG_DU, m_ctlGraphDu);
	DDX_Control(pDX, IDC_CWG_K_DEC, m_ctlGraphDecCoeff);
	DDX_Text(pDX, IDC_LBL_DEC_COEFF, m_strDecCoeff);
	DDX_Control(pDX, IDC_CWE_DU_LOW_EDGE, m_ctlNedtLowFilter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDecCoeffCalc, CDialog)
	//{{AFX_MSG_MAP(CDecCoeffCalc)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDecCoeffCalc message handlers

BOOL CDecCoeffCalc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	gl_pDecCoeffCalcParams.Reset();
	
	m_ctlNedtLowFilter.SetValue( gl_pDecCoeffCalcParams.m_dbl_du_low_filter);

	SetTimer( MY_REFRESH_TIMER, 100, NULL);
	
  //m_ctlGraphDecCoeff.GetAxes().Item( "YAxis-1").SetMinimum( 0.0325);
  //m_ctlGraphDecCoeff.GetAxes().Item( "YAxis-1").SetMaximum( 0.0327);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDecCoeffCalc::OnOK() 
{
  gl_pDecCoeffCalcParams.m_bDecCoeffCalculation = false;
	CDialog::OnOK();
}

void CDecCoeffCalc::OnTimer(UINT nIDEvent) 
{
	CSlg2App *app = (CSlg2App*) AfxGetApp();

	if( nIDEvent == MY_REFRESH_TIMER) {
		
		if( gl_pDecCoeffCalcParams.m_bGraph_overround) {
			
			CNiReal64Matrix line_dN( 2, CYCLE_BUFFER_DN_DU, 1.0);
			CNiReal64Matrix line_dU( 2, CYCLE_BUFFER_DN_DU, 1.0);
			CNiReal64Matrix line_K(  2, CYCLE_BUFFER_DN_DU, 1.0);

			for( int i=0; i < CYCLE_BUFFER_DN_DU; i++) {
				line_dN( 0, i) = i;
				line_dN( 1, i) = gl_pDecCoeffCalcParams.m_dbl_dN[ ( gl_pDecCoeffCalcParams.m_nGraph_cntr + i) % CYCLE_BUFFER_DN_DU];

				line_dU( 0, i) = i;
				line_dU( 1, i) = gl_pDecCoeffCalcParams.m_dbl_dU[ ( gl_pDecCoeffCalcParams.m_nGraph_cntr + i) % CYCLE_BUFFER_DN_DU];

				line_K( 0, i) = i;
				line_K( 1, i) = gl_pDecCoeffCalcParams.m_dbl_kDec[ ( gl_pDecCoeffCalcParams.m_nGraph_cntr + i) % CYCLE_BUFFER_DN_DU];
			}

			m_ctlGraphDn.PlotXY( line_dN, true);
			m_ctlGraphDu.PlotXY( line_dU, true);
			m_ctlGraphDecCoeff.PlotXY( line_K, true);
		}
		else {
			if( gl_pDecCoeffCalcParams.m_nGraph_cntr > 5) {
				CNiReal64Matrix line_dN( 2, gl_pDecCoeffCalcParams.m_nGraph_cntr - 1, 1.0);
				CNiReal64Matrix line_dU( 2, gl_pDecCoeffCalcParams.m_nGraph_cntr - 1, 1.0);
				CNiReal64Matrix line_K(  2, gl_pDecCoeffCalcParams.m_nGraph_cntr - 1, 1.0);

				for( int i = 0; i < gl_pDecCoeffCalcParams.m_nGraph_cntr - 1; i++) {
					line_dN( 0, i) = i;
					line_dN( 1, i) = gl_pDecCoeffCalcParams.m_dbl_dN[ i];

					line_dU( 0, i) = i;
					line_dU( 1, i) = gl_pDecCoeffCalcParams.m_dbl_dU[ i];

					line_K( 0, i) = i;
					line_K( 1, i) = gl_pDecCoeffCalcParams.m_dbl_kDec[ i];
				}

				m_ctlGraphDn.PlotXY( line_dN, true);
				m_ctlGraphDu.PlotXY( line_dU, true);
				m_ctlGraphDecCoeff.PlotXY( line_K, true);

				//Beep( 100, 10);
			}
		}

		//m_strDecCoeff.Format( _T("%.6f"), gl_dec_coeff_acc / gl_dec_coeff_acc_cntr);
		m_strDecCoeff.Format( _T("%.10f"), gl_pDecCoeffCalcParams.m_dbl_dN_acc / gl_pDecCoeffCalcParams.m_dbl_dU_acc);
		UpdateData( false);
	}

	CDialog::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CDecCoeffCalc, CDialog)
    //{{AFX_EVENTSINK_MAP(CDecCoeffCalc)
	ON_EVENT(CDecCoeffCalc, IDC_CWE_DU_LOW_EDGE, 1 /* ValueChanged */, OnValueChangedCweDuLowEdge, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	ON_EVENT(CDecCoeffCalc, IDC_CWE_DW_HIGH_EDGE, 1 /* ValueChanged */, OnValueChangedCweDwHighEdge, VTS_PVARIANT VTS_PVARIANT VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDecCoeffCalc::OnValueChangedCweDuLowEdge( VARIANT FAR *Value, VARIANT FAR *PreviousValue, BOOL OutOfRange) 
{
	gl_pDecCoeffCalcParams.m_dbl_du_low_filter = m_ctlNedtLowFilter.GetValue();
}

void CDecCoeffCalc::OnValueChangedCweDwHighEdge(VARIANT FAR* Value, VARIANT FAR* PreviousValue, BOOL OutOfRange) 
{
	gl_pDecCoeffCalcParams.m_dbl_dw_high_filter = m_ctlNedtLowFilter.GetValue();
}

void CDecCoeffCalc::OnBtnReset() 
{
  gl_pDecCoeffCalcParams.Reset();
}

void CDecCoeffCalc::OnBtnSave() 
{
  CMainFrame *pFrame = ( CMainFrame *) theApp.m_pMainWnd;
  CMainView *pView = ( CMainView *) pFrame->GetActiveView();
  pView->m_ctlNedtParam8.SetValue( gl_pDecCoeffCalcParams.m_dbl_dN_acc / gl_pDecCoeffCalcParams.m_dbl_dU_acc);
  gl_pDecCoeffCalcParams.m_bDecCoeffCalculation = false;
	CDialog::OnOK();
}
