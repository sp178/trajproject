#include"..\utility\spfactory.hpp"
#include "spguidance.h"
#include<time.h>
SPEXPORT_WITH_NAME(guidance, spguidance)
bool gtest = true;
double gspeed = 0;
bool isuse = false;
double g_dsigma = 0;
//判断地球上一个点是否在一个区域中
//并且需要判断是否已经处于边界区域
int isInACircle()
{
	return 0;
}
spfloat spguidance::choseScale(spfloat _V)
{
	if (_V > 1200)
		return m_turnScaleHigh;
	else if (_V < 600)
		return m_turnScaleLow;
	else
		return m_turnScaleLow + (_V - 600) / 2400 * (m_turnScaleHigh - m_turnScaleLow);
}
void spguidance::rangeOfAzmaangle(spfloat _V, spfloat & _up, spfloat & _down)
{
	if (_V > 1700)
		_up = 0.5;
	else if (_V < 700)
		_up = 0.01;
	else
		_up = 0.01 + (_V - 700) / 1000 * 0.49;
	_down = -1 * _up;
}
void spguidance::findCloseastSigma(numtest * _cuoutdata, uint32_t& _negindex, uint32_t& _psiindex)
{
	spfloat _close;
	uint32_t chose;
	_negindex = 0;
	_psiindex = BLOCK_NUM * THREAD_NUM / 2;
	//寻找最接近负倾侧角
	_close = fabs(_cuoutdata[0]._stogoLeft);
	for (uint32_t _index1 = 0; _index1 < BLOCK_NUM * THREAD_NUM / 2; ++_index1) {
		if (fabs(_cuoutdata[_index1]._stogoLeft) < _close)
		{
			_close = fabs(_cuoutdata[_index1]._stogoLeft);
			_negindex = _index1;
		}
	}
	//寻找最接近正倾侧角
	_close = fabs(_cuoutdata[BLOCK_NUM * THREAD_NUM / 2]._stogoLeft);
	for (uint32_t _index1 = BLOCK_NUM * THREAD_NUM / 2; _index1 < BLOCK_NUM * THREAD_NUM; ++_index1) {
		if (fabs(_cuoutdata[_index1]._stogoLeft) < _close)
		{
			_close = fabs(_cuoutdata[_index1]._stogoLeft);
			_psiindex = _index1;
		}
	}
	//if (m_crosscuidance._usePositive)
	//{
	//	_negindex = _psiindex - BLOCK_NUM * THREAD_NUM / 2;
	//}
	//else
	//{
	//	_psiindex = _negindex + BLOCK_NUM * THREAD_NUM / 2;
	//}
}
spfloat spguidance::choseBeginSignOfSigma(numtest * _cuoutdata, uint32_t & _chose)
{
	//比对正负倾侧角最小值中横程最小的
	if (fabs(_cuoutdata[m_crosscuidance._minneg]._croseLeft) < fabs(_cuoutdata[m_crosscuidance._minposi]._croseLeft))
	{
		_chose = m_crosscuidance._minneg;
		m_crosscuidance._usePositive = false;
	}
	else
	{
		_chose = m_crosscuidance._minposi;
		m_crosscuidance._usePositive = true;
	}
	return  m_sigmasearchbeg + spfloat(_chose)*m_sigmasearchLength / (BLOCK_NUM * THREAD_NUM);
}
spfloat spguidance::choseSignOfSigma(numtest * _cuoutdata, uint32_t & _chose)
{
	//总航程
	spfloat _total = fabs(_cuoutdata[m_crosscuidance._minneg]._croseLeft) + fabs(_cuoutdata[m_crosscuidance._minposi]._croseLeft);
	//设定阈值当横向不足时固定倾侧角符号
	//判断是否存在可行解
	//判断横向偏差有正负之分
	//预测横向偏差变化趋势


	//使用最大边界横向偏差
	if (!m_crosscuidance._usePositive)//如果选用负倾侧角
	{
		m_crosscuidance._crossrate = fabs((_cuoutdata[0]._croseLeft) / (_cuoutdata[BLOCK_NUM * THREAD_NUM - 1]._croseLeft));
		if (m_crosscuidance._crossrate>choseScale(V))
		{
			m_crosscuidance._usePositive = true;
			_chose = m_crosscuidance._minposi;
		}
		_chose = m_crosscuidance._minneg;
	}
	else
	{
		m_crosscuidance._crossrate = fabs((_cuoutdata[BLOCK_NUM * THREAD_NUM - 1]._croseLeft) / (_cuoutdata[0]._croseLeft));
		if (m_crosscuidance._crossrate>choseScale(V))
		{
			m_crosscuidance._usePositive = false;
			_chose = m_crosscuidance._minneg;
		}
		_chose = m_crosscuidance._minposi;
	}
	return  m_sigmasearchbeg + spfloat(_chose)*m_sigmasearchLength / (BLOCK_NUM * THREAD_NUM);
}


spguidance::spguidance()
{
	m_cuoutdata = new numtest[BLOCK_NUM*THREAD_NUM];
	m_cuoutdataTmp = new numtest[BLOCK_NUM*THREAD_NUM];
}


spguidance::~spguidance()
{
}

int spguidance::onInitial()
{
	using boost::property_tree::wptree;
	using std::wstring;
	using std::wstringstream;
	using std::vector;
	typedef wptree ptree;
	typedef wstring string;
	typedef wstringstream stringstream;
	string  clpath, cdpath, czpath, atmopath,
		mxpath, mypath, mzpath,
		mx5path, my5path, mz5path;
	ptree _table;
	ptree _null;
	std::locale utf8Locale(std::locale(""), new std::codecvt_utf8<wchar_t>());
	read_xml(m_database, _table, 0, utf8Locale);
	ptree tables = _table.get_child(L"database", _null);

	for (auto tabledata : tables)
	{
		if (tabledata.first.data() == string(L"tabledata"))
		{
		if (string(L"CL") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
			clpath = tabledata.second.get<string>(L"", L"");
		if (string(L"CD") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
			cdpath = tabledata.second.get<string>(L"", L"");
		if (string(L"CZ") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
			czpath = tabledata.second.get<string>(L"", L"");
		if (string(L"ATMOSPH") == tabledata.second.get<string>(L"<xmlattr>.name", L""))
			atmopath = tabledata.second.get<string>(L"", L"");
		}
	}
	m_interCL = makeBlockW<2>(clpath.c_str(), L"返回段");
	m_interCD = makeBlockW<2>(cdpath.c_str(), L"返回段");
	m_interCZ = makeBlockW<2>(czpath.c_str(), L"返回段");
	m_interpho= makeBlockW<1>(atmopath.c_str(), L"大气密度");
	m_interma = makeBlockW<1>(atmopath.c_str(), L"音速");
	m_cudafunc.initialCudaData(m_interCD, m_interCL, m_interCZ, m_interpho, m_interma);
	m_cudafuncbac.initialCudaDataConst(m_interCD, m_interCL, m_interCZ, m_interpho, m_interma);
	m_cpufunc.initialData(m_interCD, m_interCL, m_interCZ, m_interpho, m_interma);
	return 0;
}

int spguidance::onClear()
{
	return 0;
}
int spguidance::guidance()
{
	m_nowtraj.V = V;
	m_nowtraj.chi = chi;
	m_nowtraj.gamma = gamma;
	m_nowtraj.lambda = lambda;
	m_nowtraj.miu = miu;
	m_nowtraj.r = r;
	m_nowtraj.lanuchangle = lanuchAngle(m_targetlongt, m_targetlat, lambda, miu);// m_lunchAngle; //当前点发射方位角
	m_nowtraj.rengeS = length(m_targetlongt, m_targetlat, lambda, miu) *m_lengthScale;  //m_x[0] * m_lengthScale;//		//剩余航程
	if (0 != m_cudafunc.TrajCaclCUDA(&m_nowtraj, (spfloat*)m_cuoutdata, m_caclstep, m_sigmasearchbeg, m_sigmasearchLength))
	{
		return -1;
	}
	fwrite(m_cuoutdata, sizeof(numtest), 512, m_file);
	return 0;
}
bool use = true;
int spguidance::onUpdate()
{
	memcpy(&r, m_in, sizeof(guidanceIn));
	//alpha = 15;
	//beta = 0;
	//sigma = 0;//getSigma(V, deg2rad(-85), 3000);
	//memcpy(m_out, &Energey, sizeof(guidanceOut));
	//return 0;
	//if (use)
	//{
	//	m_nowtraj.V = V;
	//	m_nowtraj.chi = chi;
	//	m_nowtraj.gamma = gamma;
	//	m_nowtraj.lambda = lambda;
	//	m_nowtraj.miu = miu;
	//	m_nowtraj.r = r;
	//	m_nowtraj.lanuchangle = lanuchAngle(m_targetlongt, m_targetlat, lambda, miu);// m_lunchAngle; //当前点发射方位角
	//	m_nowtraj.rengeS = length(m_targetlongt, m_targetlat, lambda, miu) *m_lengthScale;  //m_x[0] * m_lengthScale;//		//剩余航程
	//	if (0 != m_cudafunc.TrajCaclCUDA(&m_nowtraj, (spfloat*)m_cuoutdata, 0.1, m_sigmasearchbeg, m_sigmasearchLength))
	//	{
	//		return -1;
	//	}
	//	fwrite(m_cuoutdata, sizeof(numtest), 512, m_file);
	//	use = false;
	//}

	//return 0;
	if (!m_dataInitial)
	{

		m_lunchAngle = lanuchAngle(m_targetlongt, m_targetlat, lambda, miu);
		m_beforeAzuAngle = rad2deg(m_lunchAngle - chi);
		m_dataInitial = true;
		m_beglongt = lambda;
		m_beglat = miu;
		m_isfirstPredicForEnerg = true;
		m_crosscuidance._usePositive = true;
		m_beforeGamma = gamma;
		m_deltaSigma = 0;
		m_deltaEngerg = 0;
		m_beforeCross = 0;		//之前的侧向误差
		m_x[0] = deg2rad(35);;		//角度
	}
	spfloat _deltaGamma = (gamma - m_beforeGamma) / m_sys->_setp;
	m_beforeGamma = gamma;
	//能量值
	Energey = getE(V, h);
	/*direcLeft = m_x[0];*/
	//航向角偏差
	outAzmaangle = rad2deg(m_lunchAngle - chi);
	outDeltaAzu = (outAzmaangle - m_beforeAzuAngle) / m_sys->_setp;
	crossLeft = getCrose(m_targetlongt, m_targetlat, m_beglongt, m_beglat, lambda, miu);
	m_beforeAzuAngle = outAzmaangle;

	if (0==(m_sys->_stepcount%m_itercycle))
	{
		//选择的倾侧角 航程最接近的正倾侧角 航程最接近的负倾侧角
		if (V < 5000) m_itercycle = 500;
		if (V < 1400) m_itercycle = 100;
		m_nowtraj.V = V;
		m_nowtraj.chi = chi;
		m_nowtraj.gamma = gamma;
		m_nowtraj.lambda = lambda;
		m_nowtraj.miu = miu;
		m_nowtraj.r = r;
		m_nowtraj.lanuchangle = lanuchAngle(m_targetlongt, m_targetlat, lambda, miu);// m_lunchAngle; //当前点发射方位角
		m_nowtraj.rengeS = length(m_targetlongt, m_targetlat, lambda, miu) *m_lengthScale;  //m_x[0] * m_lengthScale;//		//剩余航程
																							//设置不同时间运行步长
		if (V < 400)  m_caclstep = 0.1;
		if (V < 800) m_caclstep = 0.2;
		if (V < 1200) m_caclstep = 0.5;
		else m_caclstep = 1;
		clock_t _beg, _end;
		_beg = clock();
		m_recordTime = m_sys->_time;//获取当前时间
								 //启动cuda进行全数据求解
		if (0 != m_cudafunc.TrajCaclCUDA(&m_nowtraj, (spfloat*)m_cuoutdata, m_caclstep, m_sigmasearchbeg, m_sigmasearchLength))
		{
			return -1;
		}
		//if (0 != m_cpufunc.TrajCacl(&m_nowtraj, (spfloat*)m_cuoutdata, m_caclstep, m_sigmasearchbeg, m_sigmasearchLength))
		//{
		//	return -1;
		//}

		//if (0 != m_cudafuncbac.TrajCaclCUDA(&m_nowtraj, (spfloat*)m_cuoutdata, m_caclstep, m_sigmasearchbeg, m_sigmasearchLength))
		//{
		//	return -1;
		//}	
		//fwrite(m_cuoutdata, sizeof(numtest), 512, m_filebac);
		//纵向制导(求解正负两项倾侧角)
		findCloseastSigma(m_cuoutdata, m_crosscuidance._minneg, m_crosscuidance._minposi);
		//侧向制导
		//第一次制导需要进行倾侧角符号判断
		if (m_isfirstPredic)
		{
			m_sigmaBeg = choseBeginSignOfSigma(m_cuoutdata, m_crosscuidance._chose);
			m_crosscuidance._beforeSng = m_crosscuidance._usePositive;
			m_isfirstPredic = false;
		}
		//比例校正
		if (false)
		{
			if (!m_useCrossMethod)
			{
				m_sigmaBeg = choseSignOfSigma(m_cuoutdata, m_crosscuidance._chose);

				//发生第一次反转
				//if (m_crosscuidance._beforeSng != m_crosscuidance._usePositive)
				//{
				//	m_useCrossMethod = true;
				//	//获取当前距离中线距离
				//	m_crosscuidance._beforeCrossRange=getCrose(m_targetlongt, m_targetlat, m_beglongt, m_beglat, lambda, miu);		
				//}
				//m_crosscuidance._beforeSng = m_crosscuidance._usePositive;
			}
			else
			{
				double _nowCrossRange = getCrose(m_targetlongt, m_targetlat, m_beglongt, m_beglat, lambda, miu);
				//符号相反
				if (_nowCrossRange*m_crosscuidance._beforeCrossRange < 0)
				{
					if (fabs(_nowCrossRange / m_crosscuidance._beforeCrossRange) >(1 / choseScale(V)))
					{
						if (_nowCrossRange > 0)
						{
							m_crosscuidance._chose = m_crosscuidance._minneg;
							m_crosscuidance._usePositive = false;
						}
						else {
							m_crosscuidance._chose = m_crosscuidance._minposi;
							m_crosscuidance._usePositive = true;
						}
						m_crosscuidance._beforeCrossRange = _nowCrossRange;
					}
				}
				if (m_crosscuidance._usePositive)
					m_crosscuidance._chose = m_crosscuidance._minposi;
				else
					m_crosscuidance._chose = m_crosscuidance._minneg;
				m_sigmaBeg = m_sigmasearchbeg + spfloat(m_crosscuidance._chose)*m_sigmasearchLength / (BLOCK_NUM * THREAD_NUM);
			}
		}



		//侧向制导航向角度



		_end = clock();
		outCaclTime = (_end - _beg);
		////自定义输出
		//for (size_t index = 0;index<BLOCK_NUM*THREAD_NUM;++index)
		//{
		//	m_outfilelont << m_cuoutdata[index]._longt << " ";
		//	m_outfilelat << m_cuoutdata[index]._lat << " ";
		//}
		//m_outfilelat << endl;
		//m_outfilelont << endl;
		gspeed = V;
	}
	//侧向制导(航向角校正)
	if (false)
	{
		spfloat _upAngle, _downAngle;
		rangeOfAzmaangle(V, _upAngle, _downAngle);
		if ((outAzmaangle + outDeltaAzu*0.5) > _upAngle)
		{
			m_crosscuidance._chose = m_crosscuidance._minneg;
			m_crosscuidance._usePositive = false;
		}
		else if ((outAzmaangle + outDeltaAzu*0.5) < _downAngle)
		{
			m_crosscuidance._chose = m_crosscuidance._minposi;
			m_crosscuidance._usePositive = true;
		}
		else
		{
			if (m_crosscuidance._usePositive)
			{
				m_crosscuidance._chose = m_crosscuidance._minposi;
				m_crosscuidance._usePositive = true;
			}
			else
			{
				m_crosscuidance._chose = m_crosscuidance._minneg;
				m_crosscuidance._usePositive = false;
			}
			//m_useCrossMethod = true;
		}
		m_sigmaBeg = m_sigmasearchbeg + spfloat(m_crosscuidance._chose)*m_sigmasearchLength / (BLOCK_NUM * THREAD_NUM);
	}
	//横向偏差校正
	if (false)
	{
		spfloat R = length(m_targetlongt, m_targetlat, lambda, miu);
		spfloat _cross = asin(sin(R)*sin(deg2rad(outAzmaangle)));
		spfloat _upAngle, _downAngle;
		rangeOfAzmaangle(V, _upAngle, _downAngle);
		_upAngle = deg2rad(_upAngle);
		_downAngle = deg2rad(_downAngle);
		if (_cross > _upAngle)
		{
			m_crosscuidance._chose = m_crosscuidance._minneg;
			m_crosscuidance._usePositive = false;
		}
		else if (_cross<_downAngle)
		{
			m_crosscuidance._chose = m_crosscuidance._minposi;
			m_crosscuidance._usePositive = true;
		}
		else
		{
			if (m_crosscuidance._usePositive)
			{
				m_crosscuidance._chose = m_crosscuidance._minposi;
				m_crosscuidance._usePositive = true;
			}
			else
			{
				m_crosscuidance._chose = m_crosscuidance._minneg;
				m_crosscuidance._usePositive = false;
			}
		}
		m_sigmaBeg = m_sigmasearchbeg + spfloat(m_crosscuidance._chose)*m_sigmasearchLength / (BLOCK_NUM * THREAD_NUM);
	}
	//比例反转
	m_sigmaBeg = choseSignOfSigma(m_cuoutdata, m_crosscuidance._chose);
	//能量矫正
	if (true)
	{
		//在第一次预测是记录而不更新比例参数
		if (m_isfirstPredicForEnerg)
		{
			if (fabs(m_cuoutdata[0]._engerg - m_cuoutdata[BLOCK_NUM*THREAD_NUM / 2 - 1]._engerg) > 200)
			{
				m_beforeStogo = length(m_targetlongt, m_targetlat, lambda, miu);
				m_recordTime = m_sys->_time;
				m_nextRecordTime = m_recordTime + RECORDTIME;
				memcpy(m_cuoutdataTmp, m_cuoutdata, sizeof(numtest)*BLOCK_NUM*THREAD_NUM);
				m_isfirstPredicForEnerg = false;
				m_beforeEnergy = getE(V, h);
				m_deltaEngerg = 0;
				m_choseNumBefore = m_crosscuidance._chose;
			}
		}
		else if (fabs(m_nextRecordTime - m_recordTime)<1E-4)
		{
			if (
				((m_crosscuidance._chose >= BLOCK_NUM*THREAD_NUM / 2) && (m_choseNumBefore >= BLOCK_NUM*THREAD_NUM / 2)) ||
				((m_crosscuidance._chose < BLOCK_NUM*THREAD_NUM / 2) && (m_choseNumBefore < BLOCK_NUM*THREAD_NUM / 2))
				) {
				////判断当前能量值和之前预测能量最接近的
				//uint32_t _closeChose = findCloseastEnerg(m_cuoutdataTmp, Energey, m_choseNumBefore >= BLOCK_NUM*THREAD_NUM/2);
				////获取之前预测最接近项航程与能量最接近项航程差距
				//double _choseStogo = m_beforeStogo - m_cuoutdataTmp[m_choseNumBefore]._stogoLeft;
				//double _closeStogo = m_beforeStogo - m_cuoutdataTmp[_closeChose]._stogoLeft;
				m_lengthScale = (m_beforeEnergy - Energey) / (m_beforeEnergy - m_cuoutdataTmp[m_choseNumBefore]._engerg);
				m_deltaAzumAngle = (m_cuoutdataTmp[m_choseNumBefore]._azumangle - (chi - m_nowtraj.lanuchangle));
				if (m_lengthScale< 1.02&&m_lengthScale>0.98)
				{
					m_lengthScale = 1;
				}
				else if (m_lengthScale>1.05)
				{
					//m_lengthScale = pow(m_lengthScale, 1.3);
				}
				m_deltaSigma = 5 * ((m_lengthScale - 1) + 0 * deg2rad(m_deltaAzumAngle));

			}
			m_choseNumBefore = m_crosscuidance._chose;
			m_beforeEnergy = getE(V, h);
			m_beforeStogo = length(m_targetlongt, m_targetlat, lambda, miu);
			memcpy(m_cuoutdataTmp, m_cuoutdata, sizeof(numtest)*BLOCK_NUM*THREAD_NUM);
			m_nextRecordTime += RECORDTIME;
		}
		if (V < 700)
		{
			m_lengthScale = 1;
			m_nextRecordTime = 0;
		}

	}
	m_sigmaBeg = sgn(m_sigmaBeg)*(fabs(m_sigmaBeg) - 2 * (m_lengthScale - 1));
	if (m_sigmaBeg > 0) {
		m_sigmaBeg = m_sigmaBeg > deg2rad(85) ? deg2rad(85) : m_sigmaBeg;
	}
	else {
		m_sigmaBeg = m_sigmaBeg < deg2rad(-85) ? deg2rad(-85) : m_sigmaBeg;
	}
	//各项约束
	{
		//初期倾侧角幅值约束(个人认为是为了防止初期下降速度过快)
		{
			if (V > 2000)
				m_sigmaBeg = sgn(m_sigmaBeg)*(fabs(m_sigmaBeg) > deg2rad(20) ? deg2rad(20) : fabs(m_sigmaBeg));
		}
		//准平衡滑翔约束(并非硬约束)
		{
			if ((gamma) > -0.03&&V>1200)
			{
				spfloat  sigmamin = (Constant_Earth_GM / (r*r) - V*V / r)*Mass / L;
				if (sigmamin < 1)
				{
					sigmamin = acos(sigmamin);
					m_sigmaBeg = sgn(m_sigmaBeg)*(sigmamin > fabs(m_sigmaBeg) ? sigmamin : fabs(m_sigmaBeg));
					sigmasearch = 1;
				}
				else
				{
					sigmasearch = 0;
				}
			}
			else
			{
				sigmasearch = 0;
			}
			m_beforeGamma = gamma;
		}
		//法向过载约束
		//热流约束(SRLV基本可以不考虑)
	}
	outH = h;
	sigma = m_x[0];
	outsigmaDeg = rad2deg(m_x[0]);
	alpha = getAlpha(V);//- sgn(1 - m_lengthScale)*(fabs(20 * (1 - m_lengthScale))>3 ? 3 : fabs(20 * (1 - m_lengthScale)));
	beta = 0;

	ma = m_in[6];
	outTargetLength = length(m_targetlongt, m_targetlat, lambda, miu)*Constant_Earth_Rm;
	outLongtitude = lambda;
	outLatitude = miu;
	if (0 == m_sys->_stepcount%int(5 / m_sys->_setp))
	{
		fwrite(m_cuoutdata, sizeof(numtest), BLOCK_NUM*THREAD_NUM, m_file);
		double path_[3] = { outLatitude ,outLongtitude ,h };
		fwrite(path_, sizeof(double), 3, m_filebac);
	}
	
	//if (targetlength < 800) SendUserMessage(SM_STOP);
	outV = V;
	memcpy(m_out, &Energey, sizeof(guidanceOut));
	return 0;
}

int spguidance::onStop()
{
	if (m_file)
		fclose(m_file);
	m_file = nullptr;
	if (m_filebac)
		fclose(m_filebac);
	m_filebac = nullptr;
	return 0;
}

int spguidance::onDerive()
{
	memcpy(&r, m_in, sizeof(guidanceIn));
	m_f[0] = g_dsigma;
	double _sigma = getSigma(V, m_sigmaBeg, gspeed);
	m_f[0] = sgn(_sigma - m_x[0])*(fabs(_sigma - m_x[0]) > deg2rad(10) ? deg2rad(10) : fabs(_sigma - m_x[0]));
	//m_f[0] = -V*cos(gamma)*cos(m_lunchAngle - chi)/r;
	//sigma = getSigma(V, deg2rad(-15));
	//if (m_useEQS)
	//{
	//	anglesigmatarget = getSigma(V, deg2rad(85));
	//	sigma = anglesigmatarget;

	//	
	//	//if (h > 35000) sigma = 0;
	//}
	//准平衡滑翔边界
	//if (!m_begTeam)
	//{
	//	m_f[0] = 0.04*(m_deltaAltitude - V*sin(gamma));
	//	m_f[0] = sgn(m_f[0])*(fabs(m_f[0]) > 5 ? 5 : fabs(m_f[0]));
	//}
	memcpy(m_out, &Energey, sizeof(guidanceOut));
	return 0;
}

int spguidance::onStart()
{
	m_QEC = 1;
	m_nowtraj.targetlambda = m_targetlongt = m_params[0];
	m_nowtraj.targetmiu = m_targetlat = m_params[1];
	m_turnScaleHigh = m_params[3];
	m_turnScaleLow = m_params[4];
	m_landHight = m_params[5];			//能量管理段高度
	m_landSpeed = m_params[6];			//能量管理段速度
	m_teamAngle = m_params[7];
	m_Eend = getE(m_landSpeed, m_landHight);
	m_useEQS = m_params[2]>0.5;
	m_dataInitial = false;
	m_sigmasearchbeg = deg2rad(-80);
	m_sigmasearchLength = deg2rad(160);
	m_itercycle = 300;
	m_sigmaBeg = 0;
	sigma = 0;
	m_recordTime = 0;
	m_nextRecordTime = 0;
	m_lengthScale = 1;
	m_isfirstPredic = true;
	m_isfirstPredicForEnerg = true;
	m_beforeAzuAngle = 0;
	m_useCrossMethod = false;
	g_dsigma = 0;
	if (m_file)
	{
		fclose(m_file);
		m_file = fopen(".\guidance.data", "wb+");
	}
	else
	{
		m_file = fopen(".\guidance.data", "wb+");
	}
	if (m_filebac)
	{
		fclose(m_filebac);
		m_filebac = fopen("flypath.data", "wb+");
	}
	else
	{
		m_filebac = fopen("flypath.data", "wb+");
	}

	return 0;
}
