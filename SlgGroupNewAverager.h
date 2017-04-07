// SlgGroupNewAverager.h: interface for the CSlgGroupNewAverager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLGGROUPNEWAVERAGER_H__D465F879_E8B6_4350_A325_7499FD1E5BB9__INCLUDED_)
#define AFX_SLGGROUPNEWAVERAGER_H__D465F879_E8B6_4350_A325_7499FD1E5BB9__INCLUDED_

#include "SlgNewAverager.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSlgGroupNewAverager  
{
public:
	void CommonAddPoint( double val);
	void CommonReset();
	CSlgGroupNewAverager();
	virtual ~CSlgGroupNewAverager();
  CSlgNewAverager *Get_100ms() { return &m_100ms; }
  CSlgNewAverager *Get_1s()    { return &m_1s;    }
  CSlgNewAverager *Get_10s()   { return &m_10s;   }
  CSlgNewAverager *Get_100s()  { return &m_100s;  }
private:
  CSlgNewAverager m_100ms;
  CSlgNewAverager m_1s;
  CSlgNewAverager m_10s;
  CSlgNewAverager m_100s;
};

#endif // !defined(AFX_SLGGROUPNEWAVERAGER_H__D465F879_E8B6_4350_A325_7499FD1E5BB9__INCLUDED_)
