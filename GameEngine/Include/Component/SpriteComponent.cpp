#include "SpriteComponent.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Resource/Animation2DSequence.h"
#include "../Resource/Texture.h"
#include "../Resource/ShaderManager.h"

CSpriteComponent::CSpriteComponent() :
	m_pCurrent(nullptr),
	m_pDefault(nullptr)
{
	m_eSceneComponentClassType = SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com)	:
	CSceneComponent(com)
{
	m_mapSprite.clear();

	std::unordered_map<std::string, PSpriteInfo>::const_iterator iter = com.m_mapSprite.begin();
	std::unordered_map<std::string, PSpriteInfo>::const_iterator iterEnd = com.m_mapSprite.end();

	for (; iter != iterEnd; ++iter)
	{
		PSpriteInfo pInfo = new SpriteInfo;

		*pInfo = *iter->second;

		if (pInfo->pSequence)
			pInfo->pSequence = iter->second->pSequence->Clone();

		pInfo->pSequence->AddSprite(this);

		m_mapSprite.insert(std::make_pair(iter->first, pInfo));

		if (com.m_pCurrent == iter->second)
			m_pCurrent = pInfo;

		if (com.m_pDefault == iter->second)
			m_pDefault = pInfo;

		pInfo->pFunc = nullptr;
	}

	m_tCBuffer = com.m_tCBuffer;
}

CSpriteComponent::~CSpriteComponent()
{
	std::unordered_map<std::string, PSpriteInfo>::iterator iter = m_mapSprite.begin();
	std::unordered_map<std::string, PSpriteInfo>::iterator iterEnd = m_mapSprite.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->pSequence->DeleteSprite(this);
		SAFE_RELEASE(iter->second->pSequence);
		SAFE_DELETE(iter->second);
	}
}


void CSpriteComponent::CreateSprite(const std::string& strTag, const std::string& strAni2DSeq, bool bLoop, float fMaxTime, float fPlayRate)
{
	PSpriteInfo pInfo = FindSprite(strAni2DSeq);

	if (pInfo)
		return;

	pInfo = new SpriteInfo;

	pInfo->fPlayRate = fPlayRate;
	pInfo->pSequence = GET_SINGLE(CResourceManager)->FindAni2DSeq(strAni2DSeq);
	pInfo->bLoop = bLoop;

	if (pInfo->pSequence)
		pInfo->pSequence->AddSprite(this);

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	if (!m_pDefault)
		m_pDefault = pInfo;

	m_mapSprite.insert(std::make_pair(strTag, pInfo));
}

void CSpriteComponent::CreateSprite(const std::string& strTag, CAnimation2DSequence* pSeq, bool bLoop, float fMaxTime, float fPlayRate)
{
	PSpriteInfo pInfo = FindSprite(strTag);

	if (pInfo)
		return;

	pInfo = new SpriteInfo;

	pInfo->fPlayRate = fPlayRate;
	pInfo->pSequence = pSeq;
	pInfo->bLoop = bLoop;

	if(pInfo->pSequence)
		pInfo->pSequence->AddSprite(this);

	if (pSeq)
		pSeq->AddRef();

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	if (!m_pDefault)
		m_pDefault = pInfo;

	m_mapSprite.insert(std::make_pair(strTag, pInfo));
}

PSpriteInfo CSpriteComponent::FindSprite(const std::string& strTag)
{
	std::unordered_map<std::string, PSpriteInfo>::iterator iter = m_mapSprite.find(strTag);

	if (iter == m_mapSprite.end())
		return nullptr;

	return iter->second;
}

void CSpriteComponent::ChangeSequence(const std::string& strTag)
{
	PSpriteInfo pInfo = FindSprite(strTag);

	if (!pInfo)
		return;

	if (pInfo == m_pCurrent)
		return;

	m_pCurrent = pInfo;

	pInfo->fTime = 0.f;
	pInfo->iFrame = 0;
}

void CSpriteComponent::SetDefaultSeq(const std::string& strTag)
{
	PSpriteInfo pInfo = FindSprite(strTag);

	if (!pInfo)
		return;

	m_pDefault = pInfo;
}

void CSpriteComponent::Notify(const std::string& strTag)
{
	m_pObj->Notify(strTag);
}

void CSpriteComponent::AddNotify(const std::string& strSeq, const std::string& strNot, int iFrame)
{
	PSpriteInfo pInfo = FindSprite(strSeq);

	if (!pInfo)
		return;

	pInfo->pSequence->AddNotify(strNot, iFrame);
}

void CSpriteComponent::ReturnClip()
{
	if (m_pCurrent == m_pDefault)
		return;

	m_pCurrent = m_pDefault;

	if (m_pCurrent)
	{
		m_pCurrent->fTime = 0.f;
		m_pCurrent->iFrame = 0;
	}
}

const _tagFrame& CSpriteComponent::GetFrame() const
{
	return m_pCurrent->pSequence->GetFrame(m_pCurrent->iFrame);
}

CTexture* CSpriteComponent::GetTexture() const
{
	if (m_pCurrent->pSequence->m_pTexture)
		m_pCurrent->pSequence->m_pTexture->AddRef();

	return m_pCurrent->pSequence->m_pTexture;
}

const Vector2 CSpriteComponent::GetTextureSize() const
{
	return m_pCurrent->pSequence->m_pTexture->GetSize();
}

bool CSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SAFE_RELEASE(m_pMesh);

	m_pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(m_pMesh);

	m_pMaterial->SetShader("Sprite");

	return true;
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();
}

void CSpriteComponent::Update(float fTime)
{
	CSceneComponent::Update(fTime);

	if (m_pCurrent)
	{
		m_pCurrent->fTime += m_pCurrent->fPlayRate * fTime;

		float fFrameTime = m_pCurrent->pSequence->m_fMaxTime / m_pCurrent->pSequence->m_iMaxFrame;

		if (m_pCurrent->fTime >= fFrameTime)
		{
			m_pCurrent->fTime -= fFrameTime;

			++m_pCurrent->iFrame;

			if (m_pCurrent->iFrame == m_pCurrent->pSequence->m_iMaxFrame)
			{
				if (m_pCurrent->bLoop)
					m_pCurrent->iFrame = 0;

				else
					ReturnClip();

				if (m_pCurrent->pFunc)
					m_pCurrent->pFunc();
			}

			m_pCurrent->pSequence->Update(m_pCurrent->iFrame);
		}
	}
}

void CSpriteComponent::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CSpriteComponent::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CSpriteComponent::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CSpriteComponent::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	if (m_pCurrent)
	{
		int iTex = 0;
		int iFrame = 0;

		CTexture* pTex = m_pCurrent->pSequence->m_pTexture;

		if (pTex->GetImageType() == IMAGE_TYPE::IT_ATLAS)
		{
			m_tCBuffer.iImageType = (int)IMAGE_TYPE::IT_ATLAS;
			iFrame = m_pCurrent->iFrame;
		}

		else
		{
			m_tCBuffer.iImageType = (int)IMAGE_TYPE::IT_ARRAY;
			iTex = m_pCurrent->iFrame;
		}

		m_tCBuffer.vStart = m_pCurrent->pSequence->GetFrame(iFrame).vStart;
		m_tCBuffer.vEnd = m_pCurrent->pSequence->GetFrame(iFrame).vEnd;
		m_tCBuffer.vSize.x = (float)pTex->GetWidth(iTex);
		m_tCBuffer.vSize.y = (float)pTex->GetHeigth(iTex);

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Sprite", &m_tCBuffer);

		m_pCurrent->pSequence->m_pTexture->SetTexture(0, (int)SHADER_CBUFFER_TYPE::CBUFFER_PIXEL, iTex);
	}

	m_pMesh->Render(fTime);
}

void CSpriteComponent::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
