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
#include"spdatabase.h"


#ifdef __cplusplus
extern "C"{
#endif
//(区别不大 估计是硬盘性能还是啥原因不过 linux 下就不好说了所以还是保留吧)
SPDLLEXPORT int setRecorderBuffer(recorder *_recorder, uint64_t _size);
SPDLLEXPORT int recorderWriteSig(recorder *_recorder,
                     char *_data, uint64_t _length);

SPDLLEXPORT int recorderWriteMulti(recorder *_recorder,
                       char *_data, uint64_t _length);
//只存储一次的数据数量
//多次出现数据数量
SPDLLEXPORT recorder *createrRecorder(uint64_t _signum,
                          uint64_t _mutinum,
                          datatype _datatype,
                          const char *_path,
                          uint32_t _step = 0);
SPDLLEXPORT void free_recorder(recorder *_reco);
SPDLLEXPORT int closeRecorder(recorder *_recorder);
#ifdef __cplusplus
}
#endif
#endif