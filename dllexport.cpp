#define SPDLLEXPORT
//gcc dlltest.c -shared -o dlltest.dll -Wl,--out-implib,dlltest.lib
#include <stdio.h>
extern "C" {
SPDLLEXPORT int test(int _msg, double _time, double _x, double _f,
                     double *_in, double *_out, double *_params)
{
    printf("hello\n");
    return 0;
};
}
