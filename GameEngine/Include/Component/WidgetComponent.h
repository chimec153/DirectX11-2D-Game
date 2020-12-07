#pragma once
#include "SceneComponent.h"
class CWidgetComponent :
	public CSceneComponent
{
	friend class CObj;

private:
	CWidgetComponent();
	CWidgetComponent(const CWidgetComponent& com);
	virtual ~CWidgetComponent();

private:
	class CObj* m_pUIObj;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CWidgetComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

