// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSlg2App theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSettings::CSettings()
{
}

CSettings::~CSettings()
{
}

void CSettings::LoadSettings()
{
  m_nLogLevel =       theApp.GetProfileInt( _T("SETTINGS"), _T("LOG_LEVEL"), 6);
  m_nControlButtons = GetProfileInt( _T("SETTINGS"), _T("CONTROL_BUTTONS"), 0);

  m_nComPort =        theApp.GetProfileInt( _T("SETTINGS"), _T("COM_PORT"), 1);
  m_nComBaudrate =    theApp.GetProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), 1);
  
  int nScaleCoeff =   theApp.GetProfileInt( _T("SETTINGS"), _T("SCALE_COEFF"), 2900);
  m_dKimpSec = nScaleCoeff / 1000.;
  
  m_nSaveGraph1 = GetProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), 0);
  m_nSaveGraph2 = GetProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), 0);
  m_nSaveGraph3 = GetProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), 0);
  m_nSaveGraph4 = GetProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), 0);
  m_nSaveGraph5 = GetProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), 0);
  m_nSaveGraph6 = GetProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), 0);
  m_nSaveGraph7 = GetProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), 0);
  m_nSaveGraph8 = GetProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), 0);
}

void CSettings::SaveSettings()
{
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_PORT"), m_nComPort);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), m_nComBaudrate);
  //theApp.WriteProfileInt( _T("SETTINGS"), _T("LOG_LEVEL"), m_nLogLevel);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("SCALE_COEFF"), ( int) ( m_dKimpSec * 1000.));

  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), m_nSaveGraph1);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), m_nSaveGraph2);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), m_nSaveGraph3);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), m_nSaveGraph4);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), m_nSaveGraph5);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), m_nSaveGraph6);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), m_nSaveGraph7);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), m_nSaveGraph8);
}

BOOL CSettings::EditSettings()
{
  return FALSE;
}