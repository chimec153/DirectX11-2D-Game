#pragma once

#include "../GameMode.h"
#include "../GameEngine.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	~CScene();

protected:
	bool		m_bStart;
	CGameMode*	m_pGameMode;

public:
	CGameMode* GetGameMode()	const
	{
		return m_pGameMode;
	}

public:
	CGameMode* SetGameMode()
	{
		SAFE_DELETE(m_pGameMode);

		m_pGameMode = new CGameMode;

		if (!m_pGameMode->Init())
		{
			SAFE_DELETE(m_pGameMode);
			return nullptr;
		}
		
		return m_pGameMode;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

