#include "Decal.h"
#include "Camera.h"
#include "../Camera/CameraManager.h"
#include "../Resource/ShaderManager.h"

CDecal::CDecal()	:
	m_tCBuffer()
	, m_fInTime(0.f)
	, m_fInTimeLimit(0.f)
	, m_bInComplete(false)
	, m_fDuration(0.f)
	, m_fDurationLimit(0.f)
	, m_bDurationComplete(false)
	, m_fOutTime(0.f)
	, m_fOutTimeLimit(0.f)
	, m_bLoop(false)
{
	m_tCBuffer.fAlpha = 1.f;
}

CDecal::CDecal(const CDecal& decal)	:
	CSceneComponent(decal)
	, m_tCBuffer(decal.m_tCBuffer)
	, m_fInTime(decal.m_fInTime)
	, m_fInTimeLimit(decal.m_fInTimeLimit)
	, m_bInComplete(decal.m_bInComplete)
	, m_fDuration(decal.m_fDuration)
	, m_fDurationLimit(decal.m_fDurationLimit)
	, m_bDurationComplete(decal.m_bDurationComplete)
	, m_fOutTime(decal.m_fOutTime)
	, m_fOutTimeLimit(decal.m_fOutTimeLimit)
	, m_bLoop(decal.m_bLoop)
{
}

CDecal::~CDecal()
{
}

void CDecal::SetAlpha(float fAlpha)
{
	m_tCBuffer.fAlpha = fAlpha;
}

void CDecal::SetInTime(float fTime)
{
	m_fInTimeLimit = fTime;
}

void CDecal::SetDuration(float fTime)
{
	m_fDurationLimit = fTime;
}

void CDecal::SetOutTime(float fTime)
{
	m_fOutTimeLimit = fTime;
}

void CDecal::SetLoop(bool bLoop)
{
	m_bLoop = bLoop;
}

bool CDecal::Init()
{
	if(!CSceneComponent::Init())
		return false;

	SetMesh("Cube");
	SetShader("DecalShader");
	SetMaterial("Color");

	return true;
}

void CDecal::Start()
{
	CSceneComponent::Start();
}

void CDecal::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_fInTimeLimit > 0.f && !m_bInComplete)
	{
		m_fInTime += fTime;
		m_tCBuffer.fAlpha = m_fInTime / m_fInTimeLimit;

		if (m_fInTime >= m_fInTimeLimit)
		{
			m_bInComplete = true;
			m_fInTime = 0.f;
		}
	}

	else if (m_fDurationLimit > 0.f && m_bInComplete && !m_bDurationComplete)
	{
		m_fDuration += fTime;
		m_tCBuffer.fAlpha = 1.f;
		if(m_fDuration >= m_fDurationLimit)
		{
			m_bDurationComplete = true;
			m_fDuration = 0.f;
		}
	}
	else if (m_fOutTimeLimit > 0.f && m_bDurationComplete)
	{
		m_fOutTime += fTime;

		m_tCBuffer.fAlpha = 1.f - m_fOutTime / m_fOutTimeLimit;

		if (m_fOutTime >= m_fOutTimeLimit)
		{
			m_tCBuffer.fAlpha = 0.f;

			if (m_bLoop)
			{
				m_fOutTime = 0.f;
				m_bInComplete = false;
				m_bDurationComplete = false;
			}
			else
			{
				Destroy();
			}
		}
	}
}

void CDecal::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CDecal::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CDecal::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CDecal::Render(float fTime)
{
	m_tCBuffer.matInvWVP = GetMatWorld();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	if (pCam)
	{
		Matrix matVP = pCam->GetVP();

		m_tCBuffer.matInvWVP *= matVP;
		m_tCBuffer.matInvWVP.Inverse();
		m_tCBuffer.matInvWVP.Transpose();
	}

	SAFE_RELEASE(pCam);

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Decal", &m_tCBuffer);

	CSceneComponent::Render(fTime);
}

void CDecal::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CDecal* CDecal::Clone()
{
	return new CDecal(*this);
}

void CDecal::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CDecal::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
