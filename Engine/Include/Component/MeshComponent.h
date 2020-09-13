#pragma once
#include "SceneComponent.h"
class CMeshComponent :
	public CSceneComponent
{
	friend class CObj;

protected:
	CMeshComponent();
	CMeshComponent(const CMeshComponent& com);
	virtual ~CMeshComponent();

protected:
	class CScene* m_pScene;
	class CObj* m_pObj;

public:
	class CObj* GetObj()	const;

protected:
	bool		m_bStart;

public:
	bool IsStart()	const
	{
		m_bStart;
	}

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

