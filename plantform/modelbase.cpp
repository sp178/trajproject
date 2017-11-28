#include "modelbase.h"
#include <iostream>
#include <codecvt>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#define SPDELETEARR(x){if(x) delete []x ;x=nullptr;};
#define SPDELETEDATA(x){if(x) delete x ;x=nullptr;};
using namespace boost::filesystem;
using namespace std;
typedef boost::variate_generator<boost::mt19937,
                                 boost::normal_distribution<double>>
    dissampler;
typedef boost::normal_distribution<double> distribution;
int _backfunc(int _msg, void *_model, sysinfo *_sys)
{
    printf("默认\n");
    return 0;
}
using namespace boost::property_tree;
void eraseStringHeadAndEnd(string &_string)
{
    uint32_t index = 0;
    for (auto word : _string)
    {
        if (word != '\t' && word != '\n' && word != ' ')
            break;
        index++;
    }
    _string.erase(_string.begin(), _string.begin() + index);
    index = 0;
    for (auto word = _string.rbegin(); word != _string.rend(); ++word)
    {
        if (*word != '\t' && *word != '\n' && *word != ' ')
            break;
        index++;
    }
    _string.erase(_string.end() - index, _string.end());
}
spindex findmodelindex(const string &_name, projectinfo *_themodel)
{
    spindex _index = 0;
    for (auto model_ : _themodel->_models)
    {
        if (_name == model_._modelname)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findoutindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_out_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
spindex findinindex(const string &_name, modelinfo *_themodel)
{
    spindex _index = 0;
    for (auto name_ : _themodel->_in_name)
    {
        if (_name == name_)
            return _index;
        ++_index;
    }
    return -1;
}
spprojection *make_project(projectinfo *_projectinfo)
{
    spprojection *theproject_ = new spprojection();
    theproject_->_models.resize(_projectinfo->_models.size());
    //第一步填写model _impl
    size_t index_ = 0, sizex_ = 0, sizein_ = 0, sizeout_ = 0, sizeparam_ = 0;
    double *begx_, *begin_, *begout_, *begparam_;
    for (auto &projectmodel : theproject_->_models)
    {
        projectmodel._impl._indim = _projectinfo->_models[index_]._in_name.size();
        projectmodel._impl._outdim = _projectinfo->_models[index_]._out_name.size();
        projectmodel._impl._xdim = _projectinfo->_models[index_]._x_name.size();
        projectmodel._impl._paramdim = _projectinfo->_models[index_]._param_name.size();
        projectmodel._models = theproject_->_models.data();
        projectmodel._modelinfo = &_projectinfo->_models[index_];
        index_++;
    }
    //第二部根据model _impl统计数据量
    for (auto &projectmodel : theproject_->_models)
    {
        sizex_ += projectmodel._impl._xdim;
        sizein_ += projectmodel._impl._indim;
        sizeout_ += projectmodel._impl._outdim;
        sizeparam_ += projectmodel._impl._paramdim;
    }
    //统计结果写入
    theproject_->_indim = sizein_;
    theproject_->_xdim = sizex_;
    theproject_->_outdim = sizeout_;
    theproject_->_paramdim = sizeparam_;
    //统一初始化内存
    theproject_->_x = new spfloat[sizex_];
    theproject_->_in = new spfloat[sizein_];
    theproject_->_out = new spfloat[sizeout_];
    theproject_->_param = new spfloat[sizeparam_];
    //分配给每个模型
    begx_ = theproject_->_x;
    begin_ = theproject_->_in;
    begout_ = theproject_->_out;
    begparam_ = theproject_->_param;
    for (auto &projectmodel : theproject_->_models)
    {
        projectmodel._data._x = begx_;
        projectmodel._data._in = begin_;
        projectmodel._data._out = begout_;
        projectmodel._data._param = begparam_;
        begx_ += projectmodel._impl._xdim;
        begin_ += projectmodel._impl._indim;
        begout_ += projectmodel._impl._outdim;
        begparam_ += projectmodel._impl._paramdim;
    }

    //构建连接器
    //采用输入连接(前置)(输入端口名称，输入模型名，输入模型输出端口名称)
    //vector<tuple<string, string, string>> _linker;;
    //采用输入连接(前置)(输入端口编号，输入模型编号，输入模型输出端口编号)
    //vector<tuple<spindex, spindex, spindex>> _linker;
    vector<modelinfo> &themodels = _projectinfo->_models;
    index_ = 0;
    for (auto &projectmodel : theproject_->_models)
    {
        for (auto &linker_ : themodels[index_]._linker)
        {
            const string &inname_ = std::get<0>(linker_);
            spindex inindex_ = findinindex(inname_, &themodels[index_]);
            if (inindex_ >= 0)
            {
                const string &modelname_ = std::get<1>(linker_);
                spindex modeloutindex_ = findmodelindex(modelname_, _projectinfo);
                if (modeloutindex_ >= 0 && modeloutindex_ < themodels.size())
                {
                    const string &outname_ = std::get<2>(linker_);
                    spindex outindex_ = findoutindex(outname_, &themodels[modeloutindex_]);
                    if (outindex_ >= 0)
                        projectmodel._linker.push_back(make_tuple(inindex_, modeloutindex_, outindex_));
                }
            }
        }
        index_++;
    }
    //读取定义dll
    index_ = 0;
    for (auto &projectmodel : theproject_->_models)
    {
        if (themodels[index_]._dllpath.empty() || themodels[index_]._funcname.empty())
            projectmodel._func = _backfunc;
        try
        {
            projectmodel.lib.load(themodels[index_]._dllpath);
        }
        catch (exception &_e)
        {
            std::cout<<_e.what()<<std::endl;
            projectmodel._func = _backfunc;
            index_++;
            continue;
        }
        if (projectmodel.lib.is_loaded())
        {
            try
            {
                if (projectmodel.lib.has(themodels[index_]._funcname))
                    projectmodel._func = projectmodel.lib.get<decltype(_backfunc)>(themodels[index_]._funcname);
                else
                    projectmodel._func = _backfunc;
            }
            catch (exception &_e)
            {
                std::cout << _e.what() << endl
                          << themodels[index_]._funcname << themodels[index_]._dllpath;
                projectmodel._func = _backfunc;
            }
        }
        index_++;
    }
    //默认
    theproject_->_evetype = ::rk1;
    theproject_->_step = 0.01;
    theproject_->_begtime = 0;
    theproject_->_endtime = 1000;
    return theproject_;
}
//读取param
int readparam(const ptree &_ptree, vector<string> &_param)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subparam") == param_.first.data())
        {
			string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _param.push_back(str);
        }
    }
    return _param.size();
};
//读取x
int readx(const ptree &_ptree, vector<string> &_x)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("substata") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _x.push_back(str);
        }
    }
    return _x.size();
};
//读取in
int readin(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    auto &_in = _model._in_name;
    auto &_linkers = _model._linker;
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subin") == param_.first.data())
        {
			string instr_ = param_.second.get<string>("<xmlattr>.name","名称未定义");
            _in.push_back(instr_);
            auto linkers = param_.second.get_child("");
            for (auto link : linkers)
            {
                if (string("link") == link.first)
                {
                    string outmodelstr_ = link.second.get<string>("<xmlattr>.model", "名称未定义");
                    string outname_ = link.second.get<string>("", "未定义");
                    eraseStringHeadAndEnd(instr_);
                    eraseStringHeadAndEnd(outmodelstr_);
                    eraseStringHeadAndEnd(outname_);
                    _linkers.push_back(make_tuple(instr_, outmodelstr_, outname_));
                }
            }
        }
    }
    return _in.size();
};
//读取out
int readout(const ptree &_ptree, vector<string> &_out)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("subout") == param_.first.data())
        {
            string str = param_.second.get<string>("<xmlattr>.name", "名称未定义");
            _out.push_back(str);
        }
    }
    return _out.size();
};
//读取dll路径
int readdll(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("path") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._dllpath = str;
        }
        if (string("name") == param_.first.data())
        {
            string str = param_.second.get<string>("", "");
            eraseStringHeadAndEnd(str);
            _model._funcname = (str);
        }
    }
    return 0;
}
//读取database路径
int readdatabase(const ptree &_ptree, modelinfo &_model)
{
    auto params_ = _ptree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    for (auto param_ : params_)
    {
        if (string("data") == param_.first.data())
        {
			string strdata = param_.second.get<string>("", "未定义");
            eraseStringHeadAndEnd(strdata);
            _model._database = strdata;
        }
    }
    return _model._database.size();
}
//得到一个model
modelinfo make_model(const ptree &_ptree)
{
    modelinfo themodel_;
    themodel_._modelname = _ptree.get<string>("<xmlattr>.name", "notdef");
    auto submodels = _ptree.get_child("");
    for (auto submodel : submodels)
    {
        if (string("param") == submodel.first.data())
            readparam(submodel.second, themodel_._param_name);
        if (string("stata") == submodel.first.data())
            readx(submodel.second, themodel_._x_name);
        if (string("out") == submodel.first.data())
            readout(submodel.second, themodel_._out_name);
        if (string("in") == submodel.first.data())
            readin(submodel.second, themodel_);
        if (string("function") == submodel.first.data())
            readdll(submodel.second, themodel_);
        if (string("database") == submodel.first.data())
            readdatabase(submodel.second, themodel_);
    }
    return themodel_;
};

projectinfo *readprojectfromxml(const string &_path)
{
    ptree xmltree;
    ptree pt_empty;
    projectinfo *theinfp_ = nullptr;
    try
    {
#ifdef USE_UTF_8
        std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
        read_xml(_path, xmltree, 0, utf8Locale);
#else
        read_xml(_path, xmltree);
#endif
    }
    catch (exception &e)
    {
        printf("%s", e.what());
    }
    theinfp_ = new projectinfo();
    const ptree _models = xmltree.get_child("tarjet");
    for (auto &_model : _models)
    {
        //模型标签
        if (string("model") == _model.first.data())
        {
            theinfp_->_models.push_back(make_model(_model.second));
        }
    }
    return theinfp_;
};
int getparamData(const ptree &_tree, model &_model, spprojection *_projection, spindex &_paramindex)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._param;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subparam") == param_.first.data())
        {
            auto subparams_ = param_.second.get_child("");
            boost::mt19937 seed0_(clock()), seed1_(clock());
            spfloat mean0_ = 0, sigma0_ = 0, mean1_ = 1, sigma1_ = 0;
            bool hasrand_ = false;
            for (auto &subparam_ : subparams_)
            {
                auto distris_ = subparam_.second.get_child("");
                if (string("data") == subparam_.first.data())
                {
                    spfloat data = subparam_.second.get<spfloat>("", 0);
                    *beg_ = data;
                    beg_++;
                    index_++;
                }
                if (string("random") == subparam_.first.data())
                {
                    if (string("on") == subparam_.second.get<string>("<xmlattr>.trig", "off"))
                    {
                        hasrand_ = true;
                        auto distris = subparam_.second.get_child("");
                        int ranknum_ = subparam_.second.get<int>("<xmlattr>.ranknum", 0);
                        switch (ranknum_)
                        {
                        case 0:
                        {
                            for (auto _distri : distris)
                            {
                                if (string("seed") == _distri.first.data())
                                {
                                    if (false == _distri.second.get<bool>("<xmlattr>.same", false))
                                    {
                                        seed0_.seed(clock());
                                    }
                                    else
                                    {
                                        int seednum_ = _distri.second.get<int>("", 0);
                                        seed0_.seed(seednum_);
                                    }
                                }
                                if (string("mean") == _distri.first.data())
                                {
                                    mean0_ = _distri.second.get<spfloat>("", 0);
                                }
                                if (string("sigma") == _distri.first.data())
                                {
                                    sigma0_ = _distri.second.get<spfloat>("", 0);
                                }
                            }
                            break;
                        }
                        case 1:
                        {
                            for (auto _distri : distris)
                            {
                                if (string("seed") == _distri.first.data())
                                {
                                    if (false == _distri.second.get<bool>("<xmlattr>.same", false))
                                    {
                                        seed1_.seed(clock());
                                    }
                                    else
                                    {
                                        int seednum_ = _distri.second.get<int>("", 0);
                                        seed1_.seed(seednum_);
                                    }
                                }
                                if (string("mean") == _distri.first.data())
                                {
                                    mean1_ = _distri.second.get<spfloat>("", 0);
                                }
                                if (string("sigma") == _distri.first.data())
                                {
                                    sigma1_ = _distri.second.get<spfloat>("", 0);
                                }
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                }
            }
            if (hasrand_)
            {
                distibuter dis_ = {dissampler(seed0_, distribution(mean0_, sigma0_)),
                                   dissampler(seed1_, distribution(mean1_, sigma1_))};
                _projection->_distribute.push_back(make_tuple(dis_,
                                                              _paramindex));
            }
            _paramindex++;
        }
    }
    return index_;
};
int getstataData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._x;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("substata") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}

int getoutData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._out;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subout") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}

int getinData(const ptree &_tree, model &_model)
{
    auto params_ = _tree.get_child("");
    //_param.push_back(make_pair(submodel.second.get<string>("<xmlattr>.name", "名称未定义"), vector<pair<char *, string>>()));
    spfloat *beg_ = _model._data._in;
    spindex index_ = 0;
    for (auto param_ : params_)
    {
        if (string("subin") == param_.first.data())
        {
            spfloat data = param_.second.get<spfloat>("", 0);
            *beg_ = data;
            beg_++;
            index_++;
        }
    }
    return index_;
}
int getevelType(const ptree &_tree, spprojection *_projection)
{
    auto rktype_ = _tree.get("<xmlattr>.name", "rk1");
    eraseStringHeadAndEnd(rktype_);
    if (string("rk1") == rktype_)
        _projection->_evetype = ::rk1;
    if (string("rk2") == rktype_)
        _projection->_evetype = ::rk2;
    if (string("rk4") == rktype_)
        _projection->_evetype = ::rk4;
    if (string("rk8") == rktype_)
        _projection->_evetype = ::rk8;
    auto rktypes_ = _tree.get_child("");
    for (auto rktype_ : rktypes_)
    {
        if (string("step") == rktype_.first)
            _projection->_step = rktype_.second.get<spfloat>("", 0.01);
        if (string("timebeg") == rktype_.first)
            _projection->_begtime = rktype_.second.get<spfloat>("", 0);
        if (string("timeend") == rktype_.first)
            _projection->_endtime = rktype_.second.get<spfloat>("", 1000);
    }
    return 0;
};
int getrecorder(const ptree &_tree, spprojection *_projection)
{
    auto &recorders_ = _tree.get_child("");
    string name_;
    string director_;
    uint32_t step_ = 0;
    for (auto &recorder_ : recorders_)
    {
        if (string("director") == recorder_.first)
        {
            //最后一个字符必须是/
            director_ = recorder_.second.get<string>("", "");
            eraseStringHeadAndEnd(director_);
        }
        if (string("name") == recorder_.first)
        {
            name_ = recorder_.second.get<string>("", "");
            eraseStringHeadAndEnd(director_);
        }
        if (string("step") == recorder_.first)
        {
            step_ = recorder_.second.get<uint32_t>("", 0);
        }
    }
    string path_ = director_ + name_;
    if (path_.empty())
        path_ = "./recorder.rec";
    else
    {
        try
        {
            if (!boost::filesystem::exists(director_))
                boost::filesystem::create_directories(director_);
        }
        catch (exception &_e)
        {
            path_ = "./recorder.rec";
        }
    }
    _projection->_recorderinfo = new recorderinfo();
    _projection->_recorderinfo->_path = path_;
    _projection->_recorderinfo->_step = step_;
    return 0;
};
int InitalDataFromXml(spprojection *_projection, const string &_path)
{
    if (!_projection)
        return -1;
    auto &modeldata = _projection->_models;
    ptree xmltree;
    ptree pt_empty;
#ifdef USE_UTF_8
    std::locale utf8Locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    read_xml(_path, xmltree, 0, utf8Locale);
#else
    read_xml(_path, xmltree);
#endif
    const ptree _models = xmltree.get_child("tarjet");
    spindex index_ = 0;
    spindex paramindex_ = 0;
    for (auto _model : _models)
    {
        //模型标签
        if (string("model") == _model.first.data())
        {
            auto &subodels = _model.second.get_child("");
            for (auto &submodel : subodels)
            {
                if (string("param") == submodel.first.data())
                {
                    getparamData(submodel.second, modeldata[index_], _projection, paramindex_);
                }
                if (string("stata") == submodel.first.data())
                    getstataData(submodel.second, modeldata[index_]);
                if (string("out") == submodel.first.data())
                    getoutData(submodel.second, modeldata[index_]);
                if (string("in") == submodel.first.data())
                    getinData(submodel.second, modeldata[index_]);
            }
            ++index_;
        }
        if (string("eveltype") == _model.first.data())
        {
            getevelType(_model.second, _projection);
        }
        if (string("recorder") == _model.first.data())
        {
            getrecorder(_model.second, _projection);
        }
    }
	return 0;
};

void free_project(spprojection * _project)
{
    SPDELETEARR(_project->_x);                //状态变量维度
    SPDELETEARR(_project->_in);               //输入维度
    SPDELETEARR(_project->_out);              //输出维度
    SPDELETEARR(_project->_param);            //参数维度
    SPDELETEDATA(_project->_recorderinfo);
    SPDELETEDATA(_project);
}
void free_projectinfo(projectinfo* _info)
{
    SPDELETEDATA(_info);
}
