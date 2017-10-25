#include "recorder.h"
int setRecorderBuffer(recorder *_recorder, uint64_t _size)
{
    if (_size < 256)
    {
        _size = 256;
    }
    if (_recorder)
    {
        if (_recorder->_file)
        {
            return setvbuf(_recorder->_file, nullptr, _IOFBF, _size);
        }
        else
        {

            return -1;
        }
    }
    else
    {
        return -1;
    }
};
int recorderWriteSig(recorder *_recorder,
                     char *_data, uint64_t _length)
{
    uint64_t datalength_ = _recorder->_info._signum * _recorder->_info._datatype;
    if (_length != datalength_)
    {
        return -1;
    }
    else
    {
        memcpy(_recorder->_sigbuff, _data, _length);
        return 0;
    }
    return 0;
};

int recorderWriteMulti(recorder *_recorder,
                       char *_data, uint64_t _length)
{
    uint64_t size_ = _recorder->_info._mutinum * _recorder->_info._datatype;
    if (!_recorder->_step)
    {
        if (size_ != _length || !_recorder->_file)
            return -1;
        _recorder->_info._mutilength += _length;
        fwrite(_data, _length, 1, _recorder->_file);
        return 0;
    }
    if (0 == _recorder->_count % _recorder->_step)
    {
        if (size_ != _length || !_recorder->_file)
            return -1;
        _recorder->_info._mutilength += _length;
        fwrite(_data, _length, 1, _recorder->_file);
        _recorder->_count = 0;
    }
    _recorder->_count++;
    return 0;
};
//只存储一次的数据数量
//多次出现数据数量
recorder *createrRecorder(uint64_t _signum,
                          uint64_t _mutinum,
                          datatype _datatype,
                          const char *_path,
                          uint32_t _step)
{
    FILE *file_ = fopen(_path, "wb+");
    if (file_)
    {
        recorder *therecorder_ = new recorder();
        therecorder_->_info._signum = _signum;
        therecorder_->_info._datatype = _datatype;
        therecorder_->_info._mutinum = _mutinum;
        therecorder_->_info._mutilength = 0;
        therecorder_->_sigbuff = new char[_datatype * _signum];
        therecorder_->_file = file_;
        therecorder_->_count = 0;
        therecorder_->_step = _step;
        fwrite((char *)&therecorder_->_info, sizeof(datainfo), 1, file_);
        fflush(therecorder_->_file);
        return therecorder_;
    }
    else
    {
        return nullptr;
    }
};

int closeRecorder(recorder *_recorder)
{
    if (!_recorder)
        return -1;
    if (_recorder->_file)
    {
        fwrite(_recorder->_sigbuff, _recorder->_info._datatype * _recorder->_info._signum, 1, _recorder->_file);
        fflush(_recorder->_file);
        fseek(_recorder->_file, 0, SEEK_SET);
        // fsetpos(_recorder->_file, &_pos);
        //写文件头
        fflush(_recorder->_file);
        fwrite(&_recorder->_info, sizeof(datainfo),
               1,
               _recorder->_file);
        fflush(_recorder->_file);
        fclose(_recorder->_file);
        _recorder->_file = nullptr;
    }
    else
    {
        return -1;
    }
    return 0;
};
