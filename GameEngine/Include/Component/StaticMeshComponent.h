#pragma once
#include "SceneComponent.h"
class CStaticMeshComponent :
	public CSceneComponent
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

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

