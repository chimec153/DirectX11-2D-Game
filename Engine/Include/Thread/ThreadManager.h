#pragma once

#include "../GameEngine.h"

class CThreadManager
{
private:
	std::unordered_map<std::string, class CThread*>	m_mapThread;
	std::unordered_map<std::string, CRITICAL_SECTION*>	m_mapCritical;

public:
	bool Init();
	CRITICAL_SECTION* FindCritical(const std::string& strTag);
	bool CreateCritical(const std::string& strTag);
	void DeleteThread(const std::string& strTag);

	template <typename T>
	T* CreateThread(const std::string& strTag)
	{
		T* pThread = FindThread(strTag);

		if (pThread)
		{
			pThread->Release();
			return false;
		}

		pThread = new T;

		pThread->AddRef();

		pThread->Create();

		m_mapThread.insert(std::make_pair(strTag, pThread));

		return pThread;
	}

private:
	class CThread* FindThread(const std::string& strTag);

	DECLARE_SINGLE(CThreadManager)
};

