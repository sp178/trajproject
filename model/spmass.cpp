#include "spmass.h"

SPEXPORT_WITH_NAME(mass, spmass);

spmass::spmass()
{
}


spmass::~spmass()
{
}

int spmass::onInitial()
{
	return 0;
}

int spmass::onClear()
{
	return 0;
}

int spmass::onUpdate()
{
	m_out[0] = m_mass;
	return 0;
}

int spmass::onStop()
{
	return 0;
}

int spmass::onDerive()
{
	return 0;
}

int spmass::onStart()
{
	m_mass = m_params[0];
	m_out[0] = m_mass;
	return 0;
}
