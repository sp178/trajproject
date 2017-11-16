#pragma once
#include "spmodel.h"
#include"..\utility\spfactory.hpp"
#include<Eigen\Core>
#include <Eigen\Dense>
#define   PRINT_MACRO_HELPER(x)   #x 
#define   PRINT_MACRO(x)   #x"="PRINT_MACRO_HELPER(x) 

class spdynamic :
	public spmodel
{

public:
	spdynamic();
	~spdynamic();
protected:
	int onInitial(); //ÿ����������ֻ�����һ��
	int onClear() ;	 //ж�ظ�ģ��ʱ��Ҫɾ��������
	int onUpdate() ; //ÿ������ʱ����ʼǰ����
	int onStop() ;		 //һ�η�����̽���
	int onDerive() ;	//ÿ�λ�������ʱ����
	int onStart() ;		//һ�η������㿪ʼ
protected:
	//������ϵͳת��
	Eigen::Matrix3d m_matrixBV;
	Eigen::Matrix3d m_matrixVO;
	Eigen::Matrix3d m_matrixVH;
	Eigen::Matrix3d m_matrixBO;
	Eigen::Matrix3d m_matrixOA;
	Eigen::Matrix3d m_matrixHT;
	Eigen::Matrix3d m_matrixTe;
	Eigen::Matrix3d m_matrixEO;
	Eigen::Matrix3d m_matrixEI;
	Eigen::Matrix3d m_matrixTA;
	Eigen::Matrix3d m_matrixA;
	Eigen::Matrix3d m_matrixB;
protected:
	/*����ϵͳ֮�����*/

protected:
	//����ϵͳ����
	double m_alpha;
	double m_beta;
	double m_theta;
	double m_sigma;
	double m_thetaT;
	double m_sigmaT;
	double m_upsilon;
	double m_gamma;//321ŷ����
	double m_psi;
	double m_varphi;
	double m_phi;			//����γ��
	double m_lambda;		//����
	double m_B;				//���γ��
	double m_h;				//�߶�
	double m_r;				//���ľ���
	double m_Re;			//���������µ���ľ���
	double m_AO;			//���䷽λ��
	double m_lambdaO;		//�������꾭��
	double m_phiO;			//��������γ��

	double m_OmegaG;		//�ƶ�ʱ�̺ʹ���ʱ�̾���ֵ֮��
	double m_te;			//�ƶ�ʱ�������ʱ��
	double m_Deltalambda;	//����������������꾭�Ȳ�
	double m_RO;			//���ĵ���������ϵ����
	double m_phie;			//���µ����γ��
	double m_gr;		//���ķ�����������
	double m_ge;		//���᷽����������
	Eigen::Vector3d m_positionE;	//��������ϵλ��
	Eigen::Vector3d m_omega;		//������
	Eigen::Vector3d m_omegae;		//������ת���ٶ��ڵ�������ϵ�������������
protected:
	//��ֵ����
	interdata<1>* m_rho;	//�����ܶ�
	interdata<1>* m_ma;		//�����
	interdata<2>* m_ca;		//����
	interdata<2>* m_cn;		//����
	interdata<2>* m_cz;		//����

	interdata<2>* m_cmx;		//��������
	interdata<2>* m_cmy;		//��������
	interdata<2>* m_cmz;		//��������

	interdata<2>* m_cmx5;		//5��������
	interdata<2>* m_cmy5;		//5��������
	interdata<2>* m_cmz5;		//5��������
protected:
	double m_V;					//�ٶ�
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
	//����������
	double m_mass;
	Eigen::Map<Eigen::Matrix3d> m_I;
protected:
	//������
	Eigen::Map<Eigen::Vector3d> m_colF;		//������
	Eigen::Map<Eigen::Vector3d> m_colM;		//��������

	Eigen::Map<Eigen::Vector3d> m_boost;			//����������
	Eigen::Map<Eigen::Vector3d> m_boostM;		//������������

	Eigen::Map<Eigen::Vector3d> m_AeroM;		//��������������
	Eigen::Map<Eigen::Vector3d> m_AeroF;		//������������
};