#include "ThreadManager.h"
#include "Thread.h"

DEFINITION_SINGLE(CThreadManager)

CThreadManager::CThreadManager()
{

}

CThreadManager::~CThreadManager()
{
	SAFE_RELEASE_MAP(m_mapThread);

	std::unordered_map<std::string, CRITICAL_SECTION*>::iterator iter = m_mapCritical.begin();
	std::unordered_map<std::string, CRITICAL_SECTION*>::iterator iterEnd = m_mapCritical.end();

	for (; iter != iterEnd; ++iter)
	{
		DeleteCriticalSection(iter->second);
		SAFE_DELETE(iter->second);
	}
}

CThread* CThreadManager::FindThread(const std::string& strTag)
{
	std::unordered_map<std::string, CThread*>::iterator iter = m_mapThread.find(strTag);

	if (iter == m_mapThread.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CThreadManager::Init()
{
	return true;
}

CRITICAL_SECTION* CThreadManager::FindCritical(const std::string& strTag)
{
	std::unordered_map<std::string, CRITICAL_SECTION*>::iterator iter = m_mapCritical.find(strTag);

	if (iter == m_mapCritical.end())
		return nullptr;

	return iter->second;
}

bool CThreadManager::CreateCritical(const std::string& strTag)
{
	CRITICAL_SECTION* pCrt = FindCritical(strTag);

	if (pCrt)
	{
		return false;
	}

	pCrt = new CRITICAL_SECTION;

	InitializeCriticalSection(pCrt);

	m_mapCritical.insert(std::make_pair(strTag, pCrt));

	return true;
}

void CThreadManager::DeleteThread(const std::string& strTag)
{
	std::unordered_map<std::string, CThread*>::iterator iter = m_mapThread.find(strTag);

	if (iter == m_mapThread.end())
		return;

	SAFE_RELEASE(iter->second);

	m_mapThread.erase(iter);

	return;
}
