#include "Animation2DSequence.h"
#include "Texture.h"
#include "Animation2DNotify.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/ResourceManager.h"

CAnimation2DSequence::CAnimation2DSequence()	:
	m_pTexture(nullptr),
	m_fPlayRate(1.f),
	m_fMaxTime(1.f),
	m_fTime(0.f),
	m_iFrame(0),
	m_iMaxFrame(1)
{
}

CAnimation2DSequence::CAnimation2DSequence(const CAnimation2DSequence& seq)	:
	CRef(seq)
{
	m_pTexture = seq.m_pTexture;

	if (m_pTexture)
		m_pTexture->AddRef();

	m_fPlayRate = seq.m_fPlayRate;
	m_fMaxTime = seq.m_fMaxTime;
	m_fTime = 0.f;
	m_iFrame = 0;
	m_iMaxFrame = seq.m_iMaxFrame;

	m_vecFrame.clear();

	size_t iSz = seq.m_vecFrame.size();

	m_vecFrame.resize(iSz);

	memcpy(&m_vecFrame[0], &seq.m_vecFrame[0], iSz * sizeof(Frame));

	m_NotifyList.clear();

	std::list<CAnimation2DNotify*>::const_iterator iter = seq.m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::const_iterator iterEnd = seq.m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		CAnimation2DNotify* pNot = new CAnimation2DNotify;

		pNot->m_fAccTime = 0.f;
		pNot->m_fTime = (*iter)->m_fTime;
		pNot->m_iFrame = (*iter)->m_iFrame;
		pNot->m_strTag = (*iter)->m_strTag;
		pNot->m_bCall = false;

		m_NotifyList.push_back(pNot);
	}
}

CAnimation2DSequence::~CAnimation2DSequence()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_DELETE_VECLIST(m_NotifyList);
}

const Frame CAnimation2DSequence::GetFrame(int idx) const
{
	if (m_vecFrame.size() <= idx)
		return Frame();

	return m_vecFrame[idx];
}

void CAnimation2DSequence::AddSprite(CSpriteComponent* pCom)
{
	m_SpriteList.push_back(pCom);
}

void CAnimation2DSequence::DeleteSprite(CSpriteComponent* pCom)
{
	std::list<CSpriteComponent*>::iterator iter = m_SpriteList.begin();
	std::list<CSpriteComponent*>::iterator iterEnd = m_SpriteList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCom)
		{
			m_SpriteList.erase(iter);
			return;
		}
	}
}

bool CAnimation2DSequence::Init(CTexture* pTex, float fMaxTime, float fPlayRate)
{
	SAFE_RELEASE(m_pTexture);

	m_pTexture = pTex;

	if (m_pTexture)
		m_pTexture->AddRef();

	m_fMaxTime = fMaxTime;
	m_fPlayRate = fPlayRate;

	return true;
}

bool CAnimation2DSequence::Init(const std::string& strKey, float fMaxTime, float fPlayRate)
{
	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strKey);

	m_fMaxTime = fMaxTime;
	m_fPlayRate = fPlayRate;

	return true;
}

bool CAnimation2DSequence::Init(const TCHAR* pFileName, const std::string& strPathName, float fMaxTime, float fPlayRate)
{
	return false;
}

void CAnimation2DSequence::Update(int iFrame, float fTime, float fDeltaTime)
{
	std::list<CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Update(fTime, iFrame, fDeltaTime);
	}
}

CAnimation2DSequence* CAnimation2DSequence::Clone()
{
	return new CAnimation2DSequence(*this);
}

void CAnimation2DSequence::AddFrame(const Vector2& vStart, const Vector2& vEnd)
{
	Frame tFrame = {};

	tFrame.vStart = vStart;
	tFrame.vEnd = vEnd;

	m_vecFrame.push_back(tFrame);

	m_iMaxFrame = (int)m_vecFrame.size();
}

void CAnimation2DSequence::AddNotify(const std::string& strTag, int iFrame)
{
	CAnimation2DNotify* pNot = new CAnimation2DNotify;

	pNot->CreateNotify(strTag, iFrame);

	m_NotifyList.push_back(pNot);
}

void CAnimation2DSequence::AddNotify(const std::string& strTag, float fTime)
{
	CAnimation2DNotify* pNot = new CAnimation2DNotify;

	pNot->CreateNotify(strTag, fTime);

	m_NotifyList.push_back(pNot);
}

void CAnimation2DSequence::AddCallBack(const std::string& strTag, void(*pFunc)(float))
{
	std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strTag == strTag)
		{
			(*iter)->AddFunc(pFunc);
			break;
		}
	}
}

void CAnimation2DSequence::AddCallBack(const std::string& strTag, void(*pFunc)(int ,float))
{
	std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strTag == strTag)
		{
			(*iter)->AddFunc(pFunc);
			break;
		}
	}
}

void CAnimation2DSequence::Clear()
{
	std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Clear();
	}
}

void CAnimation2DSequence::DeleteNotify(const std::string& strTag)
{
	std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strTag == strTag)
		{
			m_NotifyList.erase(iter);
			SAFE_DELETE((*iter));

			return;
		}
	}
}

void CAnimation2DSequence::Save(FILE* pFile)
{
	bool bTex = false;

	if (m_pTexture)
		bTex = true;

	fwrite(&bTex, 1,1, pFile);

	if(m_pTexture)
		m_pTexture->Save(pFile);

	fwrite(&m_fPlayRate, 4, 1, pFile);
	fwrite(&m_fMaxTime, 4, 1, pFile);
	fwrite(&m_fTime, 4, 1, pFile);
	fwrite(&m_iFrame, 4, 1, pFile);
	fwrite(&m_iMaxFrame, 4, 1, pFile);

	int iSz = (int)m_vecFrame.size();

	fwrite(&iSz, 4, 1, pFile);
	fwrite(&m_vecFrame[0], sizeof(Frame), iSz, pFile);

	iSz = (int)m_NotifyList.size();

	fwrite(&iSz, 4, 1, pFile);
	
	std::list<CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
	std::list<CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Save(pFile);
}

void CAnimation2DSequence::Load(FILE* pFile)
{
	bool bTex = false;

	fread(&bTex, 1, 1, pFile);

	if (bTex)
		m_pTexture = GET_SINGLE(CResourceManager)->LoadTexture(pFile);		

	fread(&m_fPlayRate, 4, 1, pFile);
	fread(&m_fMaxTime, 4, 1, pFile);
	fread(&m_fTime, 4, 1, pFile);
	fread(&m_iFrame, 4, 1, pFile);
	fread(&m_iMaxFrame, 4, 1, pFile);

	int iSz = 0;

	fread(&iSz, 4, 1, pFile);

	m_vecFrame.resize(iSz);

	fread(&m_vecFrame[0], sizeof(Frame), iSz, pFile);

	iSz = 0;

	fread(&iSz, 4, 1, pFile);

	for (int i = 0; i < iSz; ++i)
	{
		CAnimation2DNotify* pNot = new CAnimation2DNotify;

		pNot->Load(pFile);

		m_NotifyList.push_back(pNot);
	}
}
