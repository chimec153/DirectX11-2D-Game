#pragma once

#include "../Ref.h"
#include "Animation2DNotify.h"

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
	const Frame GetFrame(int idx = 0)	const;

	void AddSprite(class CSpriteComponent* pCom);
	void DeleteSprite(class CSpriteComponent* pCom);

public:
	bool Init(class CTexture* pTex, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool Init(const std::string& strKey, float fMaxTime = 1.f, float fPlayRate = 1.f);
	bool Init(const TCHAR* pFileName, const std::string& strPathName = RESOURCE_PATH, float fMaxTime = 1.f, float fPlayRate = 1.f);
	void Update(int iFrame, float fTime, float fDeltaTime);
	virtual CAnimation2DSequence* Clone();

public:
	void AddFrame(const Vector2& vStart, const Vector2& vEnd);
	void AddNotify(const std::string& strTag, int iFrame);
	void AddNotify(const std::string& strTag, float fTime);
	void AddCallBack(const std::string& strTag, void(*pFunc)( float));
	template <typename T>
	void AddCallBack(const std::string& strTag, T* pObj, void(T::* pFunc)( float))
	{
		std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
		std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->m_strTag == strTag)
			{
				(*iter)->AddFunc<T>(pObj, pFunc);
				break;
			}
		}
	}
	void AddCallBack(const std::string& strTag, void(*pFunc)(int, float));
	template <typename T>
	void AddCallBack(const std::string& strTag, T* pObj, void(T::* pFunc)(int, float))
	{
		std::list<class CAnimation2DNotify*>::iterator iter = m_NotifyList.begin();
		std::list<class CAnimation2DNotify*>::iterator iterEnd = m_NotifyList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->m_strTag == strTag)
			{
				(*iter)->AddFunc<T>(pObj, pFunc);
				break;
			}
		}
	}
	void Clear();
	void DeleteNotify(const std::string& strTag);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

