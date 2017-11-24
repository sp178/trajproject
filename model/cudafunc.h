#pragma once
#include"guidancedata.h"

#include"..\utility\spinterplation.hpp"
#include "spmodel.h"
#include"..\utility\spmath.h"


class cudafunc
{
public:
	int initialCudaData(BLOCK<2> * _CD, BLOCK<2> * _CL, BLOCK<2> * _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma);
	int TrajCaclCUDA(Traj* _beg, spfloat* _targrt, spfloat _step, spfloat _sigma, spfloat _searchangle);
	int initialCudaDataConst(BLOCK<2>* _CD, BLOCK<2>* _CL, BLOCK<2>* _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma);
protected:
	Traj*		cuTraj = nullptr;
	spfloat*	cuoutdata = nullptr;
	BLOCK<2>* cuCD = nullptr;
	BLOCK<2>* cuCL = nullptr;
	BLOCK<2>* cuCZ = nullptr;
	BLOCK<1>* curho = nullptr;
	BLOCK<1>* cuma = nullptr;
	bool isinitial = false;
	rk4_state* m_rkdata = nullptr;
	spfloat*   m_paramdata = nullptr;
};
