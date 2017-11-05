#pragma once
#include<string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>  
#include"META.hpp"
using namespace std;
using namespace boost::property_tree;
#include<map>
#include"utiltyDataBase.h"
class xmlDataReader
{
public:
	//描述数据块的应用
	xmlDataReader();
	int ReadXmlData(const string & _tablename,const string & _path);
	size_t GetSize();				//获取参数表个数
	_BLOCK* GetData(const string& name);				//获取数据
	~xmlDataReader();
	const string& getError();
protected:
	stringstream _error;
	map<string, _BLOCK> m_blocks;
};

