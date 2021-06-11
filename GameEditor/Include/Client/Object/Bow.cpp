#include "Bow.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"

CBow::CBow()	:
	m_pMesh(nullptr)
{
}

CBow::CBow(const CBow& bow)	:
	CObj(bow)
{
	m_pMesh = bow.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

CBow::~CBow()
{
	SAFE_RELEASE(m_pMesh);
}

void CBow::ChangeClip(const std::string& strTag)
{
	m_pMesh->ChangeSequence(strTag);
}

void CBow::SetDefaultClip(const std::string& strTag)
{
	m_pMesh->SetDefaultSeq(strTag);
}

void CBow::ReturnClip()
{
	m_pMesh->ReturnClip();
}

bool CBow::Init()
{
	if (!CObj::Init())
		return false;

	m_pMesh = CreateComponent<CSpriteComponent>("Bow", m_pLayer);

	m_pMesh->SetWorldScale(16.f, 16.f, 1.f);
	m_pMesh->CreateSprite("IdleU",	"BowIdleU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRU", "BowIdleRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleR",	"BowIdleR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleRD", "BowIdleRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("IdleD",	"BowIdleD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkU",	"BowWalkU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRU", "BowWalkRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkR",	"BowWalkR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkRD", "BowWalkRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("WalkD",	"BowWalkD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunU",	"BowRunU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRU",	"BowRunRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunR",	"BowRunR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunRD",	"BowRunRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RunD",	"BowRunD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackU",	"BowAttackU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRU",	"BowAttackRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackR",	"BowAttackR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackRD",	"BowAttackRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("AttackD",	"BowAttackD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallU",	"BowCallU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRU", "BowCallRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallR",	"BowCallR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallRD", "BowCallRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("CallD",	"BowCallD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("RollU",	"BowRollU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRU", "BowRollRU", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollR",	"BowRollR", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollRD", "BowRollRD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RollD",	"BowRollD", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("SwimU", "BowSwimU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRU", "BowSwimRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimR", "BowSwimR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimRD", "BowSwimRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("SwimD", "BowSwimD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieU",	"BowDieU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRU",	"BowDieRU", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieR",	"BowDieR", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieRD",	"BowDieRD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("DieD",	"BowDieD", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Turn", "BowTurn", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("TurnIdle", "BowTurnIdle", LOOP_OPTION::LOOP);
	m_pMesh->CreateSprite("Randing", "BowRanding", LOOP_OPTION::ONCE_RETURN);
	m_pMesh->CreateSprite("RandingIdle", "BowRandingIdle", LOOP_OPTION::LOOP);

	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pMesh->AddRenderState("Character");
	m_pMesh->AddRenderState("NoCullBack");
	m_pMesh->AddRenderState("AlphaBlend");

	CMaterial* pMat = m_pMesh->GetMaterial();

	//pMat->SetGray(false);

	SAFE_RELEASE(pMat);

	SetRootComponent(m_pMesh);

	return true;
}

void CBow::Start()
{
	CObj::Start();
}

void CBow::Update(float fTime)
{
	CObj::Update(fTime);
}

void CBow::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CBow::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CBow::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CBow::Render(float fTime)
{
	CObj::Render(fTime);
}

void CBow::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CBow* CBow::Clone()
{
	return new CBow(*this);
}
