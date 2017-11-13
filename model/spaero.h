#pragma once
#include"..\utility\spfactory.hpp"
#include "spmodel.h"
#include"..\utility\spmath.h"

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
	int onInitial(); //��ʼ����ӦSM_INITIAL
	int onClear();   //��ʼ����ӦSM_INITIAL
	int onUpdate();  //��Ӧ SM_WRITE
	int onStop();    //��ӦSM_STOP
	int onDerive();  //��ӦSM_CONTINUE
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
	interdata<2>* m_interMX;		//����������
	interdata<2>* m_interMY;		//����������
	interdata<2>* m_interMZ;		//����������
	interdata<2>* m_interBeta5MX;		//����������
	interdata<2>* m_interBeta5MY;		//����������
	interdata<2>* m_interBeta5MZ;		//����������



protected:

protected:
	double   m_deltaL;
	double   m_deltaD;
	double	 m_deltaRho;
};

