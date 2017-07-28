//{{AFX_INCLUDES()
#include "NiGraph.h"
#include "NiButton.h"
#include "NiNumEdit.h"
//}}AFX_INCLUDES
#if !defined(AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_)
#define AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainView form view


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DecCoeffCalc.h"
#include "DlgGraphSetup.h"

#include "McCommandItem.h"
#include <queue>

#define POLL_PARAMS_LEN 8

class CMainView : public CFormView
{
protected:
	CMainView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMainView)

// Form Data
public:
	//{{AFX_DATA(CMainView)
	enum { IDD = IDD_MAIN_WINDOW };
	CNiGraph	m_ctlSmallGraph1;
	CNiGraph	m_ctlSmallGraph2;
	CNiGraph	m_ctlSmallGraph3;
	CNiGraph	m_ctlSmallGraph4;
	CNiGraph	m_ctlSmallGraph5;
	CNiGraph	m_ctlSmallGraph6;
	CNiGraph	m_ctlSmallGraph7;
	CNiGraph	m_ctlSmallGraph8;
	CNiGraph	m_ctlMainGraph;
	CNiButton	m_ctlComButton;
	BOOL	m_bBtnCwStart;
	int		m_nComPort;
	CString	m_strSmGr1_max;
	CString	m_strSmGr1_mean;
	CString	m_strSmGr1_min;
	CString	m_strSmGr1_rms;
	CString	m_strSmGr2_max;
	CString	m_strSmGr2_mean;
	CString	m_strSmGr2_min;
	CString	m_strSmGr2_rms;
	CString	m_strSmGr3_max;
	CString	m_strSmGr3_mean;
	CString	m_strSmGr3_min;
	CString	m_strSmGr3_rms;
	CString	m_strSmGr4_max;
	CString	m_strSmGr4_mean;
	CString	m_strSmGr4_min;
	CString	m_strSmGr4_rms;
	CString	m_strSmGr5_max;
	CString	m_strSmGr5_mean;
	CString	m_strSmGr5_min;
	CString	m_strSmGr5_rms;
	CString	m_strSmGr6_max;
	CString	m_strSmGr6_mean;
	CString	m_strSmGr6_min;
	CString	m_strSmGr6_rms;
	CString	m_strSmGr7_max;
	CString	m_strSmGr7_mean;
	CString	m_strSmGr7_min;
	CString	m_strSmGr7_rms;
	CString	m_strSmGr8_max;
	CString	m_strSmGr8_mean;
	CString	m_strSmGr8_min;
	CString	m_strSmGr8_rms;
	CString	m_strLblEmergency;
	CString	m_strParam1Val;
	CString	m_strParam2Val;
	CString	m_strParam3Val;
	CString	m_strParam4Val;
	CNiNumEdit	m_ctlNedtParam1;
	CNiNumEdit	m_ctlNedtParam2;
	CNiNumEdit	m_ctlNedtParam3;
	CNiNumEdit	m_ctlNedtParam4;
	CString	m_strParam8Val;
	CNiNumEdit	m_ctlNedtParam8;
	int		m_nTMeaningInd;
	double	m_dKimpSec;
	CNiButton	m_ctlBtnCwStart;
	CString	m_strSoftwareVersion;
	CNiNumEdit	m_ctlNedtMeasTime;
	int		m_nComPortBaudrate;
	CNiNumEdit	m_ctlNedtThermoCalibTemperature;
	CString	m_strThermoCalib_T1;
	CString	m_strThermoCalib_T2;
	CString	m_strMarkerFails;
	CString	m_strCheckSummFails;
	CString	m_strCounterFails;
	int		m_nRadGraph8;
	int		m_nRadGraph7;
	int		m_nRadGraph6;
	int		m_nRadGraph5;
	int		m_nRadGraph4;
	int		m_nRadGraph3;
	int		m_nRadGraph2;
	int		m_nRadGraph1;
	CString	m_strPackNumber;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
  void MakeDeltaLine( CTrackedParam *tp1, CTrackedParam *tp2, CNiReal64Matrix *l100ms, CNiReal64Matrix *l1s, CNiReal64Matrix *l10s, CNiReal64Matrix *l100s);
  void MakeLine( CTrackedParam *tp, CNiReal64Matrix *l100ms, CNiReal64Matrix *l1s, CNiReal64Matrix *l10s, CNiReal64Matrix *l100s);
	void SendCommandToMc( BYTE b1, BYTE b2, BYTE b3, BYTE b4);
	void SetSendButtonsState( bool bState);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMainView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMainView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickGraph1();
	afx_msg void OnClickGraph2();
	afx_msg void OnClickGraph3();
	afx_msg void OnClickGraph4();
	afx_msg void OnClickGraph5();
  afx_msg void OnClickGraph6();
	afx_msg void OnClickGraph7();
	afx_msg void OnClickGraph8();
	afx_msg void OnValueChangedCwStart(BOOL Value);
	afx_msg void OnDestroy();
	afx_msg void OnParam1Btn();
	afx_msg void OnParam2Btn();
	afx_msg void OnParam3Btn();
	afx_msg void OnParam4Btn();
	afx_msg void OnChangeParam2Edt();
	afx_msg void OnChangeParam1Edt();
	afx_msg void OnChangeParam3Edt();
	afx_msg void OnChangeParam4Edt();
	afx_msg void OnBtnSaveParams();
	afx_msg void OnParam8Btn();
	afx_msg void OnBtnRestoreParams();
	afx_msg void OnKillfocusEdtKImpSec();
	afx_msg void OnDecCoeffCalc();
	afx_msg void OnBtnThermoCalibSet();
	afx_msg void OnBtnThermoCalibClear();
	afx_msg void OnBtnLaserOff();
	afx_msg void OnBtnIntegrOff();
	afx_msg void OnBtnIntegrOn();
	afx_msg void OnBtnIntegrReset();
	afx_msg void OnBtnReqVersion();
	afx_msg void OnBtnReqAmpl();
	afx_msg void OnBtnReqTactcode();
	afx_msg void OnBtnReqMCoeff();
	afx_msg void OnBtnReqStartMode();
	afx_msg void OnBtnReqDecCoeff();
	afx_msg void OnBtnMcToOutAmplitude();
	afx_msg void OnBtnMcToOutTactcode();
	afx_msg void OnBtnMcToOutMcoeff();
	afx_msg void OnBtnMcToOutStartmode();
	afx_msg void OnBtnMcToOutDeccoeff();
	afx_msg void OnBtnSwitchWDndu();
	afx_msg void OnBtnReset();
	afx_msg void OnBtnReqHvApplies();
	afx_msg void OnBtnReqSn();
	afx_msg void OnRadMeaning1();
	afx_msg void OnRadMeaning2();
	afx_msg void OnRadMeaning3();
	afx_msg void OnRadMeaning4();
	afx_msg void OnMouseUpGraph1(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph2(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph3(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph4(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph5(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph6(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph7(short Button, short Shift, long x, long y);
	afx_msg void OnMouseUpGraph8(short Button, short Shift, long x, long y);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	bool m_bEmergencyCodeApperance;
	int m_nCounterSkippedPoints;
	int m_nPointsSkipped;
	CFont m_pFont;
	int m_nMainGraph;
	void RefreshGraphs( void);
  std::queue <CMcCommandItem *> m_queCommandQueue;
  void QueueCommandToMc( char btCmd, char btParam1, char btParam2, char btParam3);
  CString UnitsForYAxis( int nYAxisCmbResourceID);
  CString UnitsForXAxis( int nXAxisCmbResourceID);
  CDecCoeffCalc *m_dlgDecCoeffCalc;
  CDlgGraphSetup *m_dlgGraphSetup;
  int m_nPollParams[ POLL_PARAMS_LEN];
  int m_nPollCounter;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINVIEW_H__4133504C_1A2D_4A65_89A5_A033461F952F__INCLUDED_)
