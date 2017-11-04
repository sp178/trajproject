
#ifndef __SPMODEL_H
#define __SPMODEL_H
#pragma once
#include "message.h"

#include <string>
using namespace std;
class spmodel;
#define DLL_EXPORT
#ifdef DLL_EXPORT
#define SPDLLEXPORT extern __declspec(dllexport)
#else
#define SPDLLEXPORT extern __declspec(dllimport)
#endif
#ifdef __cplusplus
extern "C" {
#endif
#if defined(linux)
	#define SPEXPORT_WITH_NAME(_name, _classname)               
#elif  defined(_WIN32)

#define SPEXPORT_WITH_NAME(_name, _classname)               \
\
extern int __declspec(dllexport)                            \
		_name(int _msg, void *_model, sysinfo *_sys)          \
\
{                                                      \
	switch (_msg)                                           \
	{                                                       \
	case SP_MSG_LOAD:                                       \
	{                                                       \
		*((void **)_model) = new _classname();                \
		return 0;                                             \
	}                                                       \
	default:                                                \
	{                                                       \
		if (_model)                                           \
			return ((spmodel *)_model)->operator()(_msg, _sys); \
	}                                                       \
	}                                                       \
	return 0;                                               \
\
};\

#endif

#ifdef __cplusplus
}
#endif

class spmodel
{
public:
	spmodel();
	virtual ~spmodel();

public:
	int operator()(int, sysinfo *);
	void setData(const string &_database);

protected:
	virtual int onInitial() = 0; //初始化对应SM_INITIAL
	virtual int onClear() = 0;	 //初始化对应SM_INITIAL
	virtual int onUpdate() = 0;	//对应 SM_WRITE
	virtual int onStop() = 0;		 //对应SM_STOP
	virtual int onDerive() = 0;	//对应SM_CONTINUE
protected:
	double *m_x;
	double *m_f;
	double *m_out;
	double *m_in;
	double *m_params;

	sysinfo *m_sys = nullptr;
	string m_database;
};
#endif
