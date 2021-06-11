#include "TimerManager.h"
#include "Timer.h"

DEFINITION_SINGLE(CTimerManager);

CTimerManager::CTimerManager()	:
	m_pTimer(nullptr)
{

}

CTimerManager::~CTimerManager()
{
	SAFE_RELEASE_MAP(m_mapTimer);
	SAFE_RELEASE(m_pTimer);
}

CTimer* CTimerManager::GetMainTimer() const
{
	if (m_pTimer)
		m_pTimer->AddRef();

	return m_pTimer;
}

bool CTimerManager::Init()
{
	m_pTimer = new CTimer;

	m_pTimer->Init();

	return true;
}

void CTimerManager::Update()
{
	m_pTimer->Update();

	std::unordered_map<std::string, CTimer*>::iterator iter = m_mapTimer.begin();
	std::unordered_map<std::string, CTimer*>::iterator iterEnd = m_mapTimer.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update();
	}
}

CTimer* CTimerManager::FindTimer(const std::string& strKey)
{
	std::unordered_map<std::string, CTimer*>::iterator iter = m_mapTimer.find(strKey);

	if (iter == m_mapTimer.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

bool CTimerManager::CreateTimer(const std::string& strKey)
{
	CTimer* pTimer = new CTimer;

	if (!pTimer->Init())
	{
		pTimer->Release();
		return false;
	}

	m_mapTimer.insert(std::make_pair(strKey, pTimer));

	return true;
}
