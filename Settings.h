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

  //отображаемый параметр
  int  GetParam4Graph1() { return m_nGraph1DisplayParam;}
  void SetParam4Graph1( int nNewVal) { m_nGraph1DisplayParam = nNewVal;}

  int  GetParam4Graph2() { return m_nGraph2DisplayParam;}
  void SetParam4Graph2( int nNewVal) { m_nGraph2DisplayParam = nNewVal;}

  int  GetParam4Graph3() { return m_nGraph3DisplayParam;}
  void SetParam4Graph3( int nNewVal) { m_nGraph3DisplayParam = nNewVal;}

  int  GetParam4Graph4() { return m_nGraph4DisplayParam;}
  void SetParam4Graph4( int nNewVal) { m_nGraph4DisplayParam = nNewVal;}

  int  GetParam4Graph5() { return m_nGraph5DisplayParam;}
  void SetParam4Graph5( int nNewVal) { m_nGraph5DisplayParam = nNewVal;}

  int  GetParam4Graph6() { return m_nGraph6DisplayParam;}
  void SetParam4Graph6( int nNewVal) { m_nGraph6DisplayParam = nNewVal;}

  int  GetParam4Graph7() { return m_nGraph7DisplayParam;}
  void SetParam4Graph7( int nNewVal) { m_nGraph7DisplayParam = nNewVal;}

  int  GetParam4Graph8() { return m_nGraph8DisplayParam;}
  void SetParam4Graph8( int nNewVal) { m_nGraph8DisplayParam = nNewVal;}

  //Осреденения
  int  GetGraph1Meaning() { return m_nGraph1Meaning;}
  void SetGraph1Meaning( int nNewVal) { m_nGraph1Meaning = nNewVal;}

  int  GetGraph2Meaning() { return m_nGraph2Meaning;}
  void SetGraph2Meaning( int nNewVal) { m_nGraph2Meaning = nNewVal;}

  int  GetGraph3Meaning() { return m_nGraph3Meaning;}
  void SetGraph3Meaning( int nNewVal) { m_nGraph3Meaning = nNewVal;}

  int  GetGraph4Meaning() { return m_nGraph4Meaning;}
  void SetGraph4Meaning( int nNewVal) { m_nGraph4Meaning = nNewVal;}

  int  GetGraph5Meaning() { return m_nGraph5Meaning;}
  void SetGraph5Meaning( int nNewVal) { m_nGraph5Meaning = nNewVal;}

  int  GetGraph6Meaning() { return m_nGraph6Meaning;}
  void SetGraph6Meaning( int nNewVal) { m_nGraph6Meaning = nNewVal;}

  int  GetGraph7Meaning() { return m_nGraph7Meaning;}
  void SetGraph7Meaning( int nNewVal) { m_nGraph7Meaning = nNewVal;}

  int  GetGraph8Meaning() { return m_nGraph8Meaning;}
  void SetGraph8Meaning( int nNewVal) { m_nGraph8Meaning = nNewVal;}

  //Оси Х
  int  GetGraph1_AxX() { return m_nGraph1AxisX;}
  void SetGraph1_AxX( int nAxX) { m_nGraph1AxisX = nAxX;}

  int  GetGraph2_AxX() { return m_nGraph2AxisX;}
  void SetGraph2_AxX( int nAxX) { m_nGraph2AxisX = nAxX;}

  int  GetGraph3_AxX() { return m_nGraph3AxisX;}
  void SetGraph3_AxX( int nAxX) { m_nGraph3AxisX = nAxX;}

  int  GetGraph4_AxX() { return m_nGraph4AxisX;}
  void SetGraph4_AxX( int nAxX) { m_nGraph4AxisX = nAxX;}

  int  GetGraph5_AxX() { return m_nGraph5AxisX;}
  void SetGraph5_AxX( int nAxX) { m_nGraph5AxisX = nAxX;}

  int  GetGraph6_AxX() { return m_nGraph6AxisX;}
  void SetGraph6_AxX( int nAxX) { m_nGraph6AxisX = nAxX;}

  int  GetGraph7_AxX() { return m_nGraph7AxisX;}
  void SetGraph7_AxX( int nAxX) { m_nGraph7AxisX = nAxX;}

  int  GetGraph8_AxX() { return m_nGraph8AxisX;}
  void SetGraph8_AxX( int nAxX) { m_nGraph8AxisX = nAxX;}

private:
  int m_nLogLevel;
  
  int m_nComPort;           //сохраняемый-вычитываемый из реестра COM-порт
  int m_nComBaudrate;       //сохраняемая-вычитываемая из реестра скорость COM-порта
  int m_nControlButtons;    //сохраняемый-вычитываемый из реестра флаг показывать ли управляющие контролы
  double m_dKimpSec;        //сохраняемый-вычитываемый из реестра масштабный коэффициент ("/imp)

  int m_nGraph1DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 1
  int m_nGraph1AxisX;         //ось X (время, температуры)
  int m_nGraph1Meaning;       //осреднение графика 1

  int m_nGraph2DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 2
  int m_nGraph2AxisX;         //ось X (время, температуры)
  int m_nGraph2Meaning;       //осреднение графика 2

  int m_nGraph3DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 3
  int m_nGraph3AxisX;         //ось X (время, температуры)
  int m_nGraph3Meaning;       //осреднение графика 3

  int m_nGraph4DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 4
  int m_nGraph4AxisX;         //ось X (время, температуры)
  int m_nGraph4Meaning;       //осреднение графика 4

  int m_nGraph5DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 5
  int m_nGraph5AxisX;         //ось X (время, температуры)
  int m_nGraph5Meaning;       //осреднение графика 5

  int m_nGraph6DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 6
  int m_nGraph6AxisX;         //ось X (время, температуры)
  int m_nGraph6Meaning;       //осреднение графика 6

  int m_nGraph7DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 7
  int m_nGraph7AxisX;         //ось X (время, температуры)
  int m_nGraph7Meaning;       //осреднение графика 7

  int m_nGraph8DisplayParam;  //сохраняемый-вычитываемый из реестра параметр для отображения в графике 8
  int m_nGraph8AxisX;         //ось X (время, температуры)
  int m_nGraph8Meaning;       //осреднение графика 8
};

#endif // !defined(AFX_SETTINGS_H__D5FE8E11_FAAD_46A6_919E_09E9BC72FDAA__INCLUDED_)
