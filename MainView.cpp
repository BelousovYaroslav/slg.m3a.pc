// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "MainView.h"
#include "MainFrm.h"
#include "SlgNewAverager.h"
#include "SlgGroupNewAverager.h"
#include "DecCoeffCalc.h"
#include "McCommands.h"
#include "AnalogueParamsConstList.h"
#include "DecCoeffCalcParams.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MY_TIMER_1000			1101

#define MY_TIMER_COM_FLUSH		1001
#define MY_MAXIMIZE_VIEW_TIMER 1002
#define MY_SEND_BUTTONS_BLOCK_TIMER 1003
#define MY_TIMER_LOAD_FLASH_PARAMS 1004
#define MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS 1005
#define MY_TIMER_SWITCH_AS 1110
#define MY_TIMER_INPUT_DATA 1111
#define TIMER_SEND_CMDS_TO_MC 1112
#define MY_TIMER_POLLER 1113

extern BOOL m_bStopBigThreadFlag;
extern int gl_nCircleBufferGet;
extern int gl_nCircleBufferPut;

extern bool PutByteInCircleBuffer(BYTE bt);

extern double gl_dGlobalTime;
extern CSlgGroupNewAverager gl_avgW;
extern CSlgGroupNewAverager gl_avgI1;
extern CSlgGroupNewAverager gl_avgI2;
extern CSlgGroupNewAverager gl_avgVpc;
extern CSlgGroupNewAverager gl_avgAmplAlt;
extern CSlgGroupNewAverager gl_avgAmplDus;
extern CSlgGroupNewAverager gl_avgAmplRULA;
extern CSlgGroupNewAverager gl_avgT1;
extern CSlgGroupNewAverager gl_avgT2;
extern CSlgGroupNewAverager gl_avgTsa;

extern CDecCoeffCalcParams gl_pDecCoeffCalcParams;

/////////////////////////////////////////////////////////////////////////////
// CMainView

IMPLEMENT_DYNCREATE(CMainView, CFormView)

CMainView::CMainView()
	: CFormView(CMainView::IDD)
{
	//{{AFX_DATA_INIT(CMainView)
	m_bBtnCwStart = FALSE;
	m_nComPort = 0;
	m_strSmGr1_max = _T("");
	m_strSmGr1_mean = _T("");
	m_strSmGr1_min = _T("");
	m_strSmGr1_rms = _T("");
	m_strSmGr2_max = _T("");
	m_strSmGr2_mean = _T("");
	m_strSmGr2_min = _T("");
	m_strSmGr2_rms = _T("");
	m_strSmGr3_max = _T("");
	m_strSmGr3_mean = _T("");
	m_strSmGr3_min = _T("");
	m_strSmGr3_rms = _T("");
	m_strSmGr4_max = _T("");
	m_strSmGr4_mean = _T("");
	m_strSmGr4_min = _T("");
	m_strSmGr4_rms = _T("");
	m_strSmGr5_max = _T("");
	m_strSmGr5_mean = _T("");
	m_strSmGr5_min = _T("");
	m_strSmGr5_rms = _T("");
	m_strSmGr6_max = _T("");
	m_strSmGr6_mean = _T("");
	m_strSmGr6_min = _T("");
	m_strSmGr6_rms = _T("");
	m_strSmGr7_max = _T("");
	m_strSmGr7_mean = _T("");
	m_strSmGr7_min = _T("");
	m_strSmGr7_rms = _T("");
	m_strSmGr8_max = _T("");
	m_strSmGr8_mean = _T("");
	m_strSmGr8_min = _T("");
	m_strSmGr8_rms = _T("");
	m_strLblEmergency = _T("");
	m_strParam1Val = _T("");
	m_strParam2Val = _T("");
	m_strParam3Val = _T("");
	m_strParam4Val = _T("");
	m_strParam8Val = _T("");
	m_nTMeaningInd = 0;
	m_dKimpSec = 0.0;
	m_strSoftwareVersion = _T("");
	m_nComPortBaudrate = 1;
	m_strThermoCalib_T1 = _T("");
	m_strThermoCalib_T2 = _T("");
	m_strMarkerFails = _T("");
	m_strCheckSummFails = _T("");
	m_strCounterFails = _T("");
	m_nRadGraph8 = 0;
	m_nRadGraph7 = 0;
	m_nRadGraph6 = 0;
	m_nRadGraph5 = 0;
	m_nRadGraph4 = 0;
  m_nRadGraph3 = 0;
  m_nRadGraph2 = 0;
	m_nRadGraph1 = 0;
	//}}AFX_DATA_INIT
	m_nCounterSkippedPoints = 0;
	m_nPointsSkipped = 0;	
	m_bEmergencyCodeApperance = false;
  m_dlgDecCoeffCalc = NULL;
  m_dlgGraphSetup = NULL;
  m_nPollParams[ 0] = SIGNCOEFF;
  m_nPollParams[ 1] = AMPLITUDE;
  m_nPollParams[ 2] = TACT_CODE;
  m_nPollParams[ 3] = M_COEFF;
  m_nPollParams[ 4] = STARTMODE;
  m_nPollParams[ 5] = DECCOEFF;  
  m_nPollParams[ 6] = VERSION;
}

CMainView::~CMainView()
{
  if( m_dlgDecCoeffCalc != NULL) {
    delete m_dlgDecCoeffCalc;
    m_dlgDecCoeffCalc = NULL;
  }

  if( m_dlgGraphSetup != NULL) {
    delete m_dlgGraphSetup;
    m_dlgGraphSetup = NULL;
  }
}

void CMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainView)
	DDX_Control(pDX, IDC_GRAPH1, m_ctlSmallGraph1);
	DDX_Control(pDX, IDC_GRAPH2, m_ctlSmallGraph2);
	DDX_Control(pDX, IDC_GRAPH3, m_ctlSmallGraph3);
	DDX_Control(pDX, IDC_GRAPH4, m_ctlSmallGraph4);
	DDX_Control(pDX, IDC_GRAPH5, m_ctlSmallGraph5);
	DDX_Control(pDX, IDC_GRAPH6, m_ctlSmallGraph6);
	DDX_Control(pDX, IDC_GRAPH7, m_ctlSmallGraph7);
	DDX_Control(pDX, IDC_GRAPH8, m_ctlSmallGraph8);
	DDX_Control(pDX, IDC_MAIN_GRAPH, m_ctlMainGraph);
	DDX_Control(pDX, IDC_INDICATOR_COM, m_ctlComButton);
	DDX_OCBool(pDX, IDC_CW_START, DISPID(2), m_bBtnCwStart);
	DDX_CBIndex(pDX, IDC_CMB_COM_PORT, m_nComPort);
	DDX_Text(pDX, IDC_GRAPH1_MAXVAL_LABEL, m_strSmGr1_max);
	DDX_Text(pDX, IDC_GRAPH1_MEANVAL_LABEL, m_strSmGr1_mean);
	DDX_Text(pDX, IDC_GRAPH1_MINVAL_LABEL, m_strSmGr1_min);
	DDX_Text(pDX, IDC_GRAPH1_RMSVAL_LABEL, m_strSmGr1_rms);
	DDX_Text(pDX, IDC_GRAPH2_MAXVAL_LABEL, m_strSmGr2_max);
	DDX_Text(pDX, IDC_GRAPH2_MEANVAL_LABEL, m_strSmGr2_mean);
	DDX_Text(pDX, IDC_GRAPH2_MINVAL_LABEL, m_strSmGr2_min);
	DDX_Text(pDX, IDC_GRAPH2_RMSVAL_LABEL, m_strSmGr2_rms);
	DDX_Text(pDX, IDC_GRAPH3_MAXVAL_LABEL, m_strSmGr3_max);
	DDX_Text(pDX, IDC_GRAPH3_MEANVAL_LABEL, m_strSmGr3_mean);
	DDX_Text(pDX, IDC_GRAPH3_MINVAL_LABEL, m_strSmGr3_min);
	DDX_Text(pDX, IDC_GRAPH3_RMSVAL_LABEL, m_strSmGr3_rms);
	DDX_Text(pDX, IDC_GRAPH4_MAXVAL_LABEL, m_strSmGr4_max);
	DDX_Text(pDX, IDC_GRAPH4_MEANVAL_LABEL, m_strSmGr4_mean);
	DDX_Text(pDX, IDC_GRAPH4_MINVAL_LABEL, m_strSmGr4_min);
	DDX_Text(pDX, IDC_GRAPH4_RMSVAL_LABEL, m_strSmGr4_rms);
	DDX_Text(pDX, IDC_GRAPH5_MAXVAL_LABEL, m_strSmGr5_max);
	DDX_Text(pDX, IDC_GRAPH5_MEANVAL_LABEL, m_strSmGr5_mean);
	DDX_Text(pDX, IDC_GRAPH5_MINVAL_LABEL, m_strSmGr5_min);
	DDX_Text(pDX, IDC_GRAPH5_RMSVAL_LABEL, m_strSmGr5_rms);
	DDX_Text(pDX, IDC_GRAPH6_MAXVAL_LABEL, m_strSmGr6_max);
	DDX_Text(pDX, IDC_GRAPH6_MEANVAL_LABEL, m_strSmGr6_mean);
	DDX_Text(pDX, IDC_GRAPH6_MINVAL_LABEL, m_strSmGr6_min);
	DDX_Text(pDX, IDC_GRAPH6_RMSVAL_LABEL, m_strSmGr6_rms);
	DDX_Text(pDX, IDC_GRAPH7_MAXVAL_LABEL, m_strSmGr7_max);
	DDX_Text(pDX, IDC_GRAPH7_MEANVAL_LABEL, m_strSmGr7_mean);
	DDX_Text(pDX, IDC_GRAPH7_MINVAL_LABEL, m_strSmGr7_min);
	DDX_Text(pDX, IDC_GRAPH7_RMSVAL_LABEL, m_strSmGr7_rms);
	DDX_Text(pDX, IDC_GRAPH8_MAXVAL_LABEL, m_strSmGr8_max);
	DDX_Text(pDX, IDC_GRAPH8_MEANVAL_LABEL, m_strSmGr8_mean);
	DDX_Text(pDX, IDC_GRAPH8_MINVAL_LABEL, m_strSmGr8_min);
	DDX_Text(pDX, IDC_GRAPH8_RMSVAL_LABEL, m_strSmGr8_rms);
	DDX_Text(pDX, IDC_EMERGENCY_LBL, m_strLblEmergency);
	DDX_Text(pDX, IDC_PARAM1_VAL, m_strParam1Val);
	DDX_Text(pDX, IDC_PARAM2_VAL, m_strParam2Val);
	DDX_Text(pDX, IDC_PARAM3_VAL, m_strParam3Val);
	DDX_Text(pDX, IDC_PARAM4_VAL, m_strParam4Val);
	DDX_Control(pDX, IDC_PARAM1_NEDT, m_ctlNedtParam1);
	DDX_Control(pDX, IDC_PARAM2_NEDT, m_ctlNedtParam2);
	DDX_Control(pDX, IDC_PARAM3_NEDT, m_ctlNedtParam3);
	DDX_Control(pDX, IDC_PARAM4_NEDT, m_ctlNedtParam4);
	DDX_Text(pDX, IDC_PARAM8_VAL, m_strParam8Val);
	DDX_Control(pDX, IDC_PARAM8_NEDT, m_ctlNedtParam8);
	DDX_Radio(pDX, IDC_RAD_MEANING1, m_nTMeaningInd);
	DDX_Text(pDX, IDC_EDT_K_IMP_SEC, m_dKimpSec);
	DDX_Control(pDX, IDC_COMM, m_ctlCOM);
	DDX_Control(pDX, IDC_CW_START, m_ctlBtnCwStart);
	DDX_Text(pDX, IDC_SFTW_VER, m_strSoftwareVersion);
	DDX_Control(pDX, IDC_NEDT_MEAS_TIME, m_ctlNedtMeasTime);
	DDX_CBIndex(pDX, IDC_COM_BAUDRATE, m_nComPortBaudrate);
	DDX_Control(pDX, IDC_CWNUMEDIT_THERMO_CURR_TEMP, m_ctlNedtThermoCalibTemperature);
	DDX_Text(pDX, IDC_THERMO_CALIB_T1, m_strThermoCalib_T1);
	DDX_Text(pDX, IDC_THERMO_CALIB_T2, m_strThermoCalib_T2);
	DDX_Text(pDX, IDC_LBL_MARKER_FAILS, m_strMarkerFails);
	DDX_Text(pDX, IDC_LBL_CHECKSUMM_FAILS, m_strCheckSummFails);
	DDX_Text(pDX, IDC_LBL_COUNTER_FAILS, m_strCounterFails);
	DDX_Radio(pDX, IDC_RAD_G8_T0, m_nRadGraph8);
	DDX_Radio(pDX, IDC_RAD_G7_T0, m_nRadGraph7);
	DDX_Radio(pDX, IDC_RAD_G6_T0, m_nRadGraph6);
	DDX_Radio(pDX, IDC_RAD_G5_T0, m_nRadGraph5);
	DDX_Radio(pDX, IDC_RAD_G4_T0, m_nRadGraph4);
	DDX_Radio(pDX, IDC_RAD_G3_T0, m_nRadGraph3);
	DDX_Radio(pDX, IDC_RAD_G2_T0, m_nRadGraph2);
	DDX_Radio(pDX, IDC_RAD_G1_T0, m_nRadGraph1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainView, CFormView)
	//{{AFX_MSG_MAP(CMainView)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PARAM1_BTN, OnParam1Btn)
	ON_BN_CLICKED(IDC_PARAM2_BTN, OnParam2Btn)
	ON_BN_CLICKED(IDC_PARAM3_BTN, OnParam3Btn)
	ON_BN_CLICKED(IDC_PARAM4_BTN, OnParam4Btn)
	ON_BN_CLICKED(IDC_BTN_SAVE_PARAMS, OnBtnSaveParams)
	ON_BN_CLICKED(IDC_PARAM8_BTN, OnParam8Btn)
	ON_BN_CLICKED(IDC_BTN_RESTORE_PARAMS, OnBtnRestoreParams)
	ON_EN_KILLFOCUS(IDC_EDT_K_IMP_SEC, OnKillfocusEdtKImpSec)
	ON_BN_CLICKED(IDC_BTN_DEC_COEFF_CALC, OnDecCoeffCalc)
	ON_BN_CLICKED(IDC_BTN_THERMO_CALIB_SET, OnBtnThermoCalibSet)
	ON_BN_CLICKED(IDC_BTN_THERMO_CALIB_CLEAR, OnBtnThermoCalibClear)
	ON_BN_CLICKED(IDC_BTN_LASER_OFF, OnBtnLaserOff)
	ON_BN_CLICKED(IDC_BTN_INTEGR_OFF, OnBtnIntegrOff)
	ON_BN_CLICKED(IDC_BTN_INTEGR_ON, OnBtnIntegrOn)
	ON_BN_CLICKED(IDC_BTN_INTEGR_RESET, OnBtnIntegrReset)
	ON_BN_CLICKED(IDC_BTN_REQ_VERSION, OnBtnReqVersion)
	ON_BN_CLICKED(IDC_BTN_REQ_AMPL, OnBtnReqAmpl)
	ON_BN_CLICKED(IDC_BTN_REQ_TACTCODE, OnBtnReqTactcode)
	ON_BN_CLICKED(IDC_BTN_REQ_M_COEFF, OnBtnReqMCoeff)
	ON_BN_CLICKED(IDC_BTN_REQ_START_MODE, OnBtnReqStartMode)
	ON_BN_CLICKED(IDC_BTN_REQ_DEC_COEFF, OnBtnReqDecCoeff)
	ON_BN_CLICKED(IDC_BTN_MC_TO_OUT_AMPLITUDE, OnBtnMcToOutAmplitude)
	ON_BN_CLICKED(IDC_BTN_MC_TO_OUT_TACTCODE, OnBtnMcToOutTactcode)
	ON_BN_CLICKED(IDC_BTN_MC_TO_OUT_MCOEFF, OnBtnMcToOutMcoeff)
	ON_BN_CLICKED(IDC_BTN_MC_TO_OUT_STARTMODE, OnBtnMcToOutStartmode)
	ON_BN_CLICKED(IDC_BTN_MC_TO_OUT_DECCOEFF, OnBtnMcToOutDeccoeff)
	ON_BN_CLICKED(IDC_BTN_SWITCH_W_DNDU, OnBtnSwitchWDndu)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_REQ_HV_APPLIES, OnBtnReqHvApplies)
	ON_BN_CLICKED(IDC_BTN_REQ_SN, OnBtnReqSn)
	ON_BN_CLICKED(IDC_RAD_MEANING1, OnRadMeaning1)
	ON_BN_CLICKED(IDC_RAD_MEANING2, OnRadMeaning2)
	ON_BN_CLICKED(IDC_RAD_MEANING3, OnRadMeaning3)
	ON_BN_CLICKED(IDC_RAD_MEANING4, OnRadMeaning4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainView diagnostics

#ifdef _DEBUG
void CMainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainView message handlers

void CMainView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	int nGrWidth = cx / 8;
	CRect rc;

	if( m_ctlSmallGraph1.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH1_Y)->SetWindowPos( NULL, 0,                 50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph1.GetWindowRect( &rc);
		m_ctlSmallGraph1.SetWindowPos(               NULL, 0,                 70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH1_X)->SetWindowPos( NULL, 0,                 70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T0)->SetWindowPos(    NULL, nGrWidth * 1 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T1)->SetWindowPos(    NULL, nGrWidth * 1 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T2)->SetWindowPos(    NULL, nGrWidth * 1 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T3)->SetWindowPos(    NULL, nGrWidth * 1 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G1_T4)->SetWindowPos(    NULL, nGrWidth * 1 - 21, 135,              20,            12,          SWP_NOZORDER);
	}
	
	if( m_ctlSmallGraph2.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH2_Y)->SetWindowPos( NULL, nGrWidth,          50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph2.GetWindowRect( &rc);
		m_ctlSmallGraph2.SetWindowPos(               NULL, nGrWidth,          70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH2_X)->SetWindowPos( NULL, nGrWidth,          70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T0)->SetWindowPos(    NULL, nGrWidth * 2 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T1)->SetWindowPos(    NULL, nGrWidth * 2 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T2)->SetWindowPos(    NULL, nGrWidth * 2 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T3)->SetWindowPos(    NULL, nGrWidth * 2 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G2_T4)->SetWindowPos(    NULL, nGrWidth * 2 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph3.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH3_Y)->SetWindowPos( NULL, nGrWidth * 2,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph3.GetWindowRect( &rc);
		m_ctlSmallGraph3.SetWindowPos(               NULL, nGrWidth * 2,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH3_X)->SetWindowPos( NULL, nGrWidth * 2,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T0)->SetWindowPos(    NULL, nGrWidth * 3 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T1)->SetWindowPos(    NULL, nGrWidth * 3 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T2)->SetWindowPos(    NULL, nGrWidth * 3 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T3)->SetWindowPos(    NULL, nGrWidth * 3 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G3_T4)->SetWindowPos(    NULL, nGrWidth * 3 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph4.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH4_Y)->SetWindowPos( NULL, nGrWidth * 3,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph4.GetWindowRect( &rc);
		m_ctlSmallGraph4.SetWindowPos(               NULL, nGrWidth * 3,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH4_X)->SetWindowPos( NULL, nGrWidth * 3,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T0)->SetWindowPos(    NULL, nGrWidth * 4 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T1)->SetWindowPos(    NULL, nGrWidth * 4 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T2)->SetWindowPos(    NULL, nGrWidth * 4 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T3)->SetWindowPos(    NULL, nGrWidth * 4 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G4_T4)->SetWindowPos(    NULL, nGrWidth * 4 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph5.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH5_Y)->SetWindowPos( NULL, nGrWidth * 4,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph5.GetWindowRect( &rc);
		m_ctlSmallGraph5.SetWindowPos(               NULL, nGrWidth * 4,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH5_X)->SetWindowPos( NULL, nGrWidth * 4,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T0)->SetWindowPos(    NULL, nGrWidth * 5 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T1)->SetWindowPos(    NULL, nGrWidth * 5 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T2)->SetWindowPos(    NULL, nGrWidth * 5 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T3)->SetWindowPos(    NULL, nGrWidth * 5 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G5_T4)->SetWindowPos(    NULL, nGrWidth * 5 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph6.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH6_Y)->SetWindowPos( NULL, nGrWidth * 5,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph6.GetWindowRect( &rc);
		m_ctlSmallGraph6.SetWindowPos(               NULL, nGrWidth * 5,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH6_X)->SetWindowPos( NULL, nGrWidth * 5,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T0)->SetWindowPos(    NULL, nGrWidth * 6 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T1)->SetWindowPos(    NULL, nGrWidth * 6 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T2)->SetWindowPos(    NULL, nGrWidth * 6 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T3)->SetWindowPos(    NULL, nGrWidth * 6 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G6_T4)->SetWindowPos(    NULL, nGrWidth * 6 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph7.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH7_Y)->SetWindowPos( NULL, nGrWidth * 6,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph7.GetWindowRect( &rc);
		m_ctlSmallGraph7.SetWindowPos(               NULL, nGrWidth * 6,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH7_X)->SetWindowPos( NULL, nGrWidth * 6,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T0)->SetWindowPos(    NULL, nGrWidth * 7 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T1)->SetWindowPos(    NULL, nGrWidth * 7 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T2)->SetWindowPos(    NULL, nGrWidth * 7 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T3)->SetWindowPos(    NULL, nGrWidth * 7 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G7_T4)->SetWindowPos(    NULL, nGrWidth * 7 - 21, 135,              20,            12,          SWP_NOZORDER);
	}

	if( m_ctlSmallGraph8.m_hWnd) {
    GetDlgItem( IDC_CMB_GRAPH8_Y)->SetWindowPos( NULL, nGrWidth * 7,      50,               nGrWidth - 2,  12,          SWP_NOZORDER);
		m_ctlSmallGraph8.GetWindowRect( &rc);
		m_ctlSmallGraph8.SetWindowPos(               NULL, nGrWidth * 7,      70,               nGrWidth - 22, rc.Height(), SWP_NOZORDER);
    GetDlgItem( IDC_CMB_GRAPH8_X)->SetWindowPos( NULL, nGrWidth * 7,      70 + rc.Height(), nGrWidth - 2,  12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T0)->SetWindowPos(    NULL, nGrWidth * 8 - 21, 75,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T1)->SetWindowPos(    NULL, nGrWidth * 8 - 21, 90,               20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T2)->SetWindowPos(    NULL, nGrWidth * 8 - 21, 105,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T3)->SetWindowPos(    NULL, nGrWidth * 8 - 21, 120,              20,            12,          SWP_NOZORDER);
    GetDlgItem( IDC_RAD_G8_T4)->SetWindowPos(    NULL, nGrWidth * 8 - 21, 135,              20,            12,          SWP_NOZORDER);
	}


	int base = 70 + rc.Height() + 15;
	CWnd *pWnd;
	/*pWnd = GetDlgItem( IDC_GRAPH1_MIN_LABEL);
	if( pWnd != NULL) {
		CRect rc1;
		
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH1_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_MAX_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH1_RMS_LABEL);
		pWnd->SetWindowPos( NULL, 0, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH1_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		
		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH2_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH2_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH2_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH2_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH3_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH3_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH3_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH3_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 2 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH4_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH4_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH4_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH4_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 3 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH5_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH5_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH5_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH5_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 4 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH6_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH6_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH6_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH6_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 5 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH7_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH7_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH7_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH7_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 6 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		

		base = 50 + rc.Height();
		pWnd = GetDlgItem( IDC_GRAPH8_MIN_LABEL);	
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd->GetWindowRect( &rc1);
		pWnd = GetDlgItem( IDC_GRAPH8_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_MAX_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();

		pWnd = GetDlgItem( IDC_GRAPH8_RMS_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7, base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		pWnd = GetDlgItem( IDC_GRAPH8_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, nGrWidth * 7 + rc1.Width(), base, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		base += rc1.Height();
	}*/

	CRect m_rcStatusBar;
	if( m_ctlMainGraph.m_hWnd) {
		(( CMainFrame*) this->GetParentFrame())->m_wndStatusBar.GetWindowRect( m_rcStatusBar);
		m_ctlMainGraph.SetWindowPos( NULL, 5, base + 5, cx - 270 - 10, cy - base - 10, SWP_NOZORDER);
	}

	pWnd = GetDlgItem( IDC_STATISTIC_FRAME);
	if( pWnd != NULL) {
    GetDlgItem( IDC_LBL_REGIME_SYNC_ASYNC)->SetWindowPos( NULL, cx - 270, base + 5, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem( IDC_LBL_REGIME_DNDU_W)->SetWindowPos(     NULL, cx - 270, base + 20, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem( IDC_LBL_HV_APPLIES_THIS_TIME)->SetWindowPos( NULL, cx - 270, base + 35, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    GetDlgItem( IDC_BTN_REQ_HV_APPLIES)->SetWindowPos( NULL, cx - 60, base + 35, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    

		pWnd->SetWindowPos( NULL, cx - 270, base + 50, 270, cy - base - 30, SWP_NOZORDER);
		
    base += 50;

		pWnd = GetDlgItem( IDC_MIN_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 20, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MINVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 40, 200, 20, SWP_NOZORDER);

		pWnd = GetDlgItem( IDC_MEAN_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 70, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MEANVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 90, 200, 20, SWP_NOZORDER);
		
		pWnd = GetDlgItem( IDC_MAX_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 120, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_MAXVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 140, 200, 20, SWP_NOZORDER);
		
		pWnd = GetDlgItem( IDC_RMS_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 170, 200, 20, SWP_NOZORDER);
		pWnd = GetDlgItem( IDC_RMSVAL_LABEL);
		pWnd->SetWindowPos( NULL, cx - 270 + 5, base + 190, 200, 20, SWP_NOZORDER);


    //**********************************************************************
    //Амплитуда колебания
		int x = cx - 270 + 5;
		base += 220;
		//Амплитуда колебания.заголовок
    pWnd = GetDlgItem( IDC_PARAM1_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Амплитуда колебания.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_AMPL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Амплитуда колебания.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM1_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //кнопка копирования значения
		pWnd = GetDlgItem( IDC_BTN_MC_TO_OUT_AMPLITUDE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //Амплитуда колебания.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM1_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE /*+ SWP_SHOWWINDOW*/);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Амплитуда колебания.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM1_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);

    //**********************************************************************
    //Код такта подставки
		base += rc.Height(); base += 5;
		x = cx - 270 + 5;
    //Код такта подставки.заголовок
		pWnd = GetDlgItem( IDC_PARAM2_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Код такта подставки.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_TACTCODE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Код такта подставки.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM2_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //кнопка копирования значения
		pWnd = GetDlgItem( IDC_BTN_MC_TO_OUT_TACTCODE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //Код такта подставки.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM2_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Код такта подставки.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM2_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);

    //**********************************************************************
    //Коэффициент ошумления
		base += rc.Height(); base += 5;
		x = cx - 270 + 5;
    //Коэффициент ошумления.заголовок
		pWnd = GetDlgItem( IDC_PARAM3_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Коэффициент ошумления.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_M_COEFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
    x += rc.Width(); x += 5;
    //Коэффициент ошумления.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM3_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //кнопка копирования значения
		pWnd = GetDlgItem( IDC_BTN_MC_TO_OUT_MCOEFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //Коэффициент ошумления.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM3_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Коэффициент ошумления.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM3_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);

		//**********************************************************************
    //Начальная мода
    base += rc.Height(); base += 5;
		x = cx - 270 + 5;
    //Начальная мода.заголовок
		pWnd = GetDlgItem( IDC_PARAM4_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Начальная мода.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_START_MODE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
    x += rc.Width(); x += 5;
    //Начальная мода.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM4_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //кнопка копирования значения
		pWnd = GetDlgItem( IDC_BTN_MC_TO_OUT_STARTMODE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //Начальная мода.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM4_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Начальная мода.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM4_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);

		//**********************************************************************
    //Коэффициент вычета
    base += rc.Height(); base += 5;
		x = cx - 270 + 5;
    //Коэффициент вычета.заголовок
		pWnd = GetDlgItem( IDC_PARAM8_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Коэффициент вычета.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_DEC_COEFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
    x += rc.Width(); x += 5;
    //Коэффициент вычета.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM8_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //кнопка копирования значения
		pWnd = GetDlgItem( IDC_BTN_MC_TO_OUT_DECCOEFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 2;
    //Коэффициент вычета.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM8_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Коэффициент вычета.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM8_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		

    /*
    //**********************************************************************
    //Знаковый коэффициент.
		base += rc.Height(); base += 5;
		x = cx - 270 + 5;
    //Знаковый коэффициент.заголовок
		pWnd = GetDlgItem( IDC_PARAM9_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
    //Знаковый коэффициент.кнопка "запрос"
    pWnd = GetDlgItem( IDC_BTN_REQ_SIGN_COEFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
    x += rc.Width(); x += 5;
    //Знаковый коэффициент.значение, полученное из микроконтроллера
		pWnd = GetDlgItem( IDC_PARAM9_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Знаковый коэффициент.значение для отправки в микроконтроллер
		pWnd = GetDlgItem( IDC_PARAM9_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
    //Знаковый коэффициент.кнопка "послать"
		pWnd = GetDlgItem( IDC_PARAM9_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		*/
		
   

    /*
    base += rc.Height(); base += 5;
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_PARAM10_TITLE);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height();
		pWnd = GetDlgItem( IDC_PARAM10_VAL);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
		pWnd = GetDlgItem( IDC_PARAM10_NEDT);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;
		pWnd = GetDlgItem( IDC_PARAM10_BTN);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		
		base += rc.Height(); base += 5;
    */
		
    //кнопка "Сохранить параметры"
    base += rc.Height(); base += 5;
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_BTN_SAVE_PARAMS);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;

    /*
    //кнопка "Перезагрузить параметры из флэш-памяти"
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_BTN_RESTORE_PARAMS);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;
    */

    //кнопка "Вычисление коэффициента вычета"
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_BTN_DEC_COEFF_CALC);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;

    //кнопка "Выключение лазера"
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_BTN_LASER_OFF);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;
		
    //кнопка "Переключить выдачу прибора"
		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_BTN_SWITCH_W_DNDU);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;


		/*x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_THERMO_CALIB_T1);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width();

		pWnd = GetDlgItem( IDC_THERMO_CALIB_T2);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		base += rc.Height(); base += 5;

		x = cx - 270 + 5;
		pWnd = GetDlgItem( IDC_CWNUMEDIT_THERMO_CURR_TEMP);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;

		pWnd = GetDlgItem( IDC_BTN_THERMO_CALIB_SET);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);
		x += rc.Width(); x += 5;

		pWnd = GetDlgItem( IDC_BTN_THERMO_CALIB_CLEAR);
		pWnd->SetWindowPos( NULL, x, base, 0, 0, SWP_NOZORDER + SWP_NOSIZE);
		pWnd->GetWindowRect( rc);*/
		

	}

	
}

void CMainView::MakeLine( CTrackedParam *tp, CNiReal64Matrix *l100ms, CNiReal64Matrix *l1s, CNiReal64Matrix *l10s, CNiReal64Matrix *l100s) {
  int jndx, j, j1, j2, j3;

  //100 msec
  j1 = tp->Get_100ms()->GetFirstIndex();
	j2 = tp->Get_100ms()->GetLastIndex();
	j3 = tp->Get_100ms()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp->Get_100ms()->GetSize()) % tp->Get_100ms()->GetSize();
		l100ms->operator()( 0, j) = tp->Get_100ms()->GetDataX()[ jndx];
		l100ms->operator()( 1, j) = tp->Get_100ms()->GetDataY()[ jndx];
	}
	
	//1 sec
	j1 = tp->Get_1s()->GetFirstIndex();
	j2 = tp->Get_1s()->GetLastIndex();
	j3 = tp->Get_1s()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp->Get_1s()->GetSize()) % tp->Get_1s()->GetSize();
		l1s->operator()( 0, j) = tp->Get_1s()->GetDataX()[ jndx];
		l1s->operator()( 1, j) = tp->Get_1s()->GetDataY()[ jndx];
	}

	//10 sec
	j1 = tp->Get_10s()->GetFirstIndex();
	j2 = tp->Get_10s()->GetLastIndex();
	j3 = tp->Get_10s()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp->Get_10s()->GetSize()) % tp->Get_10s()->GetSize();
		l10s->operator()( 0, j) = tp->Get_10s()->GetDataX()[ jndx];
		l10s->operator()( 1, j) = tp->Get_10s()->GetDataY()[ jndx];
	}

	//100 sec
	j1 = tp->Get_100s()->GetFirstIndex();
	j2 = tp->Get_100s()->GetLastIndex();
	j3 = tp->Get_100s()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp->Get_100s()->GetSize()) % tp->Get_100s()->GetSize();
		l100s->operator()( 0, j) = tp->Get_100s()->GetDataX()[ jndx];
		l100s->operator()( 1, j) = tp->Get_100s()->GetDataY()[ jndx];
	}
}



void CMainView::MakeDeltaLine( CTrackedParam *tp1, CTrackedParam *tp2, CNiReal64Matrix *l100ms, CNiReal64Matrix *l1s, CNiReal64Matrix *l10s, CNiReal64Matrix *l100s) {
  int jndx, j, j1, j2, j3;

  //100 msec
  j1 = tp1->Get_100ms()->GetFirstIndex();
	j2 = tp1->Get_100ms()->GetLastIndex();
	j3 = tp1->Get_100ms()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp1->Get_100ms()->GetSize()) % tp1->Get_100ms()->GetSize();
		l100ms->operator()( 0, j) = tp1->Get_100ms()->GetDataX()[ jndx];
		l100ms->operator()( 1, j) = tp1->Get_100ms()->GetDataY()[ jndx] - tp2->Get_100ms()->GetDataY()[ jndx];
	}
	
	//1 sec
	j1 = tp1->Get_1s()->GetFirstIndex();
	j2 = tp1->Get_1s()->GetLastIndex();
	j3 = tp1->Get_1s()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp1->Get_1s()->GetSize()) % tp1->Get_1s()->GetSize();
		l1s->operator()( 0, j) = tp1->Get_1s()->GetDataX()[ jndx];
		l1s->operator()( 1, j) = tp1->Get_1s()->GetDataY()[ jndx] - tp2->Get_1s()->GetDataY()[ jndx];
	}

	//10 sec
	j1 = tp1->Get_10s()->GetFirstIndex();
	j2 = tp1->Get_10s()->GetLastIndex();
	j3 = tp1->Get_10s()->GetActualSize();
	if( !j3) j3 = 1;
	for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp1->Get_10s()->GetSize()) % tp1->Get_10s()->GetSize();
		l10s->operator()( 0, j) = tp1->Get_10s()->GetDataX()[ jndx];
		l10s->operator()( 1, j) = tp1->Get_10s()->GetDataY()[ jndx] - tp2->Get_10s()->GetDataY()[ jndx];
	}

  //100 sec
  j1 = tp1->Get_100s()->GetFirstIndex();
  j2 = tp1->Get_100s()->GetLastIndex();
  j3 = tp1->Get_100s()->GetActualSize();
  if( !j3) j3 = 1;
  for( j = 0; j < j3; j++) {
    jndx = ( j1 + j + tp1->Get_100s()->GetSize()) % tp1->Get_100s()->GetSize();
    l100s->operator()( 0, j) = tp1->Get_100s()->GetDataX()[ jndx];
    l100s->operator()( 1, j) = tp1->Get_100s()->GetDataY()[ jndx] - tp2->Get_100s()->GetDataY()[ jndx];
  }
}


void CMainView::RefreshGraphs()
{
  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs: p1");

  //цикл по всем 8 графикам
  for( int i=0; i<8; i++) {

    CNiGraph *obj = NULL, *obj2 = NULL;

    //Заодно нарисуем главный график
    if( m_nMainGraph == i+1) {
      obj2 = &m_ctlMainGraph;
    }

    //определим ID комбобокса графика, и его время осреднения
    int nResId, nResIdX, nMeaningTime;
    switch( i) {
      case 1:  obj = &m_ctlSmallGraph2; nResId = IDC_CMB_GRAPH2_Y; nResIdX = IDC_CMB_GRAPH2_X; nMeaningTime = m_nRadGraph2; break;
      case 2:  obj = &m_ctlSmallGraph3; nResId = IDC_CMB_GRAPH3_Y; nResIdX = IDC_CMB_GRAPH3_X; nMeaningTime = m_nRadGraph3; break;
      case 3:  obj = &m_ctlSmallGraph4; nResId = IDC_CMB_GRAPH4_Y; nResIdX = IDC_CMB_GRAPH4_X; nMeaningTime = m_nRadGraph4; break;
      case 4:  obj = &m_ctlSmallGraph5; nResId = IDC_CMB_GRAPH5_Y; nResIdX = IDC_CMB_GRAPH5_X; nMeaningTime = m_nRadGraph5; break;
      case 5:  obj = &m_ctlSmallGraph6; nResId = IDC_CMB_GRAPH6_Y; nResIdX = IDC_CMB_GRAPH6_X; nMeaningTime = m_nRadGraph6; break;
      case 6:  obj = &m_ctlSmallGraph7; nResId = IDC_CMB_GRAPH7_Y; nResIdX = IDC_CMB_GRAPH7_X; nMeaningTime = m_nRadGraph7; break;
      case 7:  obj = &m_ctlSmallGraph8; nResId = IDC_CMB_GRAPH8_Y; nResIdX = IDC_CMB_GRAPH8_X; nMeaningTime = m_nRadGraph8; break;
      default: obj = &m_ctlSmallGraph1; nResId = IDC_CMB_GRAPH1_Y; nResIdX = IDC_CMB_GRAPH1_X; nMeaningTime = m_nRadGraph1; break;
    }

    if( obj != NULL) {
      BOOL bMinY = theApp.GetSettings()->GetGraphSettings( i)->Get_bMinY();
      double dblMinY = theApp.GetSettings()->GetGraphSettings( i)->Get_dblMinY();
      BOOL bMaxY = theApp.GetSettings()->GetGraphSettings( i)->Get_bMaxY();
      double dblMaxY = theApp.GetSettings()->GetGraphSettings( i)->Get_dblMaxY();
      COLORREF clrLineColor = theApp.GetSettings()->GetGraphSettings( i)->GetLineColor();

      CNiAxis axis = obj->GetAxes().Item( "YAxis-1");
      if( bMinY || bMaxY) {
        axis.SetAutoScale( false);
        if( bMinY) axis.SetMinimum( dblMinY);
        if( bMaxY) axis.SetMaximum( dblMaxY);
      }
      else {
        axis.SetAutoScale( true);
      }

      CNiPlot plot = obj->GetPlots().Item( "Data");
      CNiColor colLine = CNiColor( clrLineColor);
      plot.SetLineColor( colLine);

      //и большой график
      if( obj2 != NULL) {
        axis = obj2->GetAxes().Item( "YAxis-1");
        if( bMinY || bMaxY) {
          axis.SetAutoScale( false);
          if( bMinY) axis.SetMinimum( dblMinY);
          if( bMaxY) axis.SetMaximum( dblMaxY);
        }
        else {
          axis.SetAutoScale( true);
        }

        plot = obj2->GetPlots().Item( "Data");
        plot.SetLineColor( colLine);
      }
    }

    

    //определим отображаемый параметр для обрабатываемого графика
    int nSelectedDisplayableParam = ( ( CComboBox *) GetDlgItem( nResId))->GetCurSel();

    //определим какой кольцефой буфер нам использовать
    CSlgCircleBuffer *cbfrY = NULL;
    switch( nSelectedDisplayableParam) {
      case 0:  cbfrY = theApp.m_tpW->Get_CB( nMeaningTime);          break;   //w, Угловая скорость, ["/sec]
      case 1:  cbfrY = theApp.m_tpI1->Get_CB( nMeaningTime);         break;   //I1, Разрядный ток 1, [mA]
      case 2:  cbfrY = theApp.m_tpI2->Get_CB( nMeaningTime);         break;   //I2, Разрядный ток 2, [mA]
      case 3:  cbfrY = theApp.m_tpVpc->Get_CB( nMeaningTime);        break;   //Vrpc, Напряжение на пьезокрр., [V]
      case 4:  cbfrY = theApp.m_tpAmplAngAlt->Get_CB( nMeaningTime); break;   //Ampl_alt, Амплитуда колебаний (altera), ["]
      case 5:  cbfrY = theApp.m_tpAmplAngDus->Get_CB( nMeaningTime); break;   //Ampl_dus, Амплитуда колебаний (ДУС), ["]
      case 6:  cbfrY = theApp.m_tpAmplAngRULA->Get_CB( nMeaningTime);break;   //RULA, Задатчик амплитуды, []
      case 7:  cbfrY = theApp.m_tpT1->Get_CB( nMeaningTime);         break;   //T1, Термодатчик 1, [°C]
      case 8:  cbfrY = theApp.m_tpT2->Get_CB( nMeaningTime);         break;   //T2, Термодатчик 2, [°C]
      case 9:  cbfrY = theApp.m_tpT3->Get_CB( nMeaningTime);         break;   //T3, Термодатчик 3, [°C]
      case 10: cbfrY = theApp.m_tpT1->Get_CB( nMeaningTime);         break;   //dT12, Разница T1 T2, [°C]
      case 11: cbfrY = theApp.m_tpT1->Get_CB( nMeaningTime);         break;   //dT13, Разница T1 T3, [°C]
      case 12: cbfrY = theApp.m_tpT2->Get_CB( nMeaningTime);         break;   //dT23, Разница T2 T3, [°C]
      case 13: cbfrY = theApp.m_tpTsaMs->Get_CB( nMeaningTime);      break;   //dTsa, Время такта, [msec]
      case 14: cbfrY = theApp.m_tpTsaMcs->Get_CB( nMeaningTime);     break;   //dTsa, Время такта, [mcsec]
      case 15: cbfrY = theApp.m_tpTsaHz->Get_CB( nMeaningTime);      break;   //dTsa, Время такта, [Hz]
      case 16: cbfrY = theApp.m_tpDecCoeff->Get_CB( nMeaningTime);   break;   //dc, Коэффициент вычета, ["/В]
    }

    if( cbfrY == NULL) {
      theApp.GetLogger()->LogError( "Не смогли определить какой параметр отображается для графика #%d", i);
      continue;
    }

    //определеим ось X
    int nSelectedAxisX = ( ( CComboBox *) GetDlgItem( nResIdX))->GetCurSel();

    //заготовим объект линии
    int len;
    if( cbfrY->GetActualSize() > 0) len = cbfrY->GetActualSize(); else len = 1;
    CNiReal64Matrix line( 2, len, 1.0);
    
    //рассчитаем линию
    int jndx, j, j1, j2, j3;
    j1 = cbfrY->GetFirstIndex();
    j2 = cbfrY->GetLastIndex();
    j3 = cbfrY->GetActualSize();
    if( !j3) j3 = 1;

    //Флаг есть ли у нас в этом диапазоне на графике недостоверные точки
    int nInveracityPoints = 0;
    
    //просмотрим недостоверные точки
    for( j = 0; j < j3; j++) {
      jndx = ( j1 + j + cbfrY->GetSize()) % cbfrY->GetSize();
      if( cbfrY->GetInveracity()[ jndx])
        nInveracityPoints++;
    }

    //подготовим буфер для недостоверных точек
    CNiReal64Matrix lineInveracity( 2, nInveracityPoints, 1.0);

    int nInveracityFillIndex = 0;
    //особые случаи - где надо считать дельту
    switch( nSelectedDisplayableParam) {
      case 10:  //delta T12
        for( j = 0; j < j3; j++) {
          jndx = ( j1 + j + cbfrY->GetSize()) % cbfrY->GetSize();
          double x = cbfrY->GetDataX()[ jndx];
          switch( nSelectedAxisX) {
            case 1: x = theApp.m_tpT1->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 2: x = theApp.m_tpT2->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 3: x = theApp.m_tpT3->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
          }
          line( 0, j) = x;
          line( 1, j) = cbfrY->GetDataY()[ jndx] - theApp.m_tpT2->Get_CB( nMeaningTime)->GetDataY()[ jndx];
          if( cbfrY->GetInveracity()[ jndx]) {
            lineInveracity( 0, nInveracityFillIndex)   = x;
            lineInveracity( 1, nInveracityFillIndex++) = cbfrY->GetDataY()[ jndx] - theApp.m_tpT2->Get_CB( nMeaningTime)->GetDataY()[ jndx];
          }
        }
      break;

      case 11:  //delta 13
      case 12:  //delta 23
        for( j = 0; j < j3; j++) {
          jndx = ( j1 + j + cbfrY->GetSize()) % cbfrY->GetSize();
          double x = cbfrY->GetDataX()[ jndx];
          switch( nSelectedAxisX) {
            case 1: x = theApp.m_tpT1->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 2: x = theApp.m_tpT2->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 3: x = theApp.m_tpT3->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
          }
          line( 0, j) = x;
          line( 1, j) = cbfrY->GetDataY()[ jndx] - theApp.m_tpT3->Get_CB( nMeaningTime)->GetDataY()[ jndx];
          if( cbfrY->GetInveracity()[ jndx]) {
            lineInveracity( 0, nInveracityFillIndex) = x;
            lineInveracity( 1, nInveracityFillIndex++) = cbfrY->GetDataY()[ jndx] - theApp.m_tpT3->Get_CB( nMeaningTime)->GetDataY()[ jndx];
          }
        }
      break;

      //основные случаи - где просто рисуем линию
      default:
        for( j = 0; j < j3; j++) {
          jndx = ( j1 + j + cbfrY->GetSize()) % cbfrY->GetSize();
          double x = cbfrY->GetDataX()[ jndx];
          switch( nSelectedAxisX) {
            case 1: x = theApp.m_tpT1->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 2: x = theApp.m_tpT2->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
            case 3: x = theApp.m_tpT3->Get_CB( nMeaningTime)->GetDataY()[ jndx]; break;
          }
          line( 0, j) = x;
          line( 1, j) = cbfrY->GetDataY()[ jndx];
          if( cbfrY->GetInveracity()[ jndx]) {
            lineInveracity( 0, nInveracityFillIndex) = x;
            lineInveracity( 1, nInveracityFillIndex++) = cbfrY->GetDataY()[ jndx];
          }
        }
    }

    //нарисуем линию
    obj->GetPlots().Item( "Data").PlotXY( line, true);

    

    //если большой график совпадает с маленьким - нарисуем это же самое на большом
    if( obj2 != NULL) {
      obj2->PlotXY( line, true);

      if( nInveracityPoints != 0)
        obj2->GetPlots().Item( "Inveracity").PlotXY( lineInveracity, true);
      else
        obj2->GetPlots().Item( "Inveracity").ClearData();
    }
  }

  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs: p2");



  /*
  //int jndx, j, j1, j2, j3;
  int len;

  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs:p1");


  //******************************************************************************************
  //угловая скорость
  if( theApp.m_tpW->Get_Tacts()->GetActualSize() > 0) len = theApp.m_tpW->Get_Tacts()->GetActualSize(); else len = 1;
  CNiReal64Matrix lineW_Tacts( 2, len, 1.0);

  if( theApp.m_tpW->Get_100ms()->GetActualSize() > 0) len = theApp.m_tpW->Get_100ms()->GetActualSize(); else len = 1;
  CNiReal64Matrix lineW_100ms( 2, len, 1.0);

  if( theApp.m_tpW->Get_1s()->GetActualSize() > 0)    len = theApp.m_tpW->Get_1s()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineW_1s(    2, len,    1.0);

  if( theApp.m_tpW->Get_10s()->GetActualSize() > 0)   len = theApp.m_tpW->Get_10s()->GetActualSize();   else len = 1;
  CNiReal64Matrix lineW_10s(   2, len,   1.0);

  if( theApp.m_tpW->Get_100s()->GetActualSize() > 0)  len = theApp.m_tpW->Get_100s()->GetActualSize();  else len = 1;
  CNiReal64Matrix lineW_100s(  2, len,  1.0);

  MakeLine( theApp.m_tpW, &lineW_100ms, &lineW_1s, &lineW_10s, &lineW_100s);


  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs:p2");



  //разрядный ток I1
  if( theApp.m_tpI1->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpI1->Get_100ms()->GetActualSize(); else len = 1;
	CNiReal64Matrix lineI1_100ms( 2, len, 1.0);

  if( theApp.m_tpI1->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpI1->Get_1s()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineI1_1s(    2, len, 1.0);
  
  if( theApp.m_tpI1->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpI1->Get_10s()->GetActualSize();   else len = 1;
  CNiReal64Matrix lineI1_10s(   2, len, 1.0);

  if( theApp.m_tpI1->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpI1->Get_100s()->GetActualSize();  else len = 1;
  CNiReal64Matrix lineI1_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpI1, &lineI1_100ms, &lineI1_1s, &lineI1_10s, &lineI1_100s);



  //разрядный ток I2
  if( theApp.m_tpI2->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpI2->Get_100ms()->GetActualSize();  else len = 1;
	CNiReal64Matrix lineI2_100ms( 2, len, 1.0);

  if( theApp.m_tpI2->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpI2->Get_1s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineI2_1s(    2, len, 1.0);

  if( theApp.m_tpI2->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpI2->Get_10s()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineI2_10s(   2, len, 1.0);

  if( theApp.m_tpI2->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpI2->Get_100s()->GetActualSize();   else len = 1;
  CNiReal64Matrix lineI2_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpI2, &lineI2_100ms, &lineI2_1s, &lineI2_10s, &lineI2_100s);



  //Напряжение на пьезокорректорах
  if( theApp.m_tpVpc->Get_100ms()->GetActualSize() > 0)   len = theApp.m_tpVpc->Get_100ms()->GetActualSize(); else len = 1;
	CNiReal64Matrix lineVpc_100ms( 2, len, 1.0);

  if( theApp.m_tpVpc->Get_1s()->GetActualSize() > 0)      len = theApp.m_tpVpc->Get_1s()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineVpc_1s(    2, len, 1.0);

  if( theApp.m_tpVpc->Get_10s()->GetActualSize() > 0)     len = theApp.m_tpVpc->Get_10s()->GetActualSize();   else len = 1;
  CNiReal64Matrix lineVpc_10s(   2, len, 1.0);

  if( theApp.m_tpVpc->Get_100s()->GetActualSize() > 0)    len = theApp.m_tpVpc->Get_100s()->GetActualSize();  else len = 1;
  CNiReal64Matrix lineVpc_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpVpc, &lineVpc_100ms, &lineVpc_1s, &lineVpc_10s, &lineVpc_100s);



  //AmplAng точки
  if( theApp.m_tpAmplAng->Get_100ms()->GetActualSize() > 0)   len = theApp.m_tpAmplAng->Get_100ms()->GetActualSize();  else len = 1;
  CNiReal64Matrix lineAA_100ms( 2, len, 1.0);

  if( theApp.m_tpAmplAng->Get_1s()->GetActualSize() > 0)      len = theApp.m_tpAmplAng->Get_1s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineAA_1s(    2, len, 1.0);

  if( theApp.m_tpAmplAng->Get_10s()->GetActualSize() > 0)     len = theApp.m_tpAmplAng->Get_10s()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineAA_10s(   2, len, 1.0);

  if( theApp.m_tpAmplAng->Get_100s()->GetActualSize() > 0)    len = theApp.m_tpAmplAng->Get_100s()->GetActualSize();   else len = 1;
  CNiReal64Matrix lineAA_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpAmplAng, &lineAA_100ms, &lineAA_1s, &lineAA_10s, &lineAA_100s);



  //AmplAngDus точки
  if( theApp.m_tpAmplAngDus->Get_100ms()->GetActualSize() > 0)  len = theApp.m_tpAmplAngDus->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineAADus_100ms( 2, len, 1.0);

  if( theApp.m_tpAmplAngDus->Get_1s()->GetActualSize() > 0)     len = theApp.m_tpAmplAngDus->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineAADus_1s(    2, len,    1.0);

  if( theApp.m_tpAmplAngDus->Get_10s()->GetActualSize() > 0)    len = theApp.m_tpAmplAngDus->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineAADus_10s(   2, len,   1.0);

  if( theApp.m_tpAmplAngDus->Get_100s()->GetActualSize() > 0)   len = theApp.m_tpAmplAngDus->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineAADus_100s(  2, len,  1.0);

  MakeLine( theApp.m_tpAmplAngDus, &lineAADus_100ms, &lineAADus_1s, &lineAADus_10s, &lineAADus_100s);



	//T1 точки
  if( theApp.m_tpT1->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT1->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineT1_100ms( 2, len, 1.0);

  if( theApp.m_tpT1->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT1->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineT1_1s(    2, len, 1.0);

  if( theApp.m_tpT1->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT1->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineT1_10s(   2, len, 1.0);

  if( theApp.m_tpT1->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT1->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineT1_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpT1, &lineT1_100ms, &lineT1_1s, &lineT1_10s, &lineT1_100s);



	//T2 точки
  if( theApp.m_tpT2->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT2->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineT2_100ms( 2, len, 1.0);

  if( theApp.m_tpT2->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT2->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineT2_1s(    2, len, 1.0);

  if( theApp.m_tpT2->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT2->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineT2_10s(   2, len, 1.0);

  if( theApp.m_tpT2->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT2->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineT2_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpT2, &lineT2_100ms, &lineT2_1s, &lineT2_10s, &lineT2_100s);


  
  //T3 точки
  if( theApp.m_tpT3->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT3->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineT3_100ms( 2, len, 1.0);

  if( theApp.m_tpT3->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT3->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineT3_1s(    2, len, 1.0);

  if( theApp.m_tpT3->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT3->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineT3_10s(   2, len, 1.0);

  if( theApp.m_tpT3->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT3->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineT3_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpT3, &lineT3_100ms, &lineT3_1s, &lineT3_10s, &lineT3_100s);


  //deltaT12 точки
  if( theApp.m_tpT1->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT1->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineDeltaT12_100ms( 2, len, 1.0);

  if( theApp.m_tpT1->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT1->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineDeltaT12_1s(    2, len, 1.0);

  if( theApp.m_tpT1->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT1->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineDeltaT12_10s(   2, len, 1.0);

  if( theApp.m_tpT1->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT1->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineDeltaT12_100s(  2, len, 1.0);

  MakeDeltaLine( theApp.m_tpT1, theApp.m_tpT2, &lineDeltaT12_100ms, &lineDeltaT12_1s, &lineDeltaT12_10s, &lineDeltaT12_100s);
  

  // ************************************************************
  //deltaT13 точки
  if( theApp.m_tpT3->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT3->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineDeltaT13_100ms( 2, len, 1.0);

  if( theApp.m_tpT3->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT3->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineDeltaT13_1s(    2, len, 1.0);

  if( theApp.m_tpT3->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT3->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineDeltaT13_10s(   2, len, 1.0);

  if( theApp.m_tpT3->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT3->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineDeltaT13_100s(  2, len, 1.0);

  MakeDeltaLine( theApp.m_tpT1, theApp.m_tpT3, &lineDeltaT13_100ms, &lineDeltaT13_1s, &lineDeltaT13_10s, &lineDeltaT13_100s);


  // ************************************************************
  //deltaT23 точки
  if( theApp.m_tpT3->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpT3->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineDeltaT23_100ms( 2, len, 1.0);

  if( theApp.m_tpT3->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpT3->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineDeltaT23_1s(    2, len, 1.0);

  if( theApp.m_tpT3->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpT3->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineDeltaT23_10s(   2, len, 1.0);

  if( theApp.m_tpT3->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpT3->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineDeltaT23_100s(  2, len, 1.0);

  MakeDeltaLine( theApp.m_tpT2, theApp.m_tpT3, &lineDeltaT23_100ms, &lineDeltaT23_1s, &lineDeltaT23_10s, &lineDeltaT23_100s);


  // ************************************************************
	//Tsa.mcs точки
  if( theApp.m_tpTsaMcs->Get_100ms()->GetActualSize() > 0)    len = theApp.m_tpTsaMcs->Get_100ms()->GetActualSize();    else len = 1;
  CNiReal64Matrix lineTsaMcs_100ms( 2, len, 1.0);

  if( theApp.m_tpTsaMcs->Get_1s()->GetActualSize() > 0)       len = theApp.m_tpTsaMcs->Get_1s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineTsaMcs_1s(    2, len, 1.0);

  if( theApp.m_tpTsaMcs->Get_10s()->GetActualSize() > 0)      len = theApp.m_tpTsaMcs->Get_10s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineTsaMcs_10s(   2, len, 1.0);

  if( theApp.m_tpTsaMcs->Get_100s()->GetActualSize() > 0)     len = theApp.m_tpTsaMcs->Get_100s()->GetActualSize();     else len = 1;
  CNiReal64Matrix lineTsaMcs_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpTsaMcs, &lineTsaMcs_100ms, &lineTsaMcs_1s, &lineTsaMcs_10s, &lineTsaMcs_100s);



  //Tsa.ms точки
  if( theApp.m_tpTsaMs->Get_100ms()->GetActualSize() > 0)     len = theApp.m_tpTsaMs->Get_100ms()->GetActualSize();     else len = 1;
	CNiReal64Matrix lineTsaMs_100ms( 2, len, 1.0);

  if( theApp.m_tpTsaMs->Get_1s()->GetActualSize() > 0)        len = theApp.m_tpTsaMs->Get_1s()->GetActualSize();        else len = 1;
  CNiReal64Matrix lineTsaMs_1s(    2, len, 1.0);

  if( theApp.m_tpTsaMs->Get_10s()->GetActualSize() > 0)       len = theApp.m_tpTsaMs->Get_10s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineTsaMs_10s(   2, len, 1.0);

  if( theApp.m_tpTsaMs->Get_100s()->GetActualSize() > 0)      len = theApp.m_tpTsaMs->Get_100s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineTsaMs_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpTsaMs, &lineTsaMs_100ms, &lineTsaMs_1s, &lineTsaMs_10s, &lineTsaMs_100s);




  //Tsa.hz точки
  if( theApp.m_tpTsaHz->Get_100ms()->GetActualSize() > 0)     len = theApp.m_tpTsaHz->Get_100ms()->GetActualSize();     else len = 1;
	CNiReal64Matrix lineTsaHz_100ms( 2, len, 1.0);

  if( theApp.m_tpTsaHz->Get_1s()->GetActualSize() > 0)        len = theApp.m_tpTsaHz->Get_1s()->GetActualSize();        else len = 1;
  CNiReal64Matrix lineTsaHz_1s(    2, len, 1.0);

  if( theApp.m_tpTsaHz->Get_10s()->GetActualSize() > 0)       len = theApp.m_tpTsaHz->Get_10s()->GetActualSize();       else len = 1;
  CNiReal64Matrix lineTsaHz_10s(   2, len, 1.0);

  if( theApp.m_tpTsaHz->Get_100s()->GetActualSize() > 0)      len = theApp.m_tpTsaHz->Get_100s()->GetActualSize();      else len = 1;
  CNiReal64Matrix lineTsaHz_100s(  2, len, 1.0);

  MakeLine( theApp.m_tpTsaHz, &lineTsaHz_100ms, &lineTsaHz_1s, &lineTsaHz_10s, &lineTsaHz_100s);

  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs:p3");

  //маленькие экранчики
  for( int nGraph=0; nGraph<8; nGraph++) {

    CNiGraph *obj = NULL, *obj2 = NULL;
    int nCombo, nRad;
    switch( nGraph) {
      case 0: obj = &m_ctlSmallGraph1; nCombo = IDC_CMB_GRAPH1; nRad = m_nRadGraph1; break;
      case 1: obj = &m_ctlSmallGraph2; nCombo = IDC_CMB_GRAPH2; nRad = m_nRadGraph2; break;
      case 2: obj = &m_ctlSmallGraph3; nCombo = IDC_CMB_GRAPH3; nRad = m_nRadGraph3; break;
      case 3: obj = &m_ctlSmallGraph4; nCombo = IDC_CMB_GRAPH4; nRad = m_nRadGraph4; break;
      case 4: obj = &m_ctlSmallGraph5; nCombo = IDC_CMB_GRAPH5; nRad = m_nRadGraph5; break;
      case 5: obj = &m_ctlSmallGraph6; nCombo = IDC_CMB_GRAPH6; nRad = m_nRadGraph6; break;
      case 6: obj = &m_ctlSmallGraph7; nCombo = IDC_CMB_GRAPH7; nRad = m_nRadGraph7; break;
      case 7: obj = &m_ctlSmallGraph8; nCombo = IDC_CMB_GRAPH8; nRad = m_nRadGraph8; break;
    }

    theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs:p4:nCombo=%d nRad=%d m_nMainGraph=%d", nCombo, nRad, m_nMainGraph);

    //Заодно нарисуем главный график
    if( m_nMainGraph == ( nGraph + 1)) {
      obj2 = &m_ctlMainGraph;
    }
    
    if( obj != NULL) {
      int nSelection = ( ( CComboBox *) GetDlgItem( nCombo))->GetCurSel();
      switch( nSelection) {
        case  1:    //I1
          switch( nRad) {
            case 0: obj->PlotXY( lineI1_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineI1_100ms, true); break;
            case 1: obj->PlotXY( lineI1_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineI1_1s,    true); break;
            case 2: obj->PlotXY( lineI1_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineI1_10s,   true); break;
            case 3: obj->PlotXY( lineI1_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineI1_100s,  true); break;
          }
        break;
        case  2:    //I2
          switch( nRad) {
            case 0: obj->PlotXY( lineI2_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineI2_100ms, true); break;
            case 1: obj->PlotXY( lineI2_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineI2_1s,    true); break;
            case 2: obj->PlotXY( lineI2_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineI2_10s,   true); break;
            case 3: obj->PlotXY( lineI2_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineI2_100s,  true); break;
          }
        break;
        case  3:    //Vrpc
          switch( nRad) {
            case 0: obj->PlotXY( lineVpc_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineVpc_100ms, true); break;
            case 1: obj->PlotXY( lineVpc_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineVpc_1s,    true); break;
            case 2: obj->PlotXY( lineVpc_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineVpc_10s,   true); break;
            case 3: obj->PlotXY( lineVpc_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineVpc_100s,  true); break;
          }
        break;
        case  4:    //Ampl_alt
          switch( nRad) {
            case 0: obj->PlotXY( lineAA_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineAA_100ms, true); break;
            case 1: obj->PlotXY( lineAA_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineAA_1s,    true); break;
            case 2: obj->PlotXY( lineAA_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineAA_10s,   true); break;
            case 3: obj->PlotXY( lineAA_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineAA_100s,  true); break;
          }
        break;
        case  5:    //Ampl_dus
          switch( nRad) {
            case 0: obj->PlotXY( lineAADus_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineAADus_100ms, true); break;
            case 1: obj->PlotXY( lineAADus_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineAADus_1s,    true); break;
            case 2: obj->PlotXY( lineAADus_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineAADus_10s,   true); break;
            case 3: obj->PlotXY( lineAADus_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineAADus_100s,  true); break;
          }
        break;
        case  6:    //Ampl_RULA
          //switch( nRad) {
          //  case 0: obj->PlotXY( lineAArula_100ms, true);  if( obj2! = NULL) obj2->PlotXY( lineAArula_100ms, true); break;
          //  case 1: obj->PlotXY( lineAArula_1s,    true);  if( obj2! = NULL) obj2->PlotXY( lineAArula_1s,    true); break;
          //  case 2: obj->PlotXY( lineAArula_10s,   true);  if( obj2! = NULL) obj2->PlotXY( lineAArula_10s,   true); break;
          //  case 3: obj->PlotXY( lineAArula_100s,  true);  if( obj2! = NULL) obj2->PlotXY( lineAArula_100s,  true); break;
          //}
        break;
        case  7:    //T1
          switch( nRad) {
            case 0: obj->PlotXY( lineT1_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineT1_100ms, true); break;
            case 1: obj->PlotXY( lineT1_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineT1_1s,    true); break;
            case 2: obj->PlotXY( lineT1_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineT1_10s,   true); break;
            case 3: obj->PlotXY( lineT1_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineT1_100s,  true); break;
          }
        break;
        case  8:    //T2
          switch( nRad) {
            case 0: obj->PlotXY( lineT2_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineT2_100ms, true); break;
            case 1: obj->PlotXY( lineT2_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineT2_1s,    true); break;
            case 2: obj->PlotXY( lineT2_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineT2_10s,   true); break;
            case 3: obj->PlotXY( lineT2_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineT2_100s,  true); break;
          }
        break;
        case  9:    //T3
          switch( nRad) {
            case 0: obj->PlotXY( lineT3_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineT3_100ms, true); break;
            case 1: obj->PlotXY( lineT3_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineT3_1s,    true); break;
            case 2: obj->PlotXY( lineT3_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineT3_10s,   true); break;
            case 3: obj->PlotXY( lineT3_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineT3_100s,  true); break;
          }
        break;
        case 10:    //deltaT12
          switch( nRad) {
            case 0: obj->PlotXY( lineDeltaT12_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT12_100ms, true); break;
            case 1: obj->PlotXY( lineDeltaT12_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT12_1s,    true); break;
            case 2: obj->PlotXY( lineDeltaT12_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT12_10s,   true); break;
            case 3: obj->PlotXY( lineDeltaT12_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT12_100s,  true); break;
          }
        break;
        case 11:    //deltaT13
          switch( nRad) {
            case 0: obj->PlotXY( lineDeltaT13_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT13_100ms, true); break;
            case 1: obj->PlotXY( lineDeltaT13_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT13_1s,    true); break;
            case 2: obj->PlotXY( lineDeltaT13_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT13_10s,   true); break;
            case 3: obj->PlotXY( lineDeltaT13_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT13_100s,  true); break;
          }
        break;
        case 12:    //deltaT23
          switch( nRad) {
            case 0: obj->PlotXY( lineDeltaT23_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT23_100ms, true); break;
            case 1: obj->PlotXY( lineDeltaT23_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT23_1s,    true); break;
            case 2: obj->PlotXY( lineDeltaT23_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT23_10s,   true); break;
            case 3: obj->PlotXY( lineDeltaT23_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineDeltaT23_100s,  true); break;
          }
        break;
        case 13:    //dt_sa,msec
          switch( nRad) {
            case 0: obj->PlotXY( lineTsaMs_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMs_100ms, true); break;
            case 1: obj->PlotXY( lineTsaMs_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMs_1s,    true); break;
            case 2: obj->PlotXY( lineTsaMs_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMs_10s,   true); break;
            case 3: obj->PlotXY( lineTsaMs_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMs_100s,  true); break;
          }
        break;
        case 14:    //dt_sa, mcsec
          switch( nRad) {
            case 0: obj->PlotXY( lineTsaMcs_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMcs_100ms, true); break;
            case 1: obj->PlotXY( lineTsaMcs_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMcs_1s,    true); break;
            case 2: obj->PlotXY( lineTsaMcs_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMcs_10s,   true); break;
            case 3: obj->PlotXY( lineTsaMcs_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineTsaMcs_100s,  true); break;
          }
        break;
        case 15:    //dt_sa, hz
          switch( nRad) {
            case 0: obj->PlotXY( lineTsaHz_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineTsaHz_100ms, true); break;
            case 1: obj->PlotXY( lineTsaHz_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineTsaHz_1s,    true); break;
            case 2: obj->PlotXY( lineTsaHz_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineTsaHz_10s,   true); break;
            case 3: obj->PlotXY( lineTsaHz_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineTsaHz_100s,  true); break;
          }
        break;        

        default:
          switch( nRad) {
            case 0: obj->PlotXY( lineW_100ms, true);  if( obj2 != NULL) obj2->PlotXY( lineW_100ms, true); break;
            case 1: obj->PlotXY( lineW_1s,    true);  if( obj2 != NULL) obj2->PlotXY( lineW_1s,    true); break;
            case 2: obj->PlotXY( lineW_10s,   true);  if( obj2 != NULL) obj2->PlotXY( lineW_10s,   true); break;
            case 3: obj->PlotXY( lineW_100s,  true);  if( obj2 != NULL) obj2->PlotXY( lineW_100s,  true); break;
          }
      }
    }
  }
  */

  //Перерасчёт статистики
  theApp.m_tpW->RecalculateStatisticUnder();
  theApp.m_tpI1->RecalculateStatisticUnder();
  theApp.m_tpI2->RecalculateStatisticUnder();
  theApp.m_tpVpc->RecalculateStatisticUnder();
  theApp.m_tpAmplAngAlt->RecalculateStatisticUnder();
  theApp.m_tpAmplAngDus->RecalculateStatisticUnder();
  theApp.m_tpAmplAngRULA->RecalculateStatisticUnder();
  theApp.m_tpT1->RecalculateStatisticUnder();
  theApp.m_tpT2->RecalculateStatisticUnder();
  theApp.m_tpT3->RecalculateStatisticUnder();
  theApp.m_tpTsaMcs->RecalculateStatisticUnder();
  theApp.m_tpTsaMs->RecalculateStatisticUnder();
  theApp.m_tpTsaHz->RecalculateStatisticUnder();
  theApp.m_tpDecCoeff->RecalculateStatisticUnder();

  //******************************************************************************************
  //Обновим панель статистики справа от большого графика
  BOOL bShowStatistic = FALSE;

  int nCombo = IDC_CMB_GRAPH1_Y;
  int nMeaningTimeRad = 0;
  switch( m_nMainGraph) {
    case 1: nCombo = IDC_CMB_GRAPH1_Y; nMeaningTimeRad = m_nRadGraph1; break;
    case 2: nCombo = IDC_CMB_GRAPH2_Y; nMeaningTimeRad = m_nRadGraph2; break;
    case 3: nCombo = IDC_CMB_GRAPH3_Y; nMeaningTimeRad = m_nRadGraph3; break;
    case 4: nCombo = IDC_CMB_GRAPH4_Y; nMeaningTimeRad = m_nRadGraph4; break;
    case 5: nCombo = IDC_CMB_GRAPH5_Y; nMeaningTimeRad = m_nRadGraph5; break;
    case 6: nCombo = IDC_CMB_GRAPH6_Y; nMeaningTimeRad = m_nRadGraph6; break;
    case 7: nCombo = IDC_CMB_GRAPH7_Y; nMeaningTimeRad = m_nRadGraph7; break;
    case 8: nCombo = IDC_CMB_GRAPH8_Y; nMeaningTimeRad = m_nRadGraph8; break;
  }
  
  int nMainDisplayedParam = ( ( CComboBox *) GetDlgItem( nCombo))->GetCurSel();
  CSlgCircleBuffer *cbfr;
  switch( nMainDisplayedParam) {
    case  0:  cbfr = theApp.m_tpW->Get_CB( nMeaningTimeRad);          bShowStatistic = TRUE; break;    //w  
    case  1:  cbfr = theApp.m_tpI1->Get_CB( nMeaningTimeRad);         bShowStatistic = TRUE; break;    //I1
    case  2:  cbfr = theApp.m_tpI2->Get_CB( nMeaningTimeRad);         bShowStatistic = TRUE; break;    //I2
    case  3:  cbfr = theApp.m_tpVpc->Get_CB( nMeaningTimeRad);        bShowStatistic = TRUE; break;    //Vpc
    case  4:  cbfr = theApp.m_tpAmplAngAlt->Get_CB( nMeaningTimeRad); bShowStatistic = TRUE; break;    //Ampl_alt
    case  5:  cbfr = theApp.m_tpAmplAngDus->Get_CB( nMeaningTimeRad); bShowStatistic = TRUE; break;    //Ampl_alt
    case  6:  cbfr = theApp.m_tpAmplAngRULA->Get_CB( nMeaningTimeRad);bShowStatistic = TRUE; break;    //Ampl_alt_rula
    case  7:  cbfr = theApp.m_tpT1->Get_CB( nMeaningTimeRad);         bShowStatistic = TRUE; break;    //T1
    case  8:  cbfr = theApp.m_tpT2->Get_CB( nMeaningTimeRad);         bShowStatistic = TRUE; break;    //T2
    case  9:  cbfr = theApp.m_tpT3->Get_CB( nMeaningTimeRad);         bShowStatistic = TRUE; break;    //T3
    case 10:  break; //deltaT12
    case 11:  break; //deltaT13
    case 12:  break; //deltaT23
    case 13:  cbfr = theApp.m_tpTsaMs->Get_CB( nMeaningTimeRad);      bShowStatistic = TRUE; break;    //dt_sa,msec
    case 14:  cbfr = theApp.m_tpTsaMcs->Get_CB( nMeaningTimeRad);     bShowStatistic = TRUE; break;    //dt_sa, mcsec
    case 15:  cbfr = theApp.m_tpTsaHz->Get_CB( nMeaningTimeRad);      bShowStatistic = TRUE; break;    //dt_sa, hz
    case 16:  cbfr = theApp.m_tpDecCoeff->Get_CB( nMeaningTimeRad);   bShowStatistic = TRUE; break;    //dc, коэффициент вычета, ["/В]
  }
  
  theApp.GetLogger()->LogDebug( "CMainView::RefreshGraphs: p3: bShowStatistic=%s nMainDisplayedParam=%d",
          bShowStatistic?"TRUE":"FALSE",
          nMainDisplayedParam);

  CStatic *st;
  CString strValue;
  if( bShowStatistic) {
    st = (( CStatic *) GetDlgItem( IDC_MINVAL_LABEL));
    st->ShowWindow( SW_SHOW);
    strValue.Format( _T("%.4f"),  cbfr->GetMin()); st->SetWindowText( strValue);

    st = (( CStatic *) GetDlgItem( IDC_MEANVAL_LABEL));
    st->ShowWindow( SW_SHOW);
    strValue.Format( _T("%.4f"),  cbfr->GetMean()); st->SetWindowText( strValue);

    st = (( CStatic *) GetDlgItem( IDC_MAXVAL_LABEL));
    st->ShowWindow( SW_SHOW);
    strValue.Format( _T("%.4f"),  cbfr->GetMax()); st->SetWindowText( strValue);

    st = (( CStatic *) GetDlgItem( IDC_RMSVAL_LABEL));
    st->ShowWindow( SW_SHOW);
    strValue.Format( _T("%.4f"),  cbfr->GetRMS()); st->SetWindowText( strValue);
  }
  else {
    GetDlgItem( IDC_MINVAL_LABEL)->ShowWindow(  SW_HIDE);
    GetDlgItem( IDC_MEANVAL_LABEL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_MAXVAL_LABEL)->ShowWindow(  SW_HIDE);
    GetDlgItem( IDC_RMSVAL_LABEL)->ShowWindow(  SW_HIDE);
  }

  //лампочка "СОМ-порт открыт"
  m_ctlComButton.SetValue( m_ctlCOM.GetPortOpen());

  UpdateData( FALSE);
}

void CMainView::OnInitialUpdate() 
{
  m_dKimpSec = theApp.GetSettings()->GetScaleCoeff();
  m_nComPort = theApp.GetSettings()->GetComPort();
  m_nComPortBaudrate = theApp.GetSettings()->GetComBaudrate();

  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(0)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(1)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(2)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(3)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(4)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(5)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(6)->GetAxisY());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_Y))->SetCurSel( theApp.GetSettings()->GetGraphSettings(7)->GetAxisY());

  m_nRadGraph1 = theApp.GetSettings()->GetGraphSettings(0)->GetMeaning();
  m_nRadGraph2 = theApp.GetSettings()->GetGraphSettings(1)->GetMeaning();
  m_nRadGraph3 = theApp.GetSettings()->GetGraphSettings(2)->GetMeaning();
  m_nRadGraph4 = theApp.GetSettings()->GetGraphSettings(3)->GetMeaning();
  m_nRadGraph5 = theApp.GetSettings()->GetGraphSettings(4)->GetMeaning();
  m_nRadGraph6 = theApp.GetSettings()->GetGraphSettings(5)->GetMeaning();
  m_nRadGraph7 = theApp.GetSettings()->GetGraphSettings(6)->GetMeaning();
  m_nRadGraph8 = theApp.GetSettings()->GetGraphSettings(7)->GetMeaning();

  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(0)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(1)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(2)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(3)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(4)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(5)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(6)->GetAxisX());
  ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_X))->SetCurSel( theApp.GetSettings()->GetGraphSettings(7)->GetAxisX());

  m_nMainGraph = 1;

  CFormView::OnInitialUpdate();

  LOGFONT lf;
  memset(&lf, 0, sizeof(LOGFONT));                  // Zero out the structure.
  lf.lfHeight = 20;                                 // Request a 12-pixel-height font.
  lstrcpy(lf.lfFaceName, _T("Times New Roman"));    // Request a face name "Arial".
  VERIFY(m_pFont.CreateFontIndirect(&lf));          // Create the font.

  m_ctlCOM.SetInBufferSize( 2048);
  m_ctlCOM.SetCommPort( 1);
  m_ctlCOM.SetSettings( _T("115200,N,8,1"));
  m_ctlCOM.SetInputLen( 200);
  m_ctlCOM.SetInputMode( 1);
  m_ctlCOM.SetInBufferCount( 0);
  m_ctlCOM.SetOutBufferCount( 0);
  m_ctlCOM.SetOutBufferSize( 3);

  GetDlgItem( IDC_MIN_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MINVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MEAN_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MEANVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MAX_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_MAXVAL_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_RMS_LABEL)->SetFont( &m_pFont);
  GetDlgItem( IDC_RMSVAL_LABEL)->SetFont( &m_pFont);

  if( theApp.GetSettings()->GetControlButtons()) {
    m_ctlNedtParam1.SetDiscreteInterval( m_dKimpSec);
    m_ctlNedtParam1.SetIncDecValue( m_dKimpSec);
    m_ctlNedtParam1.SetAccelInc( m_dKimpSec * 5.);
    m_ctlNedtParam1.SetMaximum( m_dKimpSec * 255.);

    GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( false);
    GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( false);
    GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( false);
    GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( false);
    GetDlgItem( IDC_BTN_EXPORT)->EnableWindow( false);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( false);
  }
  else {
    GetDlgItem( IDC_PARAM1_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_REQ_AMPL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM1_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM1_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM1_BTN)->ShowWindow( SW_HIDE);

    GetDlgItem( IDC_PARAM2_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_REQ_TACTCODE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM2_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM2_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM2_BTN)->ShowWindow( SW_HIDE);

    GetDlgItem( IDC_PARAM3_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_REQ_M_COEFF)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM3_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM3_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM3_BTN)->ShowWindow( SW_HIDE);

    GetDlgItem( IDC_PARAM4_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_REQ_START_MODE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM4_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM4_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM4_BTN)->ShowWindow( SW_HIDE);

    /*
    GetDlgItem( IDC_PARAM5_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM5_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM5_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM5_BTN)->ShowWindow( SW_HIDE);
    */
    /*
    GetDlgItem( IDC_PARAM6_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM6_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM6_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM6_BTN)->ShowWindow( SW_HIDE);
    */
    /*
    GetDlgItem( IDC_PARAM7_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM7_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM7_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM7_BTN)->ShowWindow( SW_HIDE);
    */
    GetDlgItem( IDC_PARAM8_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_REQ_DEC_COEFF)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM8_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM8_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM8_BTN)->ShowWindow( SW_HIDE);

    /*
    GetDlgItem( IDC_PARAM9_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM9_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM9_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM9_BTN)->ShowWindow( SW_HIDE);
    */

    /*
    GetDlgItem( IDC_PARAM10_TITLE)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM10_VAL)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM10_NEDT)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_PARAM10_BTN)->ShowWindow( SW_HIDE);
    */

    GetDlgItem( IDC_BTN_SAVE_PARAMS)->ShowWindow( SW_HIDE);
    //GetDlgItem( IDC_BTN_RESTORE_PARAMS)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_LASER_OFF)->ShowWindow( SW_HIDE);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->ShowWindow( SW_HIDE);
  }

  SetTimer( MY_MAXIMIZE_VIEW_TIMER, 100, NULL);
  SetTimer( TIMER_SEND_CMDS_TO_MC, 500, NULL);

  m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
}

void CMainView::OnTimer(UINT nIDEvent) 
{
	CString strError;
	UpdateData( TRUE);
	CSlg2App *app = ((CSlg2App *) AfxGetApp());

	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//событие обработки COM-порта
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_TIMER_COM_FLUSH) {
		if( m_nPointsSkipped > 10) {
			if( m_ctlCOM.GetPortOpen()) {
				
				unsigned short nBytes = m_ctlCOM.GetInBufferCount();
				if( nBytes > 10) {
					VARIANT var = m_ctlCOM.GetInput();
					
					
					char *buff;//[2048];
					int count = var.parray->rgsabound[0].cElements;
					buff = new char[count];
					/*
					if( count<0 || count > 2048) {
						CString strError;
						strError.Format( _T("COUNT: %d"), count);
						AfxMessageBox( strError);
						Beep( 1000, 1000);
					}
					*/
					memcpy( buff, var.parray->pvData, count);

					for( int qq=0; qq<count; qq++) {
						//fprintf( ((CSlg2App*) AfxGetApp())->fhb, "%x", buff[qq]);
						if( !PutByteInCircleBuffer( buff[qq])) {
							m_ctlCOM.SetPortOpen( false);
							for( int err=0; err < ERROR_2; err++) {
								Beep( 100, 100);
								Sleep( 200);
							}
							app->m_nEmergencyCode = 1002;
							m_strLblEmergency.Format( _T("1002"));
							UpdateData( false);

							GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( false);
							//GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( false);
							GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( false);
							GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( false);
              GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( false);
							
							GetDlgItem( IDC_CMB_COM_PORT)->EnableWindow( true);
							GetDlgItem( IDC_EDT_K_IMP_SEC)->EnableWindow( true);
							m_ctlComButton.SetValue( false);

							m_ctlCOM.SetPortOpen( false);

							KillTimer( MY_TIMER_COM_FLUSH);
							KillTimer( MY_TIMER_1000);
		
				
							m_bStopBigThreadFlag = true;
		
							

							gl_nCircleBufferGet = 0;
							gl_nCircleBufferPut = 0;

							gl_dGlobalTime = 0.;

							gl_avgW.CommonReset();
              gl_avgI1.CommonReset();
							gl_avgI2.CommonReset();
							gl_avgVpc.CommonReset();
							gl_avgAmplAlt.CommonReset();
              gl_avgAmplDus.CommonReset();
              gl_avgAmplRULA.CommonReset();
							gl_avgT1.CommonReset();
							gl_avgT2.CommonReset();
							gl_avgTsa.CommonReset();

              theApp.m_tpW->ResetUnder();
              theApp.m_tpI1->ResetUnder();
              theApp.m_tpI2->ResetUnder();
              theApp.m_tpVpc->ResetUnder();
							theApp.m_tpAmplAngAlt->ResetUnder();
              theApp.m_tpAmplAngDus->ResetUnder();
              theApp.m_tpAmplAngRULA->ResetUnder();
							theApp.m_tpT1->ResetUnder();
							theApp.m_tpT2->ResetUnder();
              theApp.m_tpT3->ResetUnder();
							theApp.m_tpTsaMcs->ResetUnder();
              theApp.m_tpTsaMs->ResetUnder();
              theApp.m_tpTsaHz->ResetUnder();
              theApp.m_tpDecCoeff->ResetUnder();
						}
					}

					delete buff;
				}
			}
			else {
				for( int err=0; err < ERROR_5; err++) {
					Beep( 100, 100);
					Sleep( 200);
				}
				app->m_nEmergencyCode = 1005;
				m_strLblEmergency.Format( _T("1005"));
				UpdateData( false);
			}
		}
		else {
			//пропуск первых 10 ЧТЕНИЙ!!!! не попаданий по таймеру, а ЧТЕНИЙ!
			short nBytes = m_ctlCOM.GetInBufferCount();
			if( nBytes > 10) {

				KillTimer( MY_TIMER_INPUT_DATA);
			
				SYSTEMTIME sysTime;  // Win32 time information
				GetLocalTime(&sysTime);
				CString strStatFileName;

        /*
				strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d.txt"),
													sysTime.wYear, sysTime.wMonth, sysTime.wDay,
													sysTime.wHour, sysTime.wMinute, sysTime.wSecond);*/
				/*if( app->fh == NULL)
					app->fh = fopen( strStatFileName, "w");*/
				/*else
					AfxMessageBox( _T("Попытка открыть файл при открытом дескрипторе."));*/

        if( theApp.m_bDeviceSerialNumber)
				  strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d_bin.dev%03d.raw"),
					  								sysTime.wYear, sysTime.wMonth, sysTime.wDay,
						  							sysTime.wHour, sysTime.wMinute, sysTime.wSecond, theApp.m_nDeviceSerialNumber);
        else
          strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d.raw"),
					  								sysTime.wYear, sysTime.wMonth, sysTime.wDay,
						  							sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

				if( app->fhNew == NULL)
					app->fhNew = fopen( app->strDirName + strStatFileName, "wb");

				m_nPointsSkipped++;
				m_ctlCOM.SetInputLen( 0);
			}
		}
	}
	

	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//событие 1000 мсек
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_TIMER_1000) {

    //обновление серийного номера
    if( theApp.m_bDeviceSerialNumber) {
      CString strDevSerialNumber;
      strDevSerialNumber.Format( _T("%d"), theApp.m_nDeviceSerialNumber);
      GetDlgItem( IDC_LBL_DEVICE_SERIAL_NUMBER)->SetWindowText( strDevSerialNumber);
    }
    else {
      GetDlgItem( IDC_LBL_DEVICE_SERIAL_NUMBER)->SetWindowText( _T("-"));
    }

    //окошки ошибок по маркеру и чексумме
    m_strMarkerFails.Format( _T("%d"), theApp.m_nMarkerFails);
    m_strCheckSummFails.Format( _T("%d"), theApp.m_nCheckSummFails);
    m_strCounterFails.Format( _T("%d"), theApp.m_nCounterFails);

		//доступность кнопки "Экспорт"
		GetDlgItem( IDC_BTN_EXPORT)->EnableWindow( gl_avgW.Get_100ms()->GetCounter());

		//обновление на экране версии прошивки аппарата
    m_strSoftwareVersion = app->m_strSoftwareVer;
		
		//////////////////////////////////////////////////////////////////////
		// Обновляем управленческие параметры
		//////////////////////////////////////////////////////////////////////
		m_strParam1Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam1 * m_dKimpSec);		      //код амплитуды
		m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											            //код такта подставки
		m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		            //коэффициент M
		m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		            //начальная мода
		//m_strParam5Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
		//m_strParam6Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
		//m_strParam7Val.Format( _T("%.2f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.0));
		m_strParam8Val.Format( _T("%.5f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);   //коэффициент вычета
		//m_strParam9Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_shSignCoeff);
		//m_strParam10Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_shPhaseShift);
		
		if( (( CSlg2App *) AfxGetApp())->m_shFlashT1 >= MIN_T_THERMO_CALIBRATION &&
				(( CSlg2App *) AfxGetApp())->m_shFlashT1 <= MAX_T_THERMO_CALIBRATION) {
			m_strThermoCalib_T1.Format( _T("T1 = %d"), (( CSlg2App *) AfxGetApp())->m_shFlashT1);
		}
		else
			m_strThermoCalib_T1 = _T("T1 = (NO)");

		if( (( CSlg2App *) AfxGetApp())->m_shFlashT2 >= MIN_T_THERMO_CALIBRATION &&
				(( CSlg2App *) AfxGetApp())->m_shFlashT2 <= MAX_T_THERMO_CALIBRATION) {
			m_strThermoCalib_T2.Format( _T("T2 = %d"), (( CSlg2App *) AfxGetApp())->m_shFlashT2);
		}
		else
			m_strThermoCalib_T2 = _T("T2 = (NO)");

    //sync/async - regime  
    switch( theApp.m_bSyncAsync) {
      case 0:  GetDlgItem( IDC_LBL_REGIME_SYNC_ASYNC)->SetWindowText( "Синхронный режим"); break;
      case 1:  GetDlgItem( IDC_LBL_REGIME_SYNC_ASYNC)->SetWindowText( "Асинхронный режим"); break;
      default: GetDlgItem( IDC_LBL_REGIME_SYNC_ASYNC)->SetWindowText( ""); break;
    }
    
    
    //dNdU / w - regime
    switch( theApp.m_bWdNdU) {
      case 0:  GetDlgItem( IDC_LBL_REGIME_DNDU_W)->SetWindowText( "Прибор выдаёт приращения угла"); break;
      case 1:  GetDlgItem( IDC_LBL_REGIME_DNDU_W)->SetWindowText( "Прибор выдаёт dNdU"); break;
      default: GetDlgItem( IDC_LBL_REGIME_DNDU_W)->SetWindowText( ""); break;
    }    
    
    //число 3kV импульсов поджига в этом запуске
    CString strHvAppliesThisRun;
    if( theApp.m_nHvAppliesThisRun != -1) {
      strHvAppliesThisRun.Format( _T("3kV импульсов в этом запуске: %d"), theApp.m_nHvAppliesThisRun);
    }
    else
      strHvAppliesThisRun.Format( _T("3kV импульсов в этом запуске: --"));

    //lock bit
    if( theApp.m_bLockBit) {      
      //Прибор залочен
      GetDlgItem( IDC_LBL_HV_APPLIES_THIS_TIME)->SetWindowText( "Прибор в пользовательском режиме");
    }
    else {
      GetDlgItem( IDC_LBL_HV_APPLIES_THIS_TIME)->SetWindowText( strHvAppliesThisRun);
    }

    GetDlgItem( IDC_BTN_REQ_HV_APPLIES)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

    GetDlgItem( IDC_PARAM1_TITLE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_REQ_AMPL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_PARAM1_VAL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM1_NEDT)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_MC_TO_OUT_AMPLITUDE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM1_BTN)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

		GetDlgItem( IDC_PARAM2_TITLE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_REQ_TACTCODE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM2_VAL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM2_NEDT)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_MC_TO_OUT_TACTCODE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM2_BTN)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

		GetDlgItem( IDC_PARAM3_TITLE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_REQ_M_COEFF)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM3_VAL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM3_NEDT)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_MC_TO_OUT_MCOEFF)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM3_BTN)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

		GetDlgItem( IDC_PARAM4_TITLE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_REQ_START_MODE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM4_VAL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM4_NEDT)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_MC_TO_OUT_STARTMODE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM4_BTN)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

    GetDlgItem( IDC_PARAM8_TITLE)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_REQ_DEC_COEFF)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM8_VAL)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM8_NEDT)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_MC_TO_OUT_DECCOEFF)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_PARAM8_BTN)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);

    GetDlgItem( IDC_BTN_SAVE_PARAMS)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
	  //GetDlgItem( IDC_BTN_RESTORE_PARAMS)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
		GetDlgItem( IDC_BTN_LASER_OFF)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->ShowWindow( theApp.m_bLockBit ? SW_HIDE : SW_SHOW);
    

    //кнопки специфичные для асинх режима
    if( theApp.m_bSyncAsync == 2) {
      GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( FALSE);
      GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( FALSE);
    }
    else {
      GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( m_ctlCOM.GetPortOpen() && theApp.m_bSyncAsync && theApp.m_bWdNdU);
      GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( m_ctlCOM.GetPortOpen() && theApp.m_bSyncAsync);
    }

		//////////////////////////////////////////////////////////////////////
		// обработка выявления ошибки из протокола
		//////////////////////////////////////////////////////////////////////
		if( (( CSlg2App *) AfxGetApp())->m_nEmergencyCode != 0 && !m_bEmergencyCodeApperance) {
			m_bEmergencyCodeApperance = true;
			for( int err = 0; err < ERROR_1; err++) {
				Beep( 100, 100);
				Sleep( 200);
			}

			/*
      CDC *dc = GetDlgItem( IDC_EMERGENCY_LBL)->GetDC();
			dc->SetBkColor( RGB( 120, 0, 0));
			dc->FloodFill( 10, 10, RGB( 120, 0, 0));
			GetDlgItem( IDC_EMERGENCY_LBL)->ReleaseDC( dc);
      */
			GetDlgItem( IDC_EMERGENCY_LBL)->SetFont( &m_pFont);
		}

		m_strLblEmergency.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_nEmergencyCode);

		//////////////////////////////////////////////////////////////////////
		// Обновляем графики
		//////////////////////////////////////////////////////////////////////
		RefreshGraphs();

		//////////////////////////////////////////////////////////////////////
		//проверка времени запуска (режим запуска на определённое время
		//////////////////////////////////////////////////////////////////////
		double dMeasTime = m_ctlNedtMeasTime.GetValue();
		if( dMeasTime > 0.5) {
			if( gl_dGlobalTime >= dMeasTime) {
				KillTimer( MY_TIMER_INPUT_DATA);
				KillTimer( MY_TIMER_COM_FLUSH);
				KillTimer( MY_TIMER_1000);
				m_bStopBigThreadFlag = true;

				if( m_ctlCOM.GetPortOpen())
					m_ctlCOM.SetPortOpen( false);
	
				GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( false);
				//GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( false);
				GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( false);
				GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( false);
        GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( false);
        
				
				GetDlgItem( IDC_CMB_COM_PORT)->EnableWindow( true);
				GetDlgItem( IDC_EDT_K_IMP_SEC)->EnableWindow( true);
				m_ctlComButton.SetValue( false);
				m_ctlBtnCwStart.SetValue( false);		
		
				gl_nCircleBufferGet = 0;
				gl_nCircleBufferPut = 0;

				gl_dGlobalTime = 0.;

				gl_avgW.CommonReset();
        gl_avgI1.CommonReset();
				gl_avgI2.CommonReset();
				gl_avgVpc.CommonReset();
				gl_avgAmplAlt.CommonReset();
        gl_avgAmplDus.CommonReset();
        gl_avgAmplRULA.CommonReset();
				gl_avgT1.CommonReset();
				gl_avgT2.CommonReset();
				gl_avgTsa.CommonReset();
				

        theApp.m_tpW->ResetUnder();
        theApp.m_tpI1->ResetUnder();
        theApp.m_tpI2->ResetUnder();
        theApp.m_tpVpc->ResetUnder();
				theApp.m_tpAmplAngAlt->ResetUnder();
        theApp.m_tpAmplAngDus->ResetUnder();
        theApp.m_tpAmplAngRULA->ResetUnder();
				theApp.m_tpT1->ResetUnder();
				theApp.m_tpT2->ResetUnder();
        theApp.m_tpT3->ResetUnder();
				theApp.m_tpTsaMcs->ResetUnder();
        theApp.m_tpTsaMs->ResetUnder();
        theApp.m_tpTsaHz->ResetUnder();
        theApp.m_tpDecCoeff->ResetUnder();

				if( app->fhNew != NULL)
					fclose( app->fhNew);

				app->fhNew = NULL;
			}
		}
	}

  if( nIDEvent == MY_TIMER_POLLER) {
    if( m_ctlCOM.GetPortOpen()) {
        if( m_queCommandQueue.size() < 5) {          
          CMcCommandItem *item = new CMcCommandItem(
              MC_COMMAND_REQ,
              m_nPollParams[ m_nPollCounter],
              0, 0);
          m_queCommandQueue.push( item);
          m_nPollCounter = ( ++m_nPollCounter) % POLL_PARAMS_LEN;
        }
      }
      else {
        KillTimer( MY_TIMER_POLLER);
      }
  }

	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_MAXIMIZE_VIEW_TIMER) {
		(( CSlg2App *) AfxGetApp())->GetMainWnd()->ShowWindow( SW_MAXIMIZE);
		KillTimer( MY_MAXIMIZE_VIEW_TIMER);
	}
	
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_SEND_BUTTONS_BLOCK_TIMER) {
		SetSendButtonsState( TRUE);
		KillTimer( MY_SEND_BUTTONS_BLOCK_TIMER);
	}
	
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_TIMER_LOAD_FLASH_PARAMS) {
		//QueueCommandToMc( MC_COMMAND_ACT_RELOAD_FLASH_PARAMS, 4, 0, 0);
    //SendCommandToMc
		KillTimer( MY_TIMER_LOAD_FLASH_PARAMS);
	}

	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	if( nIDEvent == MY_TIMER_SWITCH_AS) {
    //QueueCommandToMc( MC_COMMAND_ACT_RELOAD_FLASH_PARAMS, 4, 0, 0);
		//SendCommandToMc( 10, 0, 0);
		KillTimer( MY_TIMER_SWITCH_AS);
	}

  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
	//**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  if( nIDEvent == TIMER_SEND_CMDS_TO_MC) {
    if( m_queCommandQueue.size() > 0) {
      //SetControlsState( FALSE, FALSE);
      CMcCommandItem *item = ( CMcCommandItem *) m_queCommandQueue.front();
      m_queCommandQueue.pop();
      SendCommandToMc( item->m_nCommand, item->m_nParam1, item->m_nParam2, item->m_nParam3);
      delete item;
    }
    else {
      //SetControlsState( m_ctlCOM.GetPortOpen(), !m_ctlCOM.GetPortOpen());
    }
  }

  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  if( nIDEvent == MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS) {
    if( theApp.GetSettings()->GetControlButtons()) {
      if( theApp.m_shSignCoeff) {
        m_ctlNedtParam1.SetValue( ( double) theApp.m_btParam1 * m_dKimpSec);
        m_ctlNedtParam2.SetValue( theApp.m_btParam2);
        m_ctlNedtParam3.SetValue( ( double) theApp.m_btParam3 / 250.);
        m_ctlNedtParam4.SetValue( 0.01 * ( double) theApp.m_btParam4);
        //m_ctlNedtParam5.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
        //m_ctlNedtParam6.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
        //m_ctlNedtParam7.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.);
        m_ctlNedtParam8.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff / 65535.);
        //m_ctlNedtParam9.SetValue( (( CSlg2App *) AfxGetApp())->m_shSignCoeff);
        //m_ctlNedtParam10.SetValue( (( CSlg2App *) AfxGetApp())->m_shPhaseShift);

        KillTimer( MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS);
      }
    }
  }

  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  //таймер таймаута потока входящих данных
  //**** **** **** **** **** **** **** **** **** **** **** **** **** **** ****
  if( nIDEvent == MY_TIMER_INPUT_DATA) {
    KillTimer( MY_TIMER_INPUT_DATA);
    KillTimer( MY_TIMER_COM_FLUSH);
    KillTimer( MY_TIMER_1000);

    if( m_ctlCOM.GetPortOpen())
      m_ctlCOM.SetPortOpen( false);

    for( int err=0; err < ERROR_6; err++) {
      Beep( 100, 100);
      Sleep( 200);
    }

    app->m_nEmergencyCode = 1006;
    m_strLblEmergency.Format( _T("1006"));
    UpdateData( false);

    AfxMessageBox( _T("Нет потока входящих данных от гироскопа."));

    GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( false);
    //GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( false);
    GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( false);
    GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( false);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( false);

    GetDlgItem( IDC_CMB_COM_PORT)->EnableWindow( true);
    GetDlgItem( IDC_EDT_K_IMP_SEC)->EnableWindow( true);
    m_ctlComButton.SetValue( false);
    m_ctlBtnCwStart.SetValue( false);

    if( m_ctlCOM.GetPortOpen())
      m_ctlCOM.SetPortOpen( false);

    m_bStopBigThreadFlag = true;

    gl_nCircleBufferGet = 0;
    gl_nCircleBufferPut = 0;

    gl_dGlobalTime = 0.;

    gl_avgW.CommonReset();
    gl_avgI1.CommonReset();
    gl_avgI2.CommonReset();
    gl_avgVpc.CommonReset();
    gl_avgAmplAlt.CommonReset();
    gl_avgAmplDus.CommonReset();
    gl_avgAmplRULA.CommonReset();
    gl_avgT1.CommonReset();
    gl_avgT2.CommonReset();
    gl_avgTsa.CommonReset();


    theApp.m_tpW->ResetUnder();
    theApp.m_tpI1->ResetUnder();
    theApp.m_tpI2->ResetUnder();
    theApp.m_tpVpc->ResetUnder();
    theApp.m_tpAmplAngAlt->ResetUnder();
    theApp.m_tpAmplAngDus->ResetUnder();
    theApp.m_tpAmplAngRULA->ResetUnder();
    theApp.m_tpT1->ResetUnder();
    theApp.m_tpT2->ResetUnder();
    theApp.m_tpT3->ResetUnder();
    theApp.m_tpTsaMcs->ResetUnder();
    theApp.m_tpTsaMs->ResetUnder();
    theApp.m_tpTsaHz->ResetUnder();
    theApp.m_tpDecCoeff->ResetUnder();
  }

  CFormView::OnTimer(nIDEvent);
}

BEGIN_EVENTSINK_MAP(CMainView, CFormView)
  //{{AFX_EVENTSINK_MAP(CMainView)
  ON_EVENT(CMainView, IDC_GRAPH1, -600 /* Click */, OnClickGraph1, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH2, -600 /* Click */, OnClickGraph2, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH3, -600 /* Click */, OnClickGraph3, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH4, -600 /* Click */, OnClickGraph4, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH5, -600 /* Click */, OnClickGraph5, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH6, -600 /* Click */, OnClickGraph6, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH7, -600 /* Click */, OnClickGraph7, VTS_NONE)
  ON_EVENT(CMainView, IDC_GRAPH8, -600 /* Click */, OnClickGraph8, VTS_NONE)
  ON_EVENT(CMainView, IDC_CW_START, 1 /* ValueChanged */, OnValueChangedCwStart, VTS_BOOL)
  ON_EVENT(CMainView, IDC_COMM, 1 /* OnComm */, OnOnCommComm, VTS_NONE)
	ON_EVENT(CMainView, IDC_GRAPH1, -607 /* MouseUp */, OnMouseUpGraph1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH2, -607 /* MouseUp */, OnMouseUpGraph2, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH3, -607 /* MouseUp */, OnMouseUpGraph3, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH4, -607 /* MouseUp */, OnMouseUpGraph4, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH5, -607 /* MouseUp */, OnMouseUpGraph5, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH6, -607 /* MouseUp */, OnMouseUpGraph6, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH7, -607 /* MouseUp */, OnMouseUpGraph7, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CMainView, IDC_GRAPH8, -607 /* MouseUp */, OnMouseUpGraph8, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

CString CMainView::UnitsForYAxis( int nYAxisCmbResourceID) {
  CString strYaxis;
  int nSelectedDisplayableParam = ( ( CComboBox *) GetDlgItem( nYAxisCmbResourceID))->GetCurSel();
  switch( nSelectedDisplayableParam) {
    case 0:  strYaxis = _T("''/sec"); break;   //w, Угловая скорость, ["/sec]
    case 1:  strYaxis = _T("mA");     break;   //I1, Разрядный ток 1, [mA]
    case 2:  strYaxis = _T("mA");     break;   //I2, Разрядный ток 2, [mA]
    case 3:  strYaxis = _T("V");      break;   //Vrpc, Напряжение на пьезокрр., [V]
    case 4:  strYaxis = _T("\"");     break;   //Ampl_alt, Амплитуда колебаний (altera), ["]
    case 5:  strYaxis = _T("V");      break;   //Ampl_dus, Амплитуда колебаний (ДУС), [V]
    case 6:  strYaxis = _T("code");   break;   // ************* RULA, Задатчик амплитуды, []
    case 7:  strYaxis = _T("°C");     break;   //T1, Термодатчик 1, [°C]
    case 8:  strYaxis = _T("°C");     break;   //T2, Термодатчик 2, [°C]
    case 9:  strYaxis = _T("°C");     break;   //T3, Термодатчик 3, [°C]
    case 10: strYaxis = _T("°C");     break;   //dT12, Разница T1 T2, [°C]
    case 11: strYaxis = _T("°C");     break;   //dT13, Разница T1 T3, [°C]
    case 12: strYaxis = _T("°C");     break;   //dT23, Разница T2 T3, [°C]
    case 13: strYaxis = _T("msec");   break;   //dTsa, Время такта, [msec]
    case 14: strYaxis = _T("mcsec");  break;   //dTsa, Время такта, [mcsec]
    case 15: strYaxis = _T("Hz");     break;   //dTsa, Время такта, [Hz]
    case 16: strYaxis = _T("\"/V");   break;   //dc, Коэффициент вычета, ["/В]
  }
  return strYaxis;
}

CString CMainView::UnitsForXAxis( int nXAxisCmbResourceID) {
  CString strXaxis;
  int nSelectedDisplayableParam = ( ( CComboBox *) GetDlgItem( nXAxisCmbResourceID))->GetCurSel();
  switch( nSelectedDisplayableParam) {
    case 0:  strXaxis = _T("sec");    break;   //T, время измерения
    case 1:  strXaxis = _T("°C");     break;   //T1, Термодатчик 1, [°C]
    case 2:  strXaxis = _T("°C");     break;   //T2, Термодатчик 2, [°C]
    case 3:  strXaxis = _T("°C");     break;   //T3, Термодатчик 3, [°C]
    
  }
  return strXaxis;
}

void CMainView::OnClickGraph1()
{
  m_nMainGraph = 1;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH1_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH1_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph2() 
{
  m_nMainGraph = 2;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH2_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH2_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph3() 
{
  m_nMainGraph = 3;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH3_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH3_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph4() 
{
  m_nMainGraph = 4;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH4_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH4_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph5() 
{
  m_nMainGraph = 5;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH5_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH5_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph6() 
{
  m_nMainGraph = 6;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH6_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH6_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph7() 
{
  m_nMainGraph = 7;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH7_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH7_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnClickGraph8()
{
  UpdateData( TRUE);
  m_nMainGraph = 8;
  //m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
  CString strYaxisUnits = UnitsForYAxis( IDC_CMB_GRAPH8_Y);
  m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( strYaxisUnits);
  CString strXaxisUnits = UnitsForXAxis( IDC_CMB_GRAPH8_X);
  m_ctlMainGraph.GetAxes().Item( "XAxis").SetCaption( strXaxisUnits);
}

void CMainView::OnValueChangedCwStart(BOOL Value)
{
  m_nPointsSkipped = 0;
  UpdateData( TRUE);
  theApp.GetSettings()->SetScaleCoeff( m_dKimpSec);

  GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( Value);
  //GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( Value);	
  GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( Value);

  if( theApp.m_bSyncAsync == 2) {
    GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( FALSE);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( FALSE);
  }
  else {
    GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( Value && theApp.m_bSyncAsync);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( Value && theApp.m_bSyncAsync);
  }


  GetDlgItem( IDC_CMB_COM_PORT)->EnableWindow( !Value);
  GetDlgItem( IDC_COM_BAUDRATE)->EnableWindow( !Value);
  GetDlgItem( IDC_EDT_K_IMP_SEC)->EnableWindow( !Value);
  GetDlgItem( IDC_NEDT_MEAS_TIME)->EnableWindow( !Value);

  m_ctlComButton.SetValue( Value);



  if( Value) {
    CSlg2App *app = (CSlg2App *) AfxGetApp();
    //theApp.m_bDeviceSerialNumber = false;

    theApp.m_tpW->FreeUnder();
    theApp.m_tpI1->FreeUnder();
    theApp.m_tpI2->FreeUnder();
    theApp.m_tpVpc->FreeUnder();
    theApp.m_tpAmplAngAlt->FreeUnder();
    theApp.m_tpAmplAngDus->FreeUnder();
    theApp.m_tpAmplAngRULA->FreeUnder();
    theApp.m_tpT1->FreeUnder();
    theApp.m_tpT2->FreeUnder();
    theApp.m_tpT3->FreeUnder();
    theApp.m_tpTsaMcs->FreeUnder();
    theApp.m_tpTsaMs->FreeUnder();
    theApp.m_tpTsaHz->FreeUnder();
    theApp.m_tpDecCoeff->FreeUnder();


    delete theApp.m_tpW;          theApp.m_tpW   = NULL;
    delete theApp.m_tpI1;         theApp.m_tpI1  = NULL;
    delete theApp.m_tpI2;         theApp.m_tpI2  = NULL;
    delete theApp.m_tpVpc;        theApp.m_tpVpc = NULL;
    delete theApp.m_tpAmplAngAlt; theApp.m_tpAmplAngAlt = NULL;
    delete theApp.m_tpAmplAngDus; theApp.m_tpAmplAngDus = NULL;
    delete theApp.m_tpAmplAngRULA;theApp.m_tpAmplAngRULA = NULL;
    delete theApp.m_tpT1;         theApp.m_tpT1 = NULL;
    delete theApp.m_tpT2;         theApp.m_tpT2 = NULL;
    delete theApp.m_tpT3;         theApp.m_tpT3 = NULL;
    delete theApp.m_tpTsaMcs;     theApp.m_tpTsaMcs = NULL;
    delete theApp.m_tpTsaMs;      theApp.m_tpTsaMs = NULL;
    delete theApp.m_tpTsaHz;      theApp.m_tpTsaHz = NULL;
    delete theApp.m_tpDecCoeff;   theApp.m_tpDecCoeff = NULL;


    double dMeasTime = m_ctlNedtMeasTime.GetValue();
    int nArraySize = 0;
    if( dMeasTime > 0.5) {
      nArraySize = (int) ( ( dMeasTime + 1.) * 10.);
    }
    else {
      nArraySize = 1024;
    }

    theApp.m_tpW = new CTrackedParam( nArraySize);
    theApp.m_tpI1 = new CTrackedParam( nArraySize);
    theApp.m_tpI2 = new CTrackedParam( nArraySize);
    theApp.m_tpVpc = new CTrackedParam( nArraySize);
    theApp.m_tpAmplAngAlt = new CTrackedParam( nArraySize);
    theApp.m_tpAmplAngDus = new CTrackedParam( nArraySize);
    theApp.m_tpAmplAngRULA = new CTrackedParam( nArraySize);
    theApp.m_tpT1 = new CTrackedParam( nArraySize);
    theApp.m_tpT2 = new CTrackedParam( nArraySize);
    theApp.m_tpT3 = new CTrackedParam( nArraySize);
    theApp.m_tpTsaMcs = new CTrackedParam( nArraySize);
    theApp.m_tpTsaMs = new CTrackedParam( nArraySize);
    theApp.m_tpTsaHz = new CTrackedParam( nArraySize);
    theApp.m_tpDecCoeff = new CTrackedParam( nArraySize);

    theApp.m_nCheckSummFails = 0;
    theApp.m_nCounterFails = 0;
    theApp.m_nMarkerFails = 0;

    theApp.m_shFlashDecCoeff = 0;
    theApp.m_shSignCoeff = 0;
    theApp.m_nHvAppliesThisRun = -1;

    ( ( CSlg2App *) AfxGetApp())->StartThreads();

    if( m_ctlCOM.GetPortOpen())
      m_ctlCOM.SetPortOpen( false);

    m_ctlCOM.SetCommPort( 1 + m_nComPort);
    switch( m_nComPortBaudrate) {
      case 0: m_ctlCOM.SetSettings( _T("57600,N,8,1")); break;
      case 1: m_ctlCOM.SetSettings( _T("115200,N,8,1")); break;
      case 2: m_ctlCOM.SetSettings( _T("128000,N,8,1")); break;
      case 3: m_ctlCOM.SetSettings( _T("256000,N,8,1")); break;
      case 4: m_ctlCOM.SetSettings( _T("460800,N,8,1")); break;
      case 5: m_ctlCOM.SetSettings( _T("512000,N,8,1")); break;
      case 6: m_ctlCOM.SetSettings( _T("921600,N,8,1")); break;
    }
    m_ctlCOM.SetPortOpen( true);

    GetDlgItem( IDC_BTN_EXPORT)->EnableWindow( false);

    SetTimer( MY_TIMER_COM_FLUSH, 10, NULL);
    SetTimer( MY_TIMER_1000,	  1000, NULL);

    //опросник параметров
    m_nPollCounter = 0;
    SetTimer( MY_TIMER_POLLER, 1000, NULL);

    SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 1000, NULL);
    SetTimer( MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS, 2000, NULL);

    SetTimer( MY_TIMER_INPUT_DATA, 10000, NULL);

    m_bBtnCwStart = true;

    m_strLblEmergency.Format( _T("0"));
    app->m_nEmergencyCode = 0;
    UpdateData( false);
  }
  else {
    m_ctlCOM.SetPortOpen( false);

    KillTimer( MY_TIMER_COM_FLUSH);
    KillTimer( MY_TIMER_1000);
    KillTimer( MY_TIMER_POLLER);

    m_bStopBigThreadFlag = true;

    CSlg2App *app = ((CSlg2App *) AfxGetApp());

    /*
    if( app->fh != NULL)
      fclose( app->fh);
    app->fh = NULL;
    */

    if( app->fhNew != NULL)
      fclose( app->fhNew);

    app->fhNew = NULL;

    gl_nCircleBufferGet = 0;
    gl_nCircleBufferPut = 0;

    gl_dGlobalTime = 0.;

    gl_avgW.CommonReset();
    gl_avgI1.CommonReset();
    gl_avgI2.CommonReset();
    gl_avgVpc.CommonReset();
    gl_avgAmplAlt.CommonReset();
    gl_avgAmplDus.CommonReset();
    gl_avgAmplRULA.CommonReset();
    gl_avgT1.CommonReset();
    gl_avgT2.CommonReset();
    gl_avgTsa.CommonReset();


    theApp.m_tpW->ResetUnder();
    theApp.m_tpI1->ResetUnder();
    theApp.m_tpI2->ResetUnder();
    theApp.m_tpVpc->ResetUnder();
    theApp.m_tpAmplAngAlt->ResetUnder();
    theApp.m_tpAmplAngDus->ResetUnder();
    theApp.m_tpAmplAngRULA->ResetUnder();
    theApp.m_tpT1->ResetUnder();
    theApp.m_tpT2->ResetUnder();
    theApp.m_tpT3->ResetUnder();
    theApp.m_tpTsaMcs->ResetUnder();
    theApp.m_tpTsaMs->ResetUnder();
    theApp.m_tpTsaHz->ResetUnder();
    theApp.m_tpDecCoeff->ResetUnder();
  }

  UpdateData( false);
}

void CMainView::OnDestroy() 
{
  UpdateData( true);
  theApp.GetSettings()->SetScaleCoeff( m_dKimpSec);
  theApp.GetSettings()->SetComPort( m_nComPort);
  theApp.GetSettings()->SetComBaudrate( m_nComPortBaudrate);

  theApp.GetSettings()->GetGraphSettings(0)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(1)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(2)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(3)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(4)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(5)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(6)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_Y))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(7)->SetAxisY( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_Y))->GetCurSel());

  theApp.GetSettings()->GetGraphSettings(0)->SetMeaning( m_nRadGraph1);
  theApp.GetSettings()->GetGraphSettings(1)->SetMeaning( m_nRadGraph2);
  theApp.GetSettings()->GetGraphSettings(2)->SetMeaning( m_nRadGraph3);
  theApp.GetSettings()->GetGraphSettings(3)->SetMeaning( m_nRadGraph4);
  theApp.GetSettings()->GetGraphSettings(4)->SetMeaning( m_nRadGraph5);
  theApp.GetSettings()->GetGraphSettings(5)->SetMeaning( m_nRadGraph6);
  theApp.GetSettings()->GetGraphSettings(6)->SetMeaning( m_nRadGraph7);
  theApp.GetSettings()->GetGraphSettings(7)->SetMeaning( m_nRadGraph8);

  theApp.GetSettings()->GetGraphSettings(0)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH1_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(1)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH2_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(2)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH3_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(3)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH4_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(4)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH5_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(5)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH6_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(6)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH7_X))->GetCurSel());
  theApp.GetSettings()->GetGraphSettings(7)->SetAxisX( ( ( CComboBox *) GetDlgItem( IDC_CMB_GRAPH8_X))->GetCurSel());

  CFormView::OnDestroy();
  VERIFY( m_pFont.DeleteObject());
}

void CMainView::OnParam1Btn() 
{
	UpdateData( true);
	char byte = 0;
	
	double q = m_ctlNedtParam1.GetValue() / m_dKimpSec;
	byte = ( char) q;
	QueueCommandToMc( MC_COMMAND_SET, AMPLITUDE, byte, 0);
	
	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);	
}

void CMainView::OnParam2Btn() 
{
	UpdateData( true);

  QueueCommandToMc( MC_COMMAND_SET, TACT_CODE, ( char) m_ctlNedtParam2.GetValue(), 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

void CMainView::OnParam3Btn() 
{
	UpdateData( true);
	
	char byte = 0;
	double q = m_ctlNedtParam3.GetValue() * 250;
	double q1 = q - floor(q);
	if( q1 > 0.5) q = ceil( q);
	else q = floor( q);
	byte = ( char) q;

  QueueCommandToMc( MC_COMMAND_SET, M_COEFF, byte, 0);
  //SendCommandToMc( 2, byte, 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

void CMainView::OnParam4Btn() 
{
	UpdateData( true);
	
	char byte = 0;
	double q = m_ctlNedtParam4.GetValue() * 100;
	double q1 = q - floor(q);
	if( q1 > 0.5) q = ceil( q);
	else q = floor( q);
	byte = ( char) q;

  QueueCommandToMc( MC_COMMAND_SET, STARTMODE, byte, 0);
	//SendCommandToMc( 3, byte, 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

//DEL void CMainView::OnParam5Btn() 
//DEL {
//DEL 	UpdateData( true);
//DEL 	short val = ( short) ( m_ctlNedtParam5.GetValue() * 65535. / 0.75);
//DEL 	SendCommandToMc( 4, (char) ( val & 0xFF), (char) ( ( val & 0xFF00) >> 8));
//DEL 	
//DEL 	SetSendButtonsState( FALSE);
//DEL 	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
//DEL 	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
//DEL }

//DEL void CMainView::OnParam6Btn() 
//DEL {
//DEL 	UpdateData( true);
//DEL 	short val = ( short) ( m_ctlNedtParam6.GetValue() * 65535. / 0.75);
//DEL 	SendCommandToMc( 5, (char) ( val & 0xFF), (char) ( ( val & 0xFF00) >> 8));
//DEL 	
//DEL 	SetSendButtonsState( FALSE);
//DEL 	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
//DEL 	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
//DEL }

//DEL void CMainView::OnParam7Btn() 
//DEL {
//DEL 	UpdateData( true);
//DEL 	short val = ( short) ( m_ctlNedtParam7.GetValue() / 6. * 65535.);
//DEL 	SendCommandToMc( 6, (char) ( val & 0xFF), (char) ( ( val & 0xFF00) >> 8));
//DEL 	
//DEL 	SetSendButtonsState( FALSE);
//DEL 	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
//DEL 	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
//DEL }

void CMainView::OnParam8Btn() 
{
	UpdateData( true);
	short val = ( short) ( m_ctlNedtParam8.GetValue() * 65535.);


  QueueCommandToMc( MC_COMMAND_SET, DECCOEFF,
        ( char) ( val & 0xFF),
        ( char) ( ( val & 0xFF00) >> 8));
  //SendCommandToMc( 7, ( char) ( val & 0xFF), ( char) ( ( val & 0xFF00) >> 8));
	
	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

//DEL void CMainView::OnParam9Btn() 
//DEL {
//DEL 	UpdateData( true);
//DEL 	short val = ( short) m_ctlNedtParam9.GetValue();
//DEL 	val++;
//DEL 	SendCommandToMc( 8, (char) ( val & 0xFF), (char) ( ( val & 0xFF00) >> 8));
//DEL 	SetSendButtonsState( FALSE);
//DEL 	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
//DEL 	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
//DEL }

void CMainView::OnBtnSaveParams() 
{
	//SendCommandToMc( 50, 0, 0);
  QueueCommandToMc( MC_COMMAND_ACT_SAVE_FLASH_PARAMS, 0, 0, 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

void CMainView::SetSendButtonsState(bool bState)
{
	GetDlgItem(IDC_PARAM1_BTN)->EnableWindow( bState);
	GetDlgItem(IDC_PARAM2_BTN)->EnableWindow( bState);
	GetDlgItem(IDC_PARAM3_BTN)->EnableWindow( bState);
	GetDlgItem(IDC_PARAM4_BTN)->EnableWindow( bState);
	//GetDlgItem(IDC_PARAM5_BTN)->EnableWindow( bState);
	//GetDlgItem(IDC_PARAM6_BTN)->EnableWindow( bState);
	//GetDlgItem(IDC_PARAM7_BTN)->EnableWindow( bState);
	GetDlgItem(IDC_PARAM8_BTN)->EnableWindow( bState);
	//GetDlgItem(IDC_PARAM9_BTN)->EnableWindow( bState);

	GetDlgItem(IDC_BTN_SAVE_PARAMS)->EnableWindow( bState);
	//GetDlgItem(IDC_BTN_RESTORE_PARAMS)->EnableWindow( bState);
  
  GetDlgItem(IDC_BTN_SWITCH_W_DNDU)->EnableWindow( bState && theApp.m_bSyncAsync && ( theApp.m_bSyncAsync != 2));
  GetDlgItem(IDC_BTN_DEC_COEFF_CALC)->EnableWindow( bState && theApp.m_bSyncAsync && ( theApp.m_bSyncAsync != 2));
	
  GetDlgItem(IDC_BTN_THERMO_CALIB_SET)->EnableWindow( bState);
	GetDlgItem(IDC_BTN_THERMO_CALIB_CLEAR)->EnableWindow( bState);
}



void CMainView::OnBtnRestoreParams() 
{
  QueueCommandToMc( MC_COMMAND_ACT_RELOAD_FLASH_PARAMS, 4, 0, 0);
	//SendCommandToMc( 51, 0, 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOAD_FLASH_PARAMS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

void CMainView::OnKillfocusEdtKImpSec() 
{
  UpdateData( true);
  if( theApp.GetSettings()->GetControlButtons()) {
    m_ctlNedtParam1.SetDiscreteInterval( m_dKimpSec);
    m_ctlNedtParam1.SetIncDecValue( m_dKimpSec);
    m_ctlNedtParam1.SetAccelInc( m_dKimpSec * 5.);
    m_ctlNedtParam1.SetMaximum( m_dKimpSec * 255.);
  }
}

void CMainView::OnDecCoeffCalc() 
{
  if( gl_pDecCoeffCalcParams.m_bDecCoeffCalculation)
    return;

  //stop screen refresh and other timers
  //KillTimer( MY_TIMER_1000);

  gl_pDecCoeffCalcParams.Reset();

  //switch our thread to dn_dU input
  gl_pDecCoeffCalcParams.m_bDecCoeffCalculation = true;


  if( m_dlgDecCoeffCalc != NULL) {
    m_dlgDecCoeffCalc->ShowWindow( SW_SHOW);
  }
  else {
    m_dlgDecCoeffCalc = new CDecCoeffCalc( this);
    m_dlgDecCoeffCalc->Create( IDD_K_DEC_COEFF_CALC, this);
    m_dlgDecCoeffCalc->ShowWindow( SW_SHOW);
  }

}

void CMainView::OnOnCommComm() 
{
	short event = m_ctlCOM.GetCommEvent();
	
	if( event > 1000) {
		if( event == 1004) return;

		UpdateData( TRUE);
		theApp.GetSettings()->SetScaleCoeff( m_dKimpSec);
		theApp.GetSettings()->SetComPort( m_nComPort);

		GetDlgItem( IDC_BTN_SAVE_PARAMS)->EnableWindow( false);
		//GetDlgItem( IDC_BTN_RESTORE_PARAMS)->EnableWindow( false);
		GetDlgItem( IDC_BTN_DEC_COEFF_CALC)->EnableWindow( false);
		GetDlgItem( IDC_BTN_LASER_OFF)->EnableWindow( false);
    GetDlgItem( IDC_BTN_SWITCH_W_DNDU)->EnableWindow( false);
		
		GetDlgItem( IDC_CMB_COM_PORT)->EnableWindow( true);
		GetDlgItem( IDC_EDT_K_IMP_SEC)->EnableWindow( true);
		m_ctlComButton.SetValue( false);

		m_ctlCOM.SetPortOpen( false);
		
		KillTimer( MY_TIMER_1000);
		KillTimer( MY_TIMER_COM_FLUSH);
				
		m_bStopBigThreadFlag = true;
		
		CSlg2App *app = ((CSlg2App *) AfxGetApp());

		gl_nCircleBufferGet = 0;
		gl_nCircleBufferPut = 0;

		gl_dGlobalTime = 0.;

		gl_avgW.CommonReset();
    gl_avgI1.CommonReset();
		gl_avgI2.CommonReset();
		gl_avgVpc.CommonReset();
		gl_avgAmplAlt.CommonReset();
    gl_avgAmplDus.CommonReset();
    gl_avgAmplRULA.CommonReset();
		gl_avgT1.CommonReset();
		gl_avgT2.CommonReset();
		gl_avgTsa.CommonReset();

    theApp.m_tpW->ResetUnder();
    theApp.m_tpI1->ResetUnder();
    theApp.m_tpI2->ResetUnder();
    theApp.m_tpVpc->ResetUnder();
    theApp.m_tpAmplAngAlt->ResetUnder();
    theApp.m_tpAmplAngDus->ResetUnder();
    theApp.m_tpAmplAngRULA->ResetUnder();
		theApp.m_tpT1->ResetUnder();
		theApp.m_tpT2->ResetUnder();
    theApp.m_tpT3->ResetUnder();
		theApp.m_tpTsaMcs->ResetUnder();
    theApp.m_tpTsaMs->ResetUnder();
    theApp.m_tpTsaHz->ResetUnder();
    theApp.m_tpDecCoeff->ResetUnder();
	
		m_ctlBtnCwStart.SetValue( false);
		m_bBtnCwStart = false;
		UpdateData( false);
		
		for( int err=0; err<ERROR_4; err++) {
			Beep( 100, 100);
			Sleep( 200);
		}
		m_strLblEmergency.Format( _T("1004"));
		app->m_nEmergencyCode = 1004;
		UpdateData( false);
		
		switch( event) {
			//1001 - Получен сигнал Break.
			case 1001:
				AfxMessageBox( _T("Получен сигнал Break."));
			break;

	    //1002 - Тайм-аут по сигналу CTS. При попытке передать символ сигнал Clear To Send был сброшен в течение CTSTimeout миллисекунд.
			case 1002:
				AfxMessageBox( _T("Тайм-аут по сигналу CTS. При попытке передать символ сигнал Clear To Send был сброшен в течение CTSTimeout миллисекунд."));
			break;

	    //1003 - Тайм-аут по сигналу DSR. При попытке передать символ сигнал Data Set Ready была сброшен в течение DSRTimeout миллисекунд.
			case 1003:
				AfxMessageBox( _T("Тайм-аут по сигналу DSR. При попытке передать символ сигнал Data Set Ready была сброшен в течение DSRTimeout миллисекунд."));
			break;

			
	    //1004 - Ошибка кадра (Framing Error). Аппаратные средства зафиксировали ошибку в последовательности кадров.
			case 1004:
				AfxMessageBox( _T("Ошибка кадра (Framing Error). Аппаратные средства зафиксировали ошибку в последовательности кадров."));
			break;
			
	    //1006 - Переполнение порта (Port Overrun). Символ еще не был считан из аппаратуры, как уже прибыл и был утерян следующий символ. Если у вас возникла эта ошибка при работе под Windows 3.0, уменьшите значение свойства Interval. Подробнее см. описание свойства Interval ниже в данной главе.
		  case 1006:
				AfxMessageBox( _T("Переполнение порта (Port Overrun). Символ еще не был считан из аппаратуры, как уже прибыл и был утерян следующий символ. Если у вас возникла эта ошибка при работе под Windows 3.0, уменьшите значение свойства Interval. Подробнее см. описание свойства Interval ниже в данной главе."));
			break;

			//1007 - Тайм-аут по сигналу CD. При попытке передать символ сигнал Carrier Detect был сброшен в течение CDTimeout миллисекунд. Сигнал Carrier Detect имеет еще одно название Receive Line Signal Detect (RLSD).
			case 1007:
				AfxMessageBox( _T("Тайм-аут по сигналу CD. При попытке передать символ сигнал Carrier Detect был сброшен в течение CDTimeout миллисекунд. Сигнал Carrier Detect имеет еще одно название Receive Line Signal Detect (RLSD)."));
			break;

			//1008 - Переполнение буфера приема (Receive Buffer Overflow). В буфере приема не осталось свободного места.
			case 1008:
				AfxMessageBox( _T("Переполнение буфера приема (Receive Buffer Overflow). В буфере приема не осталось свободного места."));
			break;

	    //1009 - Ошибка четности (Parity Error). Аппаратные средства зафиксировали ошибку четности.
			case 1009:
				AfxMessageBox( _T("Ошибка четности (Parity Error). Аппаратные средства зафиксировали ошибку четности."));
			break;

			//1010 - Переполнение буфера передачи (Transmit Buffer Full). Буфер передачи оказался заполненным при попытке поставить символ в очередь.
			case 1010:
				AfxMessageBox( _T("Переполнение буфера передачи (Transmit Buffer Full). Буфер передачи оказался заполненным при попытке поставить символ в очередь."));
			break;
		}		
	}
}

void CMainView::SendCommandToMc(BYTE b1, BYTE b2, BYTE b3, BYTE b4)
{
	char str[4];
	str[0] = b1;
	str[1] = b2;
	str[2] = b3;
  str[3] = b4;

	SAFEARRAY *psa;
  SAFEARRAYBOUND rgsabound[1];
  rgsabound[0].lLbound = 0;
  rgsabound[0].cElements = 4;
  psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
	memcpy( psa->pvData, str, 4);
	
	VARIANT var;
	var.vt = VT_ARRAY | VT_UI1;
	var.parray = psa;

	if( m_ctlCOM.GetPortOpen()) {
		m_ctlCOM.SetOutput( var);
		int s = m_ctlCOM.GetOutBufferCount();
	}
	else {
		Beep( 5000, 100);
	}

	SafeArrayDestroy( psa);

}

void CMainView::OnBtnThermoCalibSet() 
{
	UpdateData( true);
	short val = ( short) m_ctlNedtThermoCalibTemperature.GetValue();
	val += THERMO_CALIB_PARAMS_BASE;

  QueueCommandToMc( MC_COMMAND_ACT_T_CALIBRATION, 
        ( char) ( val & 0xFF),
        ( char) ( ( val & 0xFF00) >> 8),
        0);
	//SendCommandToMc( 11, ( char) ( val & 0xFF), ( char) ( ( val & 0xFF00) >> 8));
	
	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS, 4000, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 5000, NULL);
}

void CMainView::OnBtnThermoCalibClear() 
{
  QueueCommandToMc( MC_COMMAND_ACT_RESET_T_CALIB, 0, 0, 0);
	//SendCommandToMc( 12, 0, 0);
	
	SetSendButtonsState( FALSE);
	SetTimer( MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS, 500, NULL);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}

//DEL void CMainView::OnParam10Btn() 
//DEL {
//DEL 	UpdateData( true);
//DEL 	short val = ( short) m_ctlNedtParam10.GetValue();
//DEL 	
//DEL 	SendCommandToMc( 14, (char) ( val & 0xFF), (char) ( ( val & 0xFF00) >> 8));
//DEL 	
//DEL 	SetSendButtonsState( FALSE);
//DEL 	SetTimer( MY_TIMER_LOADED_FLASH_PARAMS_TO_WNDS, 500, NULL);
//DEL 	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
//DEL }

void CMainView::OnBtnLaserOff() 
{
  QueueCommandToMc( MC_COMMAND_ACT_LASER_OFF, 0, 0, 0);
	//SendCommandToMc( 15, 0, 0);

	SetSendButtonsState( FALSE);
	SetTimer( MY_SEND_BUTTONS_BLOCK_TIMER, 1000, NULL);
}


void CMainView::OnBtnIntegrOff() 
{
  //2014-09-10.Выключение интегратора
  QueueCommandToMc( MC_COMMAND_ACT_INTEGR_OFF, 0, 0, 0);
  //SendCommandToMc( 16, 0, 0);
}

void CMainView::OnBtnIntegrOn() 
{
  //2014-09-10.Включение интегратора
  QueueCommandToMc( MC_COMMAND_ACT_INTEGR_ON, 0, 0, 0);
  //SendCommandToMc( 17, 0, 0);
}

void CMainView::OnBtnIntegrReset() 
{
  //2014-09-10.Сброс интегратора
  QueueCommandToMc( MC_COMMAND_ACT_INTEGR_RESET, 0, 0, 0);
  //SendCommandToMc( 18, 0, 0);
}


/*
void CMainView::OnRadT1Td1() 
{
	m_ctlSmallGraph6.SetCaption( _T("Термодатчик 1, [°C]"));
}

void CMainView::OnRadT1Td2() 
{
	m_ctlSmallGraph6.SetCaption( _T("Термодатчик 2, [°C]"));
}

void CMainView::OnRadT1Td3() 
{
	m_ctlSmallGraph6.SetCaption( _T("Термодатчик 3, [°C]"));
}

void CMainView::OnRadT2Td1() 
{
	m_ctlSmallGraph7.SetCaption( _T("Термодатчик 1, [°C]"));
}

void CMainView::OnRadT2Td2() 
{
	m_ctlSmallGraph7.SetCaption( _T("Термодатчик 2, [°C]"));
}

void CMainView::OnRadT2Td3() 
{
	m_ctlSmallGraph7.SetCaption( _T("Термодатчик 3, [°C]"));
}

void CMainView::OnRadTsaMcs() 
{
  UpdateData( TRUE);
	m_ctlSmallGraph8.SetCaption( _T("Время такта, [мксек]"));
  if( m_nMainGraph == 8) {
    switch( m_nTsaRadSelection) {
      case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мксек")); break;
      case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек"));  break;
      case 2: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("Гц"));    break;
    }
  }
}

void CMainView::OnRadTsaMs() 
{
  UpdateData( TRUE);
	m_ctlSmallGraph8.SetCaption( _T("Время такта, [мсек]"));
  if( m_nMainGraph == 8) {
    switch( m_nTsaRadSelection) {
      case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мксек")); break;
      case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек"));  break;
      case 2: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("Гц"));    break;
    }
  }
}

void CMainView::OnRadTsaHz() 
{
  UpdateData( TRUE);
	m_ctlSmallGraph8.SetCaption( _T("Время такта, [Гц]"));
  if( m_nMainGraph == 8) {
    switch( m_nTsaRadSelection) {
      case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мксек")); break;
      case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек"));  break;
      case 2: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("Гц"));    break;
    }
  }
}
*/
void CMainView::OnBtnReqVersion() 
{
  /*for( int i=0; i<100; i++) {
    gl_avgTsa.CommonAddPoint( i);
    
    if( (i%10) == 0)
      gl_avgTsa.Get_100ms()->Reset();
    
    if( (i%20) == 0)
      gl_avgTsa.Get_1s()->Reset();

    if( (i%30) == 0)
      gl_avgTsa.Get_10s()->Reset();

    if( (i%50) == 0)
      gl_avgTsa.Get_100s()->Reset();

    CString strTmp;
    strTmp.Format( "NPoints: 100ms=%d 1s=%d 10s=%d 100s=%d\n",
          gl_avgTsa.Get_100ms()->GetCounter(), gl_avgTsa.Get_1s()->GetCounter(),
          gl_avgTsa.Get_10s()->GetCounter(), gl_avgTsa.Get_100s()->GetCounter());
    OutputDebugString( strTmp);

    strTmp.Format( "Summ: 100ms=%f 1s=%f 10s=%f 100s=%f\n\n",
          gl_avgTsa.Get_100ms()->GetSumm(), gl_avgTsa.Get_1s()->GetSumm(),
          gl_avgTsa.Get_10s()->GetSumm(), gl_avgTsa.Get_100s()->GetSumm());
    OutputDebugString( strTmp);
  }*/

  QueueCommandToMc( MC_COMMAND_REQ, VERSION, 0, 0);
}

void CMainView::QueueCommandToMc(char btCmd, char btParam1, char btParam2, char btParam3)
{
  m_queCommandQueue.push( new CMcCommandItem( btCmd, btParam1, btParam2, btParam3));
}

void CMainView::OnBtnReqAmpl() 
{
	QueueCommandToMc( MC_COMMAND_REQ, AMPLITUDE, 0, 0);
}

void CMainView::OnBtnReqTactcode() 
{
	QueueCommandToMc( MC_COMMAND_REQ, TACT_CODE, 0, 0);
}

void CMainView::OnBtnReqMCoeff() 
{
	QueueCommandToMc( MC_COMMAND_REQ, M_COEFF, 0, 0);
}

void CMainView::OnBtnReqStartMode() 
{
	QueueCommandToMc( MC_COMMAND_REQ, STARTMODE, 0, 0);
}

void CMainView::OnBtnReqDecCoeff() 
{
	QueueCommandToMc( MC_COMMAND_REQ, DECCOEFF, 0, 0);
}

void CMainView::OnBtnMcToOutAmplitude() 
{
  m_ctlNedtParam1.SetValue( theApp.m_btParam1 * theApp.GetSettings()->GetScaleCoeff());
}

void CMainView::OnBtnMcToOutTactcode() 
{
	m_ctlNedtParam2.SetValue( theApp.m_btParam2);
}

void CMainView::OnBtnMcToOutMcoeff() 
{
  m_ctlNedtParam3.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);	
}

void CMainView::OnBtnMcToOutStartmode() 
{
	m_ctlNedtParam4.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);
}

void CMainView::OnBtnMcToOutDeccoeff() 
{
	m_ctlNedtParam8.SetValue( ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);
}

void CMainView::OnBtnSwitchWDndu() 
{
  if( theApp.m_bSyncAsync == 1) {
    if( theApp.m_bWdNdU == 0)
      QueueCommandToMc( MC_COMMAND_ACT_SWC_DW_DNDU_OUTPUT, 1, 0, 0);
    else if( theApp.m_bWdNdU == 1)
      QueueCommandToMc( MC_COMMAND_ACT_SWC_DW_DNDU_OUTPUT, 0, 0, 0);
  }
}

void CMainView::OnBtnReset() 
{
  m_nPointsSkipped = 0;

  theApp.m_tpW->ResetUnder();
  theApp.m_tpI1->ResetUnder();
  theApp.m_tpI2->ResetUnder();
  theApp.m_tpVpc->ResetUnder();
  theApp.m_tpAmplAngAlt->ResetUnder();
  theApp.m_tpAmplAngDus->ResetUnder();
  theApp.m_tpAmplAngRULA->ResetUnder();
	theApp.m_tpT1->ResetUnder();
	theApp.m_tpT2->ResetUnder();
  theApp.m_tpT3->ResetUnder();
  theApp.m_tpTsaMcs->ResetUnder();
  theApp.m_tpTsaMs->ResetUnder();
  theApp.m_tpTsaHz->ResetUnder();
  theApp.m_tpDecCoeff->ResetUnder();

  theApp.m_nCheckSummFails = 0;
  theApp.m_nMarkerFails = 0;
  theApp.m_nCounterFails = 0;

  gl_nCircleBufferGet = 0;
	gl_nCircleBufferPut = 0;

	gl_dGlobalTime = 0.;
	gl_avgW.CommonReset();
  gl_avgI1.CommonReset();
	gl_avgI2.CommonReset();
	gl_avgVpc.CommonReset();
	gl_avgAmplAlt.CommonReset();
  gl_avgAmplDus.CommonReset();
  gl_avgAmplRULA.CommonReset();
	gl_avgT1.CommonReset();
	gl_avgT2.CommonReset();
	gl_avgTsa.CommonReset();
	
}

//DEL void CMainView::OnRadAaImp() 
//DEL {
//DEL 	m_ctlSmallGraph5.SetCaption( _T("Ампл. част. подст., ['']"));
//DEL }

//DEL void CMainView::OnRadAaDus() 
//DEL {
//DEL 	m_ctlSmallGraph5.SetCaption( _T("Ампл. с ДУСа, [В]"));
//DEL }

void CMainView::OnBtnReqHvApplies()
{
  QueueCommandToMc( MC_COMMAND_REQ, HV_APPLY_COUNT_TR , 0, 0);
}

void CMainView::OnBtnReqSn()
{
  theApp.m_bDeviceSerialNumber = false;
  GetDlgItem( IDC_LBL_DEVICE_SERIAL_NUMBER)->SetWindowText( _T("-"));
  if( m_ctlCOM.GetPortOpen())
    QueueCommandToMc( MC_COMMAND_REQ, DEVNUM, 0, 0);
}

void CMainView::OnRadMeaning1()
{
  GetDlgItem( IDC_RAD_G1_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T1)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T1)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T1)->SendMessage( WM_LBUTTONUP);
}

void CMainView::OnRadMeaning2()
{
  GetDlgItem( IDC_RAD_G1_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T2)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T2)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T2)->SendMessage( WM_LBUTTONUP);
}

void CMainView::OnRadMeaning3() 
{
  GetDlgItem( IDC_RAD_G1_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T3)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T3)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T3)->SendMessage( WM_LBUTTONUP);
}

void CMainView::OnRadMeaning4() 
{
  GetDlgItem( IDC_RAD_G1_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G1_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G2_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G2_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G3_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G3_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G4_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G4_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G5_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G5_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G6_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G6_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G7_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G7_T4)->SendMessage( WM_LBUTTONUP);
  GetDlgItem( IDC_RAD_G8_T4)->SendMessage( WM_LBUTTONDOWN);
  GetDlgItem( IDC_RAD_G8_T4)->SendMessage( WM_LBUTTONUP);
}

void CMainView::OnMouseUpGraph1(short Button, short Shift, long x, long y) 
{
  if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 0);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);

    /*
    theApp.GetSettings()->
    //CNiPlot *plot = m_ctlMainGraph.GetPlots().Item( 0);
    //plot->SetPointStyle( CNiPlot::PointStyles

    /*
    PointNone                 = 0,      // None
            PointEmptySquare          = 1,      // Empty square
            PointSolidSquare          = 2,      // Solid square
            PointAsterisk             = 3,      // Asterisk
            PointDottedEmptySquare    = 4,      // Dotted empty square
            PointDottedSolidSquare    = 5,      // Dotted solid square
            PointSolidDiamond         = 6,      // Solid diamond
            PointEmptySquareWithX     = 7,      // Empty square with X
            PointEmptySquareWithCross = 8,      // Empty square with cross
            PointEmptyCircle          = 9,      // Empty circle
            PointSolidCircle          = 10,     // Solid circle
            PointDottedEmptyCircle    = 11,     // Dotted empty circle
            PointDottedSolidCircle    = 12,     // Dotted solid circle
            PointX                    = 13,     // X
            PointBoldX                = 14,     // Bold X
            PointSmallX               = 15,     // Small X
            PointCross                = 16,     // Cross
            PointBoldCross            = 17,     // Bold cross
            PointSmallCross           = 18,     // Small cross
            PointSmallEmptySquare     = 19,     // Small empty square
            PointSmallSolidSquare     = 20,     // Small solid square
            PointSimpleDot            = 21,     // Simple dot
            PointEmptyDiamond         = 22      // Empty diamond
            */
  }
}

void CMainView::OnMouseUpGraph2(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 1);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph3(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 2);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph4(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 3);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph5(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 4);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph6(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 5);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph7(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 6);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}

void CMainView::OnMouseUpGraph8(short Button, short Shift, long x, long y) 
{
	if( Button == MK_RBUTTON) {
    if( m_dlgGraphSetup == NULL) {
      m_dlgGraphSetup = new CDlgGraphSetup( this);
      m_dlgGraphSetup->Create( IDD_GRAPH_SETUP, this);
    }

    m_dlgGraphSetup->Init( 7);
    m_dlgGraphSetup->ShowWindow( SW_SHOW);
  }
}
