#pragma once
#include "UIControl.h"
class CTitleBar :
	public CUIControl
{
	friend class CObj;

private:
	CTitleBar();
	CTitleBar(const CTitleBar& ui);
	virtual ~CTitleBar();

private:
	class CColliderRect*	m_pRC;
	Vector2					m_vPos;
	bool					m_bPush;
	class CUIFont*			m_pTitle;

public:
	void SetTitle(const TCHAR* pTitle);
	void SetExtent(float x, float y);
	void SetTitleRect(float l, float t, float r, float b);
	void SetTitleAlignV(TEXT_ALIGN_VERTICAL eAlign);
	void SetTitleAlignH(TEXT_ALIGN_HORISONTAL eAlign);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CTitleBar* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);
};

