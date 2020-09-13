#include "SceneManager.h"
#include "Scene.h"

DEFINITION_SINGLE(CSceneManager)

CSceneManager::CSceneManager()	:
	m_pScene(nullptr)
{

}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pScene);
}

bool CSceneManager::Init()
{
	m_pScene = new CScene;

	m_pScene->Init();

	return true;
}

int CSceneManager::Input(float fTime)
{
	m_pScene->Input(fTime);

	return 0;
}

int CSceneManager::Update(float fTime)
{
	m_pScene->Update(fTime);

	return 0;
}

int CSceneManager::PostUpdate(float fTime)
{
	m_pScene->PostUpdate(fTime);
	return 0;
}

int CSceneManager::Collision(float fTime)
{
	m_pScene->Collision(fTime);
	return 0;
}

int CSceneManager::PreRender(float fTime)
{
	m_pScene->PreRender(fTime);
	return 0;
}

int CSceneManager::Render(float fTime)
{
	m_pScene->Render(fTime);
	return 0;
}

int CSceneManager::PostRender(float fTime)
{
	m_pScene->PostRender(fTime);
	return 0;
}
