/*
积分接口
*/
#include <gsl/gsl_odeiv2.h>
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
#include "modelbase.h"

struct odeengine;
struct odecore;
typedef int (*gslfunc)(double t, const double *y, double *dydt, void *params);
typedef int (*gsljacobian)(double t, const double *y, double *dfdx, double *f,
                           void *params);

struct ode_step
{
    const gsl_odeiv2_step_type *type;
    size_t dimension;
    void *state;
};
struct ode_step_type
{
    const char *name;
    int can_use_dydt_in;
    int gives_exact_dydt_out;
    void *(*alloc)(size_t dim);
    int (*apply)(void *state, size_t dim, double t, double h, double y[],
                 double yerr[], const double dydt_in[], double dydt_out[],
                 const gsl_odeiv2_system *dydt);
    int (*set_driver)(void *state, const gsl_odeiv2_driver *d);
    int (*reset)(void *state, size_t dim);
    unsigned int (*order)(void *state);
    void (*free)(void *state);
};
struct ode_driver
{
    gsl_odeiv2_system *sys; /* ODE system */
    gsl_odeiv2_step *s;     /* step object */
    gsl_odeiv2_control *c;  /* 控制器 object */
    gsl_odeiv2_evolve *e;   /* 求解器 object */
    double h;               /* step size */
    double hmin;            /* minimum step size allowed */
    double hmax;            /* maximum step size allowed */
    unsigned long int n;    /* number of steps taken */
    unsigned long int nmax; /* Maximum number of steps allowed */
};

struct ode_system
{
    int (*function)(double t, const double y[], double dydt[], void *params);
    int (*jacobian)(double t, const double y[], double *dfdy, double dfdt[],
                    void *params);
    size_t dimension;
    void *params;
};

struct ode_evolve
{
    size_t dimension;
    double *y0;
    double *yerr;
    double *dydt_in;
    double *dydt_out;
    double last_step;
    unsigned long int count;
    unsigned long int failed_steps;
    const gsl_odeiv2_driver *driver;
};
struct ode_control
{
    const char *name;
    void *(*alloc)(void);
    int (*init)(void *state, double eps_abs, double eps_rel, double a_y,
                double a_dydt);
    int (*hadjust)(void *state, size_t dim, unsigned int ord, const double y[],
                   const double yerr[], const double yp[], double *h);
    int (*errlevel)(void *state, const double y, const double dydt,
                    const double h, const size_t ind, double *errlev);
    int (*set_driver)(void *state, const gsl_odeiv2_driver *d);
    void (*free)(void *state);
};
struct odecore
{
    gsl_odeiv2_control *_control; //控制器
    gsl_odeiv2_evolve *_evolue;   //求解器
    gsl_odeiv2_system *_system;   //系统
    gsl_odeiv2_driver *_driver;   //驱动
    gsl_odeiv2_step *_step;       //单步求解
};
struct odeengine
{
    odecore *_core;
    struct modelimpl _model;
    struct modeldataimpl _modeldata;  
};
