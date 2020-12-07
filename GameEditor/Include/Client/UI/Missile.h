#pragma once
#include "Object\Obj.h"

class CMissile :
	public CObj
{
	friend class CScene;

private:
	CMissile();
	CMissile(const CMissile& obj);
	virtual ~CMissile();

private:
	float	m_fSpeed;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMissile* Clone();
	virtual void Notify(const std::string& strKey);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void ColEnter(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDst, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDst, float fTime);
};

