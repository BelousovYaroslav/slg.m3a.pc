// TrackedParam.h: interface for the CTrackedParam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKEDPARAM_H__D2EBEA36_9E11_49BD_B4DC_479ED4C644FF__INCLUDED_)
#define AFX_TRACKEDPARAM_H__D2EBEA36_9E11_49BD_B4DC_479ED4C644FF__INCLUDED_

#include "SlgCircleBuffer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrackedParam  
{
public:
	
	CTrackedParam( int n);
	virtual ~CTrackedParam();
  CSlgCircleBuffer *Get_100ms() { return m_cbfr100ms;}
  CSlgCircleBuffer *Get_1s()    { return m_cbfr1s;}
  CSlgCircleBuffer *Get_10s()   { return m_cbfr10s;}
  CSlgCircleBuffer *Get_100s()  { return m_cbfr100s;}
  void RecalculateStatisticUnder();
  void ResetUnder();
  void FreeUnder( void);
private:
  CSlgCircleBuffer *m_cbfr100ms;
  CSlgCircleBuffer *m_cbfr1s;
  CSlgCircleBuffer *m_cbfr10s;
  CSlgCircleBuffer *m_cbfr100s;
};

#endif // !defined(AFX_TRACKEDPARAM_H__D2EBEA36_9E11_49BD_B4DC_479ED4C644FF__INCLUDED_)
