#pragma once
#include "UIControl.h"

typedef struct _tagButtonInfo
{
	class CTexture* pTex;
	Vector4			vClr;

	_tagButtonInfo() :
		pTex(nullptr),
		vClr()
	{
	}
}BUTTONINFO, *PBUTTONINFO;

class CUIButton :
	public CUIControl
{
	friend class CObj;

protected:
	CUIButton();
	CUIButton(const CUIButton& ui);
	virtual ~CUIButton();

protected:
	std::vector<PBUTTONINFO>				m_vecInfo;
	BUTTON_STATE							m_eState;
	std::vector<std::function<void(float)>>	m_vecCallBack[(int)BUTTON_STATE::END];
	class CColliderRect*					m_pRC;
	class CSound*							m_pSnd;
	std::string								m_strSnd[(int)BUTTON_STATE::END];

public:
	void SetTexture(BUTTON_STATE eStat, const std::string& strKey);
	void SetTexture(BUTTON_STATE eStat, class CTexture* pTex);
	void SetColor(BUTTON_STATE eStat, const Vector4& vClr);
	void SetColor(BUTTON_STATE eStat, float r, float g, float b, float a);
	void SetColor(BUTTON_STATE eStat, BYTE r, BYTE g, BYTE b, BYTE a);
	void SetSound(BUTTON_STATE eStat, const std::string& strKey);
	BUTTON_STATE GetState()	const;

public:
	void SetCallBack(BUTTON_STATE eState, void(*pFunc)(float));

	template <typename T>
	void SetCallBack(BUTTON_STATE eState, T* pObj, void(T::* pFunc)(float))
	{
		m_vecCallBack[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1));
	}

	void Call(float fTime);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIButton* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);
};

