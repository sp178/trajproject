/*
积分接口
*/

/*
gsl_odeiv2_step_rk2;
gsl_odeiv2_step_rk4;
gsl_odeiv2_step_rkf45;
gsl_odeiv2_step_rkck;
gsl_odeiv2_step_rk8pd;
gsl_odeiv2_step_rk2imp;
gsl_odeiv2_step_rk4imp;
gsl_odeiv2_step_bsimp;
gsl_odeiv2_step_rk1imp;
gsl_odeiv2_step_msadams;
gsl_odeiv2_step_msbdf;
gsl_odeiv2_step_rk8pd;
*/
#ifndef __ODEENGIN_H
#define __ODEENGIN_H
#include "spdatabase.h"
#ifdef __cplusplus
extern "C"{
#endif
SPDLLEXPORT  int linker_add(model *_themodel);
SPDLLEXPORT  int func(double t, const double *_y, double *_f, void *_param);
SPDLLEXPORT  int jac(double t, const double y[], double *dfdy, double dfdt[], void *params);
SPDLLEXPORT  int initial(engine *_eigen);
SPDLLEXPORT  msgqueue *generaMsgqueue(size_t _size);
SPDLLEXPORT  int insertTomsgqueue(msgqueue *_queue, const function<int(int)> &_handel);
SPDLLEXPORT  engine *make_engine(spprojection *_projection);
SPDLLEXPORT  void free_eigen(engine* _eigen);
SPDLLEXPORT  int load(engine *_eigen); //第一次读取
SPDLLEXPORT  int derive(engine *_eigen);
SPDLLEXPORT  int linker_add(model *_themodel);
SPDLLEXPORT  int initial(engine *_eigen);
SPDLLEXPORT  int initialWithRandam(engine *_eigen); //会对随机数进行响应
SPDLLEXPORT  int update(engine *_eigen);
SPDLLEXPORT  int stop(engine *_eigen);
SPDLLEXPORT  int start(engine *_eigen);
#ifdef __cplusplus
}
#endif
#endif