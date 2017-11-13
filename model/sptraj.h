#pragma once
#include "spmodel.h"
#include"..\utility\spmath.h"
struct Traj {
	double r, miu, lambda, V, gamma, chi, rengeS;
};
struct indata {
	double D, L, Z, sigma, Mass;
};
struct outData :public Traj {
	double h, Energy, Dgamma, Dchi;
};
class sptraj :
	public spmodel, public outData, public indata
{
public:
	sptraj();
	~sptraj();
protected:
	int onInitial(); //��ʼ����ӦSM_INITIAL
	int onClear();   //��ʼ����ӦSM_INITIAL
	int onUpdate();  //��Ӧ SM_WRITE
	int onStop();    //��ӦSM_STOP
	int onDerive();  //��ӦSM_CONTINUE
	int onStart();
protected:
	bool m_InitialData = false;
protected:
	double B, Re;
	double graveV, graveT;
	double P, Ma;
	double angleofdirection;			//��ʼ���䷽λ�Ǻͺ���ƫ�Ǽн�
	double SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
	double POWRA_R;
	double sigmamin;
	double	m_lunchAngle;		//���䷽λ��
	double	m_landhigh;			//��½��߶�
	double	m_landSpeed;		//��½���ٶ�
	double	Dgamma, Dchi;
};

