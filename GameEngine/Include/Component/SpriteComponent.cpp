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
	m_pDefault(nullptr),
	m_bCall(false)
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


void CSpriteComponent::CreateSprite(const std::string& strTag, const std::string& strAni2DSeq, 
	LOOP_OPTION eOp, float fPlayRate, int iLFrame, float fLimit)
{
	PSpriteInfo pInfo = FindSprite(strAni2DSeq);

	if (pInfo)
		return;

	pInfo = new SpriteInfo;

	pInfo->fPlayRate = fPlayRate;

	CAnimation2DSequence* pSeq = GET_SINGLE(CResourceManager)->FindAni2DSeq(strAni2DSeq);

	pInfo->pSequence = pSeq->Clone();

	SAFE_RELEASE(pSeq);

	pInfo->eOption = eOp;
	pInfo->iLFrame = iLFrame;
	pInfo->fLimitTime = fLimit;

	if (pInfo->pSequence)
		pInfo->pSequence->AddSprite(this);

	if (!m_pCurrent)
		m_pCurrent = pInfo;

	if (!m_pDefault)
		m_pDefault = pInfo;

	m_mapSprite.insert(std::make_pair(strTag, pInfo));
}

void CSpriteComponent::CreateSprite(const std::string& strTag, CAnimation2DSequence* pSeq,
	LOOP_OPTION eOp,float fPlayRate, int iLFrame, float fLimit)
{
	PSpriteInfo pInfo = FindSprite(strTag);

	if (pInfo)
		return;

	pInfo = new SpriteInfo;

	pInfo->fPlayRate = fPlayRate;

	pInfo->pSequence = pSeq->Clone();

	pInfo->eOption = eOp;
	pInfo->iLFrame = iLFrame;
	pInfo->fLimitTime = fLimit;

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
	pInfo->pSequence->Clear();
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

void CSpriteComponent::AddNotify(const std::string& strSeq, const std::string& strNot, float fTime)
{
	PSpriteInfo pInfo = FindSprite(strSeq);

	if (!pInfo)
		return;

	pInfo->pSequence->AddNotify(strNot, fTime);
}

void CSpriteComponent::AddCallBack(const std::string& strSeq, const std::string& strNot, void(*pFunc)(float))
{
	PSpriteInfo pInfo = FindSprite(strSeq);

	if (!pInfo)
		return;

	pInfo->pSequence->AddCallBack(strNot, pFunc);
}

void CSpriteComponent::AddCallBack(const std::string& strSeq, const std::string& strNot, void(*pFunc)(int, float))
{
	PSpriteInfo pInfo = FindSprite(strSeq);

	if (!pInfo)
		return;

	pInfo->pSequence->AddCallBack(strNot, pFunc);
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
		m_pCurrent->pSequence->Clear();
	}
}

const _tagFrame CSpriteComponent::GetFrame() const
{
	return m_pCurrent->pSequence->GetFrame(m_pCurrent->iFrame);
}

CTexture* CSpriteComponent::GetTexture() const
{
	if (m_pCurrent->pSequence->m_pTexture)
		m_pCurrent->pSequence->m_pTexture->AddRef();

	return m_pCurrent->pSequence->m_pTexture;
}

void CSpriteComponent::SetPlayRate(float fRate)
{
	if (m_pCurrent)
	{
		m_pCurrent->fPlayRate = fRate;
	}
}

const Vector2 CSpriteComponent::GetTextureSize() const
{
	return m_pCurrent->pSequence->m_pTexture->GetSize();
}

bool CSpriteComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	CMesh* pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);

	SetMaterial("Color");

	SetShader("Sprite");

	return true;
}

bool CSpriteComponent::Init(const char* pFileName, const std::string& strPathKey)
{
	Init();

	if (!CSceneComponent::Init(pFileName, strPathKey))
		return false;

	SetMaterial("Color");

	SetShader("Sprite");

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

		m_pCurrent->pSequence->Update(m_pCurrent->iFrame, m_pCurrent->fTime, m_pCurrent->fPlayRate * fTime);

		if (m_pCurrent->fTime >= fFrameTime)
		{
			m_pCurrent->fTime -= fFrameTime;

			++m_pCurrent->iFrame;

			if (m_pCurrent->iFrame >= m_pCurrent->pSequence->m_iMaxFrame)
			{
				m_pCurrent->pSequence->Update(m_pCurrent->iFrame, m_pCurrent->fTime, m_pCurrent->fPlayRate * fTime);

				if (m_pCurrent->pFunc)
					m_pCurrent->pFunc();

				switch (m_pCurrent->eOption)
				{
				case LOOP_OPTION::ONCE_RETURN:
					ReturnClip();
					break;
				case LOOP_OPTION::ONCE_DESTROY:
					Destroy();
					break;
				case LOOP_OPTION::ONCE_FRAME:
					m_pCurrent->iFrame = m_pCurrent->iLFrame;
					break;
				case LOOP_OPTION::LOOP:
					m_pCurrent->iFrame = 0;
					m_pCurrent->pSequence->Clear();
					break;
				case LOOP_OPTION::LOOP_FRAME:
					m_pCurrent->iFrame = m_pCurrent->iLFrame;
					m_pCurrent->pSequence->Clear();
					break;
				}
			}
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

	CSceneComponent::Render(fTime);
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

	int iSize = (int)m_mapSprite.size();

	fwrite(&iSize, 4, 1, pFile);

	if (iSize > 0)
	{
		std::string strTag = m_pCurrent->pSequence->GetName();
		int iLength = (int)strTag.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);
		strTag = m_pDefault->pSequence->GetName();
		iLength = (int)strTag.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);
	}

	std::unordered_map<std::string, PSpriteInfo>::iterator iter = m_mapSprite.begin();
	std::unordered_map<std::string, PSpriteInfo>::iterator iterEnd = m_mapSprite.end();

	for (; iter != iterEnd; iter++)
	{
		int iLength = (int)iter->first.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(iter->first.c_str(), 1, iLength, pFile);
		std::string strTag = iter->second->pSequence->GetName();
		iLength = (int)strTag.length();
		fwrite(&iLength, 4, 1, pFile);
		fwrite(strTag.c_str(), 1, iLength, pFile);
		fwrite(&iter->second->fPlayRate, 4, 1, pFile);
		fwrite(&iter->second->fTime, 4, 1, pFile);
		fwrite(&iter->second->iFrame, 4, 1, pFile);
		fwrite(&iter->second->iLFrame, 4, 1, pFile);
		fwrite(&iter->second->fLimitTime, 4, 1, pFile);
		fwrite(&iter->second->eOption, 4, 1, pFile);
	}

	fwrite(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	char strCurrent[256] = {};
	char strDefault[256] = {};

	if (iSize > 0)
	{
		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strCurrent, 1, iLength, pFile);
		}
		iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strDefault, 1, iLength, pFile);
		}
	}

	for (int i=0;i<iSize;++i)
	{
		PSpriteInfo pInfo = new SpriteInfo;

		int iLength = 0;
		fread(&iLength, 4, 1, pFile);
		char strTag[256] = {};
		if (iLength > 0)
		{
			fread(strTag, 1, iLength, pFile);
		}
		char strSeq[256] = {};
		iLength = 0;
		fread(&iLength, 4, 1, pFile);
		if (iLength > 0)
		{
			fread(strSeq, 1, iLength, pFile);
		}
		pInfo->pSequence = GET_SINGLE(CResourceManager)->FindAni2DSeq(strSeq);
		fread(&pInfo->fPlayRate, 4, 1, pFile);
		fread(&pInfo->fTime, 4, 1, pFile);
		fread(&pInfo->iFrame, 4, 1, pFile);
		fread(&pInfo->iLFrame, 4, 1, pFile);
		fread(&pInfo->fLimitTime, 4, 1, pFile);
		fread(&pInfo->eOption, 4, 1, pFile);

		if (strcmp(strCurrent, strSeq) == 0)
			m_pCurrent = pInfo;

		if (strcmp(strDefault, strSeq) == 0)
			m_pDefault = pInfo;

		pInfo->pSequence->AddSprite(this);

		pInfo->pFunc = nullptr;

		m_mapSprite.insert(std::make_pair(strTag, pInfo));
	}

	fread(&m_tCBuffer, sizeof(m_tCBuffer), 1, pFile);
}

void CSpriteComponent::SpawnWindow()
{
	if (ImGui::Begin("Sprite"))
	{
		Vector3 vPos = GetWorldPos();
		ImGui::SliderFloat3("Pos", &vPos.x, -2500.f, 2500.f);
		SetWorldPos(vPos);
		Vector3 vScale = GetWorldScale();
		ImGui::SliderFloat3("Scale", &vScale.x, -2500.f, 2500.f);
		SetWorldScale(vScale);

		CMaterial* pMtrl = GetMaterial();

		Vector4 vDiff = pMtrl->GetDif();
		Vector4 vAmb = pMtrl->GetAmb();

		ImGui::SliderFloat4("DiffuseColor", &vDiff.x, 0.f, 1.f);
		ImGui::SliderFloat4("AmbientColor", &vAmb.x, 0.f, 1.f);

		pMtrl->SetDiffuseColor(vDiff);
		pMtrl->SetAmbientColor(vAmb);

		SAFE_RELEASE(pMtrl);
		
		ImGui::Text(m_pCurrent->pSequence->GetName().c_str());
		ImGui::SliderInt("Frame", &m_pCurrent->iFrame, 0, m_pCurrent->pSequence->m_iMaxFrame);
	}
	ImGui::End();
}
