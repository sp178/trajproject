#include "spmodel.h"
struct Traj
{
    double r, miu, lambda, V, gamma, chi, rengeS;
};
struct indata
{
    double D, L, Z, sigma, Mass;
};
struct outData : public Traj
{
    double h, Energy, Dgamma, Dchi;
};
class spCtraj : public spmodel, public outData, public indata /*,public outData*/
{
  public:
    spCtraj();
    ~spCtraj();

  protected:
    int OnInfo();     //对应的SM_INFO
    int OnInitial();  //初始化对应SM_INITIAL
    int OnWrite();    //对应 SM_WRITE
    int OnRstart();   //对应SM_RESTART
    int OnStop();     //对应SM_STOP
    int OnContinue(); //对应SM_CONTINUE
    int OnFault();    //对应SM_FAULT
    int PreCreate();  //函数初始化前应做事情！！！（初始化是指:平台调用该模块前。比如设定该模块输入输出名称,预先设置数据等。）

  protected:
    double *m_f;
    double *m_x;
    double *m_in;
    double *m_out;

  protected:
    bool m_InitialData = false;

  protected:
    double B, Re;
    double graveV, graveT;
    double P, Ma;
    double angleofdirection; //初始发射方位角和航迹偏角夹角
    double SINMIU, COSMIU, SINGAMMA, COSGAMMA, SINCHI, COSCHI, SINSIGMA, COSSIGMA;
    double POWRA_R;
    double sigmamin;
    double m_lunchAngle; //发射方位角
    double m_landhigh;   //着陆点高度
    double m_landSpeed;  //着陆点速度
    double Dgamma, Dchi;
};