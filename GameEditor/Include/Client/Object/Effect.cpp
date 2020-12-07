#include "Effect.h"
#include "Component/SpriteComponent.h"

CEffect::CEffect()
{
}

CEffect::CEffect(const CEffect& eft)	:
	CObj(eft)
{
}

CEffect::~CEffect()
{
}

bool CEffect::Init()
{
	if (!CObj::Init())
		return false;

	CSpriteComponent* pCom = CreateComponent<CSpriteComponent>("Effect");

	pCom->CreateSprite("Fire", "Fire", true);
	pCom->SetPivot(0.5f, 0.5f, 0.f);
	pCom->SetWorldScale(16.f, 16.f, 1.f);

	SetRootComponent(pCom);

	SAFE_RELEASE(pCom);

	return true;
}

void CEffect::Start()
{
	CObj::Start();
}

void CEffect::Update(float fTime)
{
	CObj::Update(fTime);

	AddWorldPos(0.f, 100.f * fTime, 0.f);
}

void CEffect::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CEffect::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CEffect::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CEffect::Render(float fTime)
{
	CObj::Render(fTime);
}

void CEffect::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CEffect* CEffect::Clone()
{
	return new CEffect(*this);
}

void CEffect::Notify(const std::string& strTag)
{
	if (strTag == "FireEnd")
		Destroy();
}

void CEffect::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CEffect::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
