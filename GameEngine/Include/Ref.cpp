#include "Ref.h"

CRef::CRef()	:
	m_iRef(0),
	m_bActive(true),
	m_bEnable(true)
{
}

void CRef::AddRef()
{
	++m_iRef;
}

int CRef::Release()
{
	--m_iRef;

	if (m_iRef == 0)
	{
		delete this;
		return 0;
	}

	return m_iRef;
}
