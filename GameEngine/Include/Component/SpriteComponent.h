#pragma once
#include "SceneComponent.h"

typedef struct _tagSpriteInfo
{
	class CAnimation2DSequence* pSequence;
	float						fPlayRate;
	float						fTime;
	int							iFrame;
	bool						bLoop;
	std::function<void()>		pFunc;

	_tagSpriteInfo()	:
		fPlayRate(1.f),
		fTime(0.f),
		iFrame(0),
		bLoop(false)
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
	void CreateSprite(const std::string& strTag, const std::string& strAni2DSeq, bool bLoop =false, float fMaxTime = 1.f, float fPlayRate = 1.f);
	void CreateSprite(const std::string& strTag, class CAnimation2DSequence* pSeq, bool bLoop = false, float fMaxTime = 1.f, float fPlayRate = 1.f);
	PSpriteInfo FindSprite(const std::string& strTag);
	void ChangeSequence(const std::string& strTag);
	void SetDefaultSeq(const std::string& strTag);
	void Notify(const std::string& strTag);
	void AddNotify(const std::string& strSeq, const std::string& strNot, int iFrame);
	void ReturnClip();
	const _tagFrame& GetFrame()	const;
	const Vector2 GetTextureSize()	const;
	class CTexture* GetTexture()	const;

public:
	bool Init();
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
};

