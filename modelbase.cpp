#include "modelbase.h"

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
    return theproject_;
}