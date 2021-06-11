#include "Bar.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"

CBar::CBar()	:
	m_fPercent(1.f),
	m_tCBuffer(),
	m_eDir(BAR_DIR::R)
{
	m_eSceneComponentType = SCENE_COMPONENT_TYPE::SCT_UI;
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::UI_BAR;
}

CBar::CBar(const CBar& ui)	:
	CUIControl(ui)
{
	m_fPercent = ui.m_fPercent;
	m_tCBuffer = ui.m_tCBuffer;
	m_eDir = ui.m_eDir;
}

CBar::~CBar()
{
}

float CBar::GetPercent() const
{
	return m_fPercent;
}

void CBar::SetPercent(float f)
{
	m_fPercent = f;
}

bool CBar::Init()
{
	if (!CUIControl::Init())
		return false;

	CMesh* pImage = GET_SINGLE(CResourceManager)->GetUIMesh();

	SetMesh(pImage);

	SAFE_RELEASE(pImage);

	/*CMaterial* pMtrl = m_pImage->GetMaterial();

	if (pMtrl)
	{
		m_pMaterial = pMtrl->Clone();

		pMtrl->Release();
	}*/

	SetShader("Bar");

	return true;
}

void CBar::Start()
{
	CUIControl::Start();
}

void CBar::Update(float fTime)
{
	CUIControl::Update(fTime);
}

void CBar::PostUpdate(float fTime)
{
	CUIControl::PostUpdate(fTime);
}

void CBar::Collision(float fTime)
{
	CUIControl::Collision(fTime);
}

void CBar::PreRender(float fTime)
{
	CUIControl::PreRender(fTime);
}

void CBar::Render(float fTime)
{
	CUIControl::Render(fTime);

	m_tCBuffer.fPercent = m_fPercent;
	m_tCBuffer.iDir = (int)m_eDir;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Bar", &m_tCBuffer);

	if (m_pTexture)
		m_pTexture->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	m_pImage->Render(fTime);
}

void CBar::PostRender(float fTime)
{
	CUIControl::PostRender(fTime);
}

CBar* CBar::Clone()
{
	return new CBar(*this);
}

void CBar::Save(FILE* pFile)
{
	CUIControl::Save(pFile);
}

void CBar::Load(FILE* pFile)
{
	CUIControl::Load(pFile);
}
