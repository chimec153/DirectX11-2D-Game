#pragma once

#include "../GameMode.h"
#include "../GameEngine.h"
#include "../Object/Obj.h"
#include "../Layer.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	~CScene();

protected:
	CGameMode*					m_pGameMode;
	SCENE_TYPE					m_eType;
	std::list<class CLayer*>	m_LayerList;

public:
	SCENE_TYPE GetSceneType()	const;
	void CreateLayer(const std::string& strKey, int iZ);
	class CLayer* FindLayer(const std::string& strKey);

protected:
	std::function<void(const std::string&)>		m_EditorCreate;
	std::function<void(const std::string&)>		m_EditorDelete;
	std::function<class CObj* (int)>			m_EditorObjCreate;
	std::function<class CGameMode* (int)>		m_EditorGameModeCreate;

public:
	CGameMode* GetGameMode()	const;

protected:
	bool						m_bStart;
	std::list<class CObj*>		m_ObjList;

private:
	static std::unordered_map<std::string, class CObj*>	m_mapProtoType[(int)SCENE_TYPE::SC_END];

public:
	template <typename T>
	bool SetGameMode()
	{
		SAFE_DELETE(m_pGameMode);

		m_pGameMode = new T;
		m_pGameMode->m_pScene = this;

		if (!m_pGameMode->Init())
		{
			SAFE_DELETE(m_pGameMode);
			return false;
		}
		
		return true;
	}

public:
	template <typename T>
	T* CreateObject(const std::string& strName, class CLayer* pLayer, SCENE_TYPE eType = SCENE_TYPE::SC_CURRENT)
	{
		T* pObj = new T;

		pObj->SetName(strName);

		if (eType == SCENE_TYPE::SC_CURRENT)
			pObj->m_pScene = this;

		if (!pObj->Init())
		{
			SAFE_RELEASE(pObj);
			return nullptr;
		}

		if (m_EditorCreate)
			m_EditorCreate(strName);

		pObj->m_EditorDelete = m_EditorDelete;

		pLayer->AddObj(pObj);

		return pObj;
	}

	template <typename T>
	static T* CreateProtoObj(const std::string& strKey, CScene* pScene, SCENE_TYPE eType = SCENE_TYPE::SC_CURRENT)
	{
		T* pProto = (T*)FindProtoType(strKey, eType);

		if (pProto)
		{
			SAFE_RELEASE(pProto);
			return nullptr;
		}

		pProto = new T;

		pProto->m_pScene = pScene;

		if (!pProto->Init())
		{
			SAFE_RELEASE(pProto);
			return nullptr;
		}

		pProto->AddRef();
		m_mapProtoType[(int)eType].insert(std::make_pair(strKey, pProto));

		return pProto;
	}

	class CObj* CreateCloneObj(const std::string& strName, const std::string& strProto, class CLayer* pLayer, SCENE_TYPE eType = SCENE_TYPE::SC_CURRENT)
	{
		CObj* pProto = FindProtoType(strProto, eType);

		if (!pProto)
			return nullptr;

		CObj* pObj = pProto->Clone();

		SAFE_RELEASE(pProto);

		pObj->SetScene(this);
		pObj->SetLayer(pLayer);
		pObj->SetName(strName);

		if (m_EditorCreate)
			m_EditorCreate(strName);

		pObj->m_EditorDelete = m_EditorDelete;

		pLayer->AddObj(pObj);

		return pObj;
	}

	class CObj* EditorCreate(int iType);

private:
	static class CObj* FindProtoType(const std::string& strKey, SCENE_TYPE eType);

public:
	void DelEditDeFunc();

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

public:
	virtual void SaveFromFullPath(const char* pFullPath);
	virtual void Save(const char* pFileName, const std::string& strRootPath = DATA_PATH);
	virtual void LoadFromFullPath(const char* pFullPath);
	virtual void Load(const char* pFileName, const std::string& strRootPath = DATA_PATH);

public:
	static bool SortZ(class CLayer* pSrc, class CLayer* pDst);
	void SortInstText();
};

