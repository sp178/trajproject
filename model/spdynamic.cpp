#include "spdynamic.h"
#define Constant_PI 3.1415926535898
#define Constant_RAD 0.01745329251994
#define Constant_DEG 57.29577951308
#define Constant_Je 1.623945E-3
#define Constant_MIU 3.986005E14
#define Constant_Earth_GM 3.986005E+14        // 地心引力常数   (m^3/s^2)
#define Constant_Earth_J2 1.08263E-3          // 动力形状因子
#define Constant_Earth_RA 6378140             // 地球长半轴       (m)
#define Constant_Earth_RP 6356755.28856       // 地球短半轴       (m)
#define Constant_Earth_Rm 6371003.7814        // 地球平均半径   (m)
#define Constant_Earth_Rae 6378140            // 地球赤道半径   (m)
#define Constant_Earth_Alpha 0.00335281311553 // 地球扁率
#define Constant_Earth_g0 9.80665             // 标准重力加速度 (m/s^2)
#define Constant_Earth_We 7.292115E-5         // 地球自旋角速度 (rad/s)
//#define Constant_Earth_e2             6.69438487525E-3        //
//第一偏心率的平方
#define Constant_Earth_Ae 0.00335281311553 // 地球扁率
#define Constant_Earth_ae (1. / 298.257)
#define Constant_Earth_e 0.9977635403573926373497851397775
#define Constant_Earth_e2 0.99553208246651828615184517583943
#define rad2deg(_x) (57.2958 * (_x))
#define deg2rad(_x) (0.0174533 * (_x))

using namespace Eigen;
typedef Map<Vector3d> vector3dM;
typedef Map<Vector4d> vector4dM;
typedef Map<Matrix3d> matrix3dM;
typedef Map<Matrix4d> matrix4dM;

typedef Vector3d vector3d;
typedef Vector4d vector4d;
typedef Matrix3d matrix3d;
typedef Matrix4d matrix4d;


spdynamic::spdynamic():m_BV(nullptr),m_VO(nullptr),m_VH(nullptr),m_BO(nullptr),
m_OA(nullptr),m_HT(nullptr),m_Te(nullptr),m_EO(nullptr),m_EI(nullptr),m_TA(nullptr),
m_dposition(nullptr),m_dspeed(nullptr),m_domegaT(nullptr),m_dq(nullptr),m_position(nullptr),
m_speed(nullptr),m_omegaT(nullptr),m_q(nullptr),m_I(nullptr),m_colF(nullptr),m_colM(nullptr)
{

}

spdynamic::~spdynamic() {}

int spdynamic::onInitial() { 
	new (&m_position) vector3dM(m_x, 3);
	new (&m_speed) vector3dM(m_x+3, 3);
	new (&m_omegaT) vector3dM(m_x+6, 3);
	new (&m_q) vector3dM(m_x+9, 3);
	

	return 0; 
}

int spdynamic::onClear() { return 0; }

int spdynamic::onUpdate() { return 0; }

int spdynamic::onStop() { return 0; }

int spdynamic::onDerive() {
  double cosvarphi_, sinvarphi_, cospsi_, sinpsi_, cosgamma_, singamma_,
      cossigma_, sinsigma_, costheta_, sintheta_, cosupsilon_, sinupsilon_,
      cosalpha_, sinalpha_, cosbeta_, sinbeta_;
  double cosphiO_, sinphiO_, cosAO_, sinAO_;

  m_psi = asin(-2 * (m_q(0) * m_q(3) - m_q(0) * m_q(2)));
  m_varphi = atan2(2 * (m_q(1) * m_q(2) + m_q(0) * m_q(3)),
                   m_q(0) * m_q(0) + m_q(1) * m_q(1) - m_q(2) * m_q(2) -
                       m_q(3) * m_q(3));
  m_gamma = atan2(2 * (m_q(2) * m_q(3) + m_q(0) * m_q(1)),
                  m_q(0) * m_q(0) - m_q(1) * m_q(1) - m_q(2) * m_q(2) +
                      m_q(3) * m_q(3));
  m_V = m_speed.norm();
  m_position(2) += m_RO;
  m_r = m_position.norm();
  m_thetaT = m_position.dot(m_speed)/(m_r*m_V);
  m_theta = atan2(m_speed(1) , m_speed(0));
  m_sigma = asin(-m_speed(2)/m_V);
  m_positionE.noalias() =  m_EO*m_position;

  m_lambda = m_lambdaO + atan2(m_positionE(1), m_positionE(0));
  m_phi = asin(m_position.dot(m_omegae))/(m_r*Constant_Earth_We);

  m_position(2) -= m_RO;

  cosvarphi_ = cos(m_varphi);  cospsi_ = cos(m_psi);  cosgamma_ = cos(m_gamma);
  cossigma_ = cos(m_sigma); costheta_ = cos(m_theta); cosupsilon_ = cos(m_upsilon);
  cosalpha_ = cos(m_alpha); cosbeta_ = cos(m_beta);  cosalpha_ = cos(m_alpha);
  cosbeta_ = cos(m_beta);  cosphiO_ = cos(m_phiO);  cosAO_ = cos(m_AO);
  sinvarphi_ = sin(m_varphi);  sinpsi_ = sin(m_psi);  singamma_ = sin(m_gamma);
  sinsigma_ = sin(m_sigma);  sintheta_ = sin(m_theta);  sinupsilon_ = sin(m_upsilon);
  sinalpha_ = sin(m_alpha);  sinbeta_ = sin(m_beta);  sinalpha_ = sin(m_alpha);
  sinbeta_ = sin(m_beta);  sinphiO_ = sin(m_phiO);  sinAO_ = sin(m_AO);
  
  
  return 0;
}

int spdynamic::onStart() {
  //初始化数据
  m_theta = 0;  //弹道倾角
  m_sigma = 0;  //弹道偏角
  m_varphi = 0; //俯仰角
  m_gamma = 0;  //滚转角
  m_psi = 0;    //偏航角
  m_phi = 0;    //地心纬度
  m_lambda = 0; //经度
  m_h = 0;      //高度
  m_speed << 0, 0, 0;
  m_omegaT << 0, 0, 0;
  return 0;
}

