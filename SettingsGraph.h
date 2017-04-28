// SettingsGraph.h: interface for the CSettingsGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_)
#define AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSettingsGraph  
{
public:
	CSettingsGraph();
	virtual ~CSettingsGraph();

  //отображаемый параметр (значение по оси Y)
  int  GetAxisY() { return m_nAxisY;}
  void SetAxisY( int nAxY) { m_nAxisY = nAxY;}

  //Осреденения
  int  GetMeaning() { return m_nMeaning;}
  void SetMeaning( int nNewVal) { m_nMeaning = nNewVal;}

  //Значение оси Х
  int  GetAxisX() { return m_nAxisX;}
  void SetAxisX( int nAxX) { m_nAxisX = nAxX;}

  //Min-max графиков
  BOOL Get_bMinY()            {  return m_bMinY; }
  void Set_bMinY( BOOL bMinY) {  m_bMinY = bMinY; }
  BOOL Get_bMaxY()            {  return m_bMaxY; }
  void Set_bMaxY( BOOL bMaxY) {  m_bMaxY = bMaxY; } 
  double Get_dblMinY()                {  return m_dblMinY; }
  void   Set_dblMinY( double dblMinY) {  m_dblMinY = dblMinY; }
  double Get_dblMaxY()                {  return m_dblMaxY; }
  void   Set_dblMaxY( double dblMaxY) {  m_dblMaxY = dblMaxY; }

  //настройки точек и линии графиков
  COLORREF GetLineColor() { return m_clrLineColor;}
  void SetLineColor( COLORREF clr) { m_clrLineColor = clr;}
  
private:
  int m_nAxisY;                   //отображаемый по оси Y параметр
  int m_nAxisX;                   //ось X (время, температуры)
  int m_nMeaning;                 //осреднение графика 1
  BOOL m_bMinY;                   //включенная опция minY
  double m_dblMinY;               //minY
  BOOL m_bMaxY;                   //включенная опция maxY
  double m_dblMaxY;               //maxY
  COLORREF m_clrLineColor;        //цвет линии
  //BOOL m_bLine;                   //соединять линиями
  //int m_nLineWidth;         //толщина линии
};

#endif // !defined(AFX_SETTINGSGRAPH_H__B2BBF022_411B_4ECF_BC21_C3E020A53076__INCLUDED_)
