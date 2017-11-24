#include "sptraj.h"
SPEXPORT_WITH_NAME(traj, sptraj);

sptraj::sptraj()
{
};


sptraj::~sptraj()
{
};

int sptraj::onInitial()
{


	return 0;
};

int sptraj::onClear()
{
	return 0;
};

int sptraj::onUpdate()
{
	memcpy(&r, m_x, 7 * sizeof(double));		//复制到弹道参数(注意这里利用类分配空间的连续性)
	h = altitude(miu, r);
	if ((m_landhigh*Constant_Earth_g0+0.5*m_landSpeed*m_landSpeed)>(m_out[7]* Constant_Earth_g0 +0.5*V*V) && m_sys->_time>20)	//能量管理段
	{
		return -100;
	}
	memcpy(m_out, &r, sizeof(outData));
	return 0;
};

int sptraj::onStop()
{
	return 0;
};

int sptraj::onDerive()
{
	memcpy(&r, m_x, 7 * sizeof(double));		//复制到弹道参数(注意这里利用类分配空间的连续性)
	memcpy(&D, m_in, 5 * sizeof(double));		//复制输入参数		
	POWRA_R = pow(Constant_Earth_RA / r, 2);
	SINMIU = sin(miu);
	COSMIU = cos(miu);
	SINGAMMA = sin(gamma);
	COSGAMMA = cos(gamma);
	SINCHI = sin(chi);
	COSCHI = cos(chi);
	graveV = Constant_Earth_GM / (r * r) * (1 - 1.5*Constant_Earth_J2*POWRA_R*(5 * SINMIU*SINMIU - 1));
	graveT = 3 * Constant_Earth_J2*Constant_Earth_GM / (r*r)*POWRA_R;

	h = altitude(miu, r);
	Energy = 0.5*V*V + Constant_Earth_g0*h;



	SINSIGMA = sin(sigma);
	COSSIGMA = cos(sigma);

	angleofdirection = chi - m_lunchAngle;
	m_f[0] = V*SINGAMMA;
	m_f[1] = V*COSGAMMA*COSCHI / r;
	m_f[2] = V*COSGAMMA*SINCHI / (r*COSMIU);
	m_f[3] = -D / Mass - graveV *SINGAMMA \
		- graveT* SINMIU*(COSCHI*COSGAMMA*COSMIU + SINGAMMA*SINMIU)\
		- Constant_Earth_We*Constant_Earth_We * r*COSMIU*(COSCHI*COSGAMMA*SINMIU - SINGAMMA*COSMIU);
	Dgamma = m_f[4] = L / (Mass*V)*COSSIGMA - Z / (Mass*V)*SINGAMMA\
		- graveV *COSGAMMA / V\
		+ graveT * SINMIU*(COSCHI*SINGAMMA*COSMIU - COSGAMMA*SINMIU) / V\
		+ V*COSGAMMA / r;
	Dchi = m_f[5] = (-Z * COSSIGMA - L * SINSIGMA + graveT * Mass*SINMIU*COSMIU*SINCHI \
		+ Mass*V*V * SINMIU / COSMIU*COSGAMMA*COSGAMMA* SINCHI / r) / (Mass*V*COSGAMMA);
	m_f[6] = V*COSGAMMA*cos(angleofdirection);


	memcpy(m_out, &r, sizeof(outData));
	return 0;
}
int sptraj::onStart()
{
	m_x[2] = m_params[4];
	m_x[1] = m_params[5];
	m_lunchAngle = lanuchAngle(m_params[1], m_params[0], m_x[2], m_x[1]);
	m_landhigh = m_params[2];
	m_landSpeed = m_params[3];
	return 0;
}
;

