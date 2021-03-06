// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__D22C2EFC_C6F9_4978_B8F1_787ADB7C450A__INCLUDED_)
#define AFX_STDAFX_H__D22C2EFC_C6F9_4978_B8F1_787ADB7C450A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "NiAll.h"
using namespace NI;

#define CYCLE_BUFFER_DN_DU 2500
//#define K_IMP_SEC   1.866						//������� �����
//#define K_IMP_SEC   2.903					//����� �����
#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5
#define ERROR_6 6
//#define ERROR_7 7

#define MEANING_TIME_TACTS 0
#define MEANING_TIME_100MS 1
#define MEANING_TIME_1S    2
#define MEANING_TIME_10S   3
#define MEANING_TIME_100S  4


#define MIN_T_THERMO_CALIBRATION -60
#define MAX_T_THERMO_CALIBRATION 60
#define THERMO_CALIB_PARAMS_BASE 10000

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_STDAFX_H__D22C2EFC_C6F9_4978_B8F1_787ADB7C450A__INCLUDED_)
