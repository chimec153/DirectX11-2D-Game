#include "Effect.h"
#include "Component/SpriteComponent.h"
#include "Resource/Material.h"

CEffect::CEffect()	:
	m_fSpeed(100.f),
	m_vScaleSpeed(),
	m_fAlphaSpeed(0.f)
{
}

CEffect::CEffect(const CEffect& eft)	:
	CObj(eft)
{
	m_fSpeed = eft.m_fSpeed;
	m_vScaleSpeed = eft.m_vScaleSpeed;
	m_fAlphaSpeed = eft.m_fAlphaSpeed;
}

CEffect::~CEffect()
{
}

void CEffect::SetScaleCallBack(float)
{
	SetScaleSpeed(Vector3(-32.f, -32.f, 0.f));
}

bool CEffect::Init()
{
	if (!CObj::Init())
		return false;

	return true;
}

void CEffect::Start()
{
	CObj::Start();
}

void CEffect::Update(float fTime)
{
	CObj::Update(fTime);

	AddWorldPos(0.f, m_fSpeed * fTime, 0.f);

	AddWorldScale(m_vScaleSpeed * fTime);

	if (m_fAlphaSpeed != 0.f)
	{
		CSpriteComponent* pCom = FindComByType<CSpriteComponent>();

		CMaterial* pMtrl = pCom->GetMaterial();

		Vector4 vDif = pMtrl->GetDif();

		vDif.w -= fTime * m_fAlphaSpeed;

		pMtrl->SetDiffuseColor(vDif);

		SAFE_RELEASE(pMtrl);

		SAFE_RELEASE(pCom);
	}
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
		CRef::Destroy();
}

void CEffect::Destroy(float fTime)
{
	CRef::Destroy();
}

void CEffect::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CEffect::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
