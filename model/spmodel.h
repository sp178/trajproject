
#ifndef __SPMODEL_H
#define __SPMODEL_H
#pragma once


#include <string>
#include "message.h"
class spmodel
{
public:
	spmodel();
	virtual ~spmodel();

public:
	int operator()(int, sysinfo *);
protected:
	virtual int onInitial() = 0; //初始化对应SM_INITIAL
	virtual int onClear() = 0;	 //初始化对应SM_INITIAL
	virtual int onUpdate() = 0;	//对应 SM_WRITE
	virtual int onStop() = 0;		 //对应SM_STOP
	virtual int onDerive() = 0;	//对应SM_CONTINUE
	virtual int onStart() = 0;	
protected:
	double *m_x;
	double *m_f;
	double *m_out;
	double *m_in;
	double *m_params;

	sysinfo *m_sys = nullptr;
	std::string m_database;
};
#endif
