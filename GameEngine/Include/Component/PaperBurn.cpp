#include "PaperBurn.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Texture.h"

CPaperBurn::CPaperBurn()	:
	m_tCBuffer()
	, m_bReverse(false)
{
}

CPaperBurn::CPaperBurn(const CPaperBurn& burn)	:
	CComponent(burn)
	, m_tCBuffer(burn.m_tCBuffer)
	, m_bReverse(burn.m_bReverse)
{
}

CPaperBurn::~CPaperBurn()
{
}

void CPaperBurn::SetInFilter(float f)
{
	m_tCBuffer.fInFilter = f;
}

void CPaperBurn::SetMidFilter(float f)
{
	m_tCBuffer.fMidFilter = f;
}

void CPaperBurn::SetOutFilter(float f)
{
	m_tCBuffer.fOutFilter = f;
}

void CPaperBurn::SetInColor(float r, float g, float b, float a)
{
	m_tCBuffer.vInColor = Vector4(r,g,b,a);
}

void CPaperBurn::SetOutColor(float r, float g, float b, float a)
{
	m_tCBuffer.vOutColor = Vector4(r,g,b,a);
}

void CPaperBurn::Reset()
{
	m_bReverse ^= true;
}

bool CPaperBurn::Init()
{
	if (!CComponent::Init())
		return false;

	if (CResourceManager::GetInst()->LoadTexture("Paper", TEXT("DefaultBurn.png")))
	{
		CTexture* pTex = CResourceManager::GetInst()->FindTexture("Paper");

		pTex->SetTexture(90, static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));

		SAFE_RELEASE(pTex);
	}

	return true;
}

void CPaperBurn::Start()
{
	CComponent::Start();
}

void CPaperBurn::Update(float fTime)
{
	CComponent::Update(fTime);

	m_tCBuffer.fFilter += fTime * (1 + m_bReverse * -2);

	if (m_tCBuffer.fFilter > 1.f + m_tCBuffer.fOutFilter)
	{
		m_tCBuffer.fFilter = 1.f + m_tCBuffer.fOutFilter;
	}

	else if (m_tCBuffer.fFilter < 0.f)
	{
		m_tCBuffer.fFilter = 0.;
	}
}

void CPaperBurn::PostUpdate(float fTime)
{
	CComponent::PostUpdate(fTime);
}

void CPaperBurn::Collision(float fTime)
{
	CComponent::Collision(fTime);
}

void CPaperBurn::PreRender(float fTime)
{
	CComponent::PreRender(fTime);
}

void CPaperBurn::Render(float fTime)
{
	CComponent::Render(fTime);

	CShaderManager::GetInst()->UpdateCBuffer("Paper", &m_tCBuffer);
}

void CPaperBurn::PostRender(float fTime)
{
	CComponent::PostRender(fTime);
}

CPaperBurn* CPaperBurn::Clone()
{
	return new CPaperBurn(*this);
}

void CPaperBurn::Save(FILE* pFile)
{
	CComponent::Save(pFile);
}

void CPaperBurn::Load(FILE* pFile)
{
	CComponent::Load(pFile);
}
