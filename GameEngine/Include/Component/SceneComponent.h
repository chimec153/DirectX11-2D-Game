#pragma once
#include "Component.h"
class CSceneComponent :
	public CComponent
{
	friend class CObj;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent() = 0;

protected:
	class CTransform*		m_pTransform;

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
};

