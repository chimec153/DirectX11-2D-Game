#pragma once

#include "GameEngine.h"
#include "Object/SoulMonster.h"

typedef struct _tagSlotInfo
{
	bool	bStart;
	float	fPlayTime;
	int		iDeath;
	int		iKill;
	BOSS_TYPE	eType;

	_tagSlotInfo() :
		bStart(false)
		, fPlayTime(0.f)
		, iDeath(0)
		, iKill(0)
		, eType((BOSS_TYPE)0)
	{
	}
}SLOTINFO, *PSLOTINFO;

class CBossManager
{
private:
	std::unordered_map<std::string, class CSoulMonster*>	m_mapBoss;
	bool		m_bSave;
	int			m_iSlot;
	std::vector<SLOTINFO>		m_vecSlot;

public:
	bool IsSaved()	const
	{
		return m_bSave;
	}
	void SetSlot(int iSlot)
	{
		m_iSlot = iSlot;
	}
	int GetSlot()	const
	{
		return m_iSlot;
	}
	void SetSaved()
	{
		m_bSave = true;
	}

public:
	bool Init();
	class CSoulMonster* FindMonster(const std::string& strKey)	const;
	void AddMonster(class CSoulMonster* pBoss);
	const SLOTINFO GetInfo(int iSlot)	const
	{
		if (iSlot >= 0 && iSlot < 4)
		{
			return m_vecSlot[iSlot];
		}

		else
		{
			return SLOTINFO();
		}
	}
	void DeleteSlot(int iSlot)
	{

		memset(&m_vecSlot[iSlot], 0, sizeof(SLOTINFO));
	}

public:
	void Save();
	void Load();

public:
	void SaveSlotInfo();
	void LoadSlotInfo();

	DECLARE_SINGLE(CBossManager);
};

