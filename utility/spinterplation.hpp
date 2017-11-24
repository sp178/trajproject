#pragma once

#include "META.hpp"
//可以用于cuda差值

#include "stdint.h"
#include <string>
#include <strstream>
#include <vector>
#include "spdatadef.h"

#ifndef TPOW2
#define TPOW2(_VAL) TPOW<2, _VAL>::VAL
#endif

template <unsigned _T>
struct BLOCK
{
  enum
  {
    DIM = _T
  };
  uint32_t m_cordial_demtion[_T] = {0}; //每个维度上的坐标个数
  double *m_cordials;                   //每个维度上的坐标刻度
  double *m_beg;                        //差值数据初始位置
};
template <unsigned _T>
struct InterRecod
{
  uint32_t m_lag_cordinate[_T] = {0}; //差值坐标位置
  double m_lag[_T] = {0};             //需要差值数据
};
template <unsigned _T>
struct interdata
{
  BLOCK<_T> *_data;
  InterRecod<_T> *_interRecod;
};
template <unsigned _T>
__device__ __host__ void findcordi(uint32_t m_temp_position[TPOW2(_T)],
                                   double m_temp_proper[_T], BLOCK<_T> *_block,
                                   InterRecod<_T> *_record)
{
  uint32_t tempmul = 1; //位置迭代用于乿
  uint32_t temppow = 1; //位置迭代用于平方
  uint32_t temp = 0;    //位置迭代用于边界
  uint32_t _length = 0;
  double *_cordials = nullptr;
  uint32_t _cordial = 0;
  double *m_cordials = _block->m_cordials;
  uint32_t *m_cordial_demtion = _block->m_cordial_demtion;
  uint32_t *m_lag_cordinate = _record->m_lag_cordinate;
  double *m_lag = _record->m_lag;
  for (uint32_t index1 = 0; index1 < _T; index1++)
  {

    _length = m_cordial_demtion[index1]; //寻找边界长度

    _cordials = &m_cordials[temp];      //插值坐标
    _cordial = m_lag_cordinate[index1]; //记录插值坐标位置
                                        //如果当前维度在边界时

    /*判断当前插值点是在上一步插值点左方还是右方*/
    if (m_lag[index1] > _cordials[_cordial])
    { //右方
      for (uint32_t index2 = _cordial; index2 < _length; ++index2)
      {
        if ((_length - 1) == index2)
        {
          //直接将坐标设置为右边界
          m_temp_proper[index1] = 0; //递归因数
          m_lag_cordinate[index1] = _length - 1;
        }
        else
        {
          if (m_lag[index1] < _cordials[index2 + 1])
          {
            m_lag_cordinate[index1] = index2 + 1;
            m_temp_proper[index1] = (_cordials[index2 + 1] - m_lag[index1]) /
                                    (_cordials[index2 + 1] - _cordials[index2]);
            break;
          }
        }
      }
    }
    else
    { //左方
      for (uint32_t index2 = _cordial; index2 >= 0; --index2)
      {
        if (0 == index2)
        {
          m_temp_proper[index1] = 1; //递归因数
          m_lag_cordinate[index1] = 1;
          break;
        }
        else
        {
          if (m_lag[index1] > _cordials[index2 - 1])
          {
            m_lag_cordinate[index1] = index2;
            m_temp_proper[index1] = (_cordials[index2] - m_lag[index1]) /
                                    (_cordials[index2] - _cordials[index2 - 1]);
            break;
          }
        }
      }
    }
    for (uint32_t index2 = 0; index2 < TPOW2(_T); index2++)
    {
      if (0 != (index2 / temppow) % 2) //左忍
        m_temp_position[index2] += m_lag_cordinate[index1] * tempmul;
      else //右忍
        m_temp_position[index2] += (m_lag_cordinate[index1] - 1) * tempmul;
    }
    tempmul *= _length;
    temp += _length;
    temppow = temppow << 1;
  }
}

template <unsigned _T>
__device__ __host__ double interplate(BLOCK<_T> *_block, double _lag[_T],
                                      InterRecod<_T> *_record)
{
  uint32_t _temp_position[TPOW2(_T)] = {0}; //递归数据临时数据
  double _temp_reducdata[TPOW2(_T)];        //递归数据的临时数捿
  double _temp_proper[_T];                  //递归的比例系数
  double *m_beg = _block->m_beg;
  memcpy(_record->m_lag, _lag, sizeof(double) * _T);
  findcordi<_T>(_temp_position, _temp_proper, _block, _record);
  //迭代递归计算
  uint32_t tempindex = TPOW2(_T) / 2;
  for (uint32_t index = 0; index < TPOW2(_T); index++)
  {
    _temp_reducdata[index] = m_beg[_temp_position[index]];
  }
  for (uint32_t index = _T - 1; index > 0; index--)
  {
    for (uint32_t index2 = 0; index2 < tempindex; index2++)
    {
      _temp_reducdata[index2] =
          _temp_reducdata[index2 + tempindex] -
          _temp_proper[index] *
              (_temp_reducdata[index2 + tempindex] - _temp_reducdata[index2]);
    }
    tempindex = tempindex >> 1;
  }
  return _temp_reducdata[1] -
         _temp_proper[0] * (_temp_reducdata[1] - _temp_reducdata[0]);
};