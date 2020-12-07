#pragma once

#include "../Ref.h"

class CComponent :
	public CRef
{
	friend class CObj;

protected:
	CComponent();
	CComponent(const CComponent& com);
	virtual ~CComponent() = 0;

protected:
	class CScene*		m_pScene;
	class CLayer*		m_pLayer;
	class CObj*			m_pObj;
	COMPONENT_TYPE		m_eType;

public:
	class CObj* GetObj()	const;
	COMPONENT_TYPE GetType()	const;
	class CLayer* GetLayer()	const;

protected:
	bool		m_bStart;

public:
	bool IsStart()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CComponent* Clone() = 0;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

