#pragma once
#include "Collider.h"
class CColliderLine :
	public CCollider
{
	friend class CObj;

private:
	CColliderLine();
	CColliderLine(const CColliderLine& col);
	virtual ~CColliderLine();

private:
	LineInfo	m_tInfo;

public:
	const LineInfo& GetInfo()	const;
	void SetInfo(const Vector2& vStart, const Vector2& vEnd);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CColliderLine* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);
};

