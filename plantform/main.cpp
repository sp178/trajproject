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
        info = readprojectfromxml("d:\\home\\sptraj\\trajproject\\data\\simulation.xml");
        project = make_project(info);
        InitalDataFromXml(project, "d:\\home\\sptraj\\trajproject\\data\\simulation.xml");

        engine_ = make_engine(project);
        load(engine_);
    }
    catch (exception &_e)
    {
        cout << _e.what();
        return 0;
    }
    cout << "开始仿真" << endl;
    boost::posix_time::ptime ptStart = boost::posix_time::microsec_clock::local_time();
    //initial(engine_);
    initialWithRandam(engine_);
    recorder *_rec = createrRecorder(0, project->_outdim + 1, datatype::_size8, "./test.spdata", 0);
    double *_data = new double[project->_outdim + 1];
    int status = 0;
    //setRecorderBuffer(_rec, 40960);(区别不大 估计是硬盘性能还是啥原因不过 linux 下就不好说了所以还是保留吧)
    if (start(engine_))
    {
        printf("开始错误");
        return 0;
    }

    while (project->_endtime > project->_time)
    {
        if (status)
        {
            _data[0] = project->_time;
            memcpy(_data + 1, project->_out, project->_outdim * sizeof(double));
            recorderWriteMulti(_rec, (char *)_data, sizeof(double) * (project->_outdim + 1));
            break;
        }
        status = update(engine_);
        if (status)
        {
            _data[0] = project->_time;
            memcpy(_data + 1, project->_out, project->_outdim * sizeof(double));
            recorderWriteMulti(_rec, (char *)_data, sizeof(double) * (project->_outdim + 1));
            break;
        }

        if (0 == (project->_count % project->_recorderinfo->_step))
        {
            _data[0] = project->_time;
            memcpy(_data + 1, project->_out, project->_outdim * sizeof(double));
            recorderWriteMulti(_rec, (char *)_data, sizeof(double) * (project->_outdim + 1));
        }
        status = derive(engine_);
        //cout << project->_time << endl;
    }
    stop(engine_);
    closeRecorder(_rec);
    boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::local_time();
 
    //free_project(project);
    free_recorder(_rec);
    free_eigen(engine_);  
    free_projectinfo(info);  
    cout<<_rec<<endl;  
    cout << "时间" << boost::posix_time::to_iso_string(ptEnd - ptStart) << endl;
    return GSL_SUCCESS;
}