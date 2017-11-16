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
	bool _isRunCycle;		//飞行器是否旋转超过180°
	bool _beforeSng;		//之前的符号
	uint32_t _minposi;		//最小正倾侧角
	uint32_t _minneg;		//最小负倾侧角
	uint32_t _chose;		//选测的倾侧角
	spfloat _preCrosslength;	//之前横向误差
	spfloat _nowCrossLength;	//当前横向误差
	spfloat _prechi;	//之前航向偏角(速度坐标)
	spfloat _nowchi;	//当前航向偏角(速度坐标)
	spfloat _crossrate;		//横向航程比率
	spfloat _rangChiUp;		//航程上
	spfloat _rangChiDown;	//航程下
	spfloat _beforeCrossRange;
};




class spguidance :
	public spmodel, public guidanceIn, public guidanceOut
{
public:
	spguidance();
	~spguidance();
protected:
	int onInitial(); //初始化对应SM_INITIAL
	int onClear();   //初始化对应SM_INITIAL
	int onUpdate();  //对应 SM_WRITE
	int onStop();    //对应SM_STOP
	int onDerive();  //对应SM_CONTINUE
	int onStart();
protected:
	struct numtest {
		spfloat _stogoLeft,		//预测的航程
			_lat,			//纬度
			_longt,			//经度
			_length,		//预测点和目标点距离
			_croseLeft,		//横向误差
			_engerg,		//能量值(十秒后的预测值)
			_azumangle;	    //弹道偏角(十秒后的预测值)
	};
	double crose(numtest* _cuoutdata);			//获取侧向误差
	double getSigmaWithStogo(numtest* _cuoutdata, uint32_t& _chose);
	double getSigmaWithLength(numtest* _cuoutdata);
	bool	m_useEQS;		//是否进行准平衡判断
	double m_lunchAngle;		//发射方位角
	double m_targetlongt;
	double m_beglongt;
	double m_beglat;
	double m_targetlat;
	double	m_Stogo;//航程
	Traj	m_nowtraj;			//弹道数据
	numtest* m_cuoutdata;		//cuda输出数据
	numtest* m_cuoutdataTmp;		//临时数据保存十秒之前的数据
	double  m_sigmasearchbeg;		//sigma初始搜索点
	double	m_sigmasearchLength;	//sigma搜索长度
	double  m_recordTime, m_nextRecordTime;        //记录数据时间
	double  m_caclstep = 1;
	crossGuidance	m_crosscuidance = { true,false,0,0 };
	unsigned int m_itercycle;															//vector<string, spfloat>	m_oncedata;	
															//string		m_onceFullPath;									//只输出一次文件名称
protected:
	uint32_t findCloseastEnerg(numtest* _cuoutdata, double _energ, bool _ispositive);
	void findCloseastSigma(numtest* _cuoutdata, uint32_t& _negindex, uint32_t& _psiindex);	//寻找航程最接近的正负倾侧角索引
	spfloat choseBeginSignOfSigma(numtest* _cuoutdata, uint32_t& _chose);															//选择倾侧角初始符号
	spfloat choseSignOfSigma(numtest* _cuoutdata, uint32_t& _chose);//选择倾侧角符号
	spfloat choseSignOfSigmaWithRange(uint32_t& _chose);		//通过航程偏差选择
	spfloat choseScale(spfloat _V);
	uint32_t m_QEC = 0;
	bool	m_dataInitial = false;
	//double	m_lengthScale;									//航程比例因子
	uint32_t m_choseNumBefore;								//之前选取的倾侧角
	double	m_beforeEnergy;									//之前的真实能量
	double  m_beforeStogo;
	spfloat m_beforeCross;
	bool	m_isfirstPredic;
	bool	m_isfirstPredicForEnerg;
	spfloat m_sigmaBeg;										//插值初始点sigma
	spfloat m_turnScaleHigh;									//反转比例高速	
	spfloat m_turnScaleLow;										//反转比例低速
	void rangeOfAzmaangle(spfloat _V, spfloat& _up, spfloat& _down);	//航向偏角边界
	spfloat rangeOfEnergRange(spfloat _energscal);
	spfloat m_beforeAzuAngle;
	spfloat m_beforeGamma;
	spfloat m_deltaAzumAngle;
	spfloat m_deltaSigma;
	spfloat m_deltaEngerg;
	bool	m_useCrossMethod;
	bool	m_begTeam;
	spfloat	m_landHight;			//能量管理段高度
	spfloat	m_landSpeed;			//能量管理段速度
	spfloat m_teamAngle;			//能量管理段角度	//

protected:
	spfloat m_deltaAltitude;		//高度变化率
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

