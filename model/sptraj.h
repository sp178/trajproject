#pragma once
#include "spmodel.h"
#include"../utility/spmath.h"
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
	int onInitial(); //初始化对应SM_INITIAL
	int onClear();   //初始化对应SM_INITIAL
	int onUpdate();  //对应 SM_WRITE
	int onStop();    //对应SM_STOP
	int onDerive();  //对应SM_CONTINUE
	int onStart();
protected:
	bool m_InitialData = false;
protected:
	double B, Re;
	double graveV, graveT;
	double P, Ma;
	double angleofdirection;			//初始发射方位角和航迹偏角夹角
	double SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
	double POWRA_R;
	double sigmamin;
	double	m_lunchAngle;		//发射方位角
	double	m_landhigh;			//着陆点高度
	double	m_landSpeed;		//着陆点速度
	double	Dgamma, Dchi;
};

