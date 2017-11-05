#pragma once
//计算类的指数运算

typedef double spfloat;
template<typename DATA, size_t NUM, size_t _DIM = DATA::DIM>
struct POWER
{
	enum { VALUE = POWER<DATA, NUM, _DIM - 1>::VALUE *NUM };
};
template<typename DATA, size_t NUM>
struct POWER<DATA, NUM, 0>
{
	enum { VALUE = 1 };
};

template< unsigned int _base, unsigned int _exponent>
struct TPOW
{
	enum
	{
		VAL = TPOW<_base, _exponent - 1>::VAL * _base
	};
};

template< unsigned int _base>
struct TPOW<_base, 0>
{
	enum
	{
		VAL = 1
	};
};