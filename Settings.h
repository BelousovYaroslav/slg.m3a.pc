// Settings.h: interface for the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_)
#define AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

  int  GetParam4Graph1() { return m_nSaveGraph1;}
  void SetParam4Graph1( int nNewVal) { m_nSaveGraph1 = nNewVal;}

  int  GetParam4Graph2() { return m_nSaveGraph2;}
  void SetParam4Graph2( int nNewVal) { m_nSaveGraph2 = nNewVal;}

  int  GetParam4Graph3() { return m_nSaveGraph3;}
  void SetParam4Graph3( int nNewVal) { m_nSaveGraph3 = nNewVal;}

  int  GetParam4Graph4() { return m_nSaveGraph4;}
  void SetParam4Graph4( int nNewVal) { m_nSaveGraph4 = nNewVal;}

  int  GetParam4Graph5() { return m_nSaveGraph5;}
  void SetParam4Graph5( int nNewVal) { m_nSaveGraph5 = nNewVal;}

  int  GetParam4Graph6() { return m_nSaveGraph6;}
  void SetParam4Graph6( int nNewVal) { m_nSaveGraph6 = nNewVal;}

  int  GetParam4Graph7() { return m_nSaveGraph7;}
  void SetParam4Graph7( int nNewVal) { m_nSaveGraph7 = nNewVal;}

  int  GetParam4Graph8() { return m_nSaveGraph8;}
  void SetParam4Graph8( int nNewVal) { m_nSaveGraph8 = nNewVal;}

private:
  int m_nLogLevel;
  
  int m_nComPort;           //сохраняемый-вычитываемый из реестра COM-порт
  int m_nComBaudrate;       //сохраняемая-вычитываемая из реестра скорость COM-порта
  int m_nControlButtons;    //сохраняемый-вычитываемый из реестра флаг показывать ли управляющие контролы
  double m_dKimpSec;        //сохраняемый-вычитываемый из реестра масштабный коэффициент ("/imp)

  int m_nSaveGraph1;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 1
  int m_nSaveGraph2;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 2
  int m_nSaveGraph3;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 3
  int m_nSaveGraph4;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 4
  int m_nSaveGraph5;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 5
  int m_nSaveGraph6;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 6
  int m_nSaveGraph7;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 7
  int m_nSaveGraph8;        //сохраняемый-вычитываемый из реестра параметр для отображения в графике 8
};

#endif // !defined(AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_)
