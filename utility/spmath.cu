#include "spmath.h"

__device__ __host__ double sgn(double _data)
{
	return _data < 0 ? -1 : 1;
}
__device__ __host__ double length(double _longdest, double _ladest, double _longtsrc, double _latsrc)
{
	return acos(cos(_ladest) * cos(_latsrc) * cos(_longdest - _longtsrc) + sin(_ladest) * sin(_latsrc));
}
__device__ __host__ double lanuchAngle(double _longdest, double _ladest, double _longtsrc, double _latsrc)
{
	double a;
	double c = length(_longdest, _ladest, _longtsrc, _latsrc);
	//判断是否在右半平面
	if (_longdest > _longtsrc)
	{
		if (_ladest > _latsrc)
		{ //上
			a = length(_longdest, _ladest, _longtsrc, _ladest);
			if (c < 1E-13)
				return 0;
			return asin(sin(a) / sin(c));
		}
		else
		{
			a = length(_longdest, _ladest, _longdest, _latsrc);
			if (c < 1E-13)
				return Constant_PI * 0.5;
			return Constant_PI * 0.5 + asin(sin(a) / sin(c));
		}
	}
	else
	{ //左半平面
		if (_ladest > _latsrc)
		{ //上
			a = length(_longdest, _ladest, _longtsrc, _ladest);
			if (c < 1E-13)
				return 0;
			return -asin(sin(a) / sin(c));
		}
		else
		{
			a = length(_longdest, _ladest, _longdest, _latsrc);
			if (c < 1E-13)
				return -Constant_PI;
			return -(Constant_PI + asin(sin(a) / sin(c)));
		}
	}
}

__device__ __host__ int rt4_step(
	const rk4_state &state,
	const spfloat h, const spfloat t, uint32_t _index);
/*
外部程序调用接口
*/

/*用于每一步输出*/
__device__ __host__ int CalOut(spfloat _time, const rk4_state &state, uint32_t _index)
{
	if (state.funcOut)
		return state.funcOut(_time, state.x0, state.param, _index);
	return 0;
}
__device__ __host__ int CalCulate(const rk4_state &state,
	const spfloat h, const spfloat t, uint32_t _index)
{
	spfloat time = 0, step = h;
	short _bastate;
	for (;;)
	{
		_bastate = CalOut(time, state, _index);
		if (time > 1000)
			_bastate = 1;
		CHACK_RETURN0(_bastate);
		_bastate = rt4_step(state, step, time, _index);
		time += step;
		CHACK_RETURN0(_bastate);
	}
}

/*
以下函数全部用于cuda使用 普通程序调用禁止
*/

__device__ __host__ double altitude(double _lat, double _r)
{
	double SINMIU, COSMIU, B, Re, h;
	SINMIU = sin(_lat);
	COSMIU = cos(_lat);
	B = atan2(SINMIU / COSMIU, e2);
	Re = R_a * e / sqrt((SINMIU * SINMIU + e2 * COSMIU * COSMIU));
	h = sqrt(_r * _r - Re * Re * pow(sin(B - _lat), 2)) - Re * cos(B - _lat);
	return h;
}

__device__ __host__ int rt4_step(
	const rk4_state &state,
	const spfloat h, const spfloat t, uint32_t _index)
{
	spfloat *x = state.x; //状态过渡
	spfloat *k = state.k;
	spfloat *x0 = state.x0;         //状态初始值
	spfloat *xtmp = state.xtmp;     //状态过渡值
	spfloat *param = state.param;   //外部参数
	unsigned short dim = state.dim; //积分维度
	Derivefunc func = state.func;   //积分函数
	uint32_t index = 0;
	int s = 0;

	/* k1 */
	s = func(t, x0, k, param, _index);
	/*rk1*/
	//if (s != 0)
	//	return s;
	//for (index = 0; index < dim; index++)
	//{
	//	//x[index] += h / 6.0 * k[index];
	//	x0[index] += h*k[index];
	//}
	//return s;
	/*rk1*/
	if (s != 0)
		return s;
	for (index = 0; index < dim; index++)
	{
		x[index] = h / 6.0 * k[index];
		xtmp[index] = x0[index] + 0.5 * h * k[index];
	}

	/* k2 */
	s = func(t + 0.5 * h, xtmp, k, param, _index);
	if (s != 0)
		return s;
	for (index = 0; index < dim; index++)
	{
		x[index] += h / 3.0 * k[index];
		xtmp[index] = x0[index] + 0.5 * h * k[index];
	}

	/* k3 */
	s = func(t + 0.5 * h, xtmp, k, param, _index);
	if (s != 0)
		return s;
	for (index = 0; index < dim; index++)
	{
		x[index] += h / 3.0 * k[index];
		xtmp[index] = x0[index] + h * k[index];
	}

	/* k4*/
	s = func(t + h, xtmp, k, param, _index);
	if (s != 0)
		return s;
	for (index = 0; index < dim; index++)
	{
		x[index] += h / 6.0 * k[index];
		x0[index] += x[index];
	}
	return 0;
}

__device__ __host__ double getE(double _V, double _H)
{
	return 0.5 * _V * _V + g_0 * _H;
}

__device__ __host__ double getAlpha(double _speed, double _rangeAngle)
{
	return 5 + (_speed - SPEEDMIN) / (SPEEDMAX - SPEEDMIN) * _rangeAngle;
}

__device__ __host__ double getSigmaWithE(double _E, double _max, double _Edmax)
{
	if (fabs(_E) > ENERGINMAX)
		return _max;
	if (fabs(_E) < ENERGINMIN)
		return 0;
	if (_max > 0)
	{
		return (_E - SPEEDMIN) / (_Edmax - SPEEDMIN) * (_max);
	}
	else
	{
		return -((_E - SPEEDMIN) / (_Edmax - SPEEDMIN) * (-_max));
	}
}
__device__ __host__ double getSigma(double _speed, double _max, double _speedmax)
{
	if (fabs(_speed) > SPEEDMAX)
		return _max;
	if (fabs(_speed) < SPEEDMIN)
		return 0;
	if (_max > 0)
	{
		return (_speed - SPEEDMIN) / (_speedmax - SPEEDMIN) * (_max);
	}
	else
	{
		return -((_speed - SPEEDMIN) / (_speedmax - SPEEDMIN) * (-_max));
	}
}

//获取横向误差
//目标经纬度 当前经纬度 预测经纬度
__device__ __host__ double getCrose(double _deslongt, double _deslat, double _srclongt, double _srclat, double _prelongt, double _prelat)
{
	spfloat tmp1 = lanuchAngle(_prelongt, _prelat, _srclongt, _srclat); //预测点航向角
	spfloat tmp2 = lanuchAngle(_deslongt, _deslat, _srclongt, _srclat); //目标点航向角
	spfloat dltapsi = tmp2 - tmp1;                                      //目标点在预测点上为正
	spfloat lengthc = length(_deslongt, _deslat, _srclongt, _srclat);
	return asin(sin(lengthc) * sin(dltapsi)); //记录横向误差
}
