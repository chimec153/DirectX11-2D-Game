#include "Minion.h"
#include "Component/SpriteComponent.h"
#include "IceBall.h"
#include "Scene/Scene.h"
#include "Component/Mesh2DComponent.h"
#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/ColliderLine.h"
#include "Component/Sound.h"

CMinion::CMinion()	:
	m_pMesh(nullptr),
	m_fFireLimitTime(0.6f),
	m_fTime(0.f),
	m_iPhase(0),
	m_bIdleEnable(true),
	m_bRolling(false),
	m_fDist(0.f),
	m_vDir(),
	m_fSpeed(500.f),
	m_fTargetAngle(0.f)
{
}

CMinion::CMinion(const CMinion& min)	:
	CObj(min)
{
}

CMinion::~CMinion()
{
	SAFE_RELEASE(m_pMesh);
}

bool CMinion::Init()
{
	if (!CObj::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("Sprite", m_pLayer);

	SetRootComponent(m_pMesh);

	m_pMesh->SetWorldPos(200.f, 10.f, 0.f);
	m_pMesh->SetRelativeScale(96.f, 96.f, 0.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.5f);

	m_pMesh->CreateSprite("IdleR", "YetiIdleR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleD", "YetiIdleD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleU", "YetiIdleU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRU", "YetiIdleRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRD", "YetiIdleRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollR", "YetiRollR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollD", "YetiRollD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollU", "YetiRollU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollRU", "YetiRollRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollRD", "YetiRollRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackR", "YetiAttackR");
	m_pMesh->CreateSprite("AttackD", "YetiAttackD");
	m_pMesh->CreateSprite("AttackU", "YetiAttackU");
	m_pMesh->CreateSprite("AttackRU", "YetiAttackRU");
	m_pMesh->CreateSprite("AttackRD", "YetiAttackRD");
	m_pMesh->AddNotify("AttackR", "Fire", 2);
	m_pMesh->AddNotify("AttackD", "Fire", 2);
	m_pMesh->AddNotify("AttackU", "Fire", 2);
	m_pMesh->AddNotify("AttackRU", "Fire", 2);
	m_pMesh->AddNotify("AttackRD", "Fire", 2);

	m_pMesh->SetEndFunc<CMinion>("AttackR", this, &CMinion::AttackEnd);
	m_pMesh->SetEndFunc<CMinion>("AttackD", this, &CMinion::AttackEnd);
	m_pMesh->SetEndFunc<CMinion>("AttackU", this, &CMinion::AttackEnd);
	m_pMesh->SetEndFunc<CMinion>("AttackRU", this, &CMinion::AttackEnd);
	m_pMesh->SetEndFunc<CMinion>("AttackRD", this, &CMinion::AttackEnd);

	CColliderRect* pRC = CreateComponent<CColliderRect>("YetiBody", m_pLayer);

	pRC->SetExtent(96.f, 96.f);
	pRC->SetProfile("Base");

	m_pMesh->AddChild(pRC);

	SAFE_RELEASE(pRC);

	CSound* pSound = CreateComponent<CSound>("Sound", m_pLayer);

	pSound->SetSound("Slam");

	m_pMesh->AddChild(pSound);

	SAFE_RELEASE(pSound);

	/*CColliderOBB2D* pOC = CreateComponent<CColliderOBB2D>("MinionBody");

	pOC->SetExtend(500.f, 50.f, 1.f);

	m_pMesh->AddChild(pOC);

	SAFE_RELEASE(pOC);*/
/*
	CColliderCircle* pCC = CreateComponent<CColliderCircle>("MinionBody");

	pCC->SetRadius(50.f);

	m_pMesh->AddChild(pCC);

	SAFE_RELEASE(pCC);*/
	/*
	CColliderPixel* pPC = CreateComponent<CColliderPixel>("MinBody");

	pPC->SetPixel(TEXT("Animals.png"));
	pPC->SetAlpha(125);
	pPC->SetPivot(0.5f, 0.5f, 0.f);

	m_pMesh->AddChild(pPC);

	SAFE_RELEASE(pPC);*/
	/*
	CColliderLine* pLC = CreateComponent<CColliderLine>("MinBody");

	pLC->SetWorldScale(50.f, 50.f, 1.f);

	m_pMesh->AddChild(pLC);

	SAFE_RELEASE(pLC);*/

	CSpriteComponent* pShadow = CreateComponent<CSpriteComponent>("Shadow", m_pLayer);

	pShadow->SetInheritScale(false);
	pShadow->SetRelativeScale(80.f, 32.f, 1.f);
	pShadow->CreateSprite("Shadow", "YetiShadow", LOOP_OPTION::LOOP);
	pShadow->SetRelativePos(0.f, -36.f, 0.f);
	pShadow->SetPivot(0.5f, 0.5f, 0.f);

	m_pMesh->AddChild(pShadow);

	SAFE_RELEASE(pShadow);

	return true;
}

void CMinion::Input(float fTime)
{
	CObj::Input(fTime);
}

void CMinion::Update(float fTime)
{
	CObj::Update(fTime);

	CObj* pObj = m_pScene->GetGameMode()->GetPlayer();
	
	Vector3 vRot;

	if(pObj)
		vRot = pObj->GetWorldPos() - GetWorldPos();
	vRot.Normalize();

	Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);

	Vector3 vCross = vAxis.Cross(vRot);

	float m_fTargetAngle = acosf(vRot.Dot(vAxis));

	if (m_fTargetAngle < PI && m_fTargetAngle > 0)
	{
		if (vCross.z < 0.f)
			m_fTargetAngle *= -1.f;
	}

	SAFE_RELEASE(pObj);

	if (GetVelocityAmt() == 0.f && !m_bRolling && m_bIdleEnable)
	{
		if (m_fTargetAngle > -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 / 2.f)
		{
			m_pMesh->ChangeSequence("IdleU");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle < -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle > -DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleRU");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleR");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 7.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 5.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleRD");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 7.f / 2.f || m_fTargetAngle < -DirectX::XM_PIDIV4 * 7.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleD");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleRU");
			m_pMesh->SetWorldRotY(180.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 3.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 5.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleR");
			m_pMesh->SetWorldRotY(180.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 7.f / 2.f)
		{
			m_pMesh->ChangeSequence("IdleRD");
			m_pMesh->SetWorldRotY(180.f);
		}
	}

	m_fTime += fTime;

	if (m_fTime >= m_fFireLimitTime && !m_bRolling)
	{
		m_fTime -= m_fFireLimitTime;

		CSound* pSound = FindComByType<CSound>();

		pSound->Play(fTime);

		SAFE_RELEASE(pSound);

		if (m_fTargetAngle > -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 / 2.f)
		{
			m_pMesh->ChangeSequence("AttackU");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle < -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle > -DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackRU");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackR");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 7.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 5.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackRD");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 7.f / 2.f || m_fTargetAngle < -DirectX::XM_PIDIV4 * 7.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackD");
			m_pMesh->SetWorldRotY(0.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 3.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackRU");
			m_pMesh->SetWorldRotY(180.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 3.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 5.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackR");
			m_pMesh->SetWorldRotY(180.f);
		}

		else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 7.f / 2.f)
		{
			m_pMesh->ChangeSequence("AttackRD");
			m_pMesh->SetWorldRotY(180.f);
		}
		m_bIdleEnable = false;
		m_iPhase += 1;
	}

	if (m_iPhase >= 4 && !m_bRolling && m_bIdleEnable)
		RollToPlayer();

	else if (m_bRolling)
	{
		m_iPhase = 0;
		AddWorldPos(m_vDir * fTime * m_fSpeed);

		m_fDist += m_fSpeed * fTime;

		if (m_fDist > 900.f)
		{
			m_fDist -= 900.f;

			m_bRolling = false;

			m_vDir = Vector3::Zero;

			m_iPhase = 0;
		}
	}
}

void CMinion::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CMinion::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CMinion::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CMinion::Render(float fTime)
{
	CObj::Render(fTime);
}

void CMinion::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CMinion* CMinion::Clone()
{
	return new CMinion(*this);
}

void CMinion::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CMinion::Load(FILE* pFile)
{
	CObj::Load(pFile);
}

void CMinion::AttackEnd()
{
	m_bIdleEnable = false;
}

void CMinion::Notify(const std::string& strTag)
{
	if (strTag == "Fire")
	{
		Fire();
		m_bIdleEnable = true;
		m_pMesh->ChangeSequence("Idle");
	}
}

void CMinion::Fire()
{
	CIceBall* pIce = static_cast<CIceBall*>(m_pScene->CreateCloneObj("Ice", "Ice",m_pLayer, m_pScene->GetSceneType()));

	CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

	Vector3 vPos;

	if(pObj)
		vPos = pObj->GetWorldPos();

	SAFE_RELEASE(pObj);

	Vector3 vDir = vPos - GetWorldPos();
	
	vDir.Normalize();

	pIce->SetWorldPos(GetWorldPos());
	pIce->SetDir(vDir);

	pIce->AddWorldPos(vDir * (pIce->GetWorldScale().y * pIce->GetPivot().y + GetWorldScale().y * GetPivot().y));

	SAFE_RELEASE(pIce);
}

void CMinion::RollToPlayer()
{

	CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

	Vector3 vRot;
	Vector3 vPos;

	if (pObj)
	{
		vRot = pObj->GetWorldPos() - GetWorldPos(); 
		vPos = pObj->GetWorldPos();
	}
	vRot.Normalize();

	Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);

	Vector3 vCross = vAxis.Cross(vRot);

	float m_fTargetAngle = acosf(vRot.Dot(vAxis));

	if (m_fTargetAngle < PI && m_fTargetAngle > 0)
	{
		if (vCross.z < 0.f)
			m_fTargetAngle *= -1.f;
	}

	if (m_fTargetAngle > -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 / 2.f)
	{
		m_pMesh->ChangeSequence("RollU");
		m_pMesh->SetWorldRotY(0.f);
	}

	else if (m_fTargetAngle < -DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle > -DirectX::XM_PIDIV4 * 3.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollRU");
		m_pMesh->SetWorldRotY(0.f);
	}

	else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 3.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollR");
		m_pMesh->SetWorldRotY(0.f);
	}

	else if (m_fTargetAngle > -DirectX::XM_PIDIV4 * 7.f / 2.f && m_fTargetAngle < -DirectX::XM_PIDIV4 * 5.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollRD");
		m_pMesh->SetWorldRotY(0.f);
	}

	else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 7.f / 2.f || m_fTargetAngle < -DirectX::XM_PIDIV4 * 7.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollD");
		m_pMesh->SetWorldRotY(0.f);
	}

	else if (m_fTargetAngle > DirectX::XM_PIDIV4 / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 3.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollRU");
		m_pMesh->SetWorldRotY(180.f);
	}

	else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 3.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 5.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollR");
		m_pMesh->SetWorldRotY(180.f);
	}

	else if (m_fTargetAngle > DirectX::XM_PIDIV4 * 5.f / 2.f && m_fTargetAngle < DirectX::XM_PIDIV4 * 7.f / 2.f)
	{
		m_pMesh->ChangeSequence("RollRD");
		m_pMesh->SetWorldRotY(180.f);
	}

	SAFE_RELEASE(pObj);

	m_vDir = vPos - GetWorldPos();

	m_vDir.Normalize();

	m_bRolling = true;
}
