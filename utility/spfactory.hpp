#pragma once
#include "spinterplation.hpp"
#include<locale>
#include<codecvt>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <string>
#include <strstream>

template <unsigned _T>
BLOCK<_T> *makeCuBlock(const std::string& _path,
	const std::string& _tablename)
{
	using boost::property_tree::ptree;
	using boost::property_tree::wptree;
	using std::string;
	using std::stringstream;
	using std::vector;


#ifdef USE_UTF_8
	ptree _table;
	ptree _null;
	std::locale utf8Locale(std::locale("chs"), new std::codecvt_utf8<wchar_t>());
	read_xml(_path, _table, 0, utf8Locale);
#else
	read_xml(_path, _table);
#endif
	ptree tables = _table.get_child("", _null);
	for (auto table : tables)
	{
		if (table.first.data() != string("table"))
			continue;
		ptree _blocks = table.second.get_child("");
		for (auto _block : _blocks)
		{
			if (_block.first.data() != string("block"))
				continue;
			string name = _block.second.get<string>("<xmlattr>.name", "");
			if (string(_tablename) != name)
				continue;
			if (name.empty())
				return nullptr;
			//获得数据维度
			if (_T != _block.second.get<int>("<xmlattr>.demention", 0))
			{
				return nullptr;
			}
			BLOCK<_T> *theblock_ = new BLOCK<_T>();
			uint32_t index = 0;
			uint32_t indexdementionlength = 0;
			uint32_t indexdatalength = 1;
			vector<double> _tmpcords;
			vector<double> _tmdata;
			auto dementions = _block.second.get_child("");
			for (auto demention : dementions)
			{
				if (demention.first.data() == string("DEMTION"))
				{
					if (index >= _T)
					{
						delete theblock_;
						return nullptr;
					}
					int length = demention.second.get<int>("<xmlattr>.demention", 0);
					stringstream _cordinate;
					_cordinate << demention.second.get<string>("");
					while (!_cordinate.eof())
					{
						double data;
						_cordinate >> data;
						if (_cordinate.fail())
							break;
						_tmpcords.push_back(data);
					}
					theblock_->m_cordial_demtion[index] = length;
					indexdementionlength += length;
					indexdatalength *= length;
					++index;
				}
			}
			if (_tmpcords.size() < indexdementionlength)
			{
				delete theblock_;
				return nullptr;
			}
			ptree data = dementions.get_child("Data");
			stringstream _data;
			_data << data.get<string>("");
			while (!_data.eof())
			{
				double tmp;
				_data >> tmp;
				if (_data.fail())
					break;
				_tmdata.push_back(tmp);
			}
			if (_tmdata.size() < indexdatalength)
			{
				delete theblock_;
				return nullptr;
			}
			theblock_->m_cordials = new double[indexdementionlength];
			theblock_->m_beg = new double[indexdatalength];
			memcpy(theblock_->m_cordials, _tmpcords.data(),
				indexdementionlength * sizeof(double));
			memcpy(theblock_->m_beg, _tmdata.data(), indexdatalength * sizeof(double));
			return theblock_;
		}
	}

	return nullptr;
};

template <unsigned _T>
interdata<_T> *makeInterData(const std::string& _path,
	const std::string& _tablename)
{
	interdata<_T> *_inter = new interdata<_T>;
	_inter->_data = makeCuBlock<_T>(_path, _tablename);
	if (_inter->_data)
		_inter->_interRecod = new InterRecod<_T>;
	else
	{
		delete _inter;
		return nullptr;
	}
	return _inter;
};