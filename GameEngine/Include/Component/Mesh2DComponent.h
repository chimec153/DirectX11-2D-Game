#pragma once
#include "MeshComponent.h"
class CMesh2DComponent :
	public CMeshComponent
{
	friend class CObj;

protected:
	CMesh2DComponent();
	CMesh2DComponent(const CMesh2DComponent& com);
	virtual ~CMesh2DComponent();

protected:
	class CMesh* m_pMesh;

public:
	class CMesh* GetMesh()	const;
	void SetMesh(const std::string& strName);
	void SetMesh(class CMesh* pMesh);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMesh2DComponent* Clone();
};

