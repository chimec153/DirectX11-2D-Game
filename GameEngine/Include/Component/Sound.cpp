#include "Sound.h"
#include "../SoundManager.h"
#include "../Component/Camera.h"
#include "../Camera/CameraManager.h"
#include "../Scene/Scene.h"

CSound::CSound()	:
	m_strName(),
	m_pSound(nullptr),
	m_fMax(1.f),
	m_fMin(0.f),
	m_fMaxVol(1.f),
	m_fMinVol(0.f)
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SOUND;
}

CSound::CSound(const CSound& snd)	:
	CSceneComponent(snd)
{
	m_pSound = snd.m_pSound;
}

CSound::~CSound()
{
}

void CSound::SetVol(float fVol)
{
	m_pSound->fVol = fVol;

	if (m_pSound->fVol >= 1.f)
		m_pSound->fVol = 1.f;

	if (m_pSound->eType == SOUND_TYPE::BGM)
	{
		m_pSound->pChannel->setPaused(true);
		m_pSound->pChannel->setVolume(m_pSound->fVol);
		m_pSound->pChannel->setPaused(false);
	}

	else
	{
		m_pSound->pChannel->setVolume(m_pSound->fVol);
	}
}

void CSound::Play(float fTime)
{
	if (m_pSound)
	{
		if (m_pSound->eType == SOUND_TYPE::BGM)
		{
			FMOD::System* pSystem = GET_SINGLE(CSoundManager)->GetSystem();

			FMOD_RESULT hr = pSystem->playSound(m_pSound->pSound, nullptr, true, &m_pSound->pChannel);

			m_pSound->pChannel->setVolume(m_pSound->fVol);
			m_pSound->pChannel->setPaused(false);
		}

		else
		{
			FMOD::System* pSystem = GET_SINGLE(CSoundManager)->GetSystem();

			FMOD_RESULT hr = pSystem->playSound(m_pSound->pSound, nullptr, false, &m_pSound->pChannel);

			m_pSound->pChannel->setVolume(m_pSound->fVol);
		}
	}
}

void CSound::Stop()
{
	m_pSound->pChannel->stop();
}

void CSound::SetSound(const std::string& strKey)
{
	m_strName = strKey;

	m_pSound = GET_SINGLE(CSoundManager)->FindSound(strKey);
}

void CSound::SetMax(float fMax)
{
	m_fMax = fMax;
}

void CSound::SetMin(float fMin)
{
	m_fMin = fMin;
}

void CSound::SetMaxVol(float fMax)
{
	m_fMaxVol = fMax;
}

void CSound::SetMinVol(float fMin)
{
	m_fMinVol = fMin;
}

bool CSound::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CSound::Start()
{
	CSceneComponent::Start();
}

void CSound::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_eSceneComponentType == SCENE_COMPONENT_TYPE::SCT_2D)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		Vector3 vCamPos = pCam->GetWorldPos();

		vCamPos.z = 0.f;

		SAFE_RELEASE(pCam);

		Vector3 vPos = GetWorldPos();

		float fDst = vPos.Distance(vCamPos);

		if (fDst <= m_fMin)
		{
			SetVol(m_fMaxVol);
		}

		else if (fDst >= m_fMax)
		{
			SetVol(m_fMinVol);
		}

		else
		{
			float fVol = m_fMaxVol - (m_fMaxVol - m_fMinVol) * (fDst - m_fMin) / (m_fMax - m_fMin);

			fVol = fVol* fVol;

			SetVol(fVol);
		}			
		//SetVol(10000.f / fDst / fDst);
	}
}

void CSound::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CSound::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CSound::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CSound::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CSound::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CSound* CSound::Clone()
{
	return new CSound(*this);
}

void CSound::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CSound::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
