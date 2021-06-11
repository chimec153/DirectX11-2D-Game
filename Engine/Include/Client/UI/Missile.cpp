#include "Missile.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"

CMissile::CMissile()
{
}

CMissile::CMissile(const CMissile& obj)	:
	CObj(obj)
{
}

CMissile::~CMissile()
{
}

bool CMissile::Init()
{
	if (!CObj::Init())
		return false;

	CSpriteComponent* pCom = CreateComponent<CSpriteComponent>("Sprite", m_pLayer);

	pCom->SetWorldScale(32.f, 32.f, 0.f);
	pCom->SetPivot(0.5f, 0.5f, 0.f);
	pCom->CreateSprite("Fire", "Fire");
	pCom->SetInstance();

	SetRootComponent(pCom);

	CColliderCircle* pCC = CreateComponent<CColliderCircle>("MissileBody", m_pLayer);

	pCC->SetRadius(16.f);
	pCC->SetCallBack<CMissile>(COLLISION_STATE::INIT, this, &CMissile::ColEnter);
	pCC->SetCallBack<CMissile>(COLLISION_STATE::INIT, this, &CMissile::ColStay);
	pCC->SetCallBack<CMissile>(COLLISION_STATE::INIT, this, &CMissile::ColEnd);
	pCC->SetInstance();

	pCom->AddChild(pCC);

	SAFE_RELEASE(pCC);

	SAFE_RELEASE(pCom);

	return true;
}

void CMissile::Start()
{
	CObj::Start();
}

void CMissile::Update(float fTime)
{
	CObj::Update(fTime);

	Vector3 vAxis = GetWorldAxis(WORLD_AXIS::AXIS_Y);

	AddWorldPos(vAxis * 600.f * fTime);
}

void CMissile::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CMissile::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CMissile::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CMissile::Render(float fTime)
{
	CObj::Render(fTime);
}

void CMissile::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CMissile* CMissile::Clone()
{
	return new CMissile(*this);
}

void CMissile::Notify(const std::string& strKey)
{
	if (strKey == "FireEnd")
	{
		Destroy();
	}
}

void CMissile::Save(FILE* pFile)
{
	CObj::Save(pFile);
} 

void CMissile::Load(FILE* pFile)
{
	CObj::Load(pFile);
}

void CMissile::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{

}

void CMissile::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CMissile::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
