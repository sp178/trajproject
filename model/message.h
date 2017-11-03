#pragma once
#ifndef __MESSAGE_H_
#define __MESSAGE_H_

struct sysinfo;
typedef int (*spfunc)(int _msg, void *_model, sysinfo *_sys);
struct sysinfo
{
    unsigned int _stepcount; //仿真步数
    unsigned int _xlen;
    unsigned int _outlen;
    unsigned int _inlen;
    unsigned int _parmlen;
    double _time; //时间
    double _setp; //仿真步长
    const char *_database;
    double *_x;
    double *_f;
    double *_param;
    double *_in;
    double *_out;
};
/*
消息映射表
仿真消息
only:
SP_MSG_LOAD
loop:
    SP_MSG_INITIAL 一次
    for()
    {
        SP_MSG_UPDATE 
        SP_MSG_DERIVE
    }
SP_MSG_STOP

*/

#define SP_MSG_BEG 1024                 //消息映射初始
#define SP_MSG_INITIAL (SP_MSG_BEG + 1) //初始化

#define SP_MSG_DERIVE (SP_MSG_BEG + 2) //微分计算
#define SP_MSG_UPDATE (SP_MSG_BEG + 3) //输出
#define SP_MSG_STOP (SP_MSG_BEG + 4)   //停止
#define SP_MSG_CLEAR (SP_MSG_BEG + 5)  //清理数据
#define SP_MSG_LOAD (SP_MSG_BEG + 6)   //清理数据
#endif