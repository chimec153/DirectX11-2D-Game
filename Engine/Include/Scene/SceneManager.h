#pragma once

#include "../GameEngine.h"
#include "Scene.h"

class CSceneManager
{
private:
	class CScene*	m_pScene;	
	class CScene*	m_pSceneNext;

public:
	class CScene* GetScene()	const;
	class CScene* GetNextScene()	const;

private:
	std::function<void(const std::string&)>	m_EditorCreate;
	std::function<void(const std::string&)>	m_EditorDelete;
	std::function<class CObj* (int)>		m_EditorObjCreate;
	std::function<class CGameMode* (int)>	m_EditorGameModeCreate;

public:
	bool Init();
	int Input(float fTime);
	int Update(float fTime);
	int PostUpdate(float fTime);
	int Collision(float fTime);
	int PreRender(float fTime);
	int Render(float fTime);
	int PostRender(float fTime);

public:
	template <typename T>
	void SetEditorCreate(T* pObj, void(T::* pFunc)(const std::string&))
	{
		m_EditorCreate = bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorDelete(T* pObj, void(T::* pFunc)(const std::string&))
	{
		m_EditorDelete = bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorObjCreate(T* pObj, class CObj* (T::* pFunc)(int))
	{
		m_EditorObjCreate = bind(pFunc, pObj, std::placeholders::_1);
	}

	template <typename T>
	void SetEditorGameModeCreate(T* pObj, class CGameMode*(T::* pFunc)(int))
	{
		m_EditorGameModeCreate = bind(pFunc, pObj, std::placeholders::_1);
	}

public:
	template <typename T>
	bool SetGameMode(bool bCurrent = true)
	{
		if (bCurrent)
			return m_pScene->SetGameMode<T>();

		return m_pSceneNext->SetGameMode<T>();
	}

public:
	void Save(const char* pFileName, const std::string& strRootPath = DATA_PATH);
	void SaveFromFullPath(const char* pFileName);
	void Load(const char* pFileName, const std::string& strRootPath = DATA_PATH);
	void LoadFromFullPath(const char* pFileName);

public:
	bool ChangeScene();
	void ChangeProto();
	void CreateNextScene();

	DECLARE_SINGLE(CSceneManager)
};

