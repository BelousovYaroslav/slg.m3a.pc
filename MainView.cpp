// MainView.cpp : implementation file
//

#include "stdafx.h"
#include "slg2.h"
#include "MainView.h"
#include "MainFrm.h"
#include "SlgNewAverager.h"
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
extern CSlgNewAverager gl_avW100;
extern CSlgNewAverager gl_avW1000;
extern CSlgNewAverager gl_avW10000;
extern CSlgNewAverager gl_avW100000;
extern CSlgNewAverager gl_avI1;
extern CSlgNewAverager gl_avI2;
extern CSlgNewAverager gl_avVpc;
extern CSlgNewAverager gl_avAmplAng;
extern CSlgNewAverager gl_avAmplAngDus;
extern CSlgNewAverager gl_avT1;
extern CSlgNewAverager gl_avT2;
extern CSlgNewAverager gl_avTsa;
extern CSlgNewAverager gl_avTsa1000;
extern CSlgNewAverager gl_avTsa10000;
extern CSlgNewAverager gl_avTsa100000;

extern double gl_pTsa;
extern double gl_pw100;
extern double gl_pi1;
extern double gl_pi2;
extern double gl_pVpc;
extern double gl_pAA;
extern double gl_pT1;
extern double gl_pT2;
extern double gl_pT3;
extern double gl_pTSamean;

/*extern BOOL gl_bDecCoeffCalculation;
extern double gl_dNdU_dN[];
extern double gl_dNdU_dU[];
extern double gl_dNdU_DecCoeff[];
extern int gl_dec_coeff_cntr;
extern BOOL gl_dec_coeff_overround;

extern double gl_dec_coeff_dN_acc;
extern double gl_dec_coeff_dU_acc;
extern double gl_dec_coeff_acc;
extern int gl_dec_coeff_acc_cntr;
*/
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
	m_strGraphMaxVal = _T("");
	m_strGraphMeanVal = _T("");
	m_strGraphMinVal = _T("");
	m_strGraphRmsVal = _T("");
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
	m_nT1_RadSelection = 0;
	m_nT2_RadSelection = 1;
	m_strMarkerFails = _T("");
	m_strCheckSummFails = _T("");
	m_nTsaRadSelection = 0;
	m_nRadAmplAng = 0;
	m_strCounterFails = _T("");
	m_strDeviceSerialNumber = _T("");
	//}}AFX_DATA_INIT
	m_nCounterSkippedPoints = 0;
	m_nPointsSkipped = 0;	
	m_bEmergencyCodeApperance = false;
  m_dlgDecCoeffCalc = NULL;

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
	DDX_Text(pDX, IDC_MAXVAL_LABEL, m_strGraphMaxVal);
	DDX_Text(pDX, IDC_MEANVAL_LABEL, m_strGraphMeanVal);
	DDX_Text(pDX, IDC_MINVAL_LABEL, m_strGraphMinVal);
	DDX_Text(pDX, IDC_RMSVAL_LABEL, m_strGraphRmsVal);
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
	DDX_Radio(pDX, IDC_RAD_T1_TD1, m_nT1_RadSelection);
	DDX_Radio(pDX, IDC_RAD_T2_TD1, m_nT2_RadSelection);
	DDX_Text(pDX, IDC_LBL_MARKER_FAILS, m_strMarkerFails);
	DDX_Text(pDX, IDC_LBL_CHECKSUMM_FAILS, m_strCheckSummFails);
	DDX_Radio(pDX, IDC_RAD_TSA_MCS, m_nTsaRadSelection);
	DDX_Radio(pDX, IDC_RAD_AA_IMP, m_nRadAmplAng);
	DDX_Text(pDX, IDC_LBL_COUNTER_FAILS, m_strCounterFails);
	DDX_Text(pDX, IDC_LBL_DEVICE_SERIAL_NUMBER, m_strDeviceSerialNumber);
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
	ON_BN_CLICKED(IDC_RAD_T1_TD1, OnRadT1Td1)
	ON_BN_CLICKED(IDC_RAD_T1_TD2, OnRadT1Td2)
	ON_BN_CLICKED(IDC_RAD_T1_TD3, OnRadT1Td3)
	ON_BN_CLICKED(IDC_RAD_T2_TD1, OnRadT2Td1)
	ON_BN_CLICKED(IDC_RAD_T2_TD2, OnRadT2Td2)
	ON_BN_CLICKED(IDC_RAD_T2_TD3, OnRadT2Td3)
	ON_BN_CLICKED(IDC_RAD_TSA_MCS, OnRadTsaMcs)
	ON_BN_CLICKED(IDC_RAD_TSA_MS, OnRadTsaMs)
	ON_BN_CLICKED(IDC_RAD_TSA_HZ, OnRadTsaHz)
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
	ON_BN_CLICKED(IDC_RAD_AA_IMP, OnRadAaImp)
	ON_BN_CLICKED(IDC_RAD_AA_DUS, OnRadAaDus)
	ON_BN_CLICKED(IDC_BTN_REQ_HV_APPLIES, OnBtnReqHvApplies)
	ON_BN_CLICKED(IDC_BTN_REQ_SN, OnBtnReqSn)
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
		m_ctlSmallGraph1.GetWindowRect( &rc);
		m_ctlSmallGraph1.SetWindowPos( NULL, 0, 50, nGrWidth - 2, rc.Height(), SWP_NOZORDER);
	}
	
	if( m_ctlSmallGraph2.m_hWnd) {
		m_ctlSmallGraph2.GetWindowRect( &rc);
		m_ctlSmallGraph2.SetWindowPos( NULL, nGrWidth, 50, nGrWidth - 2, rc.Height(), SWP_NOZORDER);
	}

	if( m_ctlSmallGraph3.m_hWnd) {
		m_ctlSmallGraph3.GetWindowRect( &rc);
		m_ctlSmallGraph3.SetWindowPos( NULL, nGrWidth * 2, 50, nGrWidth - 2, rc.Height(), SWP_NOZORDER);
	}

	if( m_ctlSmallGraph4.m_hWnd) {
		m_ctlSmallGraph4.GetWindowRect( &rc);
		m_ctlSmallGraph4.SetWindowPos( NULL, nGrWidth * 3, 50, nGrWidth - 2, rc.Height(), SWP_NOZORDER);
	}

	if( m_ctlSmallGraph5.m_hWnd) {
		m_ctlSmallGraph5.GetWindowRect( &rc);
		m_ctlSmallGraph5.SetWindowPos( NULL, nGrWidth * 4, 50, nGrWidth - 22, rc.Height(), SWP_NOZORDER);

    GetDlgItem( IDC_RAD_AA_IMP)->SetWindowPos( NULL, nGrWidth * 5 - 21, 70, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_AA_DUS)->SetWindowPos( NULL, nGrWidth * 5 - 21, 110, 20, 12, SWP_NOZORDER);
    

	}

	if( m_ctlSmallGraph6.m_hWnd) {
		m_ctlSmallGraph6.GetWindowRect( &rc);
		m_ctlSmallGraph6.SetWindowPos( NULL, nGrWidth * 5, 50, nGrWidth - 22, rc.Height(), SWP_NOZORDER);

    GetDlgItem( IDC_RAD_T1_TD1)->SetWindowPos( NULL, nGrWidth * 6 - 21, 70, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_T1_TD2)->SetWindowPos( NULL, nGrWidth * 6 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_T1_TD3)->SetWindowPos( NULL, nGrWidth * 6 - 21, 110, 20, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph7.m_hWnd) {
		m_ctlSmallGraph7.GetWindowRect( &rc);
		m_ctlSmallGraph7.SetWindowPos( NULL, nGrWidth * 6, 50, nGrWidth - 22, rc.Height(), SWP_NOZORDER);

    GetDlgItem( IDC_RAD_T2_TD1)->SetWindowPos( NULL, nGrWidth * 7 - 21, 70, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_T2_TD2)->SetWindowPos( NULL, nGrWidth * 7 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_T2_TD3)->SetWindowPos( NULL, nGrWidth * 7 - 21, 110, 20, 12, SWP_NOZORDER);
	}

	if( m_ctlSmallGraph8.m_hWnd) {
		m_ctlSmallGraph8.GetWindowRect( &rc);
		m_ctlSmallGraph8.SetWindowPos( NULL, nGrWidth * 7, 50, nGrWidth - 22, rc.Height(), SWP_NOZORDER);

    GetDlgItem( IDC_RAD_TSA_MCS)->SetWindowPos( NULL, nGrWidth * 8 - 21, 70, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_TSA_MS)->SetWindowPos( NULL, nGrWidth * 8 - 21, 90, 20, 12, SWP_NOZORDER);
    GetDlgItem( IDC_RAD_TSA_HZ)->SetWindowPos( NULL, nGrWidth * 8 - 21, 110, 20, 12, SWP_NOZORDER);
	}

	int base = 50 + rc.Height();
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

void CMainView::RefreshGraphs()
{
	int jndx, j, j1, j2, j3;
	//100сек точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbW100->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbW100->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbW100->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineW100( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbW100->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbW100->GetSize();
		lineW100( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbW100->GetDataX()[jndx];
		lineW100( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbW100->GetDataY()[jndx];
	}
	

	//секундные точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbW1000->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbW1000->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbW1000->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineW1000( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbW1000->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbW1000->GetSize();
		lineW1000( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbW1000->GetDataX()[jndx];
		lineW1000( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbW1000->GetDataY()[jndx];
	}

	//10сек точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbW10000->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbW10000->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbW10000->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineW10000( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbW10000->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbW10000->GetSize();
		lineW10000( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbW10000->GetDataX()[jndx];
		lineW10000( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbW10000->GetDataY()[jndx];
	}

	//100сек точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbW100000->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbW100000->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbW100000->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineW100000( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbW100000->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbW100000->GetSize();
		lineW100000( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbW100000->GetDataX()[jndx];
		lineW100000( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbW100000->GetDataY()[jndx];
	}

	//I1 точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbI1->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbI1->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbI1->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineI1( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbI1->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbI1->GetSize();
		lineI1( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbI1->GetDataX()[jndx];
		lineI1( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbI1->GetDataY()[jndx];
	}

	//I2 точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbI2->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbI2->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbI2->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineI2( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbI2->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbI2->GetSize();
		lineI2( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbI2->GetDataX()[jndx];
		lineI2( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbI2->GetDataY()[jndx];
	}

	//Vpc точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbVpc->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbVpc->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbVpc->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineVpc( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbVpc->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbVpc->GetSize();
		lineVpc( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbVpc->GetDataX()[jndx];
		lineVpc( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbVpc->GetDataY()[jndx];
	}

	//AmplAng точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineAA( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetSize();
		lineAA( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetDataX()[jndx];
		lineAA( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetDataY()[jndx];
	}

  //AmplAngDus точки
	j1 = theApp.m_cbAmplAngDus->GetFirstIndex();
	j2 = theApp.m_cbAmplAngDus->GetLastIndex();
	j3 = theApp.m_cbAmplAngDus->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineAADus( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + theApp.m_cbAmplAngDus->GetSize()) % theApp.m_cbAmplAngDus->GetSize();
		lineAADus( 0, j) = theApp.m_cbAmplAngDus->GetDataX()[ jndx];
		lineAADus( 1, j) = theApp.m_cbAmplAngDus->GetDataY()[ jndx];
	}

	//T1 точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbT1->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbT1->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbT1->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineT1( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbT1->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbT1->GetSize();
		lineT1( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbT1->GetDataX()[jndx];
		lineT1( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbT1->GetDataY()[jndx];
	}

	//T2 точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbT2->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbT2->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbT2->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineT2( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbT2->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbT2->GetSize();
		lineT2( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbT2->GetDataX()[jndx];
		lineT2( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbT2->GetDataY()[jndx];
	}

  //T3 точки
	j1 = (( CSlg2App *) AfxGetApp())->m_cbT3->GetFirstIndex();
	j2 = (( CSlg2App *) AfxGetApp())->m_cbT3->GetLastIndex();
	j3 = (( CSlg2App *) AfxGetApp())->m_cbT3->GetActualSize();
	if( !j3) j3 = 1;
	CNiReal64Matrix lineT3( 2, j3, 1.0);
	for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + (( CSlg2App *) AfxGetApp())->m_cbT3->GetSize()) % (( CSlg2App *) AfxGetApp())->m_cbT3->GetSize();
		lineT3( 0, j) = (( CSlg2App *) AfxGetApp())->m_cbT3->GetDataX()[jndx];
		lineT3( 1, j) = (( CSlg2App *) AfxGetApp())->m_cbT3->GetDataY()[jndx];
	}

	//Tsa.mcs точки
	j1 = theApp.m_cbTsaMcs->GetFirstIndex();
	j2 = theApp.m_cbTsaMcs->GetLastIndex();
	j3 = theApp.m_cbTsaMcs->GetActualSize();
	if( !j3) j3 = 1;
	
  CNiReal64Matrix lineTsaMcs( 2, j3, 1.0);
  for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + theApp.m_cbTsaMcs->GetSize()) % theApp.m_cbTsaMcs->GetSize();
		
    lineTsaMcs( 0, j) = theApp.m_cbTsaMcs->GetDataX()[jndx];
		lineTsaMcs( 1, j) = theApp.m_cbTsaMcs->GetDataY()[jndx];
	}


  //Tsa.ms точки
	j1 = theApp.m_cbTsaMs->GetFirstIndex();
	j2 = theApp.m_cbTsaMs->GetLastIndex();
	j3 = theApp.m_cbTsaMs->GetActualSize();
	if( !j3) j3 = 1;
	
  CNiReal64Matrix lineTsaMs( 2, j3, 1.0);
  for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + theApp.m_cbTsaMs->GetSize()) % theApp.m_cbTsaMs->GetSize();
		
    lineTsaMs( 0, j) = theApp.m_cbTsaMs->GetDataX()[jndx];
		lineTsaMs( 1, j) = theApp.m_cbTsaMs->GetDataY()[jndx];
	}


  //Tsa.hz точки
	j1 = theApp.m_cbTsaHz->GetFirstIndex();
	j2 = theApp.m_cbTsaHz->GetLastIndex();
	j3 = theApp.m_cbTsaHz->GetActualSize();
	if( !j3) j3 = 1;
	
  CNiReal64Matrix lineTsaHz( 2, j3, 1.0);
  for( j = 0; j < j3; j++) {
		jndx = ( j1 + j + theApp.m_cbTsaHz->GetSize()) % theApp.m_cbTsaHz->GetSize();
		
    lineTsaHz( 0, j) = theApp.m_cbTsaHz->GetDataX()[jndx];
		lineTsaHz( 1, j) = theApp.m_cbTsaHz->GetDataY()[jndx];
	}


	//маленькие экранчики
	switch( m_nTMeaningInd) {
		case 0: m_ctlSmallGraph1.PlotXY( lineW100, true); break;
		case 1: m_ctlSmallGraph1.PlotXY( lineW1000, true); break;
		case 2: m_ctlSmallGraph1.PlotXY( lineW10000, true); break;
		case 3: m_ctlSmallGraph1.PlotXY( lineW100000, true); break;
	}
	m_ctlSmallGraph2.PlotXY( lineI1, true);
	m_ctlSmallGraph3.PlotXY( lineI2, true);
	m_ctlSmallGraph4.PlotXY( lineVpc, true);
	
  switch( m_nRadAmplAng) {
    case 0: m_ctlSmallGraph5.PlotXY( lineAA, true); break;
    case 1: m_ctlSmallGraph5.PlotXY( lineAADus, true); break;
  }

  switch( m_nT1_RadSelection) {
    case 0: m_ctlSmallGraph6.PlotXY( lineT1, true); break;
    case 1: m_ctlSmallGraph6.PlotXY( lineT2, true); break;
    case 2: m_ctlSmallGraph6.PlotXY( lineT3, true); break;
  }
	
  switch( m_nT2_RadSelection) {
    case 0: m_ctlSmallGraph7.PlotXY( lineT1, true); break;
    case 1: m_ctlSmallGraph7.PlotXY( lineT2, true); break;
    case 2: m_ctlSmallGraph7.PlotXY( lineT3, true); break;
  }

  switch( m_nTsaRadSelection) {
    case 0: m_ctlSmallGraph8.PlotXY( lineTsaMcs, true); break;
    case 1: m_ctlSmallGraph8.PlotXY( lineTsaMs, true); break;
    case 2: m_ctlSmallGraph8.PlotXY( lineTsaHz, true); break;
  }

	//Статистика
	(( CSlg2App *) AfxGetApp())->m_cbW100->RecalculateStatistic();
	
	if( (( CSlg2App *) AfxGetApp())->m_cbW1000->GetLastIndex() > 0 &&
			(( CSlg2App *) AfxGetApp())->m_cbW1000->GetLastIndex() < 10) {
		CSlg2App *app = (( CSlg2App *) AfxGetApp());
		int q = sizeof( double);
	}
	(( CSlg2App *) AfxGetApp())->m_cbW1000->RecalculateStatistic();

	(( CSlg2App *) AfxGetApp())->m_cbW10000->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbW100000->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbI1->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbI2->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbVpc->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbAmplAng->RecalculateStatistic();
  (( CSlg2App *) AfxGetApp())->m_cbAmplAngDus->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbT1->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbT2->RecalculateStatistic();
  (( CSlg2App *) AfxGetApp())->m_cbT3->RecalculateStatistic();
	(( CSlg2App *) AfxGetApp())->m_cbTsaMcs->RecalculateStatistic();
  (( CSlg2App *) AfxGetApp())->m_cbTsaMs->RecalculateStatistic();
  (( CSlg2App *) AfxGetApp())->m_cbTsaHz->RecalculateStatistic();

	switch( m_nMainGraph) {
		case 1:
			if( m_nTMeaningInd == 0) {
				m_ctlMainGraph.PlotXY( lineW100, true);
				m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMin());
				m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMean());
				m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMax());
				m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetRMS());
			}

			if( m_nTMeaningInd == 1) {
				m_ctlMainGraph.PlotXY( lineW1000, true);
				m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMin());
				m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMean());
				m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMax());
				m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetRMS());
			}
			
			if( m_nTMeaningInd == 2) {
				m_ctlMainGraph.PlotXY( lineW10000, true);
				m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMin());
				m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMean());
				m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMax());
				m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetRMS());
			}

			if( m_nTMeaningInd == 3) {
				m_ctlMainGraph.PlotXY( lineW100000, true);
				m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMin());
				m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMean());
				m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMax());
				m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetRMS());
			}
			
		break;

		case 2:
			m_ctlMainGraph.PlotXY( lineI1, true);
			m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMin());
			m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMean());
			m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMax());
			m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetRMS());
		break;

		case 3:
			m_ctlMainGraph.PlotXY( lineI2, true);
			m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMin());
			m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMean());
			m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMax());
			m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetRMS());
		break;

		case 4:
			m_ctlMainGraph.PlotXY( lineVpc, true);
			m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMin());
			m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMean());
			m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMax());
			m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetRMS());
		break;

		case 5:			
      switch( m_nRadAmplAng) {
        case 0:
          m_ctlMainGraph.PlotXY( lineAA, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetRMS());
        break;

        case 1:
          m_ctlMainGraph.PlotXY( lineAADus, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAngDus->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAngDus->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAngDus->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAngDus->GetRMS());
        break;
      }
		break;

		case 6:
      switch( m_nT1_RadSelection) {
        case 0:
			    m_ctlMainGraph.PlotXY( lineT1, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetRMS());
        break;
        case 1:
			    m_ctlMainGraph.PlotXY( lineT2, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetRMS());
        break;
        case 2:
			    m_ctlMainGraph.PlotXY( lineT3, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetRMS());
        break;
      }
		break;

		case 7:
			switch( m_nT2_RadSelection) {
        case 0:
			    m_ctlMainGraph.PlotXY( lineT1, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetRMS());
        break;
        case 1:
			    m_ctlMainGraph.PlotXY( lineT2, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetRMS());
        break;
        case 2:
			    m_ctlMainGraph.PlotXY( lineT3, true);
			    m_strGraphMinVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetRMS());
        break;
      }
		break;

		case 8:
      switch( m_nTsaRadSelection) {
        case 0:
          m_ctlMainGraph.PlotXY( lineTsaMcs, true);
			    m_strGraphMinVal.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetRMS());
        break;
        case 1:
          m_ctlMainGraph.PlotXY( lineTsaMs, true);
			    m_strGraphMinVal.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), theApp.m_cbTsaMs->GetRMS());
        break;
        case 2:
          m_ctlMainGraph.PlotXY( lineTsaHz, true);
			    m_strGraphMinVal.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMin());
			    m_strGraphMeanVal.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMean());
			    m_strGraphMaxVal.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMax());
			    m_strGraphRmsVal.Format( _T("%.4f"), theApp.m_cbTsaHz->GetRMS());
        break;
      }

			
		break;

		default:
			m_nMainGraph = 1;
		break;
	}

	//статистика по каждому параметру
	switch( m_nTMeaningInd) {
		case 0:
			m_strSmGr1_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMin());
			m_strSmGr1_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMean());
			m_strSmGr1_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetMax());
			m_strSmGr1_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100->GetRMS());
		break;

		case 1:
			m_strSmGr1_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMin());
			m_strSmGr1_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMean());
			m_strSmGr1_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetMax());
			m_strSmGr1_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW1000->GetRMS());
		break;

		case 2:
			m_strSmGr1_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMin());
			m_strSmGr1_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMean());
			m_strSmGr1_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetMax());
			m_strSmGr1_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW10000->GetRMS());
		break;

		case 3:
			m_strSmGr1_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMin());
			m_strSmGr1_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMean());
			m_strSmGr1_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetMax());
			m_strSmGr1_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbW100000->GetRMS());
		break;
	}

	m_strSmGr2_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMin());
	m_strSmGr2_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMean());
	m_strSmGr2_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetMax());
	m_strSmGr2_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI1->GetRMS());

	m_strSmGr3_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMin());
	m_strSmGr3_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMean());
	m_strSmGr3_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetMax());
	m_strSmGr3_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbI2->GetRMS());
	
	m_strSmGr4_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMin());
	m_strSmGr4_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMean());
	m_strSmGr4_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetMax());
	m_strSmGr4_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbVpc->GetRMS());
	
	m_strSmGr5_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMin());
	m_strSmGr5_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMean());
	m_strSmGr5_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetMax());
	m_strSmGr5_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbAmplAng->GetRMS());
	
  switch( m_nT1_RadSelection) {
    case 0:
	    m_strSmGr6_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMin());
	    m_strSmGr6_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMean());
	    m_strSmGr6_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMax());
	    m_strSmGr6_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetRMS());
    break;
    case 1:
	    m_strSmGr6_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMin());
	    m_strSmGr6_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMean());
	    m_strSmGr6_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMax());
	    m_strSmGr6_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetRMS());
    break;
    case 2:
	    m_strSmGr6_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMin());
	    m_strSmGr6_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMean());
	    m_strSmGr6_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMax());
	    m_strSmGr6_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetRMS());
    break;
  }
	
	switch( m_nT2_RadSelection) {
    case 0:
	    m_strSmGr7_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMin());
	    m_strSmGr7_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMean());
	    m_strSmGr7_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetMax());
	    m_strSmGr7_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT1->GetRMS());
    break;
    case 1:
	    m_strSmGr7_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMin());
	    m_strSmGr7_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMean());
	    m_strSmGr7_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetMax());
	    m_strSmGr7_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT2->GetRMS());
    break;
    case 2:
	    m_strSmGr7_min.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMin());
	    m_strSmGr7_mean.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMean());
	    m_strSmGr7_max.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetMax());
	    m_strSmGr7_rms.Format( _T("%.4f"), (( CSlg2App *) AfxGetApp())->m_cbT3->GetRMS());
    break;
  }
	
  switch( m_nTsaRadSelection) {
    case 0:
	    m_strSmGr8_min.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMin());
	    m_strSmGr8_mean.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMean());
	    m_strSmGr8_max.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetMax());
	    m_strSmGr8_rms.Format( _T("%.4f"), theApp.m_cbTsaMcs->GetRMS());
    break;
    case 1:
	    m_strSmGr8_min.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMin());
	    m_strSmGr8_mean.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMean());
	    m_strSmGr8_max.Format( _T("%.4f"), theApp.m_cbTsaMs->GetMax());
	    m_strSmGr8_rms.Format( _T("%.4f"), theApp.m_cbTsaMs->GetRMS());
    break;
    case 2:
	    m_strSmGr8_min.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMin());
	    m_strSmGr8_mean.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMean());
	    m_strSmGr8_max.Format( _T("%.4f"), theApp.m_cbTsaHz->GetMax());
	    m_strSmGr8_rms.Format( _T("%.4f"), theApp.m_cbTsaHz->GetRMS());
    break;
  }
	m_ctlComButton.SetValue( m_ctlCOM.GetPortOpen());
	UpdateData( FALSE);
}

void CMainView::OnInitialUpdate() 
{
	m_dKimpSec = (( CSlg2App *) AfxGetApp())->m_dKimpSec;
	m_nComPort = (( CSlg2App *) AfxGetApp())->m_nComPort;
	m_nComPortBaudrate = (( CSlg2App *) AfxGetApp())->m_nComBaudrate;
	m_nMainGraph = 1;
	CFormView::OnInitialUpdate();	

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));									// Zero out the structure.
	lf.lfHeight = 20;																	// Request a 12-pixel-height font.
	lstrcpy(lf.lfFaceName, _T("Times New Roman"));   // Request a face name "Arial".
	VERIFY(m_pFont.CreateFontIndirect(&lf));							// Create the font.

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

	if( theApp.m_nControlButtons) {
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
							gl_avW100.Reset();
							gl_avW1000.Reset();
							gl_avW10000.Reset();
							gl_avW100000.Reset();
							gl_avI1.Reset();
							gl_avI2.Reset();
							gl_avVpc.Reset();
							gl_avAmplAng.Reset();
              gl_avAmplAngDus.Reset();
							gl_avT1.Reset();
							gl_avT2.Reset();
							gl_avTsa.Reset();
							gl_avTsa1000.Reset();
							gl_avTsa10000.Reset();
							gl_avTsa100000.Reset();
		

							app->m_cbW100->Reset();
							app->m_cbW1000->Reset();
							app->m_cbW10000->Reset();
							app->m_cbW100000->Reset();
							app->m_cbI1->Reset();
							app->m_cbI2->Reset();
							app->m_cbVpc->Reset();
							app->m_cbAmplAng->Reset();
							app->m_cbT1->Reset();
							app->m_cbT2->Reset();
              app->m_cbT3->Reset();
							app->m_cbTsaMcs->Reset();
              app->m_cbTsaMs->Reset();
              app->m_cbTsaHz->Reset();
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
				strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d.txt"),
													sysTime.wYear, sysTime.wMonth, sysTime.wDay,
													sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
				/*if( app->fh == NULL)
					app->fh = fopen( strStatFileName, "w");*/
				/*else
					AfxMessageBox( _T("Попытка открыть файл при открытом дескрипторе."));*/

				strStatFileName.Format( _T("data-%d-%02d-%02d-%02d-%02d-%02d_bin.txt"),
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
      m_strDeviceSerialNumber.Format( _T("%d"), theApp.m_nDeviceSerialNumber);
    }
    else {
      m_strDeviceSerialNumber = _T("-");
    }

    //окошки ошибок по маркеру и чексумме
    m_strMarkerFails.Format( _T("%d"), theApp.m_nMarkerFails);
    m_strCheckSummFails.Format( _T("%d"), theApp.m_nCheckSummFails);
    m_strCounterFails.Format( _T("%d"), theApp.m_nCounterFails);

		//обновление на экране версии прошивки аппарата
		GetDlgItem( IDC_BTN_EXPORT)->EnableWindow( gl_avW100.GetCounter());
		m_strSoftwareVersion = app->m_strSoftwareVer;
		
		//////////////////////////////////////////////////////////////////////
		// Обновляем управленческие параметры
		//////////////////////////////////////////////////////////////////////
		m_strParam1Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam1 * m_dKimpSec);		//код амплитуды
		m_strParam2Val.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_btParam2);											//код такта подставки
		m_strParam3Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//коэффициент M
		m_strParam4Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_btParam4 / 100.);		//начальная мода
		//m_strParam5Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI1min / 65535. * 0.75);
		//m_strParam6Val.Format( _T("%.2f"), ( double) (( CSlg2App *) AfxGetApp())->m_shFlashI2min / 65535. * 0.75);
		//m_strParam7Val.Format( _T("%.2f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashAmplAng1min / 65535. * 6.0));
		m_strParam8Val.Format( _T("%.5f"), ( ( double) (( CSlg2App *) AfxGetApp())->m_shFlashDecCoeff) / 65535.);
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
			for( int err=0; err<ERROR_1; err++) {
				Beep( 100, 100);
				Sleep( 200);
			}
			/*CDC *dc = GetDlgItem( IDC_EMERGENCY_LBL)->GetDC();
			dc->SetBkColor( RGB( 120, 0, 0));
			dc->FloodFill( 10, 10, RGB( 120, 0, 0));
			GetDlgItem( IDC_EMERGENCY_LBL)->ReleaseDC( dc);*/
			GetDlgItem( IDC_EMERGENCY_LBL)->SetFont( &m_pFont);
		}
		m_strLblEmergency.Format( _T("%d"), (( CSlg2App *) AfxGetApp())->m_nEmergencyCode);

		//////////////////////////////////////////////////////////////////////
		// Обновляем графики
		//////////////////////////////////////////////////////////////////////
		RefreshGraphs();

		//////////////////////////////////////////////////////////////////////
		//проверка времени запуска
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
				gl_avW100.Reset();
				gl_avW1000.Reset();
				gl_avW10000.Reset();
				gl_avW100000.Reset();
				gl_avI1.Reset();
				gl_avI2.Reset();
				gl_avVpc.Reset();
				gl_avAmplAng.Reset();
        gl_avAmplAngDus.Reset();
				gl_avT1.Reset();
				gl_avT2.Reset();
				gl_avTsa.Reset();
				gl_avTsa1000.Reset();
				gl_avTsa10000.Reset();
				gl_avTsa100000.Reset();
		
				app->m_cbW100->Reset();
				app->m_cbW1000->Reset();
				app->m_cbW10000->Reset();
				app->m_cbW100000->Reset();
				app->m_cbI1->Reset();
				app->m_cbI2->Reset();
				app->m_cbVpc->Reset();
				app->m_cbAmplAng->Reset();
				app->m_cbT1->Reset();
				app->m_cbT2->Reset();
        app->m_cbT3->Reset();
				app->m_cbTsaMcs->Reset();
        app->m_cbTsaMs->Reset();
        app->m_cbTsaHz->Reset();

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
		if( (( CSlg2App *) AfxGetApp())->m_nControlButtons) {
			if( (( CSlg2App *) AfxGetApp())->m_shSignCoeff) {
				m_ctlNedtParam1.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_btParam1 * m_dKimpSec);
				m_ctlNedtParam2.SetValue( (( CSlg2App *) AfxGetApp())->m_btParam2);
				m_ctlNedtParam3.SetValue( ( double) (( CSlg2App *) AfxGetApp())->m_btParam3 / 250.);		//
				m_ctlNedtParam4.SetValue( 0.01 * ( double) (( CSlg2App *) AfxGetApp())->m_btParam4);
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
		gl_avW100.Reset();
		gl_avW1000.Reset();
		gl_avW10000.Reset();
		gl_avW100000.Reset();
		gl_avI1.Reset();
		gl_avI2.Reset();
		gl_avVpc.Reset();
		gl_avAmplAng.Reset();
    gl_avAmplAngDus.Reset();
		gl_avT1.Reset();
		gl_avT2.Reset();
		gl_avTsa.Reset();
		gl_avTsa1000.Reset();
		gl_avTsa10000.Reset();
		gl_avTsa100000.Reset();
		
		app->m_cbW100->Reset();
		app->m_cbW1000->Reset();
		app->m_cbW10000->Reset();
		app->m_cbW100000->Reset();
		app->m_cbI1->Reset();
		app->m_cbI2->Reset();
		app->m_cbVpc->Reset();
		app->m_cbAmplAng->Reset();
		app->m_cbT1->Reset();
		app->m_cbT2->Reset();
    app->m_cbT3->Reset();
		app->m_cbTsaMcs->Reset();
    app->m_cbTsaMs->Reset();
    app->m_cbTsaHz->Reset();
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
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMainView::OnClickGraph1() 
{
	UpdateData( TRUE);
	m_nMainGraph = 1;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("''/sec"));
}

void CMainView::OnClickGraph2() 
{
	m_nMainGraph = 2;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
}

void CMainView::OnClickGraph3() 
{
	m_nMainGraph = 3;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("mA"));
}

void CMainView::OnClickGraph4() 
{
	m_nMainGraph = 4;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("V"));
}

void CMainView::OnClickGraph5() 
{
	m_nMainGraph = 5;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
  switch( m_nRadAmplAng) {
    case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("''")); break;
    case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("В")); break;
  }
	
}

void CMainView::OnClickGraph6() 
{
 	m_nMainGraph = 6;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 0, 127));
 	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("°C"));
}

void CMainView::OnClickGraph7() 
{
	m_nMainGraph = 7;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 0, 127, 0));
	m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("°C"));
}

void CMainView::OnClickGraph8() 
{
  UpdateData( TRUE);
	m_nMainGraph = 8;
	m_ctlMainGraph.GetPlots().Item( 1).SetLineColor( RGB( 127, 0, 0));
  switch( m_nTsaRadSelection) {
    case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мксек")); break;
    case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек")); break;
    case 2: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("Гц")); break;
  }
  /*if( m_nMainGraph == 8) {
    switch( m_nTsaRadSelection) {
      case 0: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мксек")); break;
      case 1: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("мсек")); break;
      case 2: m_ctlMainGraph.GetAxes().Item( "YAxis-1").SetCaption( _T("Гц")); break;
    }
  }*/
}

void CMainView::OnValueChangedCwStart(BOOL Value) 
{
	m_nPointsSkipped = 0;
	UpdateData( TRUE);
	( ( CSlg2App *) AfxGetApp())->m_dKimpSec = m_dKimpSec;

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
		theApp.m_bDeviceSerialNumber = false;

		if( app->m_cbW100 != NULL) { delete app->m_cbW100; app->m_cbW100 = NULL; }
		if( app->m_cbW1000 != NULL) {	delete app->m_cbW1000; app->m_cbW1000 = NULL; }
		if( app->m_cbW10000 != NULL) { delete app->m_cbW10000; app->m_cbW10000 = NULL; }
		if( app->m_cbW100000 != NULL) { delete app->m_cbW100000; app->m_cbW100000 = NULL; }
		if( app->m_cbI1 != NULL) { delete app->m_cbI1; app->m_cbI1 = NULL; }
		if( app->m_cbI2 != NULL) { delete app->m_cbI2; app->m_cbI2 = NULL; }
		if( app->m_cbVpc != NULL) { delete app->m_cbVpc; app->m_cbVpc = NULL; }
		if( app->m_cbAmplAng != NULL) { delete app->m_cbAmplAng; app->m_cbAmplAng = NULL; }
    if( app->m_cbAmplAngDus != NULL) { delete app->m_cbAmplAngDus; app->m_cbAmplAngDus = NULL; }
		if( app->m_cbT1 != NULL) { delete app->m_cbT1; app->m_cbT1 = NULL; }
		if( app->m_cbT2 != NULL) { delete app->m_cbT2; app->m_cbT2 = NULL; }
    if( app->m_cbT3 != NULL) { delete app->m_cbT3; app->m_cbT3 = NULL; }
		if( app->m_cbTsaMcs != NULL) { delete app->m_cbTsaMcs; app->m_cbTsaMcs = NULL; }
    if( app->m_cbTsaMs != NULL)  { delete app->m_cbTsaMs;  app->m_cbTsaMs = NULL; }
    if( app->m_cbTsaHz != NULL)  { delete app->m_cbTsaHz;  app->m_cbTsaHz = NULL; }

		double dMeasTime = m_ctlNedtMeasTime.GetValue();
		int nArraySize = 0;
		if( dMeasTime > 0.5) {
			nArraySize = (int) ( ( dMeasTime + 1.) * 10.);
		}
		else {
			nArraySize = 1024;
		}
		
		app->m_cbW100 = new CSlgCircleBuffer( nArraySize);
		app->m_cbW1000  = new CSlgCircleBuffer( nArraySize);
		app->m_cbW10000 = new CSlgCircleBuffer( nArraySize);
		app->m_cbW100000 = new CSlgCircleBuffer( nArraySize);
		app->m_cbI1 = new CSlgCircleBuffer( nArraySize);
		app->m_cbI2 = new CSlgCircleBuffer( nArraySize);
		app->m_cbVpc = new CSlgCircleBuffer( nArraySize);
		app->m_cbAmplAng = new CSlgCircleBuffer( nArraySize);
    app->m_cbAmplAngDus = new CSlgCircleBuffer( nArraySize);
		app->m_cbT1 = new CSlgCircleBuffer( nArraySize);
		app->m_cbT2 = new CSlgCircleBuffer( nArraySize);
    app->m_cbT3 = new CSlgCircleBuffer( nArraySize);
    app->m_cbTsaMcs = new CSlgCircleBuffer( nArraySize);
    app->m_cbTsaMs = new CSlgCircleBuffer( nArraySize);
    app->m_cbTsaHz = new CSlgCircleBuffer( nArraySize);

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
    SetTimer( MY_TIMER_POLLER,	  1000, NULL);

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

		/*if( app->fh != NULL)
			fclose( app->fh);
		app->fh = NULL;*/

		if( app->fhNew != NULL)
			fclose( app->fhNew);
		app->fhNew = NULL;

		gl_nCircleBufferGet = 0;
		gl_nCircleBufferPut = 0;

		gl_dGlobalTime = 0.;
		gl_avW100.Reset();
		gl_avW1000.Reset();
		gl_avW10000.Reset();
		gl_avW100000.Reset();
		gl_avI1.Reset();
		gl_avI2.Reset();
		gl_avVpc.Reset();
		gl_avAmplAng.Reset();
    gl_avAmplAngDus.Reset();
		gl_avT1.Reset();
		gl_avT2.Reset();
		gl_avTsa.Reset();
		gl_avTsa1000.Reset();
		gl_avTsa10000.Reset();
		gl_avTsa100000.Reset();
		

		app->m_cbW100->Reset();
		app->m_cbW1000->Reset();
		app->m_cbW10000->Reset();
		app->m_cbW100000->Reset();
		app->m_cbI1->Reset();
		app->m_cbI2->Reset();
		app->m_cbVpc->Reset();
		app->m_cbAmplAng->Reset();
    app->m_cbAmplAngDus->Reset();
		app->m_cbT1->Reset();
		app->m_cbT2->Reset();
    app->m_cbT3->Reset();
		app->m_cbTsaMcs->Reset();
    app->m_cbTsaMs->Reset();
    app->m_cbTsaHz->Reset();
	}

	UpdateData( false);
}

void CMainView::OnDestroy() 
{
	UpdateData( true);
	((CSlg2App *) AfxGetApp())->m_dKimpSec = m_dKimpSec;
	((CSlg2App *) AfxGetApp())->m_nComPort = m_nComPort;
	((CSlg2App *) AfxGetApp())->m_nComBaudrate = m_nComPortBaudrate;

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
	if( ((CSlg2App *) AfxGetApp())->m_nControlButtons) {
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

  /*
	//reset data
	CSlg2App *app = ((CSlg2App *) AfxGetApp());

	gl_nCircleBufferGet = 0;
	gl_nCircleBufferPut = 0;

	gl_dGlobalTime = 0.;
	gl_avW100.Reset();
	gl_avW1000.Reset();
	gl_avW10000.Reset();
	gl_avW100000.Reset();
	gl_avI1.Reset();
	gl_avI2.Reset();
	gl_avVpc.Reset();
	gl_avAmplAng.Reset();
  gl_avAmplAngDus.Reset();
	gl_avT1.Reset();
	gl_avT2.Reset();
	gl_avTsa.Reset();
	gl_avTsa1000.Reset();
	gl_avTsa10000.Reset();
	gl_avTsa100000.Reset();	

	app->m_cbW100->Reset();
	app->m_cbW1000->Reset();
	app->m_cbW10000->Reset();
	app->m_cbW100000->Reset();
	app->m_cbI1->Reset();
	app->m_cbI2->Reset();
	app->m_cbVpc->Reset();
	app->m_cbAmplAng->Reset();
  app->m_cbAmplAngDus->Reset();
	app->m_cbT1->Reset();
	app->m_cbT2->Reset();
  app->m_cbT3->Reset();
	app->m_cbTsaMcs->Reset();
  app->m_cbTsaMs->Reset();
  app->m_cbTsaHz->Reset();
  */


  if( m_dlgDecCoeffCalc != NULL) {
    m_dlgDecCoeffCalc->ShowWindow( SW_SHOW);
  }
  else {
    m_dlgDecCoeffCalc = new CDecCoeffCalc( this);
    m_dlgDecCoeffCalc->Create( IDD_K_DEC_COEFF_CALC, this);
    m_dlgDecCoeffCalc->ShowWindow( SW_SHOW);
  }

  /*
  //show dlg
	CDecCoeffCalc dlg;
	if( dlg.DoModal() == IDOK) {
		//double k = fabs( gl_dec_coeff_acc / ( double) gl_dec_coeff_acc_cntr);
		double k = fabs( gl_dec_coeff_dN_acc / gl_dec_coeff_dU_acc);
		m_ctlNedtParam8.SetValue( k);
	}
  */




  /*
	//switch our thread back to normal input
	gl_bDecCoeffCalculation = false;

	//switch dev back to normal output
	SetTimer( MY_TIMER_SWITCH_AS, 100, NULL);


	Sleep( 100);

	//reset data
	gl_nCircleBufferGet = 0;
	gl_nCircleBufferPut = 0;

	gl_dGlobalTime = 0.;
	gl_avW100.Reset();
	gl_avW1000.Reset();
	gl_avW10000.Reset();
	gl_avW100000.Reset();
	gl_avI1.Reset();
	gl_avI2.Reset();
	gl_avVpc.Reset();
	gl_avAmplAng.Reset();
  gl_avAmplAngDus.Reset();
	gl_avT1.Reset();
	gl_avT2.Reset();
	gl_avTsa.Reset();
	gl_avTsa1000.Reset();
	gl_avTsa10000.Reset();
	gl_avTsa100000.Reset();	

	theApp.m_cbW100->Reset();
	theApp.m_cbW1000->Reset();
	theApp.m_cbW10000->Reset();
	theApp.m_cbW100000->Reset();
	theApp.m_cbI1->Reset();
	theApp.m_cbI2->Reset();
	theApp.m_cbVpc->Reset();
	theApp.m_cbAmplAng->Reset();
  theApp.m_cbAmplAngDus->Reset();
	theApp.m_cbT1->Reset();
	theApp.m_cbT2->Reset();
  theApp.m_cbT3->Reset();
	theApp.m_cbTsaMcs->Reset();
  theApp.m_cbTsaMs->Reset();
  theApp.m_cbTsaHz->Reset();

	//start screen refresh and other timers
	SetTimer( MY_TIMER_1000, 1000, NULL);*/
}

void CMainView::OnOnCommComm() 
{
	short event = m_ctlCOM.GetCommEvent();
	
	if( event > 1000) {
		if( event == 1004) return;

		UpdateData( TRUE);
		((CSlg2App *) AfxGetApp())->m_dKimpSec = m_dKimpSec;
		((CSlg2App *) AfxGetApp())->m_nComPort = m_nComPort;

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
		gl_avW100.Reset();
		gl_avW1000.Reset();
		gl_avW10000.Reset();
		gl_avW100000.Reset();
		gl_avI1.Reset();
		gl_avI2.Reset();
		gl_avVpc.Reset();
		gl_avAmplAng.Reset();
    gl_avAmplAngDus.Reset();
		gl_avT1.Reset();
		gl_avT2.Reset();
		gl_avTsa.Reset();
		gl_avTsa1000.Reset();
		gl_avTsa10000.Reset();
		gl_avTsa100000.Reset();
		
		app->m_cbW100->Reset();
		app->m_cbW1000->Reset();
		app->m_cbW10000->Reset();
		app->m_cbW100000->Reset();
		app->m_cbI1->Reset();
		app->m_cbI2->Reset();
		app->m_cbVpc->Reset();
		app->m_cbAmplAng->Reset();
    app->m_cbAmplAngDus->Reset();
		app->m_cbT1->Reset();
		app->m_cbT2->Reset();
    app->m_cbT3->Reset();
		app->m_cbTsaMcs->Reset();
    app->m_cbTsaMs->Reset();
    app->m_cbTsaHz->Reset();
	
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

void CMainView::OnBtnReqVersion() 
{
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
  m_ctlNedtParam1.SetValue( theApp.m_btParam1 * theApp.m_dKimpSec);
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
  theApp.m_cbW100->Reset();
	theApp.m_cbW1000->Reset();
	theApp.m_cbW10000->Reset();
	theApp.m_cbW100000->Reset();
	theApp.m_cbI1->Reset();
	theApp.m_cbI2->Reset();
	theApp.m_cbVpc->Reset();
	theApp.m_cbAmplAng->Reset();
  theApp.m_cbAmplAngDus->Reset();
	theApp.m_cbT1->Reset();
	theApp.m_cbT2->Reset();
  theApp.m_cbT3->Reset();
  theApp.m_cbTsaMcs->Reset();
  theApp.m_cbTsaMs->Reset();
  theApp.m_cbTsaHz->Reset();

  theApp.m_nCheckSummFails = 0;
  theApp.m_nMarkerFails = 0;
  theApp.m_nCounterFails = 0;

  gl_nCircleBufferGet = 0;
	gl_nCircleBufferPut = 0;

	gl_dGlobalTime = 0.;
	gl_avW100.Reset();
	gl_avW1000.Reset();
	gl_avW10000.Reset();
	gl_avW100000.Reset();
	gl_avI1.Reset();
	gl_avI2.Reset();
	gl_avVpc.Reset();
	gl_avAmplAng.Reset();
  gl_avAmplAngDus.Reset();
	gl_avT1.Reset();
	gl_avT2.Reset();
	gl_avTsa.Reset();
	gl_avTsa1000.Reset();
	gl_avTsa10000.Reset();
	gl_avTsa100000.Reset();
}

void CMainView::OnRadAaImp() 
{
	m_ctlSmallGraph5.SetCaption( _T("Ампл. част. подст., ['']"));
}

void CMainView::OnRadAaDus() 
{
	m_ctlSmallGraph5.SetCaption( _T("Ампл. с ДУСа, [В]"));
}

void CMainView::OnBtnReqHvApplies() 
{
  QueueCommandToMc( MC_COMMAND_REQ, HV_APPLY_COUNT_TR , 0, 0);
}

void CMainView::OnBtnReqSn() 
{
	QueueCommandToMc( MC_COMMAND_REQ, DEVNUM, 0, 0);
}
