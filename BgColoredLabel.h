#if !defined(AFX_BGCOLOREDLABEL_H__3BF955B2_FA6E_4FFF_B947_F90BFFA635B0__INCLUDED_)
#define AFX_BGCOLOREDLABEL_H__3BF955B2_FA6E_4FFF_B947_F90BFFA635B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BgColoredLabel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBgColoredLabel window

class CBgColoredLabel : public CStatic
{
// Construction
public:
	CBgColoredLabel();

// Attributes
public:
private:
  COLORREF m_clr;

// Operations
public:
  COLORREF GetBgColor() { return m_clr; }
  void SetBgColor( COLORREF newcolor) { m_clr = newcolor;}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBgColoredLabel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBgColoredLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBgColoredLabel)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGCOLOREDLABEL_H__3BF955B2_FA6E_4FFF_B947_F90BFFA635B0__INCLUDED_)
