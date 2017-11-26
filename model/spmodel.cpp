#include "spmodel.h"
using namespace std;
int spmodel::operator()(int _msg, sysinfo *_info)
{
    switch (_msg)
    {
    case SP_MSG_INITIAL:
    {
        m_sys = _info;
        m_x = _info->_x;
        m_in = _info->_in;
        m_out = _info->_out;
        m_params = _info->_param;
		m_database = _info->_database;
        return onInitial();
    }
    case SP_MSG_UPDATE:
    {
        return onUpdate();
    }
    case SP_MSG_DERIVE:
    {
        m_f = _info->_f;
        return onDerive();
    }
    case SP_MSG_CLEAR:
    {
        return onClear();
    }
    case SP_MSG_STOP:
    {
        return onStop();
    }
	case SP_MSG_START:
	{
		return onStart();
	}
    default:
        return -1;
    }
    return 0;
};
spmodel::spmodel()
{

};
spmodel::~spmodel()
{

};

void eraseStringHeadAndEnd(std::string &_string)
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