#pragma once
#include "Object\Obj.h"
class CBullet :
	public CObj
{
public:
	CBullet();
	CBullet(const CBullet& bullet);
	virtual ~CBullet();

private:
	float						m_fDist;
	float						m_fLimitDist;
	float						m_fSpeed;
	class CSpriteComponent* m_pMesh;
	class CCamera* m_pCam;

public:
	void SetLimitDist(float fDist)
	{
		m_fLimitDist = fDist;

		if (m_fLimitDist >= 700.f)
			m_fLimitDist = 700.f;
	}

	void SetSpeed(float fSpeed)
	{
		m_fSpeed = fSpeed;
	}

	class CCamera* GetCam()	const
	{
		return m_pCam;
	}

	float GetSpeed()	const
	{
		return m_fSpeed;
	}

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CBullet* Clone();

public:
	void ColInit(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColStay(class CCollider* pSrc, class CCollider* pDest, float fTime);
	void ColEnd(class CCollider* pSrc, class CCollider* pDest, float fTime);
};

