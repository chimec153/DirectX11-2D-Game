#pragma once
#include "SceneComponent.h"
#include "../Resource/Animation2DSequence.h"

typedef struct _tagSpriteInfo
{
	class CAnimation2DSequence* pSequence;
	float						fPlayRate;
	float						fTime;
	int							iFrame;
	int							iLFrame;
	float						fLimitTime;
	LOOP_OPTION					eOption;
	std::function<void()>		pFunc;

	_tagSpriteInfo()	:
		pSequence(nullptr),
		fPlayRate(1.f),
		fTime(0.f),
		iFrame(0),
		iLFrame(0),
		fLimitTime(0.f),
		eOption(LOOP_OPTION::ONCE_RETURN)
	{

	}
}SpriteInfo, *PSpriteInfo;

class CSpriteComponent :
	public CSceneComponent
{
	friend class CObj;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

protected:
	std::unordered_map<std::string, PSpriteInfo>	m_mapSprite;
	PSpriteInfo										m_pCurrent;
	PSpriteInfo										m_pDefault;
	SpriteCBuffer									m_tCBuffer;
	bool											m_bCall;

public:
	template <typename T>
	void SetEndFunc(const std::string& strTag, T* pObj, void(T::* pFunc)())
	{
		PSpriteInfo pInfo = FindSprite(strTag);

		if (!pInfo)
			return;

		pInfo->pFunc = std::bind(pFunc, pObj);
	}

public:
	void CreateSprite(const std::string& strTag, const std::string& strAni2DSeq, 
		LOOP_OPTION eOp = LOOP_OPTION::ONCE_RETURN, float fPlayRate = 1.f, int iLFrame = 0, float fLimit = 0.f);
	void CreateSprite(const std::string& strTag, class CAnimation2DSequence* pSeq,
		LOOP_OPTION eOp = LOOP_OPTION::ONCE_RETURN, float fPlayRate = 1.f, int iLFrame = 0, float fLimit = 0.f);
	PSpriteInfo FindSprite(const std::string& strTag);
	void ChangeSequence(const std::string& strTag);
	void SetDefaultSeq(const std::string& strTag);
	void Notify(const std::string& strTag);
	void AddNotify(const std::string& strSeq, const std::string& strNot, int iFrame);
	void AddNotify(const std::string& strSeq, const std::string& strNot, float fTime);
	void AddCallBack(const std::string& strSeq, const std::string& strNot, void(*pFunc)(float));
	template <typename T>
	void AddCallBack(const std::string& strSeq, const std::string& strNot, T* pObj, void(T::* pFunc)(float))
	{
		PSpriteInfo pInfo = FindSprite(strSeq);

		if (!pInfo)
			return;

		pInfo->pSequence->AddCallBack<T>(strNot, pObj, pFunc);
	}
	void AddCallBack(const std::string& strSeq, const std::string& strNot, void(*pFunc)(int, float));
	template <typename T>
	void AddCallBack(const std::string& strSeq, const std::string& strNot, T* pObj, void(T::* pFunc)(int, float))
	{
		PSpriteInfo pInfo = FindSprite(strSeq);

		if (!pInfo)
			return;

		pInfo->pSequence->AddCallBack<T>(strNot, pObj, pFunc);
	}

	void ReturnClip();
	const _tagFrame GetFrame()	const;
	const Vector2 GetTextureSize()	const;
	class CTexture* GetTexture()	const;
	void SetPlayRate(float fRate);

public:
	bool Init();
	bool Init(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	virtual CSpriteComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void SpawnWindow();
};

