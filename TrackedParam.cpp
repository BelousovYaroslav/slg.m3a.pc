// TrackedParam.cpp: implementation of the CTrackedParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "TrackedParam.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrackedParam::CTrackedParam( int n)
{  
  m_cbfr100ms = new CSlgCircleBuffer( n);
  m_cbfr1s = new CSlgCircleBuffer( n);
  m_cbfr10s = new CSlgCircleBuffer( n);
  m_cbfr100s = new CSlgCircleBuffer( n);
}

CTrackedParam::~CTrackedParam()
{
  FreeUnder();
}

void CTrackedParam::FreeUnder()
{
  if( m_cbfr100ms != NULL) { m_cbfr100ms->free(); free( m_cbfr100ms); m_cbfr100ms = NULL; }
  if( m_cbfr1s    != NULL) { m_cbfr1s->free();    free( m_cbfr1s);    m_cbfr1s    = NULL; }
  if( m_cbfr10s   != NULL) { m_cbfr10s->free();   free( m_cbfr10s);   m_cbfr10s   = NULL; }
  if( m_cbfr100s  != NULL) { m_cbfr100s->free();  free( m_cbfr100s);  m_cbfr100s  = NULL; }
}

void CTrackedParam::RecalculateStatisticUnder() {
  if( m_cbfr100ms != NULL) { m_cbfr100ms->RecalculateStatistic(); }
  if( m_cbfr1s    != NULL) { m_cbfr1s->RecalculateStatistic();    }
  if( m_cbfr10s   != NULL) { m_cbfr10s->RecalculateStatistic();   }
  if( m_cbfr100s  != NULL) { m_cbfr100s->RecalculateStatistic();  }
}

void CTrackedParam::ResetUnder() {
  if( m_cbfr100ms != NULL) { m_cbfr100ms->Reset(); }
  if( m_cbfr1s    != NULL) { m_cbfr1s->Reset();    }
  if( m_cbfr10s   != NULL) { m_cbfr10s->Reset();   }
  if( m_cbfr100s  != NULL) { m_cbfr100s->Reset();  }
}
