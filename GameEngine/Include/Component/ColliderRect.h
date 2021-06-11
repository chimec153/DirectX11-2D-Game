#pragma once
#include "Collider.h"
class CColliderRect :
	public CCollider
{
	friend class CObj;

private:
	CColliderRect();
	CColliderRect(const CColliderRect& col);
	virtual ~CColliderRect();

private:
	RectInfo	m_tInfo;
	Vector2		m_tExtent;

public:
	const RectInfo& GetInfo()	const;
	void SetExtent(float x, float y);
	const Vector2& GetExtent()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderRect* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
};

