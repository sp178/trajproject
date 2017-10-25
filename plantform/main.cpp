#include "modelbase.h"

#include "odeengin.h"
#include <iostream>
#include <string>
#include <gsl/gsl_errno.h>
#include <tuple>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "recorder.h"
using namespace std;
int main()
{

    // string path = "./asd/asd/test.exe";
    // if (!boost::filesystem::exists("path"))
    //     boost::filesystem::create_directories(path);

    projectinfo *info = nullptr;
    spprojection *project = nullptr;
    engine *engine_ = nullptr;
    try
    {
        info = readprojectfromxml("d:/test/sptraj/data/test.xml");
        project = make_project(info);
        InitalDataFromXml(project, "d:/test/sptraj/data/test.xml");
        engine_ = make_engine(project);
        load(engine_);
    }
    catch (exception &_e)
    {
        cout << _e.what();
        return 0;
    }
    boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();
    initial(engine_);
    int status = 0;
    while (project->_endtime > project->_time)
    {
        status = update(engine_);
        if (status)
            break;
        status = derive(engine_);
        if (status)
            break;
    }
    stop(engine_);
    boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
    cout << "Ê±¼ä" << boost::posix_time::to_iso_string(ptEnd - ptStart) << endl;
    return GSL_SUCCESS;
}