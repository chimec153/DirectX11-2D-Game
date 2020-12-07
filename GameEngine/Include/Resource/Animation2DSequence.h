#pragma once

#include "../Ref.h"

class CAnimation2DSequence :
	public CRef
{
	friend class CResourceManager;
	friend class CSceneResource;
	friend class CSpriteComponent;

private:
	CAnimation2DSequence();
	CAnimation2DSequence(const CAnimation2DSequence& seq);
	virtual ~CAnimation2DSequence();

private:
	class CTexture*							m_pTexture;
	float									m_fPlayRate;
	float									m_fMaxTime;
	float									m_fTime;
	int										m_iFrame;
	int										m_iMaxFrame;
	std::vector<Frame>						m_vecFrame;
	std::list<class CAnimation2DNotify*>	m_NotifyList;
	std::list<class CSpriteComponent*>		m_SpriteList;

public:
	const Frame& GetFrame(int idx = 0)	const
	{
		return m_vecFrame[idx];
	}

	void AddSprite(class CSpriteComponent* pCom);
	void DeleteSprite(class CSpriteComponent* pCom);

public:
	bool Init(class CTexture* pTex, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool Init(const std::string& strKey, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool Init(const TCHAR* pFileName, const std::string& strPathName = RESOURCE_PATH, float fMaxTime = 1.f, float fPlayRate = 1.f);
	void Update(int iFrame);
	virtual CAnimation2DSequence* Clone();

public:
	void AddFrame(const Vector2& vStart, const Vector2& vEnd);
	void AddNotify(const std::string& strTag, int iFrame);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

