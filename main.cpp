#include "modelbase.h"
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
int main()
{
    projectinfo *info = nullptr;
    spprojection *project = nullptr;
    try
    {
        info = readprojectfromxml("d:/test/sptraj/Trajdata.xml");
        project = make_project(info);
        InitalDataFromXml(project, "d:/test/sptraj/Trajdata.xml");
    }
    catch (exception &_e)
    {
        cout << _e.what();
    }
    // for (auto model : info->_models)
    // {
    //     cout << model._modelname << endl;
    //     cout << "输入名称:" << endl;
    //     for (auto _in : model._in_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "输出名称:" << endl;
    //     for (auto _in : model._out_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "状态名称:" << endl;
    //     for (auto _in : model._x_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "参数名称:" << endl;
    //     for (auto _in : model._param_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     for (auto _link : model._linker)
    //     {
    //         cout << get<0>(_link) << "/" << get<1>(_link) << "/" << get<2>(_link) << endl;
    //     }
    // }

    for (int index = 0; index < project->_xdim; ++index)
    {
        cout << project->_x[index] << endl;
    }
    return 0;
}