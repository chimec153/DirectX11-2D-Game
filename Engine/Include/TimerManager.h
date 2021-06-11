#pragma once

#include "GameEngine.h"

class CTimerManager
{
private:
	std::unordered_map<std::string, class CTimer*>	m_mapTimer;
	class CTimer* m_pTimer;

public:
	class CTimer* GetMainTimer()	const;

public:
	bool Init();
	void Update();

public:
	class CTimer* FindTimer(const std::string& strKey);
	bool CreateTimer(const std::string& strKey);

	DECLARE_SINGLE(CTimerManager);
};

