#include "pch.h"
#include "Light.h"
#include "Component/Line.h"

CLight::CLight()
{
}

CLight::CLight(const CLight& obj)	:
	CObj(obj)
{
}

CLight::~CLight()
{
}

bool CLight::Init()
{
	if (!CObj::Init())
		return false;

	CLine* pLine = CreateComponent<CLine>("Line", m_pLayer);

	SetRootComponent(pLine);

	SAFE_RELEASE(pLine);

	return true;
}

void CLight::Start()
{
	CObj::Start();
}

void CLight::Update(float fTime)
{
	CObj::Update(fTime);
}

void CLight::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CLight::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CLight::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CLight::Render(float fTime)
{
	CObj::Render(fTime);
}

void CLight::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CLight* CLight::Clone()
{
	return new CLight(*this);
}

void CLight::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CLight::Load(FILE* pFile)
{
	CObj::Load(pFile);
}
