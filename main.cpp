#include "modelbase.h"

#include "odeengin.h"
#include <iostream>
#include <string>
#include <gsl/gsl_errno.h>
#include <tuple>
#include <vector>
#include <fstream>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;
int main()
{
    projectinfo *info = nullptr;
    spprojection *project = nullptr;
    engine *engine_ = nullptr;
    try
    {
        info = readprojectfromxml("d:/test/sptraj/test.xml");
        project = make_project(info);
        InitalDataFromXml(project, "d:/test/sptraj/test.xml");
        engine_ = make_engine(project);
        load(engine_);
    }
    catch (exception &_e)
    {
        cout << _e.what();
        return 0;
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
    getchar();
    boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();
    initial(engine_);
    while (project->_endtime > project->_time)
    {

        int status = update(engine_);
        status = derive(engine_);
    }
    stop(engine_);
    boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
    cout << "Ê±¼ä::" << boost::posix_time::to_iso_string(ptEnd - ptStart) << endl;
    return GSL_SUCCESS;
}