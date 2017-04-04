// SlgGroupNewAverager.cpp: implementation of the CSlgGroupNewAverager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "SlgGroupNewAverager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSlgGroupNewAverager::CSlgGroupNewAverager()
{
}

CSlgGroupNewAverager::~CSlgGroupNewAverager()
{
}

void CSlgGroupNewAverager::CommonReset()
{
  m_100ms.Reset();
  m_1s.Reset();
  m_10s.Reset();
  m_100s.Reset();
}

void CSlgGroupNewAverager::CommonAddPoint(double val)
{
  m_100ms.AddPoint( val);
  m_1s.AddPoint( val);
  m_10s.AddPoint( val);
  m_100s.AddPoint( val);
}
