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

