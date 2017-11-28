#ifndef __MODELBASE_H_
#define __MODELBASE_H_

#include <stdio.h>
#include <stdlib.h>
#include "spdatabase.h"
#ifdef __cplusplus
extern "C"{
#endif
 SPDLLEXPORT  spprojection *make_project(projectinfo *_themodel);
 SPDLLEXPORT  void free_project(spprojection * _project);
 SPDLLEXPORT  void free_projectinfo(projectinfo* _info);
 SPDLLEXPORT  spindex findmodelindex(const std::string &_name, projectinfo *_themodel);
 SPDLLEXPORT  spindex findoutindex(const std::string &_name, modelinfo *_themodel);
 SPDLLEXPORT  spindex findinindex(const std::string &_name, modelinfo *_themodel);
 SPDLLEXPORT  projectinfo *readprojectfromxml(const std::string &_path);
 SPDLLEXPORT  int InitalDataFromXml(spprojection *_projection, const std::string &_path);    //设置初始值
#ifdef __cplusplus
}
#endif
#endif