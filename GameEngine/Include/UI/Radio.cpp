#include "Radio.h"
#include "../Resource/Texture.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Component/Transform.h"

CRadio::CRadio()	:
	m_bSel(false),
	m_pSelTex(nullptr)
{
}

CRadio::CRadio(const CRadio& ui)	:
	CUIButton(ui)
{
	m_bSel = false;

	m_pSelTex = ui.m_pSelTex;

	if (m_pSelTex)
		m_pSelTex->AddRef();
}

CRadio::~CRadio()
{
	SAFE_RELEASE(m_pSelTex);
}

void CRadio::SetSelTexture(const std::string& strKey)
{
	SAFE_RELEASE(m_pSelTex);

	m_pSelTex = GET_SINGLE(CResourceManager)->FindTexture(strKey);
}

void CRadio::SetSelTexture(CTexture* pTex)
{
	SAFE_RELEASE(m_pSelTex);

	m_pSelTex = pTex;

	if (m_pSelTex)
		m_pSelTex->AddRef();
}

void CRadio::AddGroup(CRadio* pRadio)
{
	size_t iSz = m_vecRadio.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRadio[i]->m_vecRadio.push_back(pRadio);
		pRadio->m_vecRadio.push_back(m_vecRadio[i]);
	}

	m_vecRadio.push_back(pRadio);
	pRadio->m_vecRadio.push_back(this);
}

bool CRadio::Init()
{
	if (!CUIButton::Init())
		return false;

	SetCallBack<CRadio>(BUTTON_STATE::PUSH, this, &CRadio::Select);

	return true;
}

void CRadio::Start()
{
	CUIButton::Start();
}

void CRadio::Update(float fTime)
{
	CUIButton::Update(fTime);
}

void CRadio::PostUpdate(float fTime)
{
	CUIButton::PostUpdate(fTime);
}

void CRadio::Collision(float fTime)
{
	CUIButton::Collision(fTime);
}

void CRadio::PreRender(float fTime)
{
	CUIButton::PreRender(fTime);
}

void CRadio::Render(float fTime)
{
	CUIButton::Render(fTime);

	if (m_bSel)
	{
		m_pTransform->SetTransform();

		m_pSelTex->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

		//m_pMaterial->SetMaterial();

		m_pImage->Render(fTime);
	}
}

void CRadio::PostRender(float fTime)
{
	CUIButton::PostRender(fTime);
}

CRadio* CRadio::Clone()
{
	return new CRadio(*this);
}

void CRadio::Save(FILE* pFile)
{
	CUIButton::Save(pFile);
}

void CRadio::Load(FILE* pFile)
{
	CUIButton::Load(pFile);
}

void CRadio::Select(float fTime)
{
	if (!m_bSel)
	{
		m_bSel = true;

		size_t iSz = m_vecRadio.size();

		for (size_t i = 0; i < iSz; ++i)
			m_vecRadio[i]->DeSel();
	}
}

void CRadio::DeSel()
{
	m_bSel = false;
}
