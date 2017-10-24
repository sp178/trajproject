#define SPDLLEXPORT
//gcc dlltest.c -shared -o dlltest.dll -Wl,--out-implib,dlltest.lib
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;
extern "C" {
SPDLLEXPORT int test1(int _msg, double _time, double *_x, double *_f,
                      double *_in, double *_out, double *_params)
{
    switch (_msg)
    {
    case SP_MSG_DERIVE:
    {
        _f[0] = sin(_time) + _params[0] + _params[1];
        _f[1] = cos(_time) + _params[2] + _params[3];
        _out[0] = _x[0];
        _out[1] = _x[1];
        break;
    }
    case SP_MSG_INITIAL:
    {
        char *info_ = (char *)_params;
        cout << info_ << endl;
        break;
    }
    }
    //cout << "ÏûÏ¢:" << _msg << endl;
    return 0;
};
SPDLLEXPORT int test2(int _msg, double *_time, double *_x, double *_f,
                      double *_in, double *_out, double *_params)
{
    switch (_msg)
    {
    case SP_MSG_DERIVE:
    {
        _out[0] = _in[0];
        _out[1] = _in[1];
        break;
    }
    }
    return 0;
};
}
