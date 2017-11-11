需要外部库 GSL boost 
在 msys 下 clang编译版本会莫名其妙崩溃不知道为什么

用于动力学仿真的模块化工具包。
包括模块调度
线性插值
积分封装
和简单的数据存储以及matlab解出数据(随机数据没有解出如果愿意可以自己解析)
数据格式
{
  uint64_t _datatype;       //数据类型
  uint64_t _signum = 0;     //只存储一次的数据数量
  uint64_t _mutinum = 0;    //多次出现数据数量
  uint64_t _mutilength = 0; //多次出现数据长度(字节长度)
  double[_mutilength/8]
  double[_signum]           //只有在配置文件中出现随机数设置才会存储 --当然这都可以自己改
}
模型配置

