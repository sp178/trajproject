#pragma once
#ifndef __SPMATH_H
#define __SPMATH_H
#include "spdatadef.h"
#ifdef _WIN32
#define DLLEXPORT
#endif
#ifdef DLLEXPORT
#define spdllexport __declspec(dllexport)
#else
#define spdllexport
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

__device__ __host__ int spdllexport rt4_step(
	const rk4_state &state,
	const spfloat h, const spfloat t, uint32_t _index); //本积分代码定义在cu中
														//弹道积分相关
__device__ __host__ int spdllexport CalCulate(const rk4_state &state, const spfloat h,
											  const spfloat t, uint32_t _index);

__device__ __host__ double spdllexport getSigmaWithE(double _E, double _max, double _Edmax);
__device__ __host__ double spdllexport getSigma(double _speed, double _max, double _speedmax = SPEEDMAX);
__device__ __host__ double spdllexport getAlpha(double _speed, double _rangeAngle = AOABEG);
__device__ __host__ double spdllexport getE(double _V, double _H);

__device__ __host__ double spdllexport length(double _longdest, double _ladest, double _longtsrc, double _latsrc);
__device__ __host__ double spdllexport lanuchAngle(double _longdest, double _ladest, double _longtsrc, double _latsrc);
__device__ __host__ double spdllexport getCrose(double _deslongt, double _deslat, double _srclongt, double _srclat, double _prelongt, double _prelat);
__device__ __host__ double spdllexport altitude(double _lat, double _r);
__device__ __host__ double spdllexport sgn(double _data);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !
