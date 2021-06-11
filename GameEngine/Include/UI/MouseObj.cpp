#include "MouseObj.h"
#include "UISprite.h"
#include "../Component/ColliderPoint.h"
#include "../Component/ColliderRay.h"
#include "../Camera/CameraManager.h"
#include "../Component/Camera.h"
#include "../Device.h"
CMouseObj::CMouseObj()	:
	m_pImage(nullptr)
	, m_pCollider(nullptr)
{
}

CMouseObj::CMouseObj(const CMouseObj& obj)	:
	CUIObject(obj)
{
	m_pImage = (CUISprite*)FindSceneComponent("Mouse");

	m_pCollider = (CColliderRay*)FindSceneComponent("MouseBody");
}

CMouseObj::~CMouseObj()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pCollider);
}

bool CMouseObj::Init()
{
	if (!CUIObject::Init())
		return false;
/*
	m_pImage = CreateComponent<CUISprite>("Mouse");

	m_pImage->SetTexture("Mouse");

	m_pImage->SetPivot(0.f, 1.f, 0.f);

	m_pImage->SetWorldScale(20.f, 24.f, 0.f);*/

	//m_pImage->SetFrm(13);
	//m_pImage->SetTime(0.05f);

	m_pCollider = CreateComponent<CColliderRay>("MouseBody", m_pLayer);
	m_pCollider->SetUI();
	m_pCollider->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pCollider->SetMouse();

	SetRootComponent(m_pCollider);

	//m_pImage->AddChild(m_pPC);

	return true;
}

void CMouseObj::Start()
{
	CUIObject::Start();
}

void CMouseObj::Update(float fTime)
{
	CUIObject::Update(fTime);

	Vector3 vPos = GetWorldPos();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matInvProj = pCam->GetProjMat();

	matInvProj.Inverse();

	Matrix matInvVP = pCam->GetVP();

	Vector3 vPivot = pCam->GetPivot();

	matInvVP.Inverse();

	Resolution tRS = RESOLUTION;

	vPos.x = (vPos.x - tRS.iWidth * vPivot.x) / tRS.iWidth * 2.f;
	vPos.y = (vPos.y - tRS.iHeight * vPivot.y) / tRS.iHeight * 2.f;

	//Vector3 vViewPos = 

	Vector3 vWorldPos = vPos.TransformCoord(matInvVP);

	//vWorldPos.x = vPos.x * matInvVP[0][0] + vPos.y * matInvVP[1][0] + 0.3f * matInvVP[3][0];
	//vWorldPos.y = vPos.x * matInvVP[0][1] + vPos.y * matInvVP[1][1] + 0.3f * matInvVP[3][1];
	//vWorldPos.z = vPos.x * matInvVP[0][2] + vPos.y * matInvVP[1][2] + 0.3f * matInvVP[3][2];

	Vector3 vCamPos = pCam->GetWorldPos();

	Vector3 vDir = vWorldPos - vCamPos;

	vDir.Normalize();

	m_pCollider->SetDir(vDir);
	m_pCollider->SetOrigin(vCamPos);

	SAFE_RELEASE(pCam);
}

void CMouseObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CMouseObj::Collision(float fTime)
{
	CUIObject::Collision(fTime);
}

void CMouseObj::PreRender(float fTime)
{
	CUIObject::PreRender(fTime);
}

void CMouseObj::Render(float fTime)
{
	CUIObject::Render(fTime);
}

void CMouseObj::PostRender(float fTime)
{
	CUIObject::PostRender(fTime);
}

CMouseObj* CMouseObj::Clone()
{
	return new CMouseObj(*this);
}
