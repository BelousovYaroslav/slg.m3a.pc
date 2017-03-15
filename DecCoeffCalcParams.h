// DecCoeffCalcParams.h: interface for the CDecCoeffCalcParams class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECCOEFFCALCPARAMS_H__76DED412_AF99_4115_B52C_6377D4797BA1__INCLUDED_)
#define AFX_DECCOEFFCALCPARAMS_H__76DED412_AF99_4115_B52C_6377D4797BA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDecCoeffCalcParams  
{
public:
	void Reset();
	void AddDnDuDtPoint( short dN, short dU, short dT);
	CDecCoeffCalcParams();
	virtual ~CDecCoeffCalcParams();

  //���� ��� ������ ��� ������� ���������� ������������ ������
  BOOL   m_bDecCoeffCalculation;
  
  //������� ������ dN, dU, Kdec ��� ���������
  double m_dbl_dN[ CYCLE_BUFFER_DN_DU];
  double m_dbl_dU[ CYCLE_BUFFER_DN_DU];
  double m_dbl_kDec[ CYCLE_BUFFER_DN_DU];
  int    m_nGraph_cntr;
  BOOL   m_bGraph_overround;

  //������������� ����������
  double m_dbl_dN_acc;
  double m_dbl_dU_acc;
  
  double m_dbl_Kdec_acc;
  //�������
  int    m_n_Kdec_acc_cntr;

  //������� �� dU
  double m_dbl_du_low_filter;
  
  //������� �� W
  double m_dbl_dw_high_filter;
};

#endif // !defined(AFX_DECCOEFFCALCPARAMS_H__76DED412_AF99_4115_B52C_6377D4797BA1__INCLUDED_)
