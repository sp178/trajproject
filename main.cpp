#include "modelbase.h"
#include "odeengin.h"
#include <iostream>
#include <string>
#include <gsl/gsl_errno.h>
#include <tuple>
#include <vector>

using namespace std;
int main()
{
    projectinfo *info = nullptr;
    spprojection *project = nullptr;
    try
    {
        info = readprojectfromxml("d:/test/sptraj/test.xml");
        project = make_project(info);
        InitalDataFromXml(project, "d:/test/sptraj/test.xml");
    }
    catch (exception &_e)
    {
        cout << _e.what();
    }
    // for (auto model : info->_models)
    // {
    //     cout << model._modelname << endl;
    //     cout << "ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½:" << endl;
    //     for (auto _in : model._in_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?:" << endl;
    //     for (auto _in : model._out_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "×´Ì¬ï¿½ï¿½ï¿½ï¿½:" << endl;
    //     for (auto _in : model._x_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     cout << "ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½:" << endl;
    //     for (auto _in : model._param_name)
    //     {
    //         cout << _in << endl;
    //     }
    //     for (auto _link : model._linker)
    //     {
    //         cout << get<0>(_link) << "/" << get<1>(_link) << "/" << get<2>(_link) << endl;
    //     }
    // }
    engine *engine_ = make_engine(project);
    initial(engine_);
    while (project->_endtime > project->_time)
    {
        int status = run_fixed_one(engine_);
        cout << "Ê±¼ä:" << project->_time;
        double _data, _data2, _data3 = 3.14144;
        _data = project->_models[1]._data._out[0];
        _data2 = project->_models[1]._data._out[1];
        cout << "    " << _data << "/"
             << _data2 << ":" << _data3 << endl;
    }
    return GSL_SUCCESS;
}