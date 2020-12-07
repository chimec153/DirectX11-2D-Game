#include "InputObj.h"
#include "Input.h"

CInputObj::CInputObj()
{
}

CInputObj::~CInputObj()
{
	std::unordered_map<std::string, PInputAxis>::iterator iterAxis = m_mapInputAxis.begin();
	std::unordered_map<std::string, PInputAxis>::iterator iterAxisEnd = m_mapInputAxis.end();

	for (; iterAxis != iterAxisEnd; ++iterAxis)
	{
		GET_SINGLE(CInput)->DeleteAxisKey(iterAxis->first, this);

		SAFE_DELETE(iterAxis->second);
	}

	m_mapInputAxis.clear();

	std::unordered_map<std::string, PInputAction>::iterator iterAction = m_mapInputAction.begin();
	std::unordered_map<std::string, PInputAction>::iterator iterActionEnd = m_mapInputAction.end();

	for (; iterAction != iterActionEnd; ++iterAction)
	{
		GET_SINGLE(CInput)->DeleteActionKey(iterAction->first, this);

		SAFE_DELETE(iterAction->second);
	}

	m_mapInputAction.clear();
}

void CInputObj::AxisFunc(const std::string& strTag, float fScale, float fTime)
{
	PInputAxis pAxis = FindInputAxis(strTag);

	if (!pAxis)
		return;

	pAxis->pFunc(fScale, fTime);
}

void CInputObj::ActionFunc(const std::string& strTag, KEY_TYPE eType, float fPushTime, float fTime)
{
	PInputAction pAction = FindInputAction(strTag);

	if (!pAction)
		return;

	pAction->pFunc[eType](fPushTime, fTime);
}

PInputAxis CInputObj::FindInputAxis(const std::string& strTag)
{
	std::unordered_map<std::string, PInputAxis>::iterator iter = m_mapInputAxis.find(strTag);

	if (iter == m_mapInputAxis.end())
		return nullptr;

	return iter->second;
}

PInputAction CInputObj::FindInputAction(const std::string& strTag)
{
	std::unordered_map<std::string, PInputAction>::iterator iter = m_mapInputAction.find(strTag);

	if (iter == m_mapInputAction.end())
		return nullptr;

	return iter->second;
}
