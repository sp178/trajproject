#ifndef __MODELBASE_H_
#define __MODELBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include "spdatabase.h"
using namespace std;

spprojection *make_project(projectinfo *_themodel);
void free_project(spprojection * _project);
void free_projectinfo(projectinfo* _info);
spindex findmodelindex(const string &_name, projectinfo *_themodel);
spindex findoutindex(const string &_name, modelinfo *_themodel);
spindex findinindex(const string &_name, modelinfo *_themodel);
projectinfo *readprojectfromxml(const string &_path);
//设置初始值
int InitalDataFromXml(spprojection *_projection, const string &_path);
#endif