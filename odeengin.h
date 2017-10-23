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

#include "spdatabase.h"

int linker_add(model *_themodel);
int func(double t, const double *_y, double *_f, void *_param);
int jac(double t, const double y[], double *dfdy, double dfdt[], void *params);
int initial(engine *_eigen);
msgqueue *generaMsgqueue(size_t _size);
int insertTomsgqueue(msgqueue *_queue, const function<int(int)> &_handel);

// typedef int (*spfunc)(int _msg, double _time, double _x, double _f,
//     double *_in, double *_out, double *_params);
engine *make_engine(spprojection *_projection);

int load(engine *_eigen); //第一次读取
int derive(engine *_eigen);
int linker_add(model *_themodel);
int initial(engine *_eigen);
int update(engine *_eigen);
int stop(engine *_eigen);
