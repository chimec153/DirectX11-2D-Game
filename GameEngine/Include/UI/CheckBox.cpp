#include "CheckBox.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Component/Transform.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"

CCheckBox::CCheckBox()	:
	m_pCheckTex(nullptr),
	m_bChk(false)
{
}

CCheckBox::CCheckBox(const CCheckBox& ui)	:
	CUIButton(ui)
{
	m_pCheckTex = ui.m_pCheckTex;

	if (m_pCheckTex)
		m_pCheckTex->AddRef();

	m_bChk = ui.m_bChk;
}

CCheckBox::~CCheckBox()
{
	SAFE_RELEASE(m_pCheckTex);
}

void CCheckBox::SetChkTex(const std::string& strKey)
{
	SAFE_RELEASE(m_pCheckTex);

	m_pCheckTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);
}

void CCheckBox::SetChkTex(CTexture* pTex)
{
	SAFE_RELEASE(m_pCheckTex);

	m_pCheckTex = pTex;

	if (m_pCheckTex)
		m_pCheckTex->AddRef();
}

bool CCheckBox::Init()
{
	if (!CUIButton::Init())
		return false;

	SetCallBack<CCheckBox>(BUTTON_STATE::PUSH, this, &CCheckBox::Check);

	return true;
}

void CCheckBox::Start()
{
	CUIButton::Start();
}

void CCheckBox::Update(float fTime)
{
	CUIButton::Update(fTime);
}

void CCheckBox::PostUpdate(float fTime)
{
	CUIButton::PostUpdate(fTime);
}

void CCheckBox::Collision(float fTime)
{
	CUIButton::Collision(fTime);
}

void CCheckBox::PreRender(float fTime)
{
	CUIButton::PreRender(fTime);
}

void CCheckBox::Render(float fTime)
{
	CUIButton::Render(fTime);

	if (m_bChk)
	{
		m_pTransform->SetTransform();

		m_pCheckTex->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

		//m_pMaterial->SetMaterial();

		m_pImage->Render(fTime);
	}
}

void CCheckBox::PostRender(float fTime)
{
	CUIButton::PostRender(fTime);
}

CCheckBox* CCheckBox::Clone()
{
	return new CCheckBox(*this);
}

void CCheckBox::Save(FILE* pFile)
{
	CUIButton::Save(pFile);
}

void CCheckBox::Load(FILE* pFile)
{
	CUIButton::Load(pFile);
}

void CCheckBox::Check(float fTime)
{
	m_bChk = !m_bChk;
}
