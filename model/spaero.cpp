#include "spaero.h"
#include<string>
using namespace std;
SPEXPORT_WITH_NAME(aero, spaero);

spaero::spaero()
{
};


spaero::~spaero()
{

};
#include<boost\filesystem.hpp>
int spaero::onInitial()
{
	string fullpath = boost::filesystem::initial_path<boost::filesystem::path>().string();
	cout << fullpath;
	using boost::property_tree::wptree;
	using std::wstring;
	using std::wstringstream;
	using std::vector;
	typedef wptree ptree;
	typedef wstring string;
	typedef wstringstream stringstream;
	
	string clpath,cdpath,czpath,atmopath,
		mxpath,mypath,mzpath,
		mx5path, my5path, mz5path;
	ptree _table;
	ptree _null;
	std::locale utf8Locale(std::locale(""), new std::codecvt_utf8<wchar_t>());
	read_xml(m_database, _table, 0, utf8Locale);
	ptree tables = _table.get_child(L"database", _null);
	for (auto tabledata:tables)
	{
		try {
			if (tabledata.first.data() == string(L"tabledata"))
			{
				if (string(L"CL") == tabledata.second.get<string>(L"<xmlattr>.name",L""))
					clpath = tabledata.second.get<string>(L"", L"");
				if (string(L"CD") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					cdpath = tabledata.second.get<string>(L"", L"");
				if (string(L"CZ") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					czpath = tabledata.second.get<string>(L"", L"");
				if (string(L"ATMOSPH") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					atmopath = tabledata.second.get<string>(L"", L"");
				if (string(L"MX") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					mxpath = tabledata.second.get<string>(L"", L"");
				if (string(L"MY") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					mypath = tabledata.second.get<string>(L"", L"");
				if (string(L"MZ") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					mzpath = tabledata.second.get<string>(L"", L"");
				if (string(L"beta5_MX") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					mx5path = tabledata.second.get<string>(L"", L"");
				if (string(L"beta5_MY") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					my5path = tabledata.second.get<string>(L"", L"");
				if (string(L"beta5_MZ") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
					mz5path = tabledata.second.get<string>(L"",L"");
			}
		}
		catch (exception& e)
		{
			printf("%s:aero:\n", e.what());
			return -1;
		}		
	}
	m_interCL = makeInterDataW<2>(clpath, L"返回段");
	m_interCD = makeInterDataW<2>(cdpath, L"返回段");
	m_interCZ = makeInterDataW<2>(czpath, L"返回段");
	m_interpho = makeInterDataW<1>(atmopath, L"大气密度");
	m_interma = makeInterDataW<1>(atmopath, L"音速");
	m_interMX = makeInterDataW<2>(mxpath, L"返回段");
	m_interMY = makeInterDataW<2>(mypath, L"返回段");
	m_interMZ = makeInterDataW<2>(mzpath, L"返回段");
	m_interBeta5MX = makeInterDataW<2>(mx5path,L"返回段");
	m_interBeta5MY = makeInterDataW<2>(my5path, L"返回段");
	m_interBeta5MZ = makeInterDataW<2>(mz5path, L"返回段");
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
