#pragma once
#include"../utility/spfactory.hpp"
#include "spmodel.h"
#include"../utility/spmath.h"

struct aeroIn
{
	double h, V, alpha, beta, Mass;
};
struct aeroout
{
	double  D, L, Z, ma, A, N, CLD, turnR, MX, MY, MZ;
};
class spaero :
	public spmodel,public aeroIn,public aeroout
{
public:
	spaero();
	~spaero();

protected:
	int onInitial(); //初始化对应SM_INITIAL
	int onClear();   //初始化对应SM_INITIAL
	int onUpdate();  //对应 SM_WRITE
	int onStop();    //对应SM_STOP
	int onDerive();  //对应SM_CONTINUE
	int onStart();
protected:
	double  m_Serf;
	double	m_Lengthf;
	double  m_interAreo[2];
	interdata<2>*	m_interCL;
	interdata<2>*	m_interCD;
	interdata<2>*	m_interCZ;
	interdata<1>*	m_interpho;
	interdata<1>*	m_interma;
	interdata<2>* m_interMX;		//轴向气动力
	interdata<2>* m_interMY;		//法相气动力
	interdata<2>* m_interMZ;		//轴向气动力
	interdata<2>* m_interBeta5MX;		//法相气动力
	interdata<2>* m_interBeta5MY;		//轴向气动力
	interdata<2>* m_interBeta5MZ;		//法相气动力



protected:

protected:
	double   m_deltaL;
	double   m_deltaD;
	double	 m_deltaRho;
};

