// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_)
#define AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SettingsGraph.h"

class CSettings  
{
public:
	BOOL EditSettings( void);
	void SaveSettings();
	void LoadSettings();
	CSettings();
	virtual ~CSettings();

  int GetComPort() { return m_nComPort;}
  void SetComPort( int nNewPort) { m_nComPort = nNewPort;}

  int GetComBaudrate() { return m_nComBaudrate;}
  void SetComBaudrate( int nNewBaudrate) { m_nComBaudrate = nNewBaudrate;}

  int GetLogLevel() { return m_nLogLevel;}
  void SetLogLevel( int nNewVal) { m_nLogLevel = nNewVal;}

  int GetControlButtons() { return m_nControlButtons;}

  double GetScaleCoeff() { return m_dKimpSec;}
  void SetScaleCoeff( double dblNewVal) { m_dKimpSec = dblNewVal;}

  CSettingsGraph *GetGraphSettings( int nGraph) { return &m_aSettGraph[nGraph]; }
  
private:
  int m_nLogLevel;
  
  int m_nComPort;           //сохран€емый-вычитываемый из реестра COM-порт
  int m_nComBaudrate;       //сохран€ема€-вычитываема€ из реестра скорость COM-порта
  int m_nControlButtons;    //сохран€емый-вычитываемый из реестра флаг показывать ли управл€ющие контролы
  double m_dKimpSec;        //сохран€емый-вычитываемый из реестра масштабный коэффициент ("/imp)
  CSettingsGraph m_aSettGraph[8]; //массив настроек графиков
};

#endif // !defined(AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_)
