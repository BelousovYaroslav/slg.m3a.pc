//{{AFX_INCLUDES()
#include "commondialog.h"
#include "BgColoredLabel.h"
//}}AFX_INCLUDES
#if !defined(AFX_DLGGRAPHSETUP_H__A3F7C6B3_C11D_4ACE_A7CF_AA48FF998FB9__INCLUDED_)
#define AFX_DLGGRAPHSETUP_H__A3F7C6B3_C11D_4ACE_A7CF_AA48FF998FB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphSetup dialog

class CDlgGraphSetup : public CDialog
{
// Construction
public:
	void Init( int nGraph);
	CDlgGraphSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGraphSetup)
	enum { IDD = IDD_GRAPH_SETUP };
	CBgColoredLabel	m_ctlColorLabel;
	CCommonDialog1	m_ctlCommonDialog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphSetup)
	afx_msg void OnLblLineColor();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nGraph;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHSETUP_H__A3F7C6B3_C11D_4ACE_A7CF_AA48FF998FB9__INCLUDED_)
