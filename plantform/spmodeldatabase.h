#ifndef __SPMODELDATABASE_H
#define __SPMODELDATABASE_H
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include "recorder.h"
#include <boost/dll.hpp>
#include <boost/random.hpp>
using namespace std;
typedef double spfloat; //64位精度浮点
typedef int spindex;    //32位索引
struct modelimpl;
struct modeldataimpl;
struct spprojection;
struct model;
struct projectinfo;
struct modelinfo;
struct scop;

enum eveltype
{
    rk1 = 1,
    rk2 = 2,
    rk4 = 4,
    rk8 = 8
};
struct modelinfo
{
    std::string _modelname;
    std::vector<string> _x_name;
    std::vector<string> _in_name;
    std::vector<string> _out_name;
    std::vector<string> _param_name;
    std::vector<tuple<string, string, string>> _linker;
    std::string _database; //数据具体怎么解析按理说应该是编库的人自己的问题
    std::string _dllpath;  //库加载路径
    std::string _funcname; //加载函数名称
};
struct projectinfo
{
    std::string _projectname;
    std::string _recorderpath;
    std::vector<modelinfo> _models;
};
struct scop
{
};

struct modelimpl
{
    spindex _xdim;     //状态变量维度
    spindex _indim;    //输入维度
    spindex _outdim;   //输出维度
    spindex _paramdim; //参数维度
};

struct modeldataimpl
{
    spfloat *_x;     //状态变量维度
    spfloat *_in;    //输入维度
    spfloat *_out;   //输出维度
    spfloat *_param; //参数维度
    spfloat *_f;
};
struct model
{
    modelimpl _impl;
    modeldataimpl _data;
    spfunc _func;
    boost::dll::shared_library lib;
    model *_models;
    modelinfo *_modelinfo;
    //采用输入连接(前置)(输入端口编号，输入模型编号，输入模型输出端口编号)
    std::vector<std::tuple<spindex, spindex, spindex>> _linker;
    void *_dllmodel;
    sysinfo _sys;
};
struct distibuter
{
    boost::variate_generator<boost::mt19937,
                             boost::normal_distribution<double>>
        _sampler0;
    boost::variate_generator<boost::mt19937,
                             boost::normal_distribution<double>>
        _sampler1;
};
struct spprojection
{
    unsigned int _projectid;    //项目编号
    std::vector<model> _models; //模型
    spindex _xdim;              //状态变量维度
    spindex _indim;             //输入维度
    spindex _outdim;            //输出维度
    spindex _paramdim;          //参数维度
    spfloat *_x;                //状态变量维度
    spfloat *_in;               //输入维度
    spfloat *_out;              //输出维度
    spfloat *_param;            //参数维度
    eveltype _evetype;
    recorderinfo *_recorderinfo;
    spfloat _time;  //时间
    spfloat _step;  //时步
    spindex _count; //记步
    spfloat _endtime;
    spfloat _begtime;
    std::vector<std::tuple<distibuter, uint32_t>> _distribute;
};

#endif