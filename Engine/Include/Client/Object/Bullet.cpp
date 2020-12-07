#include "Bullet.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/Camera.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderRect.h"
#include "Player.h"
#include "Component/ColliderCircle.h"

CBullet::CBullet() :
	m_fDist(0.f),
	m_fLimitDist(1800.f),
	m_fSpeed(600.f),
	m_pMesh(nullptr),
	m_pCam(nullptr)
{
	m_iObjClassType = (int)OBJ_CLASS_TYPE::OCT_BULLET;
}

CBullet::CBullet(const CBullet& bullet) :
	CObj(bullet)
{
	m_fDist = bullet.m_fDist;
	m_fLimitDist = bullet.m_fLimitDist;
	m_fSpeed = bullet.m_fSpeed;

	if (bullet.m_pMesh)
		m_pMesh = bullet.m_pMesh->Clone();

	SetRootComponent(m_pMesh);

	if (bullet.m_pCam)
		m_pCam = bullet.m_pCam->Clone();

	CCollider* pRC = (CCollider*)FindSceneComponent("BulletBody");

	pRC->SetCallBack<CBullet>(COLLISION_STATE::INIT, this, &CBullet::ColInit);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::STAY, this, &CBullet::ColStay);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::END, this, &CBullet::ColEnd);

	SAFE_RELEASE(pRC);
	//m_pMesh->SetEndFunc<CBullet>(this, &CBullet::Destroy);
}

CBullet::~CBullet()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pCam);
}

bool CBullet::Init()
{
	m_pMesh = CreateComponent<CSpriteComponent>("Mesh");
	m_pCam = CreateComponent<CCamera>("bulcam");

	m_pCam->SetInheritRotX(false);
	m_pCam->SetInheritRotY(false);
	m_pCam->SetInheritRotZ(false);
	m_pCam->SetInheritScale(false);
	m_pCam->SetCameraType(CAMERA_TYPE::CT_2D);
	m_pCam->SetPivot(0.3f, 0.7f, 0.f);

	m_pMesh->SetWorldRotX(180.f);
	m_pMesh->AddChild(m_pCam);
	m_pMesh->SetTexture(REGISTER_TYPE::RT_DIF, "ani2", TEXT("pngegg.png"), TEXTURE_PATH, 0, 1, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);
	m_pMesh->CreateSprite("Arrow", "Arrow", true);

	CColliderCircle* pRC = CreateComponent<CColliderCircle>("BulletBody");

	//pRC->SetExtent(16.f, 16.f);
	pRC->SetRadius(8.f);
	pRC->SetProfile("Base");

	pRC->SetCallBack<CBullet>(COLLISION_STATE::INIT, this, &CBullet::ColInit);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::STAY, this, &CBullet::ColStay);
	pRC->SetCallBack<CBullet>(COLLISION_STATE::END, this, &CBullet::ColEnd);

	m_pMesh->AddChild(pRC);

	SAFE_RELEASE(pRC);

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativeScale(16.f, 16.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	return true;
}

void CBullet::Start()
{
}

void CBullet::Update(float fTime)
{
	CObj::Update(fTime);
	/*
		Vector3 vAxis = m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y);

		CObj* pObj = m_pScene->FindObj("Min");

		Vector3 vDir = pObj->GetWorldPos() -GetWorldPos();

		SAFE_RELEASE(pObj);

		float fDot = vAxis.Dot(vDir);

		if (fDot > 0)
		{
			float fCross = vAxis.Cross(vDir).z;

			if(fCross > 0.f)
				AddWorldRotZ(90.f * fTime);

			else
				AddWorldRotZ(90.f * -fTime);
		}		*/

	AddRelativePos(m_pMesh->GetRelativeAxis(WORLD_AXIS::AXIS_Y) * m_fSpeed * -fTime);

	m_fDist += fTime * m_fSpeed;

	if (m_fDist >= m_fLimitDist)
	{
		m_fSpeed = 0.f;
	}
}

void CBullet::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CBullet::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CBullet::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CBullet::Render(float fTime)
{
	CObj::Render(fTime);
}

void CBullet::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::ColInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (pDest->GetName() == "PlayerBody")
	{
		CPlayer* pObj = (CPlayer*)pDest->GetObj();

		if (!pObj->IsCharged())
		{
			pObj->Charge();
			Destroy();
		}
	}
}

void CBullet::ColStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CBullet::ColEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}
