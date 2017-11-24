#pragma once
#ifndef __SPDATADEF_H
#define __SPDATADEF_H
#ifdef __NVCC__
#pragma message("using cuda")
#endif
#ifndef __NVCC__
#ifndef __device__
#define __device__
#endif
#ifndef __constant__
#define __constant__ const
#endif // !__constant__

#ifndef __host__
#define __host__
#endif
#else
#ifndef __CUDA_INTERNAL_COMPILATION__
#define __CUDA_INTERNAL_COMPILATION__
#include<math_functions.h>
#undef __CUDA_INTERNAL_COMPILATION__
#endif
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#endif
typedef double spfloat;
#include <stdint.h>
#define Constant_PI 3.1415926535898
#define Constant_RAD  0.01745329251994
#define Constant_DEG  57.29577951308
#define Constant_Je  1.623945E-3
#define Constant_MIU  3.986005E14
#define Constant_Earth_GM  3.986005E+14		  // 地心引力常数   (m^3/s^2)
#define Constant_Earth_J2  1.08263E-3	  // 动力形状因子
#define Constant_Earth_RA  6378140			  // 地球长半轴	  (m)
#define Constant_Earth_RP  6356755.28856		  // 地球短半轴	  (m)
#define Constant_Earth_Rm  6371003.7814		  // 地球平均半径   (m)
#define Constant_Earth_Rae  6378140			  // 地球赤道半径   (m)
#define Constant_Earth_Alpha  0.00335281311553 // 地球扁率
#define Constant_Earth_g0  9.80665			  // 标准重力加速度 (m/s^2)
#define Constant_Earth_We  7.292115E-5		  // 地球自旋角速度 (rad/s)
//#define Constant_Earth_e2  6.69438487525E-3	// 第一偏心率的平方
#define Constant_Earth_Ae  0.00335281311553	// 地球扁率
#define Constant_Earth_ae  (1. / 298.257)
#define Constant_Earth_e  0.9977635403573926373497851397775
#define Constant_Earth_e2  0.99553208246651828615184517583943
#define rad2deg(_x) (57.2958 * (_x))
#define deg2rad(_x) (0.0174533 * (_x))

#define BLOCK_NUM 8
#define THREAD_NUM 128
#define CUOUTNUM 7
#define SPEEDMAX 3000
#define SPEEDMIN 300
#define HIGHTMAX 1000000
#define HIGHTMIN 20000
#define SPEEDEND 400
#define RECORDTIME 20
#define ENERGINMAX (0.5 * SPEEDMAX * SPEEDMAX + Constant_Earth_g0 * HIGHTMAX)
#define ENERGINMIN (0.5 * SPEEDMIN * SPEEDMIN + Constant_Earth_g0 * HIGHTMIN)
#define CUSTATANUM 7
#define AOABEG 35
typedef int (*Derivefunc)(spfloat _time, const spfloat *_x, spfloat *_f, spfloat *_param, uint32_t _index);
typedef int (*Outfunc)(spfloat _time, const spfloat *_x, spfloat *_param, uint32_t _index);

typedef struct
{
	spfloat *x;
	spfloat *k;
	spfloat *x0;
	spfloat *out;
	spfloat *in;
	spfloat *xtmp;
	spfloat *param;
	Derivefunc func;
	Outfunc funcOut;
	unsigned short dim;
	unsigned short dimOut;
	unsigned short dimIn;
	unsigned char paramdim;
	unsigned char isDown;
} rk4_state;

#define CHACK_RETURN0(X) \
	{                    \
		if (X)           \
			return X;    \
	};

#endif // !
