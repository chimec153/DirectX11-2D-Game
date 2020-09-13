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
	CGameMode*					m_pGameMode;
	class CSceneResource*		m_pResource;

public:
	CGameMode* GetGameMode()	const
	{
		return m_pGameMode;
	}

	class CSceneResource* GetResource()	const
	{
		return m_pResource;
	}

protected:
	bool						m_bStart;
	std::list<class CObj*>		m_ObjList;

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
	template <typename T>
	T* CreateObject(const std::string& strName)
	{
		T* pObj = new T;

		pObj->SetName(strName);
		pObj->m_pScene = this;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}

		pObj->AddRef();

		m_ObjList.push_back(pObj);

		return pObj;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

