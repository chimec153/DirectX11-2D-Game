#pragma once

#include "Object/Obj.h"

class CPlayer :
	public CObj
{
public:
	CPlayer();
	CPlayer(const CPlayer& player);
	virtual ~CPlayer();

private:
	class CMesh2DComponent*			m_pMesh;
	class CMesh2DComponent*			m_pChildMesh;
	float			m_fSpeed;

public:
	void SetMeshComponent(class CMesh2DComponent* pMesh);

public:
	virtual bool Init();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
};

