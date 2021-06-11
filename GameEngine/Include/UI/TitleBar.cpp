#include "TitleBar.h"
#include "../Input.h"
#include "../Component/ColliderRect.h"
#include "../Object/Obj.h"
#include "../UI/UIFont.h"

CTitleBar::CTitleBar()	:
	m_pRC(nullptr),
	m_vPos(),
	m_bPush(false),
	m_pTitle(nullptr)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::UI_TITLEBAR;
}

CTitleBar::CTitleBar(const CTitleBar& ui)
{
	m_vPos = ui.m_vPos;
	m_bPush = ui.m_bPush;

	m_pRC = FindComByType<CColliderRect>();
	m_pTitle = FindComByType<CUIFont>();
}

CTitleBar::~CTitleBar()
{
	SAFE_RELEASE(m_pRC);
	SAFE_RELEASE(m_pTitle);
}

void CTitleBar::SetTitle(const TCHAR* pTitle)
{
	m_pTitle->SetText(pTitle);
}

void CTitleBar::SetExtent(float x, float y)
{
	m_pRC->SetExtent(x, y);
}

void CTitleBar::SetTitleRect(float l, float t, float r, float b)
{
	m_pTitle->SetRect(l, t, r, b);
}

void CTitleBar::SetTitleAlignV(TEXT_ALIGN_VERTICAL eAlign)
{
	m_pTitle->SetAlignV(eAlign);
}

void CTitleBar::SetTitleAlignH(TEXT_ALIGN_HORISONTAL eAlign)
{
	m_pTitle->SetAlignH(eAlign);
}

bool CTitleBar::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pRC = m_pObj->CreateComponent<CColliderRect>("TitleBody", m_pLayer);

	m_pRC->SetCallBack<CTitleBar>(COLLISION_STATE::INIT, this, &CTitleBar::ColEnter);
	m_pRC->SetCallBack<CTitleBar>(COLLISION_STATE::STAY, this, &CTitleBar::ColStay);
	m_pRC->SetCallBack<CTitleBar>(COLLISION_STATE::END, this, &CTitleBar::ColEnd);

	m_pRC->SetUI();
	m_pRC->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	AddChild(m_pRC);

	m_pTitle = m_pObj->CreateComponent<CUIFont>("Title", m_pLayer);

	m_pTitle->SetInheritScale(false);
	m_pTitle->SetAlignH(TEXT_ALIGN_HORISONTAL::CENTER);
	m_pTitle->SetAlignV(TEXT_ALIGN_VERTICAL::MID);

	m_pTitle->SetPivot(0.5f, 0.5f, 0.f);

	AddChild(m_pTitle);

	return true;
}

void CTitleBar::Start()
{
	CUIControl::Start();
}

void CTitleBar::Update(float fTime)
{
	CUIControl::Update(fTime);

	if (m_bPush)
	{
		Vector2 vMove = GET_SINGLE(CInput)->GetMouseMove();

		m_pObj->AddWorldPos(vMove.x, vMove.y, 0.f);

		if (GET_SINGLE(CInput)->IsMouseUp(MOUSE_BUTTON_TYPE::LEFT))
			m_bPush = false;
	}
}

void CTitleBar::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CTitleBar::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CTitleBar::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CTitleBar::Render(float fTime)
{
	CUIControl::Render(fTime);
}

void CTitleBar::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CTitleBar* CTitleBar::Clone()
{
	return new CTitleBar(*this);
}

void CTitleBar::Save(FILE* pFile)
{
	CUIControl::Save(pFile);

	fwrite(&m_vPos, sizeof(Vector2), 1, pFile);
}

void CTitleBar::Load(FILE* pFile)
{
	CUIControl::Load(pFile);

	fread(&m_vPos, sizeof(Vector2), 1, pFile);

	m_pRC = FindComByType<CColliderRect>();
}

void CTitleBar::ColEnter(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsMouseDown(MOUSE_BUTTON_TYPE::LEFT))
		{
			m_vPos = GET_SINGLE(CInput)->GetMousePos();

			m_bPush = true;
		}
	}
}

void CTitleBar::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
	if (pDst->GetName() == "MouseBody")
	{
		if (GET_SINGLE(CInput)->IsMouseDown(MOUSE_BUTTON_TYPE::LEFT))
		{
			m_vPos = GET_SINGLE(CInput)->GetMousePos();

			m_bPush = true;
		}
	}
}

void CTitleBar::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}
