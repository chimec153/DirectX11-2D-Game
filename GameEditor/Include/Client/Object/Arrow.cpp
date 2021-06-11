#include "Arrow.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Resource/Material.h"

CArrow::CArrow()	:
	m_pMesh(nullptr)
{
}

CArrow::CArrow(const CArrow& arrow)	:
	CObj(arrow)
{
	m_pMesh = arrow.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CArrow::~CArrow()
{
	SAFE_RELEASE(m_pMesh);
}

void CArrow::ChangeClip(const std::string& strTag)
{
	m_pMesh->ChangeSequence(strTag);
}

void CArrow::SetDefaultClip(const std::string& strTag)
{
	m_pMesh->SetDefaultSeq(strTag);
}

void CArrow::ReturnClip()
{
	m_pMesh->ReturnClip();
}

bool CArrow::Init()
{
	if (!CObj::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("Arrow", m_pLayer);

	m_pMesh->SetWorldScale(16.f, 16.f, 1.f);
	m_pMesh->CreateSprite("IdleU",	"ArrowIdleU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRU", "ArrowIdleRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleR",	"ArrowIdleR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRD", "ArrowIdleRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleD",	"ArrowIdleD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkU",	"ArrowWalkU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRU", "ArrowWalkRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkR",	"ArrowWalkR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRD", "ArrowWalkRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkD",	"ArrowWalkD", LOOP_OPTION::LOOP);	
	m_pMesh->CreateSprite("RunU",	"ArrowRunU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRU", "ArrowRunRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunR",	"ArrowRunR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRD", "ArrowRunRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunD",	"ArrowRunD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackU",	"ArrowAttackU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRU",	"ArrowAttackRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackR",	"ArrowAttackR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRD",	"ArrowAttackRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackD",	"ArrowAttackD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallU",	"ArrowCallU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRU", "ArrowCallRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallR",	"ArrowCallR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRD", "ArrowCallRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallD",	"ArrowCallD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollU",	"ArrowRollU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRU", "ArrowRollRU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollR",	"ArrowRollR", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRD", "ArrowRollRD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollD",	"ArrowRollD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("SwimU", "ArrowSwimU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRU", "ArrowSwimRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimR", "ArrowSwimR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRD", "ArrowSwimRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimD", "ArrowSwimD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieU",	"ArrowDieU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRU",	"ArrowDieRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieR",	"ArrowDieR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRD",	"ArrowDieRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieD",	"ArrowDieD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Turn", "ArrowTurn", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("TurnIdle", "ArrowTurnIdle", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Randing", "ArrowRanding", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RandingIdle", "ArrowRandingIdle", LOOP_OPTION::LOOP);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pMesh);
	m_pMesh->AddRenderState("Character");
	m_pMesh->AddRenderState("NoCullBack");
	m_pMesh->AddRenderState("AlphaBlend");

	CMaterial* pMat = m_pMesh->GetMaterial();

	//pMat->SetGray(false);

	SAFE_RELEASE(pMat);

	return true;
}

void CArrow::Start()
{
	CObj::Start();
}

void CArrow::Update(float fTime)
{
	CObj::Update(fTime);
}

void CArrow::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CArrow::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CArrow::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CArrow::Render(float fTime)
{
	CObj::Render(fTime);
}

void CArrow::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CArrow* CArrow::Clone()
{
	return new CArrow(*this);
}
