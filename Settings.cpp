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
  m_nControlButtons = theApp.GetProfileInt( _T("SETTINGS"), _T("CONTROL_BUTTONS"), 0);

  m_nComPort =        theApp.GetProfileInt( _T("SETTINGS"), _T("COM_PORT"), 1);
  m_nComBaudrate =    theApp.GetProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), 1);
  
  int nScaleCoeff =   theApp.GetProfileInt( _T("SETTINGS"), _T("SCALE_COEFF"), 2900);
  m_dKimpSec = nScaleCoeff / 1000.;
  
  m_nGraph1DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), 0);
  m_nGraph2DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), 0);
  m_nGraph3DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), 0);
  m_nGraph4DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), 0);
  m_nGraph5DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), 0);
  m_nGraph6DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), 0);
  m_nGraph7DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), 0);
  m_nGraph8DisplayParam = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), 0);

  m_nGraph1Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_MEANING"), 0);
  m_nGraph2Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_MEANING"), 0);
  m_nGraph3Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_MEANING"), 0);
  m_nGraph4Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_MEANING"), 0);
  m_nGraph5Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_MEANING"), 0);
  m_nGraph6Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_MEANING"), 0);
  m_nGraph7Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_MEANING"), 0);
  m_nGraph8Meaning = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_MEANING"), 0);

  m_nGraph1AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_X"), 0);
  m_nGraph2AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_X"), 0);
  m_nGraph3AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_X"), 0);
  m_nGraph4AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_X"), 0);
  m_nGraph5AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_X"), 0);
  m_nGraph6AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_X"), 0);
  m_nGraph7AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_X"), 0);
  m_nGraph8AxisX = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_X"), 0);
}

void CSettings::SaveSettings()
{
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_PORT"), m_nComPort);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), m_nComBaudrate);
  //theApp.WriteProfileInt( _T("SETTINGS"), _T("LOG_LEVEL"), m_nLogLevel);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("SCALE_COEFF"), ( int) ( m_dKimpSec * 1000.));

  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), m_nGraph1DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), m_nGraph2DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), m_nGraph3DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), m_nGraph4DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), m_nGraph5DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), m_nGraph6DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), m_nGraph7DisplayParam);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), m_nGraph8DisplayParam);

  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_MEANING"), m_nGraph1Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_MEANING"), m_nGraph2Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_MEANING"), m_nGraph3Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_MEANING"), m_nGraph4Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_MEANING"), m_nGraph5Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_MEANING"), m_nGraph6Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_MEANING"), m_nGraph7Meaning);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_MEANING"), m_nGraph8Meaning);

  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_X"), m_nGraph1AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_X"), m_nGraph2AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_X"), m_nGraph3AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_X"), m_nGraph4AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_X"), m_nGraph5AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_X"), m_nGraph6AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_X"), m_nGraph7AxisX);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_X"), m_nGraph8AxisX);
}

BOOL CSettings::EditSettings()
{
  return FALSE;
}