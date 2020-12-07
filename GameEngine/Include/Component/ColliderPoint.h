#pragma once
#include "Collider.h"
class CColliderPoint :
	public CCollider
{
	friend class CObj;

private:
	CColliderPoint();
	CColliderPoint(const CColliderPoint& col);
	virtual ~CColliderPoint();

private:
	Vector3		m_vPt;
	Vector3		m_vUIPt;

public:
	const Vector3& GetInfo()	const;
	const Vector3& GetUIInfo()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderPoint* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pCol);

};

