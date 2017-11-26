#ifndef __SPMODEL_H
#define __SPMODEL_H
#pragma once

#ifdef __GNUC__
#define _encoding_ "utf8"
#else
#define _encoding_ "GBK"
#endif
#include <string>
#include<string.h>
#include "message.h"
class spmodel
{
public:
	spmodel();
	virtual ~spmodel();

public:
	int operator()(int, sysinfo *);
protected:
	virtual int onInitial() = 0;//每个仿真引擎只会调用一次
	virtual int onClear() = 0;	//卸载该模块时需要删除的数据
	virtual int onUpdate() = 0;	//每个仿真时步开始前调用
	virtual int onStop() = 0;		 //一次仿真过程结束
	virtual int onDerive() = 0;		//每次积分运算时调用
	virtual int onStart() = 0;		//一次仿真运算开始
protected:
	double *m_x;
	double *m_f;
	double *m_out;
	double *m_in;
	double *m_params;

	sysinfo *m_sys = nullptr;
	std::string m_database;
};

void eraseStringHeadAndEnd(std::string &_string);
#endif
