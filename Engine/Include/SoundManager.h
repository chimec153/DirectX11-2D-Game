#pragma once

#include "GameEngine.h"

class CSoundManager
{
private:
	FMOD::System*									m_pSystem;
	std::unordered_map<std::string, PSOUNDINFO>		m_mapSound;
	std::vector<FMOD::Channel*>						m_vecChannel;

public:
	bool Init();
	void Update(float fTime);

public:
	FMOD::System* GetSystem()	const;
	bool LoadSound(const std::string& strKey, SOUND_TYPE eType, const char* pFileName, const std::string strPathKey = SOUND_PATH);
	void Play(const std::string& strKey);
	void Stop(const std::string& strKey);
	void Stop(SOUND_TYPE eType);

public:
	PSOUNDINFO FindSound(const std::string& strKey);

	DECLARE_SINGLE(CSoundManager)
};

