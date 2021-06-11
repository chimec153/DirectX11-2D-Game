#include "Distortion.h"
#include "../Component/SceneComponent.h"
#include "../Resource/ShaderManager.h"

CDistortion::CDistortion()	:
	m_tCBuffer()
{
}

CDistortion::CDistortion(const CDistortion& dis)	:
	CPostProcess(dis)
	, m_tCBuffer(dis.m_tCBuffer)
{
}

CDistortion::~CDistortion()
{
}

void CDistortion::SetSpeed(float fSpeed)
{
	m_tCBuffer.fSpeed = fSpeed;
}

void CDistortion::SetAmp(float fAmp)
{
	m_tCBuffer.fAmp = fAmp;
}

bool CDistortion::Init()
{
	if (!CPostProcess::Init())
		return false;

	return true;
}

void CDistortion::Start()
{
	CPostProcess::Start();
}

void CDistortion::Update(float fTime)
{
	CPostProcess::Update(fTime);
}

void CDistortion::PostUpdate(float fTime)
{
	CPostProcess::PostUpdate(fTime);
}

void CDistortion::Collision(float fTime)
{
	CPostProcess::Collision(fTime);
}

void CDistortion::PreRender(float fTime)
{
	CPostProcess::PreRender(fTime);
}

void CDistortion::Render(float fTime)
{
	CPostProcess::Render(fTime);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Distortion", &m_tCBuffer);

	if (m_pCom)
	{
		m_pCom->Render(fTime);
	}
}

void CDistortion::PostRender(float fTime)
{
	CPostProcess::PostRender(fTime);
}

CDistortion* CDistortion::Clone()
{
	return new CDistortion(*this);
}

void CDistortion::Save(FILE* pFile)
{
	CPostProcess::Save(pFile);
}

void CDistortion::Load(FILE* pFile)
{
	CPostProcess::Load(pFile);
}
