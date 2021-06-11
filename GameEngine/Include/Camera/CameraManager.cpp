#include "CameraManager.h"
#include "../Component/Camera.h"
#include "../Object/Obj.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"

DEFINITION_SINGLE(CCameraManager)

CCameraManager::CCameraManager()
{
	m_pMainCam = new CCamera;

	m_pMainCam->SetCameraType(CAMERA_TYPE::CT_3D);

	m_pUICam = new CCamera;

	m_pUICam->SetCameraType(CAMERA_TYPE::CT_UI);
}

CCameraManager::~CCameraManager()
{
	SAFE_RELEASE(m_pMainCam);
	SAFE_RELEASE(m_pUICam);
}

void CCameraManager::SetMainCam(CCamera* pCam)
{
	SAFE_RELEASE(m_pMainCam);

	m_pMainCam = pCam;

	if (m_pMainCam)
		m_pMainCam->AddRef();
}

void CCameraManager::SetUICam(CCamera* pCam)
{
	SAFE_RELEASE(m_pUICam);

	m_pUICam = pCam;

	if (m_pUICam)
		m_pUICam->AddRef();
}

CCamera* CCameraManager::GetMainCam() const
{
	if (m_pMainCam)
		m_pMainCam->AddRef();

	return m_pMainCam;
}

CCamera* CCameraManager::GetUICam() const
{
	if (m_pUICam)
		m_pUICam->AddRef();

	return m_pUICam;
}

bool CCameraManager::Init()
{
	m_pMainCam->SetPivot(0.5f, 0.5f, 0.f);
	CObj* pObj = GET_SINGLE(CSceneManager)->GetScene()->GetGameMode()->GetPlayer();

	if (pObj)
	{
		m_pMainCam->SetTarget(pObj);
		pObj->Release();
	}


	return true;
}

void CCameraManager::Update(float fTime)
{
	CObj* pMainObj = m_pMainCam->GetObj();
	CObj* pUIObj = m_pUICam->GetObj();

	if (!pMainObj)
		m_pMainCam->Update(fTime);

	if (!pUIObj)
		m_pUICam->Update(fTime);

	SAFE_RELEASE(pMainObj);
	SAFE_RELEASE(pUIObj);
}

void CCameraManager::PostUpdate(float fTime)
{
	CObj* pMainObj = m_pMainCam->GetObj();
	CObj* pUIObj = m_pUICam->GetObj();

	if (!pMainObj)
		m_pMainCam->PostUpdate(fTime);

	if (!pUIObj)
		m_pUICam->PostUpdate(fTime);

	SAFE_RELEASE(pMainObj);
	SAFE_RELEASE(pUIObj);
}

void CCameraManager::ChangeScene()
{
	m_pMainCam->m_pScene = GET_SINGLE(CSceneManager)->GetScene();
	m_pUICam->m_pScene = GET_SINGLE(CSceneManager)->GetScene();

	CObj* pObj = GET_SINGLE(CSceneManager)->GetScene()->GetGameMode()->GetPlayer();

	m_pMainCam->SetTarget(pObj);
	m_pMainCam->SetZoom(false);

	SAFE_RELEASE(pObj);
}
