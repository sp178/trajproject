/*
recorder:数据记录功能
*/
#ifndef __RECORDER_H
#define __RECORDER_H
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
/*
  size_t   //只存储一次的数据数量
  size_t   //多次出现数据数量
  size_t   //多次出现数据计数
*/
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
//(区别不大 估计是硬盘性能还是啥原因不过 linux 下就不好说了所以还是保留吧)
int setRecorderBuffer(recorder *_recorder, uint64_t _size);
int recorderWriteSig(recorder *_recorder,
                     char *_data, uint64_t _length);

int recorderWriteMulti(recorder *_recorder,
                       char *_data, uint64_t _length);
//只存储一次的数据数量
//多次出现数据数量
recorder *createrRecorder(uint64_t _signum,
                          uint64_t _mutinum,
                          datatype _datatype,
                          const char *_path,
                          uint32_t _step = 0);
void free_recorder(recorder *_reco);
int closeRecorder(recorder *_recorder);

#endif