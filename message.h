#ifndef __MESSAGE_H_
#define __MESSAGE_H_
/*
消息映射表
仿真消息
SP_MSG_INITIAL 一次

for()
{
    SP_MSG_UPDATE 
    linker
    SP_MSG_DERIVE
}

*/
#define SP_MSG_BEG 1024                 //消息映射初始
#define SP_MSG_INITIAL (SP_MSG_BEG + 1) //初始化

#define SP_MSG_DERIVE (SP_MSG_BEG + 1) //微分计算
#define SP_MSG_UPDATE (SP_MSG_BEG + 2) //输出
#define SP_MSG_STOP (SP_MSG_BEG + 3)   //停止
#define SP_MSG_CLEAR (SP_MSG_BEG + 3)  //清理数据

#endif