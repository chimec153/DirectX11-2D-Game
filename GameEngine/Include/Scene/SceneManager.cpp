#include "SceneManager.h"
#include "Scene.h"
#include "../PathManager.h"
#include "../Object/Obj.h"
#include "../Input.h"
#include "../UI/MouseObj.h"
#include "../Camera/CameraManager.h"
#include "../RenderManager.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pScene(nullptr),
	m_pSceneNext(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pSceneNext);

	for (int i = 0; i < (int)SCENE_TYPE::SC_END; ++i)
		SAFE_RELEASE_MAP(CScene::m_mapProtoType[i]);
}

CScene* CSceneManager::GetScene() const
{
	return m_pScene;
}

CScene* CSceneManager::GetNextScene() const
{
	return m_pSceneNext;
}

bool CSceneManager::Init()
{
	m_pScene = new CScene;

	m_pScene->Init();

	m_pScene->m_eType = SCENE_TYPE::SC_CURRENT;
	m_pScene->m_EditorCreate = m_EditorCreate;
	m_pScene->m_EditorDelete = m_EditorDelete;
	m_pScene->m_EditorObjCreate = m_EditorObjCreate;
	m_pScene->m_EditorGameModeCreate = m_EditorGameModeCreate;

	GET_SINGLE(CInput)->CreateMouse();

	CMouseObj* pObj = GET_SINGLE(CInput)->GetMouse();

	pObj->SetScene(m_pScene);

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pScene->Input(fTime);

	return ChangeScene();
}

int CSceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	return ChangeScene();
}

int CSceneManager::PostUpdate(float fTime)
{
	m_pScene->PostUpdate(fTime);

	return ChangeScene();
}

int CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);

	return ChangeScene();
}

int CSceneManager::PreRender(float fTime)
{
	m_pScene->PreRender(fTime);

	return ChangeScene();
}

int CSceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);

	return ChangeScene();
}

int CSceneManager::PostRender(float fTime)
{
	m_pScene->PostRender(fTime);

	return ChangeScene();
}

void CSceneManager::Save(const char* pFileName, const std::string& strRootPath)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(CPathManager)->FindMultibytePath(strRootPath);

	if (pRootPath)
		strcpy_s(strFullPath, pRootPath);

	strcat_s(strFullPath, pFileName);

	SaveFromFullPath(strFullPath);
}

void CSceneManager::SaveFromFullPath(const char* pFileName)
{
	m_pScene->SaveFromFullPath(pFileName);
}

void CSceneManager::Load(const char* pFileName, const std::string& strRootPath)
{
	char strFullPath[MAX_PATH] = {};

	const char* pRootPath = GET_SINGLE(CPathManager)->FindMultibytePath(strRootPath);

	if (pRootPath)
		strcpy_s(strFullPath, pRootPath);

	strcat_s(strFullPath, pFileName);

	LoadFromFullPath(strFullPath);
}

void CSceneManager::LoadFromFullPath(const char* pFileName)
{
	CreateNextScene();

	m_pSceneNext->LoadFromFullPath(pFileName);
}

bool CSceneManager::ChangeScene()
{
	if (m_pSceneNext)
	{
		SAFE_DELETE(m_pScene);

		m_pScene = m_pSceneNext;

		m_pScene->m_eType = SCENE_TYPE::SC_CURRENT;

		m_pSceneNext = nullptr;

		ChangeProto();

		GET_SINGLE(CCameraManager)->ChangeScene();

		GET_SINGLE(CRenderManager)->ClearInst();

		return true;
	}

	return false;
}

void CSceneManager::ChangeProto()
{
	SAFE_RELEASE_MAP(CScene::m_mapProtoType[(int)SCENE_TYPE::SC_CURRENT]);

	CScene::m_mapProtoType[(int)SCENE_TYPE::SC_CURRENT] = CScene::m_mapProtoType[(int)SCENE_TYPE::SC_NEXT];

	CScene::m_mapProtoType[(int)SCENE_TYPE::SC_NEXT].clear();
}

void CSceneManager::CreateNextScene()
{
	m_pSceneNext = new CScene;

	m_pSceneNext->Init();

	m_pSceneNext->m_eType = SCENE_TYPE::SC_NEXT;

	m_pSceneNext->m_EditorCreate = m_EditorCreate;
	m_pSceneNext->m_EditorDelete = m_EditorDelete;
	m_pSceneNext->m_EditorObjCreate = m_EditorObjCreate;
	m_pSceneNext->m_EditorGameModeCreate = m_EditorGameModeCreate;

	//GET_SINGLE(CInput)->CreateMouse();

	CMouseObj* pObj = GET_SINGLE(CInput)->GetMouse();

	pObj->SetScene(m_pSceneNext);
}
