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
    projectinfo *info = nullptr;
    spprojection *project = nullptr;
    engine *engine_ = nullptr;
    try
    {
        info = readprojectfromxml("./data/test.xml");
        project = make_project(info);
        InitalDataFromXml(project, "./data/test.xml");

        engine_ = make_engine(project);
        load(engine_);
    }
    catch (exception &_e)
    {
        cout << _e.what();
        return 0;
    }

    boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();
    //initial(engine_);
    initialWithRandam(engine_);
    recorder *_rec = createrRecorder(0, project->_outdim + 1, datatype::_size8, "./test.spdata", 0);
    double *_data = new double[project->_outdim + 1];
    int status = 0;
    //setRecorderBuffer(_rec, 40960);(区别不大 估计是硬盘性能还是啥原因不过 linux 下就不好说了所以还是保留吧)
    while (project->_endtime > project->_time)
    {
        status = update(engine_);
        if (status)
            break;
        _data[0] = project->_time;
        memcpy(_data + 1, project->_out, project->_outdim * sizeof(double));
        recorderWriteMulti(_rec, (char *)_data, sizeof(double) * (project->_outdim + 1));
        status = derive(engine_);
        if (status)
            break;
    }
    stop(engine_);
    closeRecorder(_rec);
    boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
    cout << "时间" << boost::posix_time::to_iso_string(ptEnd - ptStart) << endl;
    return GSL_SUCCESS;
}