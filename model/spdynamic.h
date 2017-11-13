#pragma once
#include "spmodel.h"
#include"..\utility\spfactory.hpp"
#include<Eigen\Core>
#define   PRINT_MACRO_HELPER(x)   #x 
#define   PRINT_MACRO(x)   #x"="PRINT_MACRO_HELPER(x) 

class spdynamic :
	public spmodel
{

public:
	spdynamic();
	~spdynamic();
protected:
	int onInitial(); //每个仿真引擎只会调用一次
	int onClear() ;	 //卸载该模块时需要删除的数据
	int onUpdate() ; //每个仿真时步开始前调用
	int onStop() ;		 //一次仿真过程结束
	int onDerive() ;	//每次积分运算时调用
	int onStart() ;		//一次仿真运算开始
protected:
	//各坐标系统转换
	Eigen::Matrix3d m_BV;
	Eigen::Matrix3d m_VO;
	Eigen::Matrix3d m_VH;
	Eigen::Matrix3d m_BO;
	Eigen::Matrix3d m_OA;
	Eigen::Matrix3d m_HT;
	Eigen::Matrix3d m_Te;
	Eigen::Matrix3d m_EO;
	Eigen::Matrix3d m_EI;
	Eigen::Matrix3d m_TA;
protected:
	/*坐标系统之间距离*/

protected:
	//动力系统参数
	double m_alpha;
	double m_beta;
	double m_theta;
	double m_sigma;
	double m_thetaT;
	double m_sigmaT;
	double m_upsilon;
	double m_gamma;//321欧拉角
	double m_psi;
	double m_varphi;
	double m_phi;			//地心纬度
	double m_lambda;		//经度
	double m_B;				//大地纬度
	double m_h;				//高度
	double m_r;				//地心距离
	double m_AO;			//发射方位角
	double m_OmegaG;		//制动时刻和春分时刻经度值之差
	double m_lambdaO;		//制动时刻飞行器经度
	double m_phiO;			//返回坐标系制动时刻的地心纬度
	double m_te;			//制动时刻算起的时间
	double m_Deltalambda;	//地理坐标和再入坐标经度差
	double m_RO;			//地心到返回坐标系距离
	double m_phie;			//星下点地心纬度

	vector3d m_positionE;	//地心坐标系位置
	vector3d m_omegae;		//地球自转角速度在地心坐标系上三个方向分量
	double   m_omega;		//地球自转角速度
protected:
	//插值数据
	interdata<1>* m_rho;	//大气密度
	interdata<1>* m_ma;		//马赫数
	interdata<2>* m_ca;		//轴向
	interdata<2>* m_cn;		//法向
	interdata<2>* m_cz;		//横向

	interdata<2>* m_cmx;		//轴向力矩
	interdata<2>* m_cmy;		//法向力矩
	interdata<2>* m_cmz;		//横向力矩

	interdata<2>* m_cmx5;		//5轴向力矩
	interdata<2>* m_cmy5;		//5法向力矩
	interdata<2>* m_cmz5;		//5横向力矩
protected:
	double m_V;					//速度
	Eigen::Map<Eigen::Vector3d> m_dposition;
	Eigen::Map<Eigen::Vector3d> m_dspeed;
	Eigen::Map<Eigen::Vector3d> m_domegaT;
	Eigen::Map<Eigen::Vector4d> m_dq;
	double m_dlambda;
	double m_dphi;
	Eigen::Map<Eigen::Vector3d> m_position;
	Eigen::Map<Eigen::Vector3d> m_speed;
	Eigen::Map<Eigen::Vector3d> m_omegaT;
	Eigen::Map<Eigen::Vector4d> m_q;
protected:
	//飞行器特性
	double m_mass;
	Eigen::Map<Eigen::Matrix3d> m_I;
protected:
	//控制力
	Eigen::Map<Eigen::Vector3d> m_colF;		//控制力
	Eigen::Map<Eigen::Vector3d> m_colM;		//控制力矩

};

