#pragma once
#include "Collider.h"
class CColliderCircle :
	public CCollider
{
	friend class CObj;

private:
	CColliderCircle();
	CColliderCircle(const CColliderCircle& col);
	virtual ~CColliderCircle();

private:
	CircleInfo	m_tInfo;

public:
	const CircleInfo& GetInfo()	const;
	void SetRadius(float fRadius);
public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderCircle* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
};

