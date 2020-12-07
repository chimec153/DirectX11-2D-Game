#include "Bow.h"
#include "Component/SpriteComponent.h"

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

	m_pMesh = CreateComponent<CSpriteComponent>("Bow");

	m_pMesh->SetWorldScale(16.f, 16.f, 1.f);
	m_pMesh->CreateSprite("IdleU",	"BowIdleU", true);
	m_pMesh->CreateSprite("IdleRU", "BowIdleRU", true);
	m_pMesh->CreateSprite("IdleR",	"BowIdleR", true);
	m_pMesh->CreateSprite("IdleRD", "BowIdleRD", true);
	m_pMesh->CreateSprite("IdleD",	"BowIdleD", true);
	m_pMesh->CreateSprite("WalkU",	"BowWalkU", true);
	m_pMesh->CreateSprite("WalkRU", "BowWalkRU", true);
	m_pMesh->CreateSprite("WalkR",	"BowWalkR", true);
	m_pMesh->CreateSprite("WalkRD", "BowWalkRD", true);
	m_pMesh->CreateSprite("WalkD",	"BowWalkD", true);
	m_pMesh->CreateSprite("AttackU",	"BowAttackU", true);
	m_pMesh->CreateSprite("AttackRU",	"BowAttackRU", true);
	m_pMesh->CreateSprite("AttackR",	"BowAttackR", true);
	m_pMesh->CreateSprite("AttackRD",	"BowAttackRD", true);
	m_pMesh->CreateSprite("AttackD",	"BowAttackD", true);
	m_pMesh->CreateSprite("CallU",	"BowCallU", true);
	m_pMesh->CreateSprite("CallRU", "BowCallRU", true);
	m_pMesh->CreateSprite("CallR",	"BowCallR", true);
	m_pMesh->CreateSprite("CallRD", "BowCallRD", true);
	m_pMesh->CreateSprite("CallD",	"BowCallD", true);
	m_pMesh->CreateSprite("RollU",	"BowRollU", true);
	m_pMesh->CreateSprite("RollRU", "BowRollRU", true);
	m_pMesh->CreateSprite("RollR",	"BowRollR", true);
	m_pMesh->CreateSprite("RollRD", "BowRollRD", true);
	m_pMesh->CreateSprite("RollD",	"BowRollD", true);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);
	m_pMesh->AddRenderState("Character");

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
