#include "SoundManager.h"
#include "PathManager.h"

DEFINITION_SINGLE(CSoundManager)

CSoundManager::CSoundManager()	:
	m_pSystem(nullptr)
{
	m_vecChannel.resize((int)SOUND_TYPE::END);
}

CSoundManager::~CSoundManager()
{
	SAFE_DELETE_MAP(m_mapSound);

	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
	}
}

bool CSoundManager::Init()
{
	FMOD::System_Create(&m_pSystem);

	m_pSystem->init(10, FMOD_INIT_NORMAL, nullptr);

	return true;
}

void CSoundManager::Update(float fTime)
{
	m_pSystem->update();
}

FMOD::System* CSoundManager::GetSystem() const
{
	return m_pSystem;
}

bool CSoundManager::LoadSound(const std::string& strKey, SOUND_TYPE eType, const char* pFileName, const std::string strPathKey)
{
	PSOUNDINFO pInfo = FindSound(strKey);

	if (pInfo)
		return false;

	char pFull[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(pFull, pPath);

	strcat_s(pFull, pFileName);

	pInfo = new SOUNDINFO;

	pInfo->eType = eType;

	FMOD_MODE eMode = FMOD_LOOP_NORMAL;

	if (eType == SOUND_TYPE::EFFECT)
		eMode = FMOD_DEFAULT;

	FMOD_RESULT hr = m_pSystem->createSound(pFull, eMode, nullptr, &pInfo->pSound);

	if (hr != FMOD_OK)
	{
		SAFE_DELETE(pInfo);
		return false;
	}

	m_mapSound.insert(std::make_pair(strKey, pInfo));

	return true;
}

void CSoundManager::Play(const std::string& strKey)
{
	PSOUNDINFO pInfo = FindSound(strKey);

	if (!pInfo)
		return;

	m_pSystem->update();

	m_pSystem->playSound(pInfo->pSound, nullptr, (bool)pInfo->eType, &m_vecChannel[(int)pInfo->eType]);
	m_vecChannel[(int)pInfo->eType]->setVolume(pInfo->fVol);
	
}

void CSoundManager::Stop(const std::string& strKey)
{
	PSOUNDINFO pInfo = FindSound(strKey);

	if (!pInfo)
		return;

	m_vecChannel[(int)pInfo->eType]->stop();
}

void CSoundManager::Stop(SOUND_TYPE eType)
{
	if (m_vecChannel[(int)eType])
	{
		m_vecChannel[(int)eType]->stop();
	}
}

PSOUNDINFO CSoundManager::FindSound(const std::string& strKey)
{
	std::unordered_map<std::string, PSOUNDINFO>::iterator iter = m_mapSound.find(strKey);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}