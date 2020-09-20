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
	class CMesh2DComponent*		m_pMesh;

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
};

