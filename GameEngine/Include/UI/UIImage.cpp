#include "UIImage.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"

CUIImage::CUIImage()
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::UI_IMAGE;
}

CUIImage::CUIImage(const CUIImage& ui)	:
	CUIControl(ui)
{
}

CUIImage::~CUIImage()
{
}

bool CUIImage::Init()
{
	if (!CUIControl::Init())
		return false;

	m_pImage = GET_SINGLE(CResourceManager)->GetUIMesh();

	//m_pMaterial = m_pImage->GetMaterial();

	return true;
}

void CUIImage::Start()
{
	CUIControl::Start();
}

void CUIImage::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CUIImage::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CUIImage::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CUIImage::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CUIImage::Render(float fTime)
{
	CUIControl::Render(fTime);

	//m_pMaterial->SetMaterial();

	m_pTexture->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	m_pImage->Render(0.f);
}

void CUIImage::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}

void CUIImage::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CUIImage::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
