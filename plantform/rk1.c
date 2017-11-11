/* ode-initval/rk1.c
 * 
 *unfortunately the GSL dones not provide the rk1 method ,
 *so I have to provide the rk1 by myself
 */
/* Author:  sun.peng
 */

/* Reference: Abramowitz & Stegun, section 25.5. Runge-Kutta 2nd (25.5.7)
   and 3rd (25.5.8) order methods */

#include <stdlib.h>
#include <string.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include "odeiv_util.h"
#include "step_utils.c"

typedef struct
{
  double *k1;
}
rk1_state_t;

static void *
rk1_alloc (size_t dim)
{
  rk1_state_t *state = (rk1_state_t *) malloc (sizeof (rk1_state_t));

  if (state == 0)
    {
      GSL_ERROR_NULL ("failed to allocate space for rk1_state", GSL_ENOMEM);
    }

  state->k1 = (double *) malloc (dim * sizeof (double));

  if (state->k1 == 0)
    {
      free (state);
      GSL_ERROR_NULL ("failed to allocate space for k1", GSL_ENOMEM);
    }

  return state;
}


static int
rk1_apply (void *vstate,
           size_t dim,
           double t,
           double h,
           double y[],
           double yerr[],
           const double dydt_in[],
           double dydt_out[], const gsl_odeiv2_system * sys)
{
  rk1_state_t *state = (rk1_state_t *) vstate;

  size_t i;

  double *const k1 = state->k1;

  /* k1 step */
  /* k1 = f(t,y) */
  int s = GSL_ODEIV_FN_EVAL (sys, t, y, k1);

  if (s != GSL_SUCCESS)
    {
      return s;
    }

  /* final sum */
  for (i = 0; i < dim; i++)
    {
      y[i] = y[i] + h * k1[i];
    }



  return GSL_SUCCESS;
}

static int
rk1_reset (void *vstate, size_t dim)
{
  rk1_state_t *state = (rk1_state_t *) vstate;

  DBL_ZERO_MEMSET (state->k1, dim);

  return GSL_SUCCESS;
}

static unsigned int
rk1_order (void *vstate)
{
  rk1_state_t *state = (rk1_state_t *) vstate;
  state = 0;                    /* prevent warnings about unused parameters */
  return 2;
}

static void
rk1_free (void *vstate)
{
  rk1_state_t *state = (rk1_state_t *) vstate;
  free (state->k1);
  free (state);
}

static const gsl_odeiv2_step_type rk1_type = { "rk1",   /* name */
  1,                            /* can use dydt_in */
  1,                            /* gives exact dydt_out */
  &rk1_alloc,
  &rk1_apply,
  &stepper_set_driver_null,
  &rk1_reset,
  &rk1_order,
  &rk1_free
};

const gsl_odeiv2_step_type *gsl_odeiv2_step_rk1 = &rk1_type;
