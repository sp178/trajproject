#pragma once
#include "D:\home\sptraj\trajproject\model\spmodel.h"
class spmass :
	public spmodel
{
public:
	spmass();
	~spmass();
protected:
	int onInitial(); //��ʼ����ӦSM_INITIAL
	int onClear();   //��ʼ����ӦSM_INITIAL
	int onUpdate();  //��Ӧ SM_WRITE
	int onStop();    //��ӦSM_STOP
	int onDerive();  //��ӦSM_CONTINUE
	int onStart();
protected:
	double	m_mass;
};

