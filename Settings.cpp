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
  
  //параметр, отображаемый по оси Y
  m_aSettGraph[0].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), 0));
  m_aSettGraph[1].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), 0));
  m_aSettGraph[2].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), 0));
  m_aSettGraph[3].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), 0));
  m_aSettGraph[4].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), 0));
  m_aSettGraph[5].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), 0));
  m_aSettGraph[6].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), 0));
  m_aSettGraph[7].SetAxisY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), 0));

  //осреднения
  m_aSettGraph[0].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_MEANING"), 0));
  m_aSettGraph[1].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_MEANING"), 0));
  m_aSettGraph[2].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_MEANING"), 0));
  m_aSettGraph[3].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_MEANING"), 0));
  m_aSettGraph[4].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_MEANING"), 0));
  m_aSettGraph[5].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_MEANING"), 0));
  m_aSettGraph[6].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_MEANING"), 0));
  m_aSettGraph[7].SetMeaning( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_MEANING"), 0));

  //параметр, отображаемый по оси X
  m_aSettGraph[0].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_X"), 0));
  m_aSettGraph[1].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_X"), 0));
  m_aSettGraph[2].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_X"), 0));
  m_aSettGraph[3].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_X"), 0));
  m_aSettGraph[4].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_X"), 0));
  m_aSettGraph[5].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_X"), 0));
  m_aSettGraph[6].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_X"), 0));
  m_aSettGraph[7].SetAxisX( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_X"), 0));

  //флаг заданности MinY
  m_aSettGraph[0].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_BMINY"), 0));
  m_aSettGraph[1].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_BMINY"), 0));
  m_aSettGraph[2].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_BMINY"), 0));
  m_aSettGraph[3].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_BMINY"), 0));
  m_aSettGraph[4].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_BMINY"), 0));
  m_aSettGraph[5].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_BMINY"), 0));
  m_aSettGraph[6].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_BMINY"), 0));
  m_aSettGraph[7].Set_bMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_BMINY"), 0));

  //флаг заданности MinY
  m_aSettGraph[0].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_BMAXY"), 0));
  m_aSettGraph[1].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_BMAXY"), 0));
  m_aSettGraph[2].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_BMAXY"), 0));
  m_aSettGraph[3].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_BMAXY"), 0));
  m_aSettGraph[4].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_BMAXY"), 0));
  m_aSettGraph[5].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_BMAXY"), 0));
  m_aSettGraph[6].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_BMAXY"), 0));
  m_aSettGraph[7].Set_bMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_BMAXY"), 0));

  //MinY
  m_aSettGraph[0].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMINY"), 0) / 1000.);
  m_aSettGraph[1].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_DBLMINY"), 0) / 1000.);
  m_aSettGraph[2].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_DBLMINY"), 0) / 1000.);
  m_aSettGraph[3].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_DBLMINY"), 0) / 1000.);
  m_aSettGraph[4].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_DBLMINY"), 0) / 1000.);
  m_aSettGraph[5].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_DBLMINY"), 0) / 1000.);
  m_aSettGraph[6].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_DBLMINY"), 0) / 1000.);
  m_aSettGraph[7].Set_dblMinY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_DBLMINY"), 0) / 1000.);

  //MaxY
  m_aSettGraph[0].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[1].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[2].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[3].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[4].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[5].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[6].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_DBLMAXY"), 0) / 1000.);
  m_aSettGraph[7].Set_dblMaxY( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_DBLMAXY"), 0) / 1000.);

  //LineColor
  m_aSettGraph[0].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH1_LINECOLOR"), 0));
  m_aSettGraph[1].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH2_LINECOLOR"), 0));
  m_aSettGraph[2].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH3_LINECOLOR"), 0));
  m_aSettGraph[3].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH4_LINECOLOR"), 0));
  m_aSettGraph[4].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH5_LINECOLOR"), 0));
  m_aSettGraph[5].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH6_LINECOLOR"), 0));
  m_aSettGraph[6].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH7_LINECOLOR"), 0));
  m_aSettGraph[7].SetLineColor( theApp.GetProfileInt( _T("SETTINGS"), _T("GRAPH8_LINECOLOR"), 0));

  //BOOL m_bLine;                   //соединять линиями
  //int m_nGraph1LineWidth;         //толщина линии

}

void CSettings::SaveSettings()
{
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_PORT"), m_nComPort);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("COM_BAUDRATE"), m_nComBaudrate);
  //theApp.WriteProfileInt( _T("SETTINGS"), _T("LOG_LEVEL"), m_nLogLevel);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("SCALE_COEFF"), ( int) ( m_dKimpSec * 1000.));


  //параметр, отображаемый по оси Y
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_PARAM"), m_aSettGraph[0].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_PARAM"), m_aSettGraph[1].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_PARAM"), m_aSettGraph[2].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_PARAM"), m_aSettGraph[3].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_PARAM"), m_aSettGraph[4].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_PARAM"), m_aSettGraph[5].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_PARAM"), m_aSettGraph[6].GetAxisY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_PARAM"), m_aSettGraph[7].GetAxisY());

  //осреднения
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_MEANING"), m_aSettGraph[0].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_MEANING"), m_aSettGraph[1].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_MEANING"), m_aSettGraph[2].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_MEANING"), m_aSettGraph[3].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_MEANING"), m_aSettGraph[4].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_MEANING"), m_aSettGraph[5].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_MEANING"), m_aSettGraph[6].GetMeaning());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_MEANING"), m_aSettGraph[7].GetMeaning());
  
  //параметр, отображаемый по оси X
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_X"), m_aSettGraph[0].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_X"), m_aSettGraph[1].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_X"), m_aSettGraph[2].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_X"), m_aSettGraph[3].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_X"), m_aSettGraph[4].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_X"), m_aSettGraph[5].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_X"), m_aSettGraph[6].GetAxisX());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_X"), m_aSettGraph[7].GetAxisX());

  //флаг заданности MinY
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_BMINY"), m_aSettGraph[0].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_BMINY"), m_aSettGraph[1].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_BMINY"), m_aSettGraph[2].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_BMINY"), m_aSettGraph[3].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_BMINY"), m_aSettGraph[4].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_BMINY"), m_aSettGraph[5].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_BMINY"), m_aSettGraph[6].Get_bMinY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_BMINY"), m_aSettGraph[7].Get_bMinY());

  //флаг заданности MaxY
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_BMAXY"), m_aSettGraph[0].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_BMAXY"), m_aSettGraph[1].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_BMAXY"), m_aSettGraph[2].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_BMAXY"), m_aSettGraph[3].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_BMAXY"), m_aSettGraph[4].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_BMAXY"), m_aSettGraph[5].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_BMAXY"), m_aSettGraph[6].Get_bMaxY());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_BMAXY"), m_aSettGraph[7].Get_bMaxY());

  //MinY
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMINY"), ( int) m_aSettGraph[0].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_DBLMINY"), ( int) m_aSettGraph[1].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_DBLMINY"), ( int) m_aSettGraph[2].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_DBLMINY"), ( int) m_aSettGraph[3].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_DBLMINY"), ( int) m_aSettGraph[4].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_DBLMINY"), ( int) m_aSettGraph[5].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_DBLMINY"), ( int) m_aSettGraph[6].Get_dblMinY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_DBLMINY"), ( int) m_aSettGraph[7].Get_dblMinY() * 1000);
  
  //MaxY
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_DBLMAXY"), ( int) m_aSettGraph[0].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_DBLMAXY"), ( int) m_aSettGraph[1].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_DBLMAXY"), ( int) m_aSettGraph[2].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_DBLMAXY"), ( int) m_aSettGraph[3].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_DBLMAXY"), ( int) m_aSettGraph[4].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_DBLMAXY"), ( int) m_aSettGraph[5].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_DBLMAXY"), ( int) m_aSettGraph[6].Get_dblMaxY() * 1000);
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_DBLMAXY"), ( int) m_aSettGraph[7].Get_dblMaxY() * 1000);

  //LineColor
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH1_LINECOLOR"), m_aSettGraph[0].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH2_LINECOLOR"), m_aSettGraph[1].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH3_LINECOLOR"), m_aSettGraph[2].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH4_LINECOLOR"), m_aSettGraph[3].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH5_LINECOLOR"), m_aSettGraph[4].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH6_LINECOLOR"), m_aSettGraph[5].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH7_LINECOLOR"), m_aSettGraph[6].GetLineColor());
  theApp.WriteProfileInt( _T("SETTINGS"), _T("GRAPH8_LINECOLOR"), m_aSettGraph[7].GetLineColor());

  
  //BOOL m_bLine;                   //соединять линиями
  //int m_nGraph1LineWidth;         //толщина линии
}

BOOL CSettings::EditSettings()
{
  return FALSE;
}