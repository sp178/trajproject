#include "odeengin.h"
#include"modelbase.h"
#define SPDELETEARR(x){if(x) delete []x ;x=nullptr;};
#define SPDELETEDATA(x){if(x) delete x ;x=nullptr;};
msgqueue *generaMsgqueue(size_t _size)
{
    msgqueue *buffer_ = new msgqueue(_size);
    return buffer_;
}
int insertTomsgqueue(msgqueue *_queue, const function<int(int)> &_handel)
{
    _queue->push_back(_handel);
    return _queue->size();
};
int linker_add(model *_themodel)
{
    for (auto link_ : _themodel->_linker)
        _themodel->_data._in[get<0>(link_)] += _themodel->_models[get<1>(link_)]._data._out[get<2>(link_)];
    return 0;
};

int derive(engine *_eigen)
{
    _eigen->_project->_count++;
    return gsl_odeiv2_driver_apply_fixed_step(_eigen->_odecore._driver,
                                              &_eigen->_project->_time,
                                              _eigen->_project->_step,
                                              1,
                                              _eigen->_project->_x);
};
// typedef int (*spfunc)(int _msg, double _time, double _x, double _f,
//     double *_in, double *_out, double *_params);

int initial(engine *_eigen)
{
    _eigen->_project->_count = 0;
    for (auto &model_ : _eigen->_project->_models)
    {
        int stata_ = 0;
        model_._sys._database = model_._modelinfo->_database.data();
        model_._sys._setp = _eigen->_project->_step;
        model_._sys._stepcount = 0;
        stata_ = model_._func(SP_MSG_INITIAL, model_._dllmodel, &model_._sys);
        if (stata_)
            return stata_;
    }
    return 0;
};

int initialWithRandam(engine *_eigen)
{
    spfloat *_param = _eigen->_project->_param;
    for (auto &_dis : _eigen->_project->_distribute)
    {
        _param[get<1>(_dis)] =
            _param[get<1>(_dis)] * get<0>(_dis)._sampler1() + get<0>(_dis)._sampler0();
    }
    _eigen->_project->_count = 0;
    for (auto &model_ : _eigen->_project->_models)
    {
        int stata_ = 0;
        model_._sys._database = model_._modelinfo->_database.data();
        model_._sys._setp = _eigen->_project->_step;
        model_._sys._stepcount = 0;
        stata_ = model_._func(SP_MSG_INITIAL, model_._dllmodel, &model_._sys);
        if (stata_)
            return stata_;
    }
    return 0;
};

int load(engine *_eigen) //第一次读取
{
    for (auto &model_ : _eigen->_project->_models)
    {
        int stata_ = 0;
        model_._sys._x = model_._data._x;
        model_._sys._in = model_._data._in;
        model_._sys._out = model_._data._out;
        model_._sys._param = model_._data._param;
        model_._sys._xlen = model_._impl._xdim;
        model_._sys._inlen = model_._impl._indim;
        model_._sys._outlen = model_._impl._outdim;
        model_._sys._parmlen = model_._impl._paramdim;
        stata_ = model_._func(SP_MSG_LOAD,
                              &model_._dllmodel, &model_._sys);
        if (stata_)
            return stata_;
    }
    return 0;
};

int update(engine *_eigen)
{
    memset(_eigen->_project->_in, 0, sizeof(spfloat) * _eigen->_project->_indim);

    for (auto &model_ : _eigen->_project->_models)
    {
        linker_add(&model_);
        int stata_ = 0;
        stata_ = model_._func(SP_MSG_UPDATE,
                              model_._dllmodel, &model_._sys);
        model_._sys._stepcount = _eigen->_project->_count;
        if (stata_)
            return stata_;
    }

    return 0;
};
int start(engine *_eigen)
{
    _eigen->_project->_count = 0;
    for (auto &model_ : _eigen->_project->_models)
    {
        int stata_ = 0;
        stata_ = model_._func(SP_MSG_START,
                              model_._dllmodel, &model_._sys);
        model_._sys._stepcount = 0;
        if (stata_)
            return stata_;
    }
    return 0;
};
int stop(engine *_eigen)
{
    _eigen->_project->_count = 0;
    for (auto &model_ : _eigen->_project->_models)
    {
        int stata_ = 0;
        stata_ = model_._func(SP_MSG_STOP,
                              model_._dllmodel, &model_._sys);
        _eigen->_project->_count = 0;
        if (stata_)
            return stata_;
    }
    return 0;
};
engine *make_engine(spprojection *_projection)
{
    engine *engine_ = new engine();
    engine_->_project = _projection;
    engine_->_odecore._system = new gsl_odeiv2_system();
    engine_->_odecore._system->function = func;
    engine_->_odecore._system->jacobian = nullptr;
    engine_->_odecore._system->dimension = _projection->_xdim;
    engine_->_odecore._system->params = _projection;
    switch (_projection->_evetype)
    {
    case eveltype::rk1:
    {
        engine_->_odecore._driver = gsl_odeiv2_driver_alloc_y_new(engine_->_odecore._system,
                                                                  gsl_odeiv2_step_rk1,
                                                                  _projection->_step, 10, 0.1);
        break;
    }
    case eveltype::rk2:
    {
        engine_->_odecore._driver = gsl_odeiv2_driver_alloc_y_new(engine_->_odecore._system,
                                                                  gsl_odeiv2_step_rk2,
                                                                  _projection->_step, 10, 0.1);
        break;
    }
    case eveltype::rk4:
    {
        engine_->_odecore._driver = gsl_odeiv2_driver_alloc_y_new(engine_->_odecore._system,
                                                                  gsl_odeiv2_step_rk4,
                                                                  _projection->_step, 10, 0.1);
        break;
    }
    case eveltype::rk8:
    {
        engine_->_odecore._driver = gsl_odeiv2_driver_alloc_y_new(engine_->_odecore._system,
                                                                  gsl_odeiv2_step_rk8pd,
                                                                  _projection->_step, 10, 0.1);
        break;
    }
    default:
    {
        engine_->_odecore._driver = gsl_odeiv2_driver_alloc_y_new(engine_->_odecore._system,
                                                                  gsl_odeiv2_step_rk4,
                                                                  _projection->_step, 10, 0.1);
        break;
    }
    }
    engine_->_odecore._evolue = engine_->_odecore._driver->e;
    engine_->_odecore._control = engine_->_odecore._driver->c;
    return engine_;
}

int func(double t, const double *_y, double *_f, void *_param)
{
    spprojection *theproject_ = (spprojection *)_param;
    double *fbeg_ = _f;
    int status_ = 0;
    //数据传递过程中需要清零
    memset(theproject_->_in, 0, sizeof(spfloat) * theproject_->_indim);
    for (auto &model_ : theproject_->_models)
    {
        model_._sys._time = t;
        model_._sys._f = fbeg_;
        linker_add(&model_);
        status_ = model_._func(SP_MSG_DERIVE,
                               model_._dllmodel, &model_._sys);
        fbeg_ += model_._impl._xdim;
        if (status_)
            return status_;
    }
    return 0;
};
int jac(double t, const double y[], double *dfdy, double dfdt[], void *params)
{
    return 0;
}

void free_eigen(engine* _eigen)
{
    gsl_odeiv2_driver_free(_eigen->_odecore._driver);   //驱动
    //gsl_odeiv2_evolve_free(_eigen->_odecore._evolue);
    //gsl_odeiv2_control_free(_eigen->_odecore._control);
    //SPDELETEDATA(_eigen->_odecore._system);
    _eigen->_odecore._system->params = nullptr;
    free_project(_eigen->_project);
    SPDELETEDATA(_eigen);
}