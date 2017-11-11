理论上是可以同过32位和64位的编译
只需要在其根目录下 cmake . -G"这里输入需要生成的生成文件类型"
需要外部库 GSL boost
在windows下最好使用msys或者mingw来编译 
否则就需要更改cmake。为其提供 boost 和gsl的文件位置 不然cmake 的findpackage找不到GSL和boost 
在 msys 下 clang编译版本会莫名其妙崩溃不知道为什么。

用于动力学仿真的模块化工具包。
包括模块调度
线性插值
积分封装
和简单的数据存储以及matlab解出数据(随机数据没有解出如果愿意可以自己解析)--我觉得二进制文件占用体积小比较适合运算 需要看数据可以写个脚本自己看就好了
模型配置
正态随机数的蒙特卡洛打靶
极其简单的仿真时间统计
//后期可以加上数据通讯 包括 tcp/ip serial 以及实时网卡或其它特殊通讯结构(考虑在boost::io_service下统一)-异步以及同步
//反正也是开源的所以不合适可以自己改嘛


数据格式
{
  uint64_t _datatype;       //数据类型
  uint64_t _signum = 0;     //只存储一次的数据数量
  uint64_t _mutinum = 0;    //多次出现数据数量
  uint64_t _mutilength = 0; //多次出现数据长度(字节长度)
  double[_mutilength/8]
  double[_signum]           //只有在配置文件中出现随机数设置才会存储 --当然这都可以自己改
}


