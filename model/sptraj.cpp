#include "sptraj.h"
#include "spmath.h"
#include <thread>
using namespace std;
SPEXPORT_WITH_NAME(spTraj, TrajModel, spCtraj)

spCtraj::spCtraj()
{
    SetTitle("弹道模块");
    int index = findXmlNameIndes("弹道模块", "真实经度");
}

spCtraj::~spCtraj()
{
}

int spCtraj::OnInfo()
{

    return 0;
}

int spCtraj::OnInitial()
{
    m_f = GetF();
    m_x = GetX();
    m_in = GetIn();
    m_out = GetY();
    m_x[2] = GetParaData("发射点经度");
    m_x[1] = GetParaData("发射点纬度");
    m_lunchAngle = lanuchAngle(GetParaData("着陆点经度"), GetParaData("着陆点纬度"), m_x[2], m_x[1]);
    m_landhigh = GetParaData("着陆点高度");
    m_landSpeed = GetParaData("着陆点速度");

    return 0;
}

int spCtraj::OnWrite()
{
    memcpy(&r, m_x, 7 * sizeof(double)); //复制到弹道参数(注意这里利用类分配空间的连续性)
    h = altitude(miu, r);
    //if ((m_landhigh*g_0+0.5*m_landSpeed*m_landSpeed)>(m_out[7]* g_0+0.5*V*V) && GetTime()>20)	//能量管理段
    if (h < 2000 && GetTime() > 20) //自动着陆段
    {
        SendUserMessage(SM_STOP);
        setEnd(true);
    }
    memcpy(m_out, &r, sizeof(outData));
    //if (m_out[7] < 5000&&GetTime()>20)
    //	setEnd(true);
    return 0;
};

int spCtraj::OnRstart()
{
    return 0;
};

int spCtraj::OnStop()
{
    return 0;
};

int spCtraj::OnContinue()
{
    memcpy(&r, m_x, 7 * sizeof(double));  //复制到弹道参数(注意这里利用类分配空间的连续性)
    memcpy(&D, m_in, 5 * sizeof(double)); //复制输入参数
    POWRA_R = pow(Constant_Earth_RA / r, 2);
    SINMIU = sin(miu);
    COSMIU = cos(miu);
    SINGAMMA = sin(gamma);
    COSGAMMA = cos(gamma);
    SINCHI = sin(chi);
    COSCHI = cos(chi);
    graveV = Constant_Earth_GM / (r * r) * (1 - 1.5 * Constant_Earth_J2 * POWRA_R * (5 * SINMIU * SINMIU - 1));
    graveT = 3 * Constant_Earth_J2 * Constant_Earth_GM / (r * r) * POWRA_R;

    h = altitude(miu, r);
    Energy = 0.5 * V * V + Constant_Earth_g0 * h;

    SINSIGMA = sin(sigma);
    COSSIGMA = cos(sigma);

    angleofdirection = chi - m_lunchAngle;
    m_f[0] = V * SINGAMMA;
    m_f[1] = V * COSGAMMA * COSCHI / r;
    m_f[2] = V * COSGAMMA * SINCHI / (r * COSMIU);
    m_f[3] = -D / Mass - graveV * SINGAMMA - graveT * SINMIU * (COSCHI * COSGAMMA * COSMIU + SINGAMMA * SINMIU) - Constant_Earth_We * Constant_Earth_We * r * COSMIU * (COSCHI * COSGAMMA * SINMIU - SINGAMMA * COSMIU);
    Dgamma = m_f[4] = L / (Mass * V) * COSSIGMA - Z / (Mass * V) * SINGAMMA - graveV * COSGAMMA / V + graveT * SINMIU * (COSCHI * SINGAMMA * COSMIU - COSGAMMA * SINMIU) / V + V * COSGAMMA / r;
    Dchi = m_f[5] = (-Z * COSSIGMA - L * SINSIGMA + graveT * Mass * SINMIU * COSMIU * SINCHI + Mass * V * V * SINMIU / COSMIU * COSGAMMA * COSGAMMA * SINCHI / r) / (Mass * V * COSGAMMA);
    m_f[6] = V * COSGAMMA * cos(angleofdirection);

    memcpy(m_out, &r, sizeof(outData));
    return 0;
};

int spCtraj::PreCreate()
{
    return 0;
};

int spCtraj::OnFault()
{
    return 0;
};