#pragma once

#include "GameEngine.h"

class CGameMode
{
	friend class CScene;

protected:
	CGameMode();
	virtual ~CGameMode();

protected:
	class CScene*		m_pScene;
	class CObj*			m_pPlayer;
	int					m_iGameModeClassType;
	bool				m_bStart;
	class CObj*			m_pCubeMap;

public:
	void SetPlayer(class CObj* pPlayer);
	class CObj* GetPlayer()	const;
	int GetGameModeClassType()	const;
	bool IsStart()	const;
	void SetCubeMapTexture(const std::string& strKey);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

