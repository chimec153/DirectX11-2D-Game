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
	class CMaterial*		m_pMaterial;

public:
	class CMaterial* GetMaterial()	const;
	void SetMaterial(class CMaterial* pMaterial);

public:
	bool Init();
	void Start();
	void Update(float fTime);
	void PostUpdate(float fTime);
	void Collision(float fTime);
	void PreRender(float fTime);
	void Render(float fTime);
	void PostRender(float fTime);
	virtual CMeshComponent* Clone();
};

