#include "Arrow.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

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

	m_pMesh = CreateComponent<CSpriteComponent>("Arrow");

	m_pMesh->SetWorldScale(16.f, 16.f, 1.f);
	m_pMesh->CreateSprite("IdleU",	"ArrowIdleU", true);
	m_pMesh->CreateSprite("IdleRU", "ArrowIdleRU", true);
	m_pMesh->CreateSprite("IdleR",	"ArrowIdleR", true);
	m_pMesh->CreateSprite("IdleRD", "ArrowIdleRD", true);
	m_pMesh->CreateSprite("IdleD",	"ArrowIdleD", true);
	m_pMesh->CreateSprite("WalkU",	"ArrowWalkU", true);
	m_pMesh->CreateSprite("WalkRU", "ArrowWalkRU", true);
	m_pMesh->CreateSprite("WalkR",	"ArrowWalkR", true);
	m_pMesh->CreateSprite("WalkRD", "ArrowWalkRD", true);
	m_pMesh->CreateSprite("WalkD",	"ArrowWalkD", true);
	m_pMesh->CreateSprite("AttackU",	"ArrowAttackU", true);
	m_pMesh->CreateSprite("AttackRU",	"ArrowAttackRU", true);
	m_pMesh->CreateSprite("AttackR",	"ArrowAttackR", true);
	m_pMesh->CreateSprite("AttackRD",	"ArrowAttackRD", true);
	m_pMesh->CreateSprite("AttackD",	"ArrowAttackD", true);
	m_pMesh->CreateSprite("CallU",	"ArrowCallU", true);
	m_pMesh->CreateSprite("CallRU", "ArrowCallRU", true);
	m_pMesh->CreateSprite("CallR",	"ArrowCallR", true);
	m_pMesh->CreateSprite("CallRD", "ArrowCallRD", true);
	m_pMesh->CreateSprite("CallD",	"ArrowCallD", true);
	m_pMesh->CreateSprite("RollU",	"ArrowRollU", true);
	m_pMesh->CreateSprite("RollRU", "ArrowRollRU", true);
	m_pMesh->CreateSprite("RollR",	"ArrowRollR", true);
	m_pMesh->CreateSprite("RollRD", "ArrowRollRD", true);
	m_pMesh->CreateSprite("RollD",	"ArrowRollD", true);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	SetRootComponent(m_pMesh);
	m_pMesh->AddRenderState("Character");

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
