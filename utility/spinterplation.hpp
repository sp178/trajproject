#include "META.hpp"

#include "stdint.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <string>
#include <strstream>
#include <vector>
#ifndef TPOW2
#define TPOW2(_VAL) TPOW<2, _VAL>::VAL
#endif

//可以用于cuda差值
#ifndef __NVCC__
#ifndef __device__
#define __device__
#endif

#ifndef __host__
#define __host__
#endif
#endif
template <unsigned _T> struct BLOCK {
  enum { DIM = _T };
  uint32_t m_cordial_demtion[_T] = {0}; //每个维度上的坐标个数
  double *m_cordials;                   //每个维度上的坐标刻度
  double *m_beg;                        //差值数据初始位置
};
template <unsigned _T> struct InterRecod {
  uint32_t m_lag_cordinate[_T] = {0}; //差值坐标位置
  double m_lag[_T] = {0};             //需要差值数据
};
template <unsigned _T>
__device__ __host__ void findcordi(uint32_t m_temp_position[TPOW2(_T)],
                                   double m_temp_proper[_T], BLOCK<_T> *_block,
                                   InterRecod<_T> *_record) {
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
  for (uint32_t index1 = 0; index1 < _T; index1++) {

    _length = m_cordial_demtion[index1]; //寻找边界长度

    _cordials = &m_cordials[temp];      //插值坐标
    _cordial = m_lag_cordinate[index1]; //记录插值坐标位置
                                        //如果当前维度在边界时

    /*判断当前插值点是在上一步插值点左方还是右方*/
    if (m_lag[index1] > _cordials[_cordial]) { //右方
      for (uint32_t index2 = _cordial; index2 < _length; ++index2) {
        if ((_length - 1) == index2) {
          //直接将坐标设置为右边界
          m_temp_proper[index1] = 0; //递归因数
          m_lag_cordinate[index1] = _length - 1;
        } else {
          if (m_lag[index1] < _cordials[index2 + 1]) {
            m_lag_cordinate[index1] = index2 + 1;
            m_temp_proper[index1] = (_cordials[index2 + 1] - m_lag[index1]) /
                                    (_cordials[index2 + 1] - _cordials[index2]);
            break;
          }
        }
      }
    } else { //左方
      for (uint32_t index2 = _cordial; index2 >= 0; --index2) {
        if (0 == index2) {
          m_temp_proper[index1] = 1; //递归因数
          m_lag_cordinate[index1] = 1;
          break;
        } else {
          if (m_lag[index1] > _cordials[index2 - 1]) {
            m_lag_cordinate[index1] = index2;
            m_temp_proper[index1] = (_cordials[index2] - m_lag[index1]) /
                                    (_cordials[index2] - _cordials[index2 - 1]);
            break;
          }
        }
      }
    }
    for (uint32_t index2 = 0; index2 < TPOW2(_T); index2++) {
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
__device__ __host__ double interplate(BLOCK<_T> *_block, double *_lag,
                                      InterRecod<_T> *_record) {
  uint32_t _temp_position[TPOW2(_T)] = {0}; //递归数据临时数据
  double _temp_reducdata[TPOW2(_T)];        //递归数据的临时数捿
  double _temp_proper[_T];                  //递归的比例系数
  double *m_beg = _block->m_beg;
  memcpy(_record->m_lag, _lag, sizeof(double) * _T);
  findcordi<_T>(_temp_position, _temp_proper, _block, _record);
  //迭代递归计算
  uint32_t tempindex = TPOW2(_T) / 2;
  for (uint32_t index = 0; index < TPOW2(_T); index++) {
    _temp_reducdata[index] = m_beg[_temp_position[index]];
  }
  for (uint32_t index = _T - 1; index > 0; index--) {
    for (uint32_t index2 = 0; index2 < tempindex; index2++) {
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
template <unsigned _T>
__device__ __host__ BLOCK<_T> *makeCuBlock(const char *_path,
                                           const char *_tablename) {

  using boost::property_tree::ptree;
  using std::stringstream;
  using std::string;
  using std::vector;
  ptree _table;
  ptree _null;

#ifdef USE_UTF_8
  std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
  read_xml(_path, _table, 0, utf8Locale);
#else
  read_xml(_path, _table);
#endif
  ptree tables = _table.get_child("", _null);
  if (tables == _null) {
    return nullptr;
  }
  for (auto table : tables) {
    if (table.first.data() != string("table"))
      continue;
    if (string(_tablename) != table.second.get<string>("<xmlattr>.name", ""))
      return nullptr;
    ptree _blocks = table.second.get_child("");
    for (auto _block : _blocks) {
      if (_block.first.data() != string("block"))
        continue;
      string name = _block.second.get<string>("<xmlattr>.name", "");
      if (name.empty())
        return nullptr;
      //获得数据维度
      if (_T != _block.second.get<int>("<xmlattr>.demention", "0")) {
        return nullptr;
      }
      BLOCK<_T> *theblock_ = new BLOCK<_T>();
      uint32_t index = 0;
      uint32_t indexdementionlength = 0;
      uint32_t indexdatalength = 1;
      vector<double> _tmpcords;
      vector<double> _tmdata;
      auto dementions = _block.second.get_child("");
      for (auto demention : dementions) {
        if (demention.first.data() == string("DEMTION")) {
          if (index >= _T) {
            delete theblock_;
            return nullptr;
          }
          int length = demention.second.get<int>("<xmlattr>.demention", 0);
          stringstream _cordinate;
          _cordinate << demention.second.get<string>("");
          while (!_cordinate.eof()) {
            double data;
            _cordinate >> data;
            if (_cordinate.fail())
              break;
            _tmpcords.push_back(data);
          }
          theblock_->m_cordial_demtion[index] = length;
          indexdementionlength += length;
          indexdatalength *= length;
          ++index;
        }
      }
      if (_tmpcords.size() < indexdementionlength) {
        delete theblock_;
        return nullptr;
      }
      ptree data = dementions.get_child("Data");
      stringstream _data;
      _data << data.get<string>("");
      while (!_data.eof()) {

        double tmp;
        _data >> tmp;
        if (_data.fail())
          break;
        _tmdata.push_back(tmp);
      }
      if (_tmdata.size() < indexdatalength) {
        delete theblock_;
        return nullptr;
      }
      theblock_->m_cordials = new double[indexdementionlength];
      theblock_->m_beg = new double[indexdatalength];
      memcpy(theblock_->m_cordials, _tmpcords.data(),
             indexdementionlength * sizeof(double));
	  memcpy(theblock_->m_beg, _tmdata.data(), indexdatalength * sizeof(double));
	  return theblock_;
    }
  }
  return nullptr;
}