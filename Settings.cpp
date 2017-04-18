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



  m_bGraph1MinY   = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_BMINY"), 0);
  m_dblGraph1MinY = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMINY"), 0) / 1000.;
  m_bGraph1MaxY   = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_BMAXY"), 0);
  m_dblGraph1MaxY = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMAXY"), 0) / 1000.;
  m_clrGraph1LineColor = theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_LINECOLOR"), 0);
  //BOOL m_bLine;                   //соединять линиями
  //int m_nGraph1LineWidth;         //толщина линии

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

  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_BMINY"), m_bGraph1MinY);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMINY"), ( int) m_dblGraph1MinY * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_BMAXY"), m_bGraph1MaxY);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMAXY"), ( int) m_dblGraph1MaxY * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_LINECOLOR"), m_clrGraph1LineColor);
  //BOOL m_bLine;                   //соединять линиями
  //int m_nGraph1LineWidth;         //толщина линии
}

int CSettings::GetBMinY( int nGraph) {
  int nReturnValue = -1;
  switch( nGraph) {
    case 0: nReturnValue = m_bGraph1MinY; break;
    /*case 1: nReturnValue = m_bGraph2MinY; break;
    case 2: nReturnValue = m_bGraph3MinY; break;
    case 3: nReturnValue = m_bGraph4MinY; break;
    case 4: nReturnValue = m_bGraph5MinY; break;
    case 5: nReturnValue = m_bGraph6MinY; break;
    case 6: nReturnValue = m_bGraph7MinY; break;
    case 6: nReturnValue = m_bGraph8MinY; break;*/
  }
  return nReturnValue;
}

void CSettings::SetBMinY( int nGraph, BOOL bValue) {
  switch( nGraph) {
    case 0: m_bGraph1MinY = bValue; break;
    /*case 1: nReturnValue = m_bGraph2MinY; break;
    case 2: nReturnValue = m_bGraph3MinY; break;
    case 3: nReturnValue = m_bGraph4MinY; break;
    case 4: nReturnValue = m_bGraph5MinY; break;
    case 5: nReturnValue = m_bGraph6MinY; break;
    case 6: nReturnValue = m_bGraph7MinY; break;
    case 6: nReturnValue = m_bGraph8MinY; break;*/
  }
}

double CSettings::GetDblMinY( int nGraph) {
  double dblReturnValue = -1.;
  switch( nGraph) {
    case 0: dblReturnValue = m_dblGraph1MinY; break;
    /*
    case 1: dblReturnValue = m_dblGraph2MinY; break;
    case 2: dblReturnValue = m_dblGraph3MinY; break;
    case 3: dblReturnValue = m_dblGraph4MinY; break;
    case 4: dblReturnValue = m_dblGraph5MinY; break;
    case 5: dblReturnValue = m_dblGraph6MinY; break;
    case 6: dblReturnValue = m_dblGraph7MinY; break;
    case 6: dblReturnValue = m_dblGraph8MinY; break;
    */
  }
  return dblReturnValue;
}

void CSettings::SetDblMinY( int nGraph, double dblValue) {
  switch( nGraph) {
    case 0: m_dblGraph1MinY = dblValue; break;
    /*
    case 1: dblReturnValue = m_dblGraph2MinY; break;
    case 2: dblReturnValue = m_dblGraph3MinY; break;
    case 3: dblReturnValue = m_dblGraph4MinY; break;
    case 4: dblReturnValue = m_dblGraph5MinY; break;
    case 5: dblReturnValue = m_dblGraph6MinY; break;
    case 6: dblReturnValue = m_dblGraph7MinY; break;
    case 6: dblReturnValue = m_dblGraph8MinY; break;
    */
  }  
}

int CSettings::GetBMaxY( int nGraph) {
  int nReturnValue = -1;
  switch( nGraph) {
    case 0: nReturnValue = m_bGraph1MaxY; break;
    /*case 1: nReturnValue = m_bGraph2MaxY; break;
    case 2: nReturnValue = m_bGraph3MaxY; break;
    case 3: nReturnValue = m_bGraph4MaxY; break;
    case 4: nReturnValue = m_bGraph5MaxY; break;
    case 5: nReturnValue = m_bGraph6MaxY; break;
    case 6: nReturnValue = m_bGraph7MaxY; break;
    case 6: nReturnValue = m_bGraph8MaxY; break;*/
  }
  return nReturnValue;
}

void CSettings::SetBMaxY( int nGraph, BOOL bValue) {
  switch( nGraph) {
    case 0: m_bGraph1MaxY = bValue; break;
    /*case 1: nReturnValue = m_bGraph2MaxY; break;
    case 2: nReturnValue = m_bGraph3MaxY; break;
    case 3: nReturnValue = m_bGraph4MaxY; break;
    case 4: nReturnValue = m_bGraph5MaxY; break;
    case 5: nReturnValue = m_bGraph6MaxY; break;
    case 6: nReturnValue = m_bGraph7MaxY; break;
    case 6: nReturnValue = m_bGraph8MaxY; break;*/
  }
}

double CSettings::GetDblMaxY( int nGraph) {
  double dblReturnValue = -1.;
  switch( nGraph) {
    case 0: dblReturnValue = m_dblGraph1MaxY; break;
    /*
    case 1: dblReturnValue = m_dblGraph2MaxY; break;
    case 2: dblReturnValue = m_dblGraph3MaxY; break;
    case 3: dblReturnValue = m_dblGraph4MaxY; break;
    case 4: dblReturnValue = m_dblGraph5MaxY; break;
    case 5: dblReturnValue = m_dblGraph6MaxY; break;
    case 6: dblReturnValue = m_dblGraph7MaxY; break;
    case 6: dblReturnValue = m_dblGraph8MaxY; break;
    */
  }
  return dblReturnValue;
}

void CSettings::SetDblMaxY( int nGraph, double dblValue) {
  switch( nGraph) {
    case 0: m_dblGraph1MaxY = dblValue; break;
    /*
    case 1: dblReturnValue = m_dblGraph2MinY; break;
    case 2: dblReturnValue = m_dblGraph3MinY; break;
    case 3: dblReturnValue = m_dblGraph4MinY; break;
    case 4: dblReturnValue = m_dblGraph5MinY; break;
    case 5: dblReturnValue = m_dblGraph6MinY; break;
    case 6: dblReturnValue = m_dblGraph7MinY; break;
    case 6: dblReturnValue = m_dblGraph8MinY; break;
    */
  }
}

int CSettings::GetGraphSettings( int nGraph, int nParameter) {
  int nReturnValue = -1;
  
  switch( nParameter) {
    case 0: nReturnValue = GetBMinY( nGraph); break;                      //BMINY
    case 1: nReturnValue = ( int) ( GetDblMinY( nGraph) * 1000.); break;  //DBLMINY
    case 2: nReturnValue = GetBMaxY( nGraph); break;                      //BMAXY
    case 3: nReturnValue = ( int) ( GetDblMaxY( nGraph) * 1000.); break;  //DBLMAXY

    case 4: //LINECOLOR
      switch( nGraph) {
        case 0: nReturnValue = m_clrGraph1LineColor; break;
        /*case 1: nReturnValue = m_clrGraph2LineColor; break;
        case 2: nReturnValue = m_clrGraph3LineColor; break;
        case 3: nReturnValue = m_clrGraph4LineColor; break;
        case 4: nReturnValue = m_clrGraph5LineColor; break;
        case 5: nReturnValue = m_clrGraph6LineColor; break;
        case 6: nReturnValue = m_clrGraph7LineColor; break;
        case 6: nReturnValue = m_clrGraph8LineColor; break;*/
      }
    break;
  }

  return nReturnValue;
}

void CSettings::SetGraphSettings( int nGraph, int nParameter, int nValue) {
  switch( nParameter) {
    case 0: SetBMinY( nGraph, nValue); break;           //BMINY
    case 1: SetDblMinY( nGraph, nValue / 1000.); break; //DBLMINY
    case 2: SetBMaxY( nGraph, nValue); break;           //BMAXY
    case 3: SetDblMaxY( nGraph, nValue / 1000.); break; //DBLMAXY

    case 4: //LINECOLOR
      switch( nGraph) {
        case 0: m_clrGraph1LineColor = nValue; break;
        /*case 1: nReturnValue = m_clrGraph2LineColor; break;
        case 2: nReturnValue = m_clrGraph3LineColor; break;
        case 3: nReturnValue = m_clrGraph4LineColor; break;
        case 4: nReturnValue = m_clrGraph5LineColor; break;
        case 5: nReturnValue = m_clrGraph6LineColor; break;
        case 6: nReturnValue = m_clrGraph7LineColor; break;
        case 6: nReturnValue = m_clrGraph8LineColor; break;*/
      }
    break;
  }
}

BOOL CSettings::EditSettings()
{
  return FALSE;
}