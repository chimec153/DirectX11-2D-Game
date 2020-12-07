#pragma once
#include "Object\Obj.h"
class CEffect :
	public CObj
{
	friend class CScene;

private:
	CEffect();
	CEffect(const CEffect& eft);
	virtual ~CEffect();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CEffect* Clone();
	virtual void Notify(const std::string& strTag);

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

