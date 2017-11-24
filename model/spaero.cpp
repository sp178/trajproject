#include "spaero.h"
#include<string>
using namespace std;
SPEXPORT_WITH_NAME(aero, spaero);
#include"boost/locale.hpp"
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
	
	string clpath,cdpath,czpath,atmopath,
		mxpath,mypath,mzpath,
		mx5path, my5path, mz5path;
	ptree _table;
	ptree _null;
	std::ifstream t(m_database);
	std::string str_((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	std::string xmlstr_ = from_utf(str_, "GBK");
	std::istrstream xmlstream_(xmlstr_.c_str());
	boost::property_tree::read_xml(xmlstream_, _table);
	ptree tables = _table.get_child("database", _null);
	for (auto tabledata:tables)
	{
		try {
			if (tabledata.first.data() == string("tabledata"))
			{
				if (string("CL") == tabledata.second.get<string>("<xmlattr>.name",""))
					clpath = tabledata.second.get<string>("", "");
				if (string("CD") == tabledata.second.get<string>("<xmlattr>.name", ""))
					cdpath = tabledata.second.get<string>("", "");
				if (string("CZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
					czpath = tabledata.second.get<string>("", "");
				if (string("ATMOSPH") == tabledata.second.get<string>("<xmlattr>.name", ""))
					atmopath = tabledata.second.get<string>("", "");
				if (string("MX") == tabledata.second.get<string>("<xmlattr>.name", ""))
					mxpath = tabledata.second.get<string>("", "");
				if (string("MY") == tabledata.second.get<string>("<xmlattr>.name", ""))
					mypath = tabledata.second.get<string>("", "");
				if (string("MZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
					mzpath = tabledata.second.get<string>("", "");
				if (string("beta5_MX") == tabledata.second.get<string>("<xmlattr>.name", ""))
					mx5path = tabledata.second.get<string>("", "");
				if (string("beta5_MY") == tabledata.second.get<string>("<xmlattr>.name",""))
					my5path = tabledata.second.get<string>("", "");
				if (string("beta5_MZ") == tabledata.second.get<string>("<xmlattr>.name", ""))
					mz5path = tabledata.second.get<string>("","");
			}
		}
		catch (exception& e)
		{
			printf("%s:aero:\n", e.what());
			return -1;
		}		
	}
	m_interCL = makeInterData<2>(clpath, "返回段");
	m_interCD = makeInterData<2>(cdpath, "返回段");
	m_interCZ = makeInterData<2>(czpath, "返回段");
	m_interpho = makeInterData<1>(atmopath, "大气密度");
	m_interma = makeInterData<1>(atmopath, "音速");
	m_interMX = makeInterData<2>(mxpath, "返回段");
	m_interMY = makeInterData<2>(mypath, "返回段");
	m_interMZ = makeInterData<2>(mzpath, "返回段");
	m_interBeta5MX = makeInterData<2>(mx5path,"返回段");
	m_interBeta5MY = makeInterData<2>(my5path,"返回段");
	m_interBeta5MZ = makeInterData<2>(mz5path, "返回段");
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
	return 0;
};
