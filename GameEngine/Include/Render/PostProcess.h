#pragma once
#include "../Ref.h"
class CPostProcess :
	public CRef
{
	friend class CSceneComponent;

protected:
	CPostProcess();
	CPostProcess(const CPostProcess& pro);
	virtual ~CPostProcess();

protected:
	class CSceneComponent* m_pCom;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CPostProcess* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

