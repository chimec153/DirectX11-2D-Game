#include "Sound.h"
#include "../SoundManager.h"
#include "../Component/Camera.h"
#include "../Camera/CameraManager.h"
#include "../Scene/Scene.h"

CSound::CSound()	:
	m_strSound(),
	m_pSound(nullptr),
	m_fMax(1.f),
	m_fMin(0.f),
	m_fMaxVol(1.f),
	m_fMinVol(0.f),
	m_bFade(false),
	m_bIn(false)
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SOUND;
}

CSound::CSound(const CSound& snd)	:
	CSceneComponent(snd)
{
	m_strSound = snd.m_strSound;
	m_pSound = snd.m_pSound;
	m_fMax = snd.m_fMax;
	m_fMin = snd.m_fMin;
	m_fMaxVol = snd.m_fMaxVol;
	m_fMinVol = snd.m_fMinVol;
	m_bFade = snd.m_bFade;
	m_bIn = snd.m_bIn;
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
		FMOD_RESULT hr;
		if (m_pSound->eType == SOUND_TYPE::BGM)
		{
			FMOD::System* pSystem = GET_SINGLE(CSoundManager)->GetSystem();

			hr = pSystem->playSound(m_pSound->pSound, nullptr, true, &m_pSound->pChannel);

			m_pSound->pChannel->setVolume(m_pSound->fVol);
			m_pSound->pChannel->setPaused(false);
			std::cout << "BGM Volume: " << (int)(m_pSound->fVol * 100.f);
		}

		else
		{
			FMOD::System* pSystem = GET_SINGLE(CSoundManager)->GetSystem();

			hr = pSystem->playSound(m_pSound->pSound, nullptr, false, &m_pSound->pChannel);

			m_pSound->pChannel->setVolume(m_pSound->fVol);
			std::cout << "Effect Volume: " << (int)(m_pSound->fVol *100.f);
		}
		if (hr == FMOD_OK)
		{
			std::cout << "\tPlay: " << m_strSound << "\tOK"<<std::endl;
		}

		else
		{
			std::cout << "\tPlay: " << m_strSound << "\tERROR" << std::endl;
		}
	}
}

void CSound::Stop()
{
	if (m_pSound)
	{
		if (m_pSound->pChannel)
		{
			FMOD_RESULT tResult = m_pSound->pChannel->stop();
#ifdef _DEBUG
			if (tResult == FMOD_RESULT::FMOD_OK)
			{
				OutputDebugStringA("FMOD_STOP_OK");
				std::cout << "Stop: " << m_strSound << std::endl;
			}

			else
			{
				OutputDebugStringA("FMOD_STOP_FAILED");
			}
#endif
		}
	}
}

void CSound::SetSound(const std::string& strKey)
{
	m_strSound = strKey;

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

void CSound::SetFade(bool bFade, bool bIn)
{
	m_bFade = bFade;
	m_bIn = bIn;
}

void CSound::SetSoundAndPlay(const std::string& strKey)
{
	SetSound(strKey);
	SetVol(1.f);
	Play(0.f);
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
	}

	else if (m_bFade && m_pSound)
	{
		if (m_bIn)
		{
			if (m_pSound->fVol < m_fMaxVol)
			{
				SetVol(m_pSound->fVol + fTime);
			}

			else
			{
				SetVol(m_fMaxVol);
				m_bFade = false;
			}
		}

		else
		{
			if (m_pSound->fVol > m_fMinVol)
			{
				SetVol(m_pSound->fVol - fTime);
			}

			else
			{
				m_bFade = false;
				Stop();
			}
		}
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

	int iLength = (int)m_strSound.length();

	fwrite(&iLength, 4, 1, pFile);
	fwrite(m_strSound.c_str(), 1, iLength, pFile);
	fwrite(&m_fMax, 4, 1, pFile);
	fwrite(&m_fMin, 4, 1, pFile);
	fwrite(&m_fMaxVol, 4, 1, pFile);
	fwrite(&m_fMinVol, 4, 1, pFile);
	fwrite(&m_bFade, 1, 1, pFile);
	fwrite(&m_bIn, 1, 1, pFile);

}

void CSound::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	int iLength = 0;
	char strTag[256] = {};

	fread(&iLength, 4, 1, pFile);
	if (iLength > 0)
	{
		fread(strTag, 1, iLength, pFile);
	}
	fread(&m_fMax, 4, 1, pFile);
	fread(&m_fMin, 4, 1, pFile);
	fread(&m_fMaxVol, 4, 1, pFile);
	fread(&m_fMinVol, 4, 1, pFile);
	fread(&m_bFade, 1, 1, pFile);
	fread(&m_bIn, 1, 1, pFile);

	SetSound(strTag);
}
