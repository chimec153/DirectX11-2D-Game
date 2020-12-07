#pragma once
#include "SceneComponent.h"
class CPrimitiveComponent :
	public CSceneComponent
{
	friend class CObj;

protected:
	CPrimitiveComponent();
	CPrimitiveComponent(const CPrimitiveComponent& com);
	virtual ~CPrimitiveComponent() = 0;

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	virtual CPrimitiveComponent* Clone() = 0;

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

