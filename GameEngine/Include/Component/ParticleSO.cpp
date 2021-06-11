#include "ParticleSO.h"
#include "../Camera/CameraManager.h"
#include "Camera.h"
#include "../Device.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/GraphicShader.h"

CParticleSO::CParticleSO()
{
}

CParticleSO::CParticleSO(const CParticleSO& com)	:
	CSceneComponent(com)
{
}

CParticleSO::~CParticleSO()
{
}

void CParticleSO::Reset()
{
	m_bStart = false;
	m_fAge = 0.f;
}

void CParticleSO::SetParticle(CParticleSystem* pSystem, CTexture* pTexArray, UINT maxParticles)
{
}

void CParticleSO::BuildVB()
{
}

bool CParticleSO::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CParticleSO::Start()
{
	CSceneComponent::Start();
}

void CParticleSO::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CParticleSO::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CParticleSO::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CParticleSO::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CParticleSO::Render(float fTime)
{
	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	m_tCBuffer.matViewProj = pCam->GetVP();

	SAFE_RELEASE(pCam);

	m_tCBuffer.fGameTime = m_fGameTime;
	m_tCBuffer.fTimeStep = m_fTimeStep;
	m_tCBuffer.vCamPosW = m_vCamPosW;
	m_tCBuffer.vEmitPosW = m_vEmitPosW;
	m_tCBuffer.vEmitDirW = m_vEmitDirW;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("ParitcleSO", &m_tCBuffer);

	m_pTexArray->SetTexture(9, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL);

	m_pShader->SetShader();
}

void CParticleSO::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CParticleSO* CParticleSO::Clone()
{
	return new CParticleSO(*this);
}

void CParticleSO::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CParticleSO::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
