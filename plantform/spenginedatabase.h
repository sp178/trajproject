#ifndef __SPENGINEDATABASE_H
#define __SPENGINEDATABASE_H
#include <gsl/gsl_odeiv2.h>
#include <functional>
#include <boost/circular_buffer.hpp>
struct odeengine;
struct odecore;
using namespace std;
typedef int (*gslfunc)(double t, const double *y, double *dydt, void *params);
typedef int (*gsljacobian)(double t, const double *y, double *dfdx, double *f,
                           void *params);

typedef boost::circular_buffer<function<int(int)>> msgqueue;
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
    gsl_odeiv2_control *c;  /* ������ object */
    gsl_odeiv2_evolve *e;   /* ����� object */
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
    gsl_odeiv2_control *_control; //������
    gsl_odeiv2_evolve *_evolue;   //�����
    gsl_odeiv2_system *_system;   //ϵͳ
    gsl_odeiv2_driver *_driver;   //����
    gsl_odeiv2_step *_step;       //�������
};
struct engine
{
    spprojection *_project;
    odecore _odecore;
};
#endif