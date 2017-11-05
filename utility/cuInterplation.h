#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include"META.hpp"

#ifndef __device__
	#define __device__ 
#endif
#ifndef __host__
	#define __host__
#endif // !__host

#ifndef TPOW2

#define TPOW2(_VAL) TPOW<2,_VAL>::VAL

#endif

template<unsigned _T>
struct cuBLOCK {
	enum { DIM = _T };
	size_t			m_cordial_demtion[_T] = {0};	//每个维度上的坐标个数
	double*			m_cordials;				//每个维度上的坐标刻度
	double*			m_beg;					//差值数据初始位置
};
template<unsigned _T>
struct cuInterRecod
{
	unsigned int	m_lag_cordinate[_T] = {0};		//差值坐标位置
	double			m_lag[_T] = {0};				//需要差值数据
};

template<unsigned _T>
__device__ __host__ void findcordi(unsigned int m_temp_position[TPOW2(_T)], double m_temp_proper[_T], cuBLOCK<_T>* _block, cuInterRecod<_T>* _record)
{
	unsigned int tempmul = 1;		//位置迭代用于乿
	unsigned int temppow = 1;		//位置迭代用于平方
	unsigned int temp = 0;			//位置迭代用于边界
	unsigned int		_length = 0;
	double*	_cordials = nullptr;
	unsigned int		_cordial = 0;
	double* m_cordials = _block->m_cordials;
	size_t* m_cordial_demtion = _block->m_cordial_demtion;
	size_t* m_lag_cordinate = _record->m_lag_cordinate;
	double* m_lag = _record->m_lag;
	for (unsigned int index1 = 0; index1 < _T; index1++)
	{

		_length = m_cordial_demtion[index1];		//寻找边界长度

		_cordials = &m_cordials[temp];				//插值坐标
		_cordial = m_lag_cordinate[index1];			//记录插值坐标位置
													//如果当前维度在边界时


													/*判断当前插值点是在上一步插值点左方还是右方*/
		if (m_lag[index1] > _cordials[_cordial]) {//右方
			for (unsigned int index2 = _cordial; index2 < _length; ++index2) {
				if ((_length - 1) == index2)
				{
					//直接将坐标设置为右边界
					m_temp_proper[index1] = 0;				//递归因数
					m_lag_cordinate[index1] = _length - 1;
				}
				else {
					if (m_lag[index1] < _cordials[index2 + 1]) {
						m_lag_cordinate[index1] = index2 + 1;
						m_temp_proper[index1] = (_cordials[index2 + 1] - m_lag[index1]) / (_cordials[index2 + 1] - _cordials[index2]);
						break;
					}
				}
			}
		}
		else {//左方
			for (unsigned int index2 = _cordial; index2 >=0; --index2) {
				if (0 == index2)
				{
					m_temp_proper[index1] = 1;				//递归因数
					m_lag_cordinate[index1] = 1;
					break;
				}
				else {
					if (m_lag[index1] > _cordials[index2 - 1]) {
						m_lag_cordinate[index1] = index2;
						m_temp_proper[index1] = (_cordials[index2] - m_lag[index1]) / (_cordials[index2] - _cordials[index2 - 1]);
						break;
					}
				}
			}
		}
		for (unsigned int index2 = 0; index2 < TPOW2(_T); index2++)
		{
			if (0 != (index2 / temppow) % 2)	//左忍
				m_temp_position[index2] += m_lag_cordinate[index1] * tempmul;
			else			//右忍
				m_temp_position[index2] += (m_lag_cordinate[index1] - 1) * tempmul;
		}
		tempmul *= _length;
		temp += _length;
		temppow = temppow << 1;
	}
}


template<unsigned _T>
__device__  __host__ double interplate(cuBLOCK<_T>* _block,double * _lag, cuInterRecod<_T>* _record)
{
	unsigned int		_temp_position[TPOW2(_T)] = { 0 };		//递归数据临时数据
	double				_temp_reducdata[TPOW2(_T)];		//递归数据的临时数捿
	double				_temp_proper[_T];				//递归的比例系数
	double*				m_beg = _block->m_beg;
	memcpy(_record->m_lag, _lag, sizeof(double)*_T);
	findcordi<_T>(_temp_position, _temp_proper,_block, _record);
	//迭代递归计算
	unsigned int tempindex = TPOW2(_T) / 2;
	for (unsigned int index = 0; index < TPOW2(_T); index++)
	{
		_temp_reducdata[index] = m_beg[_temp_position[index]];
	}
	for (unsigned int index = _T - 1; index >0; index--)
	{
		for (unsigned int index2 = 0; index2 < tempindex; index2++)
		{
			_temp_reducdata[index2] = _temp_reducdata[index2 + tempindex] - _temp_proper[index] * (_temp_reducdata[index2 + tempindex] - _temp_reducdata[index2]);
		}
		tempindex = tempindex >> 1;
	}
	return _temp_reducdata[1] - _temp_proper[0] * (_temp_reducdata[1] - _temp_reducdata[0]);
};

template<unsigned _T>
__device__ __host__ cuBLOCK<_T>* makeCuBlock(_BLOCK* _block)
{
	if (_T != _block->m_demention)return nullptr;
	cuBLOCK<_T>* thecublock = new cuBLOCK<_T>;
	size_t _dementionlength = 0;	//坐标维度
	size_t _datalength = 1;		//数据长度
	for (size_t index = 0; index < _block->m_demention; ++index)
	{
		_dementionlength += _block->m_cordial_demtion[index];
		_datalength *= _block->m_cordial_demtion[index];
	}
	thecublock->m_beg = new spfloat[_datalength];
	thecublock->m_cordials = new spfloat[_dementionlength];
	memcpy(thecublock->m_beg, _block->m_beg, sizeof(spfloat)*_datalength);
	memcpy(thecublock->m_cordials, _block->m_cordials, sizeof(spfloat)*_dementionlength);
	memcpy(thecublock->m_cordial_demtion, _block->m_cordial_demtion, sizeof(size_t)*_T);
	return thecublock;
}
