#include "MouseObj.h"
#include "UISprite.h"
#include "../Component/ColliderPoint.h"
CMouseObj::CMouseObj()	:
	m_pImage(nullptr)
{
}

CMouseObj::CMouseObj(const CMouseObj& obj)	:
	CUIObject(obj)
{
	m_pImage = (CUISprite*)FindSceneComponent("Mouse");

	m_pPC = (CColliderPoint*)FindSceneComponent("MouseBody");
}

CMouseObj::~CMouseObj()
{
	SAFE_RELEASE(m_pImage);
	SAFE_RELEASE(m_pPC);
}

bool CMouseObj::Init()
{
	if (!CUIObject::Init())
		return false;
/*
	m_pImage = CreateComponent<CUISprite>("Mouse");

	m_pImage->SetTexture("Mouse");

	m_pImage->SetPivot(0.f, 1.f, 0.f);

	m_pImage->SetWorldScale(20.f, 24.f, 0.f);*/

	//m_pImage->SetFrm(13);
	//m_pImage->SetTime(0.05f);

	m_pPC = CreateComponent<CColliderPoint>("MouseBody");
	m_pPC->SetUI();
	m_pPC->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);
	m_pPC->SetMouse();

	SetRootComponent(m_pPC);

	//m_pImage->AddChild(m_pPC);

	return true;
}

void CMouseObj::Start()
{
	CUIObject::Start();
}

void CMouseObj::Update(float fTime)
{
	CUIObject::Update(fTime);
}

void CMouseObj::PostUpdate(float fTime)
{
	CUIObject::PostUpdate(fTime);
}

void CMouseObj::Collision(float fTime)
{
	CUIObject::Collision(fTime);
}

void CMouseObj::PreRender(float fTime)
{
	CUIObject::PreRender(fTime);
}

void CMouseObj::Render(float fTime)
{
	CUIObject::Render(fTime);
}

void CMouseObj::PostRender(float fTime)
{
	CUIObject::PostRender(fTime);
}

CMouseObj* CMouseObj::Clone()
{
	return new CMouseObj(*this);
}
