#pragma once
#include "UIObject.h"
class CMouseObj :
	public CUIObject
{
	friend class CInput;

public:
	CMouseObj();
	CMouseObj(const CMouseObj& obj);
	virtual ~CMouseObj();

private:
	class CUISprite*		m_pImage;
	class CColliderRay*		m_pCollider;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMouseObj* Clone();
};

