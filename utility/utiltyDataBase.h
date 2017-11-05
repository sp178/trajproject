#pragma once
//用于读取xml数据差值表
struct _BLOCK {
	size_t		m_demention;		//插值数据维度
	size_t*		m_cordial_demtion;	//每个维度上的坐标个数
	double*		m_cordials;			//每个维度上的坐标刻度
	double*		m_beg;				//数据开始
	_BLOCK() {
		m_cordial_demtion = nullptr;
		m_cordials = nullptr;
		m_beg = nullptr;
	};
	////复制构造形式
	_BLOCK(const _BLOCK& _block) {
		m_beg = _block.m_beg;
		m_cordials = _block.m_cordials;
		m_cordial_demtion = _block.m_cordial_demtion;
		m_demention = _block.m_demention;
		//_block.eraseData();
	};
	_BLOCK(_BLOCK&& _block)
	{
		m_beg = _block.m_beg;
		m_cordials = _block.m_cordials;
		m_cordial_demtion = _block.m_cordial_demtion;
		m_demention = _block.m_demention;
		_block.eraseData();
	};
	void operator=(const _BLOCK& _block)
	{
		m_beg = _block.m_beg;
		m_cordials = _block.m_cordials;
		m_cordial_demtion = _block.m_cordial_demtion;
		m_demention = _block.m_demention;
		//_block.eraseData();
	}
	//~_BLOCK() {

	//};
	void clear()
	{
		delete[]m_beg;
		m_beg = nullptr;
		delete[]m_cordials;
		m_cordials = nullptr;
		delete[] m_cordial_demtion;
		m_cordial_demtion = nullptr;
	}
protected:

	void const eraseData()
	{
		m_beg = nullptr;
		m_cordials = nullptr;
		m_cordial_demtion = nullptr;
		m_demention = 0;
	}
};