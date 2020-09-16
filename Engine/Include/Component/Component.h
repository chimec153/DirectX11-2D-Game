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
	class CObj*			m_pObj;
	COMPONENT_TYPE		m_eType;

public:
	class CObj* GetObj()	const;
	COMPONENT_TYPE GetType()	const;

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

