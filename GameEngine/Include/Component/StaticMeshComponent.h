#pragma once
#include "MeshComponent.h"
class CStaticMeshComponent :
	public CMeshComponent
{
	friend class CObj;

protected:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& com);
	virtual ~CStaticMeshComponent();

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	virtual CStaticMeshComponent* Clone();
};

