#ifndef __SPRECOREDERDATABASE_H
#define __SPRECOREDERDATABASE_H
/*
  size_t   //只存储一次的数据数量
  size_t   //多次出现数据数量
  size_t   //多次出现数据计数
*/
#include<string>
#include<stdint.h>
enum datatype : unsigned char
{
  _size1 = 1,
  _size2 = 2,
  _size4 = 4,
  _size8 = 8,
  _size16 = 16,
};
struct datainfo
{
  uint64_t _datatype;
  uint64_t _signum = 0;     //只存储一次的数据数量
  uint64_t _mutinum = 0;    //多次出现数据数量
  uint64_t _mutilength = 0; //多次出现数据长度(字节长度)
};
struct recorder
{
  datainfo _info; //存储数据信息
  char *_sigbuff; //一次写入数据
  FILE *_file;
  uint32_t _step;  //存储时步
  uint32_t _count; //写入计数
};
struct recorderinfo
{
  uint32_t _step;    //存储时步
  uint32_t _mutinum; //多数据记录数
  uint32_t _signum;  //单数据记录数
  std::string _path; //存储路径
};
#endif