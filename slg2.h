// slg2.h : main header file for the SLG2 application
//

#if !defined(AFX_SLG2_H__5FD609B8_5021_4079_A60E_27769053C9DB__INCLUDED_)
#define AFX_SLG2_H__5FD609B8_5021_4079_A60E_27769053C9DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
  #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
//#include "Serial.h"
#include "settings.h"
#include "logger.h"

#include "SlgCircleBuffer.h"
#include "TrackedParam.h"
/////////////////////////////////////////////////////////////////////////////
// CSlg2App:
// See slg2.cpp for the implementation of this class
//

class CSlg2App : public CWinApp
{
public:
  CSlg2App();
  CString strDirName;

  CLogger * GetLogger() { return &m_pLogger;}
  CSettings *GetSettings() { return &m_pSettings;}

  BYTE m_btParam1;          //амплитуда виброподвеса
  BYTE m_btParam2;
  BYTE m_btParam3;
  BYTE m_btParam4;
  unsigned short m_shFlashI1min;
  unsigned short m_shFlashI2min;
  unsigned short m_shFlashAmplAng1min;
  short m_shFlashDecCoeff;
  short m_shSignCoeff;

  int m_nHvAppliesThisRun;    //кол-во 3kV импульсов поджига в этом запуске


  unsigned short m_shFlashT1, m_shFlashTD1_1, m_shFlashTD2_1, m_shFlashTD3_1;
  unsigned short m_shFlashT2, m_shFlashTD1_2, m_shFlashTD2_2, m_shFlashTD3_2;
  /*
  BOOL m_bThermoCalibrated;
  double m_dThermoCalibK_1TD, m_dThermoCalibB_1TD;
  double m_dThermoCalibK_2TD, m_dThermoCalibB_2TD;
  */

  int m_nEmergencyCode;
  CString m_strSoftwareVer;
  bool m_bDeviceSerialNumber;
  int m_nDeviceSerialNumber;

  FILE *fh;
  FILE *fhb;
  FILE *fhNew;

  //ПОТОКИ
  void StartThreads( void);

  CTrackedParam *m_tpW;
  CTrackedParam *m_tpI1;
  CTrackedParam *m_tpI2;
  CTrackedParam *m_tpVpc;
  CTrackedParam *m_tpAmplAngAlt;
  CTrackedParam *m_tpAmplAngDus;
  CTrackedParam *m_tpAmplAngRULA;
  CTrackedParam *m_tpT1;
  CTrackedParam *m_tpT2;
  CTrackedParam *m_tpT3;
  CTrackedParam *m_tpTsaMcs;
  CTrackedParam *m_tpTsaMs;
  CTrackedParam *m_tpTsaHz;
  CTrackedParam *m_tpDecCoeff;

  int m_nMarkerFails;
  int m_nCounterFails;
  int m_nCheckSummFails;

  char m_bLockBit;
  char m_bSyncAsync;
  char m_bWdNdU;

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSlg2App)
  public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();
  //}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSlg2App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  CSettings m_pSettings;
  CLogger m_pLogger;
};

extern CSlg2App theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLG2_H__5FD609B8_5021_4079_A60E_27769053C9DB__INCLUDED_)
