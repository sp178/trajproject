#pragma once
#include "spmodel.h"
#include"..\utility\spmath.h"
#include"guidancedata.h"
#include"cudafunc.h"
#include"..\utility\spinterplation.hpp"
struct guidanceIn
{
	double r, miu, lambda, V, gamma, chi, ma, h, Mass, D, L;
};

struct guidanceOut
{
	double Energey, alpha, beta, sigma, outTargetLength, outLongtitude, outLatitude,
		outsigmaDeg, outAzmaangle, outCrossLength, outCaclTime, sigmasearch, outV,
		outDeltaAzu, direcLeft, crossLeft, outLanchAngle, outH, m_lengthScale;
};
struct crossGuidance
{
	bool _usePositive;
	bool _isRunCycle;		//�������Ƿ���ת����180��
	bool _beforeSng;		//֮ǰ�ķ���
	uint32_t _minposi;		//��С������
	uint32_t _minneg;		//��С������
	uint32_t _chose;		//ѡ�������
	spfloat _preCrosslength;	//֮ǰ�������
	spfloat _nowCrossLength;	//��ǰ�������
	spfloat _prechi;	//֮ǰ����ƫ��(�ٶ�����)
	spfloat _nowchi;	//��ǰ����ƫ��(�ٶ�����)
	spfloat _crossrate;		//���򺽳̱���
	spfloat _rangChiUp;		//������
	spfloat _rangChiDown;	//������
	spfloat _beforeCrossRange;
};




class spguidance :
	public spmodel, public guidanceIn, public guidanceOut
{
public:
	spguidance();
	~spguidance();
protected:
	int onInitial(); //��ʼ����ӦSM_INITIAL
	int onClear();   //��ʼ����ӦSM_INITIAL
	int onUpdate();  //��Ӧ SM_WRITE
	int onStop();    //��ӦSM_STOP
	int onDerive();  //��ӦSM_CONTINUE
	int onStart();
protected:
	struct numtest {
		spfloat _stogoLeft,		//Ԥ��ĺ���
			_lat,			//γ��
			_longt,			//����
			_length,		//Ԥ����Ŀ������
			_croseLeft,		//�������
			_engerg,		//����ֵ(ʮ����Ԥ��ֵ)
			_azumangle;	    //����ƫ��(ʮ����Ԥ��ֵ)
	};
	double crose(numtest* _cuoutdata);			//��ȡ�������
	double getSigmaWithStogo(numtest* _cuoutdata, uint32_t& _chose);
	double getSigmaWithLength(numtest* _cuoutdata);
	bool	m_useEQS;		//�Ƿ����׼ƽ���ж�
	double m_lunchAngle;		//���䷽λ��
	double m_targetlongt;
	double m_beglongt;
	double m_beglat;
	double m_targetlat;
	double	m_Stogo;//����
	Traj	m_nowtraj;			//��������
	numtest* m_cuoutdata;		//cuda�������
	numtest* m_cuoutdataTmp;		//��ʱ���ݱ���ʮ��֮ǰ������
	double  m_sigmasearchbeg;		//sigma��ʼ������
	double	m_sigmasearchLength;	//sigma��������
	double  m_recordTime, m_nextRecordTime;        //��¼����ʱ��
	double  m_caclstep = 1;
	crossGuidance	m_crosscuidance = { true,false,0,0 };
	unsigned int m_itercycle;															//vector<string, spfloat>	m_oncedata;	
															//string		m_onceFullPath;									//ֻ���һ���ļ�����
protected:
	uint32_t findCloseastEnerg(numtest* _cuoutdata, double _energ, bool _ispositive);
	void findCloseastSigma(numtest* _cuoutdata, uint32_t& _negindex, uint32_t& _psiindex);	//Ѱ�Һ�����ӽ���������������
	spfloat choseBeginSignOfSigma(numtest* _cuoutdata, uint32_t& _chose);															//ѡ�����ǳ�ʼ����
	spfloat choseSignOfSigma(numtest* _cuoutdata, uint32_t& _chose);//ѡ�����Ƿ���
	spfloat choseSignOfSigmaWithRange(uint32_t& _chose);		//ͨ������ƫ��ѡ��
	spfloat choseScale(spfloat _V);
	uint32_t m_QEC = 0;
	bool	m_dataInitial = false;
	//double	m_lengthScale;									//���̱�������
	uint32_t m_choseNumBefore;								//֮ǰѡȡ������
	double	m_beforeEnergy;									//֮ǰ����ʵ����
	double  m_beforeStogo;
	spfloat m_beforeCross;
	bool	m_isfirstPredic;
	bool	m_isfirstPredicForEnerg;
	spfloat m_sigmaBeg;										//��ֵ��ʼ��sigma
	spfloat m_turnScaleHigh;									//��ת��������	
	spfloat m_turnScaleLow;										//��ת��������
	void rangeOfAzmaangle(spfloat _V, spfloat& _up, spfloat& _down);	//����ƫ�Ǳ߽�
	spfloat rangeOfEnergRange(spfloat _energscal);
	spfloat m_beforeAzuAngle;
	spfloat m_beforeGamma;
	spfloat m_deltaAzumAngle;
	spfloat m_deltaSigma;
	spfloat m_deltaEngerg;
	bool	m_useCrossMethod;
	bool	m_begTeam;
	spfloat	m_landHight;			//��������θ߶�
	spfloat	m_landSpeed;			//����������ٶ�
	spfloat m_teamAngle;			//��������νǶ�	//

protected:
	spfloat m_deltaAltitude;		//�߶ȱ仯��
protected:
	spfloat getSigmaSin(spfloat _E);
	spfloat m_Ebeg, m_Eend;
	int guidance();
protected:
	BLOCK<2>*	m_interCL;
	BLOCK<2>*	m_interCD;
	BLOCK<2>*	m_interCZ;
	BLOCK<1>*	m_interpho;
	BLOCK<1>*	m_interma;
	cudafunc m_cudafunc;
	FILE*		m_file = nullptr;
};

