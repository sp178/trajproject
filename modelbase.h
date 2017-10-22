#ifndef __MODELBASE_H_
#define __MODELBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <tuple>
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
spprojection *make_project(projectinfo *_themodel);
spindex findmodelindex(const string &_name, projectinfo *_themodel);
spindex findoutindex(const string &_name, modelinfo *_themodel);
spindex findinindex(const string &_name, modelinfo *_themodel);
typedef int (*spfunc)(double _time, double _x, double _f,
                      double *_in, double *_out, double *_params);

struct modelinfo
{
    string _modelname;
    vector<string> _x_name;
    vector<string> _in_name;
    vector<string> _out_name;
    vector<string> _param_name;
    vector<tuple<string, string, string>> _linker;
};
struct projectinfo
{
    string _projectname;
    vector<modelinfo> _models;
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
};
struct model
{
    modelimpl _impl;
    modeldataimpl _data;
    spfunc _func;
    model *_models;
    //采用输入连接(前置)(输入端口编号，输入模型编号，输入模型输出端口编号)
    vector<tuple<spindex, spindex, spindex>> _linker;
};
struct spprojection
{
    unsigned int _projectid; //项目编号
    vector<model> _models;   //模型
    spindex _xdim;           //状态变量维度
    spindex _indim;          //输入维度
    spindex _outdim;         //输出维度
    spindex _paramdim;       //参数维度
    spfloat *_x;             //状态变量维度
    spfloat *_in;            //输入维度
    spfloat *_out;           //输出维度
    spfloat *_param;         //参数维度
    spfloat _time;           //时间
    spfloat _step;           //时步
};

#endif