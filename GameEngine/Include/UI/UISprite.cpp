#include "UISprite.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"

CUISprite::CUISprite()	:
	m_fTime(0.f),
	m_fFMT(0.f),
	m_iFrm(0),
	m_iMF(0)
{
}

CUISprite::CUISprite(const CUISprite& ui)	:
	CUIControl(ui)
{
	m_fTime = 0.f;
	m_fFMT = ui.m_fFMT;
	m_iFrm = 0;
	m_iMF = ui.m_iMF;
}

CUISprite::~CUISprite()
{
}

void CUISprite::SetFrm(int iFrm)
{
	m_iMF = iFrm;
}

void CUISprite::SetTime(float fTime)
{
	m_fFMT = fTime;
}

bool CUISprite::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pImage = GET_SINGLE(CResourceManager)->GetUIMesh();

	m_pMaterial = m_pImage->GetMaterial();

	m_tCBuffer.iAniType = (int)IMAGE_TYPE::IT_ARRAY;

	return true;
}

void CUISprite::Start()
{
	CUIControl::Start();
}

void CUISprite::Update(float fTime)
{
	CUIControl::Update(fTime);

	m_fTime += fTime;

	if (m_fTime >= m_fFMT)
	{
		m_fTime -= m_fFMT;

		++m_iFrm;

		if (m_iFrm >= m_iMF)
		{
			m_iFrm = 0;
		}
	}
}

void CUISprite::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);

	m_tCBuffer.iImageType = m_iFrm;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);
}

void CUISprite::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUISprite::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUISprite::Render(float fTime)
{
	CUIControl::Render(fTime);

	m_pMaterial->SetMaterial();

	m_pTexture->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, m_iFrm);

	m_pImage->Render(0.f);
}

void CUISprite::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CUISprite* CUISprite::Clone()
{
	return new CUISprite(*this);
}

void CUISprite::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUISprite::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
