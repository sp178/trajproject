#include "spdynamic.h"
#define Constant_PI 3.1415926535898
#define Constant_RAD 0.01745329251994
#define Constant_DEG 57.29577951308
#define Constant_Je 1.623945E-3
#define Constant_MIU 3.986005E14
#define Constant_Earth_GM 3.986005E+14        // 地心引力常数   (m^3/s^2)
#define Constant_Earth_J2 1.08263E-3          // 动力形状因子
#define Constant_Earth_RA 6378140.0             // 地球长半轴       (m)
#define Constant_Earth_RP 6356755.28856       // 地球短半轴       (m)
#define Constant_Earth_Rm 6371003.7814        // 地球平均半径   (m)
#define Constant_Earth_Rae 6378140.0            // 地球赤道半径   (m)
#define Constant_Earth_Alpha 0.00335281311553 // 地球扁率
#define Constant_Earth_g0 9.80665             // 标准重力加速度 (m/s^2)
#define Constant_Earth_We 7.292115E-5         // 地球自旋角速度 (rad/s)
//#define Constant_Earth_e2             6.69438487525E-3        //
//第一偏心率的平方
#define Constant_Earth_Ae 0.00335281311553 // 地球扁率
#define Constant_Earth_ae (1. / 298.257)
#define Constant_Earth_e 0.996647186822103
#define Constant_Earth_e2 0.993305615000412
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


spdynamic::spdynamic():m_matrixBV(nullptr),m_matrixVO(nullptr),m_matrixVH(nullptr),m_matrixBO(nullptr),m_matrixOA(nullptr),
m_matrixHT(nullptr),m_matrixTe(nullptr),m_matrixEO(nullptr),m_matrixEI(nullptr),m_matrixTA(nullptr),m_dposition(nullptr),
m_dspeed(nullptr),m_domegaT(nullptr),m_dq(nullptr),m_position(nullptr),m_speed(nullptr),m_omegaT(nullptr),m_q(nullptr),
m_I(nullptr),m_colF(nullptr),m_colM(nullptr),m_boost(nullptr), m_boostM(nullptr),m_AeroF(nullptr),m_AeroM(nullptr)
{																							

}

spdynamic::~spdynamic() {}

int spdynamic::onInitial() { 

	

	return 0; 
}

int spdynamic::onClear() { return 0; }

int spdynamic::onUpdate() { return 0; }

int spdynamic::onStop() { return 0; }

int spdynamic::onDerive() {

  m_mass = m_in[0];
  m_V = m_speed.norm();
  m_position(2) += m_RO;
  m_r = m_position.norm();
  m_thetaT = m_position.dot(m_speed)/(m_r*m_V);
  m_theta = atan2(m_speed(1) , m_speed(0));
  m_sigma = asin(-m_speed(2)/m_V);
  m_positionE.noalias() =  m_matrixEO*m_position;
  m_lambda = m_lambdaO + atan2(m_positionE(1), m_positionE(0));
  m_phi = asin(m_position.dot(m_omegae))/(m_r*Constant_Earth_We);

  double temp_ = 0;
  temp_ = Constant_Je*(Constant_Earth_RA *Constant_Earth_RA/(m_r*m_r));
  m_gr = -1*Constant_Earth_GM*(1 + temp_*(1 - 5 * pow(sin(m_phi), 2)))/(m_r*m_r);
  m_ge = -2 * Constant_Earth_GM*temp_*sin(m_phi)/(m_r*m_r);
  //迭代求解飞行器星下点地心纬度 大地纬度 高度
  double tmpphie_ = m_phi,deltaphie_;
  uint32_t itertimes = 0;
  while (itertimes>6)
  {
	  m_B = atan2(tan(tmpphie_), Constant_Earth_e2);
	  m_Re = Constant_Earth_RA*Constant_Earth_e / sqrt(pow(sin(tmpphie_),2)+pow(Constant_Earth_e*cos(tmpphie_),2));
	  m_h = sqrt(m_r*m_r - pow(m_Re*sin(m_B - tmpphie_), 2)) - m_Re*cos(m_B - tmpphie_);
	  deltaphie_ = asin(m_h*sin(m_B - tmpphie_) / m_r);
	  m_phie = m_phi - deltaphie_;
	  if (fabs(m_phie - tmpphie_) < 1E8)
		  break;
	  tmpphie_ = m_phie;
	  itertimes++;
  }
  //更新积分信息
  m_dspeed.noalias() = 1 / m_mass*m_matrixBO.transpose()*(m_boost + m_AeroF + m_colF) + m_gr / m_r*m_position + m_ge / Constant_Earth_We*m_omegae - m_matrixA*m_position - m_matrixB*m_speed;
  m_dposition = m_speed;
  m_position(2) -= m_RO;

  //姿态相关(六自由度的弹道问题)
  {
	  double cosvarphi_, sinvarphi_, cospsi_, sinpsi_, cosgamma_, singamma_,
		  cossigma_, sinsigma_, costheta_, sintheta_, cosupsilon_, sinupsilon_;
	  double q2_[4];
	  m_q.normalize();//
	  q2_[0] = m_q(0)*m_q(0); q2_[1] = m_q(1)*m_q(1); q2_[2] = m_q(2)*m_q(2); q2_[3] = m_q(3)*m_q(3);
	  m_matrixBO << q2_[0] + q2_[1] - q2_[2] - q2_[3], 2 * (m_q(1)*m_q(2) + m_q(0)*m_q(3)), 2 * (m_q(1)*m_q(3) - m_q(0)*m_q(2)),
		  2 * (m_q(1)*m_q(2) - m_q(0)*m_q(3)), q2_[0] - q2_[1] + q2_[2] - q2_[3], 2 * (m_q(2)*m_q(3) + m_q(0)*m_q(1)),
		  2 * (m_q(1)*m_q(3) + m_q(0)*m_q(2)), 2 * (m_q(2)*m_q(3) - m_q(0)*m_q(1)), q2_[0] - q2_[1] - q2_[2] + q2_[3];

	  m_psi = asin(-2 * (m_q(1) * m_q(3) - m_q(0) * m_q(2)));
	  m_varphi = atan2(2 * (m_q(1) * m_q(2) + m_q(0) * m_q(3)),
		  m_q(0) * m_q(0) + m_q(1) * m_q(1) - m_q(2) * m_q(2) -
		  m_q(3) * m_q(3));
	  m_gamma = atan2(2 * (m_q(2) * m_q(3) + m_q(0) * m_q(1)),
		  m_q(0) * m_q(0) - m_q(1) * m_q(1) - m_q(2) * m_q(2) +
		  m_q(3) * m_q(3));
	  cosvarphi_ = cos(m_varphi);  cospsi_ = cos(m_psi);  cosgamma_ = cos(m_gamma);
	  cossigma_ = cos(m_sigma); costheta_ = cos(m_theta); cosupsilon_ = cos(m_upsilon);
	  sinvarphi_ = sin(m_varphi);  sinpsi_ = sin(m_psi);  singamma_ = sin(m_gamma);
	  sinsigma_ = sin(m_sigma);  sintheta_ = sin(m_theta);  sinupsilon_ = sin(m_upsilon);
	  m_omega = m_omegaT - m_matrixBO*m_omegae;
	  matrix4d tempmatrix4d_;
	  tempmatrix4d_ << 0, -m_omega(0), -m_omega(1), -m_omega(2),
		  m_omega(0), 0, m_omega(1), -m_omega(2),
		  m_omega(1), -m_omega(2), 0, m_omega(0),
		  m_omega(2), m_omega(1), -m_omega(0), 0;

	  m_beta = sinpsi_*cosgamma_*cossigma_*cos(m_varphi - m_theta)
		  + singamma_*cossigma_*sin(m_varphi - m_theta)
		  - cospsi_*cosgamma_*sinsigma_;
	  m_beta = asin(m_beta);
	  m_alpha = -(sinpsi_*singamma_*cossigma_*cos(m_varphi - m_theta)
		  - cosgamma_*cossigma_*sin(m_varphi - m_theta)
		  - cospsi_*singamma_*sinsigma_) / cos(m_beta);
	  m_alpha = asin(m_alpha);
	  m_upsilon = (singamma_*cos(m_varphi - m_theta)
		  - sinpsi_*cosgamma_*sin(m_varphi - m_theta)) / cos(m_beta);
	  m_upsilon = asin(m_upsilon);

	  //更新积分信息
	  m_domegaT = m_I.inverse()*(-m_omegaT.cross3(m_I*m_omegaT) + m_AeroM + m_boostM + m_colM);
	  m_dq = 0.5*tempmatrix4d_*m_q;
  } 
  return 0;
}

int spdynamic::onStart() {
	//初始化数据
	new (&m_dposition) vector3dM(m_f, 3);
	new (&m_dspeed) vector3dM(m_f + 3, 3);

	new (&m_position) vector3dM(m_x, 3);
	new (&m_speed) vector3dM(m_x + 3, 3);

	m_mass = m_in[0];
	new (&m_AeroF) vector3dM(m_in + 10, 3);
	new(&m_boost) vector3dM(m_in + 16, 3);
	
	//首先确定发射坐标系位置信息
		//
	m_phiO = m_params[0];
	m_lambdaO = m_params[1];
	m_AO = m_params[2];
	//初始化位置 和速度
	m_position << m_params[3], m_params[4], m_params[5];
	m_speed << m_params[6], m_params[7], m_params[8];

	m_RO = Constant_Earth_RA*Constant_Earth_RP /
		(sqrt(pow((Constant_Earth_RA*sin(m_phiO)),2)+ pow((Constant_Earth_RP*sin(m_phiO)), 2)));
	m_matrixEO << -sin(m_phiO)*cos(m_AO), cos(m_phiO), sin(m_phiO)*sin(m_AO),
		sin(m_AO), 0, cos(m_AO),
		cos(m_AO)*cos(m_phiO), sin(m_phiO), -cos(m_phiO)*sin(m_AO);
	m_omegae << 0, 0, Constant_Earth_We;
	m_omegae.noalias() = m_matrixEO.transpose()*m_omegae;
	m_matrixA << m_omegae(0)*m_omegae(0) - Constant_Earth_We*Constant_Earth_We, m_omegae(0)*m_omegae(1), m_omegae(0)*m_omegae(2),
		m_omegae(1)*m_omegae(2), m_omegae(1)*m_omegae(1) - Constant_Earth_We*Constant_Earth_We, m_omegae(1)*m_omegae(2),
		m_omegae(0)*m_omegae(2), m_omegae(2)*m_omegae(2) - Constant_Earth_We*Constant_Earth_We,m_omegae(1)*m_omegae(2);
	m_matrixB << 0, -2 * m_omegae(2), 2*m_omegae(1),
		2 * m_omegae(2), 0, -2 * m_omegae(0),
		-2 * m_omegae(1), 2 * m_omegae(0), 0;
	//飞行器初始状态
	m_V = m_speed.norm();
	m_position(2) += m_RO;
	m_r = m_position.norm();
	m_thetaT = m_position.dot(m_speed) / (m_r*m_V);
	m_theta = atan2(m_speed(1), m_speed(0));
	m_sigma = asin(-m_speed(2) / m_V);
	m_positionE.noalias() = m_matrixEO*m_position;
	m_lambda = m_lambdaO + atan2(m_positionE(1), m_positionE(0));
	m_phi = asin(m_position.dot(m_omegae)) / (m_r*Constant_Earth_We);

	//高度初始飞行器星下点高度
	double tmpphie_ = m_phi, deltaphie_;
	uint32_t itertimes = 0;
	while (itertimes>6)
	{
		m_B = atan2(tan(tmpphie_), Constant_Earth_e2);
		m_Re = Constant_Earth_RA*Constant_Earth_e / sqrt(pow(sin(tmpphie_), 2) + pow(Constant_Earth_e*cos(tmpphie_), 2));
		m_h = sqrt(m_r*m_r - pow(m_Re*sin(m_B - tmpphie_), 2)) - m_Re*cos(m_B - tmpphie_);
		deltaphie_ = asin(m_h*sin(m_B - tmpphie_) / m_r);
		m_phie = m_phi - deltaphie_;
		if (fabs(m_phie - tmpphie_) < 1E8)
			break;
		tmpphie_ = m_phie;
		itertimes++;
	}
	//六自由度姿态问题计算
	{
		new (&m_domegaT) vector3dM(m_f + 6, 3);
		new (&m_dq) vector4dM(m_f + 9, 4);
		new (&m_omegaT) vector3dM(m_x + 6, 3);
		new (&m_q) vector4dM(m_x + 9, 4);
		new (&m_I) matrix3dM(m_in + 1, 3, 3);
		new (&m_AeroM) vector3dM(m_in + 13, 3);
		new(&m_boostM) vector3dM(m_in + 19, 3);

		m_gamma = m_params[9];  //滚转角
		m_psi = m_params[10];    //偏航角
		m_varphi = m_params[11]; //俯仰角
								 //需要初始化四元数q
		double cosvarphi2_ = cos(m_varphi / 2);
		double cosgamma2_ = cos(m_gamma / 2);
		double cospsi2_ = cos(m_psi / 2);
		double sinvarphi2_ = sin(m_varphi / 2);
		double singamma2_ = sin(m_gamma / 2);
		double sinpsi2_ = sin(m_psi / 2);

		m_q << cosvarphi2_*cospsi2_*cosgamma2_ + sinvarphi2_*sinpsi2_*singamma2_,
			cosvarphi2_*cospsi2_*singamma2_ - sinvarphi2_*sinpsi2_*cosgamma2_,
			cosvarphi2_*sinpsi2_*cosgamma2_ + sinvarphi2_*cospsi2_*singamma2_,
			-cosvarphi2_*sinpsi2_*singamma2_ + sinvarphi2_*cospsi2_*cosgamma2_;
		//求解R0

	}
	return 0;
}

