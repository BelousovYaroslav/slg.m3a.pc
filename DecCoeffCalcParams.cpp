// DecCoeffCalcParams.cpp: implementation of the CDecCoeffCalcParams class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "slg2.h"
#include "DecCoeffCalcParams.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDecCoeffCalcParams::CDecCoeffCalcParams()
{
  m_bDecCoeffCalculation = false;
  
  for( int i=0; i<CYCLE_BUFFER_DN_DU; i++) {
    m_dbl_dN[ CYCLE_BUFFER_DN_DU] = 0.;
    m_dbl_dU[ CYCLE_BUFFER_DN_DU] = 0.;
    m_dbl_kDec[ CYCLE_BUFFER_DN_DU] = 0.;
  }

  m_nGraph_cntr = 0;
  m_bGraph_overround = false;

  m_dbl_dN_acc = 0.;
  m_dbl_dU_acc = 0.;
  m_dbl_Kdec_acc = 0.;
  m_n_Kdec_acc_cntr = 0;

  m_dbl_du_low_filter = 200.;
  m_dbl_dw_high_filter = 50.;
}

CDecCoeffCalcParams::~CDecCoeffCalcParams()
{

}

void CDecCoeffCalcParams::AddDnDuDtPoint( short dN, short dU, short dT)
{
  if( fabs( dU) > m_dbl_du_low_filter) {

    double dblW = ( double) dN - ( double) dU * m_dbl_Kdec_acc / ( double) m_n_Kdec_acc_cntr;
    //if( dblW < m_dbl_dw_high_filter) {

      m_dbl_dN[ m_nGraph_cntr] = ( double) dN;
      m_dbl_dU[ m_nGraph_cntr] = ( double) dU;
      m_dbl_kDec[ m_nGraph_cntr] = ( double) dN / ( double) dU;
    
      
      
  
      m_dbl_dN_acc += fabs( dN);
	    m_dbl_dU_acc += fabs( dU);

      m_dbl_Kdec_acc += m_dbl_dN_acc / m_dbl_dU_acc;
      m_n_Kdec_acc_cntr++;


      m_dbl_kDec[ m_nGraph_cntr] = m_dbl_Kdec_acc / m_n_Kdec_acc_cntr;

      m_nGraph_cntr++;
      if( m_nGraph_cntr >= CYCLE_BUFFER_DN_DU) {
        m_bGraph_overround = true;
        m_nGraph_cntr = 0;
      }
    //}
  }
}

void CDecCoeffCalcParams::Reset( void)
{
  m_dbl_dN_acc = 0.;
  m_dbl_dU_acc = 0.;
  m_dbl_Kdec_acc = 0.;
  m_n_Kdec_acc_cntr = 0;

  m_nGraph_cntr = 0;
  m_bGraph_overround = false;
  /*
  m_n_dN_max = 0.;
  m_n_dN_min = 0.;
  m_n_dU_max = 0.;
  m_n_dU_min = 0.;
  */
}
