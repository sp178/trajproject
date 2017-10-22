#include "modelbase.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
int main()
{
    projectinfo info;
    info._projectname = "hello";
    modelinfo model1, model2;
    model1._modelname = "model1";
    model1._in_name.push_back("in");
    model1._out_name.push_back("out");
    model1._x_name.push_back("x");
    model1._param_name.push_back("_param");
    model1._in_name.push_back("in2");
    model1._out_name.push_back("out2");
    model1._x_name.push_back("x");
    model1._param_name.push_back("_param");
    model1._linker.push_back(make_tuple("in", "model2", "out"));
    model1._linker.push_back(make_tuple("in", "model2", "out2"));

    model2._modelname = "model2";
    model2._in_name.push_back("in");
    model2._out_name.push_back("out");
    model2._x_name.push_back("x");
    model2._param_name.push_back("_param");
    model2._in_name.push_back("in2");
    model2._out_name.push_back("out2");
    model2._x_name.push_back("x");
    model2._param_name.push_back("_param");

    info._models.push_back(move(model1));
    info._models.push_back(move(model2));
    int int1;
    int int2;
    int int3;
    spprojection *_theproject = make_project(&info);
    for (auto &_link : _theproject->_models[0]._linker)
    {
        int1 = get<0>(_link);
        int2 = get<1>(_link);
        int3 = get<2>(_link);
        }
    return 0;
}