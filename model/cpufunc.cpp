#include"cpufunc.h"
#include<functional>
#include <omp.h>
#define Mass	 36000
#define Sref	 156.0771
#define Lref	 20.29

InterRecod<2>* cpufunc::m_cdreod = nullptr;
InterRecod<2>* cpufunc::m_clreod = nullptr;
InterRecod<2>* cpufunc::m_czreod = nullptr;
InterRecod<1>* cpufunc::m_rhorecod = nullptr;
InterRecod<1>* cpufunc::m_marecod = nullptr;
BLOCK<2>* cpufunc::m_CD = nullptr;
BLOCK<2>* cpufunc::m_CL = nullptr;
BLOCK<2>* cpufunc::m_CZ = nullptr;
BLOCK<1>* cpufunc::m_rho = nullptr;
BLOCK<1>* cpufunc::m_ma = nullptr;
cpufunc::cpufunc()
{
}

cpufunc::~cpufunc()
{
}
int TrajTory3DCPU(spfloat _time, const spfloat* _x, spfloat* _f, spfloat* _param, uint32_t _index)
{

	interPlatheData	interdata;
	spfloat r = _x[0], miu = _x[1], lambda = _x[2], V = _x[3], gamma = _x[4], chi = _x[5];
	spfloat B, Re;
	spfloat sigma;
	spfloat graveV, graveT;
	spfloat P;
	spfloat R[3];
	spfloat angleofdirection;			//初始发射方位角和航迹偏角夹角
	spfloat SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
	//spfloat	COSGAMMALOCAL;
	spfloat POWRA_R;
	POWRA_R = pow(Constant_Earth_RA / r, 2);
	SINMIU = sin(miu);
	COSMIU = cos(miu);
	SINGAMMA = sin(gamma);
	COSGAMMA = cos(gamma);
	SINCHI = sin(chi);
	COSCHI = cos(chi);
	graveV = Constant_Earth_GM / (r * r) * (1 - 1.5*Constant_Earth_J2*POWRA_R*(5 * SINMIU*SINMIU - 1));
	graveT = 3 * Constant_Earth_J2*Constant_Earth_GM / (r*r)*POWRA_R;
	B = atan2(SINMIU / COSMIU, Constant_Earth_e2);
	Re = Constant_Earth_RA*Constant_Earth_e / sqrt((SINMIU*SINMIU + Constant_Earth_e2*COSMIU*COSMIU));
	interdata.h = sqrt(r*r - Re*Re*pow(sin(B - miu), 2)) - Re*cos(B - miu);
	interdata.alpha = getAlpha(V);
	//COSGAMMALOCAL = cos(gamma + length(_param[3], _param[4], lambda, miu));
	P = 0.5*interplate<1>(cpufunc::m_rho, &interdata.h, cpufunc::m_rhorecod + _index)*Sref*V*V;
	interdata.Ma = V / interplate<1>(cpufunc::m_ma, (double*)&interdata.h, cpufunc::m_marecod + _index);
	R[0] = -P*interplate<2>(cpufunc::m_CD, (double*)&interdata.alpha, cpufunc::m_cdreod+_index);
	R[1] = P*interplate<2>(cpufunc::m_CL, (double*)&interdata.alpha, cpufunc::m_clreod+_index);
	R[2] = P*interplate<2>(cpufunc::m_CZ, (double*)&interdata.alpha, cpufunc::m_czreod+_index);


	//	sigmamin = (GM / (r*r) - V*V / r)*Mass / R[1];
	sigma = getSigma(V, _param[0], _param[2]);
	//sigma = _param[0];
	SINSIGMA = sin(sigma);
	COSSIGMA = cos(sigma);
	angleofdirection = chi - _param[1];
	_f[0] = V*SINGAMMA;
	_f[1] = V*COSGAMMA*COSCHI / r;
	_f[2] = V*COSGAMMA*SINCHI / (r*COSMIU);
	_f[3] = -R[0] / Mass - graveV *SINGAMMA \
		- graveT* SINMIU*(COSCHI*COSGAMMA*COSMIU + SINGAMMA*SINMIU)\
		- Constant_Earth_We*Constant_Earth_We * r*COSMIU*(COSCHI*COSGAMMA*SINMIU - SINGAMMA*COSMIU);
	_f[4] = R[1] / (Mass*V)*COSSIGMA - R[2] / (Mass*V)*SINGAMMA\
		- graveV *COSGAMMA / V\
		+ graveT * SINMIU*(COSCHI*SINGAMMA*COSMIU - COSGAMMA*SINMIU) / V\
		+ V*COSGAMMA / r;
	_f[5] = (-R[2] * COSSIGMA - R[1] * SINSIGMA + graveT * Mass*SINMIU*COSMIU*SINCHI \
		+ Mass*V*V * SINMIU / COSMIU*COSGAMMA*COSGAMMA* SINCHI / r) / (Mass*V*COSGAMMA);
	_f[6] = -V*COSGAMMA*cos(angleofdirection) / r;
	if ((interdata.h*Constant_Earth_g0 + 0.5*V*V) > (HIGHTMIN*Constant_Earth_g0 + 0.5*SPEEDEND*SPEEDEND)) return 0;
	else return 1;
}
int TrajOutCPU(spfloat _time, const spfloat* _x, spfloat* _param, uint32_t _index)
{

	if (fabs(_time - RECORDTIME) < 0.01)
	{
		spfloat r = _x[0], miu = _x[1], V = _x[3];
		spfloat h;
		spfloat B, Re;
		spfloat SINMIU, COSMIU;
		SINMIU = sin(miu);
		COSMIU = cos(miu);
		B = atan2(SINMIU / COSMIU, Constant_Earth_e2);
		Re = Constant_Earth_RA*Constant_Earth_e / sqrt((SINMIU*SINMIU + Constant_Earth_e2*COSMIU*COSMIU));
		h = sqrt(r*r - Re*Re*pow(sin(B - miu), 2)) - Re*cos(B - miu);
		_param[5] = 0.5*V*V + h*Constant_Earth_g0;
		_param[6] = _x[5] - _param[1];
	}
	return 0;
}
int cpufunc::initialData(BLOCK<2>* _CD, BLOCK<2>* _CL, BLOCK<2>* _CZ, BLOCK<1>* _rho, BLOCK<1>* _ma)
{
	m_CD  = _CD;
	m_CL  = _CL;
	m_CZ  = _CZ;
	m_rho = _rho;
	m_ma  = _ma;
	m_cdreod = new InterRecod<2>[THREAD_NUM*BLOCK_NUM];
	m_clreod = new InterRecod<2>[THREAD_NUM*BLOCK_NUM];
	m_czreod = new InterRecod<2>[THREAD_NUM*BLOCK_NUM];
	m_rhorecod = new InterRecod<1>[THREAD_NUM*BLOCK_NUM];
	m_marecod = new InterRecod<1>[THREAD_NUM*BLOCK_NUM];

	m_rkdata = (rk4_state*)malloc(BLOCK_NUM*THREAD_NUM * sizeof(rk4_state));
	m_rkengindata = new double [(CUSTATANUM * 4 + CUOUTNUM + 3 * 3)* THREAD_NUM*BLOCK_NUM];
	for (size_t index1_ = 0; index1_ < BLOCK_NUM*THREAD_NUM; index1_++)
	{
		double *datatmp_ = m_rkengindata + index1_*(CUSTATANUM * 4 + CUOUTNUM);
		m_rkdata[index1_].x = datatmp_;
		m_rkdata[index1_].k = datatmp_ + CUSTATANUM;
		m_rkdata[index1_].x0 = datatmp_ + 2 * CUSTATANUM;
		m_rkdata[index1_].xtmp = datatmp_ + 3 * CUSTATANUM;
		m_rkdata[index1_].param = datatmp_ + 4 * CUSTATANUM;

		m_rkdata[index1_].dim = CUSTATANUM;							//7维积分
		m_rkdata[index1_].paramdim = 2;						//目标点经纬度
		m_rkdata[index1_].func = TrajTory3DCPU;
		m_rkdata[index1_].funcOut = TrajOutCPU;
	}
	return 0;
}

int cpufunc::TrajCacl(Traj * _beg, spfloat * _targrt, spfloat _step, spfloat _sigma, spfloat _searchangle)
{
omp_set_num_threads(8);
#pragma omp parallel for
	for (int index1_ = 0; index1_ < BLOCK_NUM*THREAD_NUM; index1_++)
	{
		memcpy(m_rkdata[index1_].x0, _beg, sizeof(spfloat)*CUSTATANUM);
		m_rkdata[index1_].param[0] = _sigma + spfloat(index1_)*_searchangle / (BLOCK_NUM*THREAD_NUM);	//起始角度
		m_rkdata[index1_].param[1] = _beg->lanuchangle;		//内部参数 方位角
		m_rkdata[index1_].param[2] = _beg->V;		//起始速度
		m_rkdata[index1_].param[3] = _beg->begLongt;		//起始经度
		m_rkdata[index1_].param[4] = _beg->begLat;		//起始纬度
		m_rkdata[index1_].isDown = CalCulate(m_rkdata[index1_], _step, 0, index1_);

		//航程差
		_targrt[CUOUTNUM * index1_] = m_rkdata[index1_].x0[6];
		//记录纬度1
		_targrt[CUOUTNUM * index1_ + 1] = m_rkdata[index1_].x0[1];
		//记录经度2
		_targrt[CUOUTNUM * index1_ + 2] = m_rkdata[index1_].x0[2];
		//记录预测点和目标点距离3
		_targrt[CUOUTNUM * index1_ + 3] = length(m_rkdata[index1_].x0[2], m_rkdata[index1_].x0[1], _beg->targetlambda, _beg->targetmiu);
		//横向误差4	
		_targrt[CUOUTNUM * index1_ + 4] = getCrose(_beg->targetlambda, _beg->targetmiu, _beg->lambda, _beg->miu, m_rkdata[index1_].x0[2], m_rkdata[index1_].x0[1]);
		//能量5
		_targrt[CUOUTNUM * index1_ + 5] = m_rkdata[index1_].param[5];
		//十秒后的弹道偏角
		_targrt[CUOUTNUM * index1_ + 6] = m_rkdata[index1_].param[6];
	}
	return 0;
}
