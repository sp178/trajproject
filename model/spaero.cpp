#include "spaero.h"
#include<string>
using namespace std;
#include"boost/locale.hpp"
SPEXPORT_WITH_NAME(aero, spaero);
spaero::spaero()
{
};


spaero::~spaero()
{

};
#include<boost/filesystem.hpp>
int spaero::onInitial()
{
	using boost::property_tree::ptree;
	using std::string;
	using std::stringstream;
	using std::vector;
	using boost::locale::conv::from_utf;
	//typedef ptree ptree;
	//typedef string string;
	//typedef stringstream stringstream;
	
	string clpath,cdpath,czpath, aerodempath, speedpath,
		mxpath,mypath,mzpath,
		mx5path, my5path, mz5path;
	string clname, cdname, czname, aerodemname, speedname,
		mxname, myname, mzname,
		mx5name, my5name, mz5name;
	ptree _table;
	ptree _null;
	std::ifstream t(m_database);
	std::string str_((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	std::string xmlstr_ = from_utf(str_, _encoding_);
	std::istringstream xmlstream_(xmlstr_.c_str());
	boost::property_tree::read_xml(xmlstream_, _table);
	ptree tables = _table.get_child("database", _null);
	for (auto tabledata:tables)
	{
		try {
			if (tabledata.first.data() == string("tabledata"))
			{
				if (string("CL") == tabledata.second.get<string>("<xmlattr>.name",""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						clpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						clname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("CD") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						cdpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						cdname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("CZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						czpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						czname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("aero") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						aerodempath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						aerodemname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("speed") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						speedpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						speedname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("MX") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						mxpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						mxname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("MY") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						mypath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						myname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("MZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						mzpath = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						mzname = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("beta5_MX") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						mx5path = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						mx5name = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("beta5_MY") == tabledata.second.get<string>("<xmlattr>.name",""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						my5path = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						my5name = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
				if (string("beta5_MZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
				{
					auto& pathtree_ = tabledata.second.get_child("path", _null);
					auto& blocktree_ = tabledata.second.get_child("block", _null);
					if (pathtree_ != _null)
					{
						mz5path = pathtree_.get<string>("", "");
					}
					if (blocktree_ != _null)
					{
						mz5name = blocktree_.get<string>("<xmlattr>.name", "");
					}
				}
			}
		}
		catch (exception& e)
		{
			printf("%s:aero:\n", e.what());
			return -1;
		}		
	}

	eraseStringHeadAndEnd(clpath);
	eraseStringHeadAndEnd(cdpath);
	eraseStringHeadAndEnd(czpath);
	eraseStringHeadAndEnd(aerodempath);
	eraseStringHeadAndEnd(speedpath);
	eraseStringHeadAndEnd(mxpath);
	eraseStringHeadAndEnd(mypath);
	eraseStringHeadAndEnd(mzpath);
	eraseStringHeadAndEnd(mx5path);
	eraseStringHeadAndEnd(my5path);
	eraseStringHeadAndEnd(mz5path);
	eraseStringHeadAndEnd(clname);
	eraseStringHeadAndEnd(cdname);
	eraseStringHeadAndEnd(czname);
	eraseStringHeadAndEnd(aerodemname);
	eraseStringHeadAndEnd(speedname);
	eraseStringHeadAndEnd(mxname);
	eraseStringHeadAndEnd(myname);
	eraseStringHeadAndEnd(mzname);
	eraseStringHeadAndEnd(mx5name);
	eraseStringHeadAndEnd(my5name);
	eraseStringHeadAndEnd(mz5name);
	m_interCL = makeInterData<2>(clpath, clname);
	m_interCD = makeInterData<2>(cdpath, cdname);
	m_interCZ = makeInterData<2>(czpath, czname);
	m_interpho = makeInterData<1>(aerodempath, aerodemname);
	m_interma = makeInterData<1>(speedpath, speedname);
	m_interMX = makeInterData<2>(mxpath, mxname);
	m_interMY = makeInterData<2>(mypath, myname);
	m_interMZ = makeInterData<2>(mzpath, mzname);
	m_interBeta5MX = makeInterData<2>(mx5path,mx5name);
	m_interBeta5MY = makeInterData<2>(my5path,my5name);
	m_interBeta5MZ = makeInterData<2>(mz5path, mz5name);
	return 0;
};

int spaero::onClear()
{
	return 0;
};

int spaero::onUpdate()
{
	memcpy(&h, m_in, sizeof(aeroIn));
	if (V*h < 10) return 0;
	if (V > 700)
		turnR = 0;
	else
		turnR = V*V*Mass / L;

	double rho = 0.5*interplate<1>(m_interpho->_data, &h, m_interpho->_interRecod)*V*V*m_deltaRho*m_Serf;
	double _betaMX, _betaMY, _betaMZ, _aerobeta;
	ma = V / interplate<1>(m_interma->_data, &h, m_interma->_interRecod);
	m_interAreo[0] = alpha; m_interAreo[1] = ma;
	//testD = -interplate<2>(cpuCD, m_interAreo, &cpuCDrecord)*rho*m_Serf;
	D = -interplate<2>(m_interCD->_data, m_interAreo, m_interCD->_interRecod)*rho*m_deltaD;
	L = interplate<2>(m_interCL->_data, m_interAreo, m_interCL->_interRecod)*rho*m_deltaL;
	Z = interplate<2>(m_interCZ->_data, m_interAreo, m_interCZ->_interRecod)*rho;

	_aerobeta = sgn(beta)*fabs(beta) > 5 ? 5 : beta;
	_betaMX = beta / 5 * interplate<2>(m_interBeta5MX->_data, m_interAreo, m_interBeta5MX->_interRecod);
	_betaMY = beta / 5 * interplate<2>(m_interBeta5MY->_data, m_interAreo, m_interBeta5MY->_interRecod);
	_betaMZ = beta / 5 * interplate<2>(m_interBeta5MZ->_data, m_interAreo, m_interBeta5MZ->_interRecod);
	MX = (interplate<2>(m_interMX->_data, m_interAreo, m_interMX->_interRecod) + _betaMX)*rho* m_Lengthf;
	MY = (interplate<2>(m_interMY->_data, m_interAreo, m_interMY->_interRecod) + _betaMY)*rho* m_Lengthf;
	MZ = (interplate<2>(m_interMZ->_data, m_interAreo, m_interMZ->_interRecod) + _betaMZ)*rho* m_Lengthf;
	CLD = fabs(L / D);
	memcpy(m_out, &D, sizeof(aeroout));
	return 0;
};

int spaero::onStop()
{
	return 0;
};

int spaero::onDerive()
{
	memcpy(&h, m_in, sizeof(aeroIn));
	if (V*h < 10) return 0;
	double rho = 0.5*interplate<1>(m_interpho->_data, &h, m_interpho->_interRecod)*V*V*m_deltaRho*m_Serf;
	double _betaMX, _betaMY, _betaMZ, _aerobeta;
	ma = V / interplate<1>(m_interma->_data, &h, m_interma->_interRecod);
	m_interAreo[0] = alpha; m_interAreo[1] = ma;
	//testD = -interplate<2>(cpuCD, m_interAreo, &cpuCDrecord)*rho*m_Serf;
	D = -interplate<2>(m_interCD->_data, m_interAreo, m_interCD->_interRecod)*rho*m_deltaD;
	L = interplate<2>(m_interCL->_data, m_interAreo, m_interCL->_interRecod)*rho*m_deltaL;
	Z = interplate<2>(m_interCZ->_data, m_interAreo, m_interCZ->_interRecod)*rho;

	_aerobeta = sgn(beta)*fabs(beta) > 5 ? 5 : beta;
	_betaMX = beta / 5 * interplate<2>(m_interBeta5MX->_data, m_interAreo, m_interBeta5MX->_interRecod);
	_betaMY = beta / 5 * interplate<2>(m_interBeta5MY->_data, m_interAreo, m_interBeta5MY->_interRecod);
	_betaMZ = beta / 5 * interplate<2>(m_interBeta5MZ->_data, m_interAreo, m_interBeta5MZ->_interRecod);
	MX = (interplate<2>(m_interMX->_data, m_interAreo, m_interMX->_interRecod) + _betaMX)*rho* m_Lengthf;
	MY = (interplate<2>(m_interMY->_data, m_interAreo, m_interMY->_interRecod) + _betaMY)*rho* m_Lengthf;
	MZ = (interplate<2>(m_interMZ->_data, m_interAreo, m_interMZ->_interRecod) + _betaMZ)*rho* m_Lengthf;
	CLD = fabs(L / D);
	memcpy(m_out, &D, sizeof(aeroout));
	return 0;
}
int spaero::onStart()
{
	m_Serf = m_params[0];
	m_Lengthf = m_params[1];
	m_deltaL = m_params[2];
	m_deltaD = m_params[3];
	m_deltaRho = m_params[4];
	alpha = 45;
	turnR = 0;
	memset(&D,0,sizeof(aeroout));
	return 0;
};
