#pragma once
#include "Object\Obj.h"
class CIceBall :
	public CObj
{
	friend class CScene;

private:
	CIceBall();
	CIceBall(const CIceBall& ball);
	virtual ~CIceBall();

private:
	class CSpriteComponent*		m_pMesh;
	Vector3						m_vDir;

public:
	void SetDir(const Vector3& v);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CIceBall* Clone();
};

