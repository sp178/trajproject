#pragma once
#include"guidancedata.h"

#include"..\utility\spfactory.hpp"
#include "spmodel.h"
#include"..\utility\spmath.h"

class cpufunc
{
public:
	cpufunc();
	~cpufunc();
	int initialData(BLOCK<2> * _CD, BLOCK<2> * _CL, BLOCK<2> * _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma);
	int TrajCacl(Traj* _beg, spfloat* _targrt, spfloat _step, spfloat _sigma, spfloat _searchangle);
	int initialDataConst(BLOCK<2>* _CD, BLOCK<2>* _CL, BLOCK<2>* _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma);
private:
	friend int TrajTory3DCPU(spfloat _time, const spfloat* _x, spfloat* _f, spfloat* _param, uint32_t _index);

	static InterRecod<2>* m_cdreod;
	static InterRecod<2>* m_clreod;
	static InterRecod<2>* m_czreod;
	static InterRecod<1>* m_rhorecod;
	static InterRecod<1>* m_marecod;
	static BLOCK<2>* m_CD ;
	static BLOCK<2>* m_CL ;
	static BLOCK<2>* m_CZ ;
	static BLOCK<1>* m_rho;
	static BLOCK<1>* m_ma ;

	Traj*		cuTraj = nullptr;
	spfloat*	cuoutdata = nullptr;
	rk4_state*  m_rkdata = nullptr;
	double*		m_rkengindata = nullptr;
	spfloat*    m_paramdata = nullptr;
};