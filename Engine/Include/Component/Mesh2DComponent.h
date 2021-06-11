#pragma once
#include "SceneComponent.h"
class CMesh2DComponent :
	public CSceneComponent
{
	friend class CObj;

protected:
	CMesh2DComponent();
	CMesh2DComponent(const CMesh2DComponent& com);
	virtual ~CMesh2DComponent();

public:
	virtual bool Init();
	virtual bool Init(const char* pFileName, const std::string& strPathKey = DATA_PATH);
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CMesh2DComponent* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

