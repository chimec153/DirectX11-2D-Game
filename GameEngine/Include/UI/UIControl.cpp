#include "UIControl.h"
#include "UITransform.h"
#include "../Resource/Material.h"
#include "../Resource/Texture.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../RenderManager.h"

CUIControl::CUIControl() :
	m_bStart(false),
	m_pViewPort(nullptr),
	m_pTexture(nullptr),
	m_pImage(nullptr),
	m_iZOrder(0)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
}

CUIControl::CUIControl(const CUIControl& con)	:
	CSceneComponent(con)
{
	m_pViewPort = con.m_pViewPort;

	m_pImage = con.m_pImage;

	if (m_pImage)
		m_pImage->AddRef();

	m_pTexture = con.m_pTexture;

	if (m_pTexture)
		m_pTexture->AddRef();
}

CUIControl::~CUIControl()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pImage);
}

void CUIControl::SetTexture(const std::string& strTex)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTex);
}

void CUIControl::SetTexture(CTexture* pTex)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = pTex;

	if (m_pTexture)
		m_pTexture->AddRef();
}

void CUIControl::SetZOrder(int iZ)
{
	m_iZOrder = iZ;
}

int CUIControl::GetZOrder() const
{
	return m_iZOrder;
}

bool CUIControl::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CUIControl::Start()
{
	CSceneComponent::Start();
}

void CUIControl::Input(float fTime)
{
}

void CUIControl::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CUIControl::PostUpdate(float fTime)
{
	//GET_SINGLE(CRenderManager)->AddCon(this);
	CSceneComponent::PostUpdate(fTime);

}

void CUIControl::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CUIControl::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CUIControl::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CUIControl::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

void CUIControl::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

}

void CUIControl::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

}
