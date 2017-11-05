#include "stdafx.h"
#include "xmlDataReader.h"


xmlDataReader::xmlDataReader()
{
}

int xmlDataReader::ReadXmlData(const string & _tablename, const string & _path)
{
	ptree _table;
	ptree _null;
	try
	{
		read_xml(_path,_table);
		ptree tables = _table.get_child("",_null);
		if (tables == _null)
		{
			_error << "数据表不存在" << endl;
			throw exception(_error.str().c_str());
		}
		for (auto table:tables)
		{
			if (table.first.data() != string("table")) continue;
			if (_tablename != table.second.get<string>("<xmlattr>.name",""))
				throw exception("数据表名称定义错误");
			ptree _blocks = table.second.get_child("");
			for (auto _block:_blocks){
				if (_block.first.data() != string("block")) continue;
				string name = _block.second.get<string>("<xmlattr>.name", "");
				if (name.empty()) throw exception("数据表block名称没有定义");			
				if (m_blocks.end() != m_blocks.find(name)) {
					_error << "数据表名称重复" << name << endl;
					throw exception(_error.str().c_str());
				}
				//获得数据维度
				_BLOCK& theblock = m_blocks[name];
				theblock.m_demention = _block.second.get<int>("<xmlattr>.demention");
				auto dementions = _block.second.get_child("",_null);
				if (dementions == _null) throw exception("数据表维度信息获取错误");
				size_t index = 0;
				size_t indexdementionlength = 0;
				size_t indexdatalength = 1;
				theblock.m_cordial_demtion = new size_t[theblock.m_demention];
				vector<double> _tmpcords;
				vector<double> _tmdata;
				for (auto demention:dementions)
				{
					if (demention.first.data() == string("DEMTION"))
					{
						if (index >= theblock.m_demention) throw exception("维度搜索超界");
						int length=demention.second.get<int>("<xmlattr>.demention");
						stringstream _cordinate;
						_cordinate << demention.second.get<string>("");
						while (!_cordinate.eof())
						{
							double data;
							_cordinate >> data;
							if (_cordinate.fail()) break;
							_tmpcords.push_back(data);
						}
						theblock.m_cordial_demtion[index] = length;
						indexdementionlength += length;
						indexdatalength *= length;
						++index;
					}

				}	
				if (_tmpcords.size() < indexdementionlength) throw exception("维度数据索引和超界");
				ptree data =  dementions.get_child("Data");
				stringstream _data;
				_data << data.get<string>("");
				while (!_data.eof()){
					
					double tmp;
					_data >> tmp;
					if (_data.fail()) break;
					_tmdata.push_back(tmp);
				}
				if (_tmdata.size()< indexdatalength) throw exception("数据维度超界");
				theblock.m_cordials = new double[indexdementionlength];
				theblock.m_beg = new double[indexdatalength];
				memcpy(theblock.m_cordials, _tmpcords.data(),indexdementionlength*sizeof(double));
				memcpy(theblock.m_beg, _tmdata.data(), indexdatalength*sizeof(double));
			}
		}
	}
	catch (const std::exception& e)
	{
		return -1;
	}
	return 0;
}


size_t xmlDataReader::GetSize()
{
	return size_t(m_blocks.size());
}

_BLOCK* xmlDataReader::GetData(const string& name)
{
	auto theblock = m_blocks.find(name);
	if (m_blocks.end() != theblock) {
		auto& _data = theblock->second;
		return &_data;
	}
	else
		throw exception("无法获取数据表");
	
	
}

xmlDataReader::~xmlDataReader()
{
	for (auto& block : m_blocks)
		block.second.clear();
	m_blocks.clear();
}

const string & xmlDataReader::getError()
{
	// TODO: 在此处插入 return 语句
	return _error.str();
}
