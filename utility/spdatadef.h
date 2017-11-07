#pragma once
#ifndef __SPDATADEF_H
#define __SPDATADEF_H
#ifndef __NVCC__

#ifndef __device__
#define __device__
#endif


#ifndef __host__
#define __host__
#endif
#else
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#endif
typedef double spfloat;
#include<stdint.h>
#define rad2deg(_x) (57.2958 * (_x))
#define deg2rad(_x) (0.0174533 * (_x))
#define Constant_PI 3.1415926535898
#define omega_E 7.292115E-5
#define R_a 6378140
#define f 0.00335281311553
#define GM 3.986005E14
#define R_b 6356755.28856
#define R_0 6371003.7814
#define g_0 9.80665
#define J_2 1.08263E-3
#define e 0.9977635403573926373497851397775
#define e2 0.99553208246651828615184517583943

#define BLOCK_NUM 4
#define THREAD_NUM 128
#define CUOUTNUM 7
#define SPEEDMAX 3000
#define SPEEDMIN 300
#define HIGHTMAX 1000000
#define HIGHTMIN 20000
#define SPEEDEND 400
#define RECORDTIME 20
#define ENERGINMAX (0.5 * SPEEDMAX * SPEEDMAX + g_0 * HIGHTMAX)
#define ENERGINMIN (0.5 * SPEEDMIN * SPEEDMIN + g_0 * HIGHTMIN)
#define CUSTATANUM 7
#define AOABEG 35
typedef int(*Derivefunc)(spfloat _time, const spfloat *_x, spfloat *_f, spfloat *_param, uint32_t _index);
typedef int(*Outfunc)(spfloat _time, const spfloat *_x, spfloat *_param, uint32_t _index);

typedef struct
{
	spfloat *x; //状态过渡
	spfloat *k;
	spfloat *x0;            //状态初始值
	spfloat *out;           //输出值
	spfloat *in;            //输入
	spfloat *xtmp;          //状态过渡值
	spfloat *param;         //外部参数
	Derivefunc func;        //积分函数
	Outfunc funcOut;        //输出函数
	unsigned short dim;     //积分维度
	unsigned short dimOut;  //输出维度
	unsigned short dimIn;   //输入维度
	unsigned char paramdim; //参数维度
	unsigned char isDown;   //积分完成标志
} rk4_state;

#define CHACK_RETURN0(X) \
    {                    \
        if (X)           \
            return X;    \
    }

#endif // !



