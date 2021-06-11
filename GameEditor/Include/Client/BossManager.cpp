#include "pch.h"
#include "BossManager.h"
#include "Object/SoulMonster.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Object/Player.h"
#include "Object/Colossus.h"
#include "Object/BrainFreeze.h"
#include "Object/Eyecube.h"

DEFINITION_SINGLE(CBossManager);

CBossManager::CBossManager()	:
	m_bSave(true)
	, m_iSlot(0)
{
	m_vecSlot.resize(4);
}

CBossManager::~CBossManager()
{
	SAFE_RELEASE_MAP(m_mapBoss);
}

bool CBossManager::Init()
{
	return true;
}

CSoulMonster* CBossManager::FindMonster(const std::string& strTag) const
{
	std::unordered_map<std::string, CSoulMonster*>::const_iterator iter = m_mapBoss.find(strTag);

	if (iter == m_mapBoss.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CBossManager::AddMonster(CSoulMonster* pBoss)
{
	if (!pBoss)
		return;

	std::string strKey = pBoss->GetName();

	class CSoulMonster* _pBoss = FindMonster(strKey);

	if (_pBoss)
	{
		SAFE_RELEASE(_pBoss);
		return;
	}

	pBoss->AddRef();

	m_mapBoss.insert(std::make_pair(strKey, pBoss));

	m_bSave = false;

	return;
}

void CBossManager::Save()
{
	char strFile[MAX_PATH] = {};

	char strNum[32] = {};

	sprintf_s(strNum, "Save%d.tss", m_iSlot);

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(DATA_PATH);

	if (pPath)
		strcpy_s(strFile, pPath);

	strcat_s(strFile, strNum);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFile, "wb");

	size_t iSz = m_mapBoss.size();
	BOSS_TYPE _eType = (BOSS_TYPE)0;

	if (pFile)
	{
		fwrite(&iSz, sizeof(size_t), 1, pFile);

		std::unordered_map<std::string, class CSoulMonster*>::iterator iter = m_mapBoss.begin();
		std::unordered_map<std::string, class CSoulMonster*>::iterator iterEnd = m_mapBoss.end();

		for (; iter != iterEnd; ++iter)
		{
			BOSS_TYPE eType = iter->second->GetBossType();

			_eType = BOSS_TYPE((int)_eType | (int)eType);

			fwrite(&eType, 4, 1, pFile);
			iter->second->Save(pFile);
		}

		fclose(pFile);
	}

	CPlayer* pPlayer = (CPlayer*)GET_SINGLE(CSceneManager)->GetScene()->GetGameMode()->GetPlayer();

	m_vecSlot[m_iSlot].fPlayTime = pPlayer->GetPlayTime();
	m_vecSlot[m_iSlot].iKill = (int)iSz;	
	m_vecSlot[m_iSlot].iDeath = pPlayer->GetDeath();
	m_vecSlot[m_iSlot].bStart = true;
	m_vecSlot[m_iSlot].eType = _eType;

	SAFE_RELEASE(pPlayer);

	SaveSlotInfo();

	m_bSave = true;
}

void CBossManager::Load()
{
	char strFile[MAX_PATH] = {};

	char strNum[32] = {};

	sprintf_s(strNum, "Save%d.tss", m_iSlot);

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(DATA_PATH);

	if (pPath)
		strcpy_s(strFile, pPath);

	strcat_s(strFile, strNum);

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFile, "rb");

	if (pFile)
	{
		size_t iSz = 0;

		fread(&iSz, sizeof(size_t), 1, pFile);

		for (size_t i = 0; i < iSz; ++i)
		{
			BOSS_TYPE eType = BOSS_TYPE::COLOSSUS;

			fread(&eType, 4, 1, pFile);

			CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

			switch (eType)
			{
			case BOSS_TYPE::COLOSSUS:
			{
				CObj* pObj = FindMonster("boss_colossus");

				if (pObj)
				{
					pObj->Release();

					m_mapBoss.erase("boss_colossus");

					pObj->Release();
				}

				CColossus* pCol = new CColossus;

				pCol->Load(pFile);

				m_mapBoss.insert(std::make_pair(pCol->GetName(), pCol));
			}
				break;
			case BOSS_TYPE::EYECUBE:
			{
				CObj* pObj = FindMonster("boss_eyecube");

				if (pObj)
				{
					pObj->Release();

					m_mapBoss.erase("boss_eyecube");

					pObj->Release();
				}

				CEyecube* pEye = new CEyecube;

				pEye->Load(pFile);

				m_mapBoss.insert(std::make_pair(pEye->GetName(), pEye));
			}
				break;
			case BOSS_TYPE::BRAINFREEZE:
			{
				CObj* pObj = FindMonster("boss_brainfreeze");

				if (pObj)
				{
					pObj->Release();

					m_mapBoss.erase("boss_brainfreeze");

					pObj->Release();
				}

				CBrainFreeze* pBrain = new CBrainFreeze;

				pBrain->Load(pFile);

				m_mapBoss.insert(std::make_pair(pBrain->GetName(), pBrain));
			}
				break;
			}

		}

		CPlayer* pPlayer = (CPlayer*)GET_SINGLE(CSceneManager)->GetScene()->GetGameMode()->GetPlayer();

		pPlayer->SetPlayTime(m_vecSlot[m_iSlot].fPlayTime);
		pPlayer->SetDeath(m_vecSlot[m_iSlot].iDeath);

		SAFE_RELEASE(pPlayer);

		fclose(pFile);
	}

	m_bSave = true;
}

void CBossManager::SaveSlotInfo()
{
	char strFull[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(DATA_PATH);

	if (pPath)
		strcpy_s(strFull, pPath);

	strcat_s(strFull, "Info.tss");

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFull, "wb");

	if (pFile)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			fwrite(&m_vecSlot[i].bStart, 1, 1, pFile);
			fwrite(&m_vecSlot[i].fPlayTime, 4, 1, pFile);
			fwrite(&m_vecSlot[i].iKill, 4, 1, pFile);
			fwrite(&m_vecSlot[i].iDeath, 4, 1, pFile);
			fwrite(&m_vecSlot[i].eType, 4, 1, pFile);
		}

		fclose(pFile);
	}
}

void CBossManager::LoadSlotInfo()
{
	char strFull[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(DATA_PATH);

	if (pPath)
		strcpy_s(strFull, pPath);

	strcat_s(strFull, "Info.tss");

	FILE* pFile = nullptr;

	fopen_s(&pFile, strFull, "rb");

	if (pFile)
	{
		for (size_t i = 0; i < 4; ++i)
		{
			fread(&m_vecSlot[i].bStart, 1, 1, pFile);
			fread(&m_vecSlot[i].fPlayTime, 4, 1, pFile);
			fread(&m_vecSlot[i].iKill, 4, 1, pFile);
			fread(&m_vecSlot[i].iDeath, 4, 1, pFile);
			fread(&m_vecSlot[i].eType, 4, 1, pFile);
		}

		fclose(pFile);
	}
}
