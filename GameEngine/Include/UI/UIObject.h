#pragma once

#include "../Object/Obj.h"

class CUIObject :
	public CObj
{
	friend class CUIManager;
	friend class CViewPort;

protected:
	CUIObject();
	CUIObject(const CUIObject& ui);
	virtual ~CUIObject();

protected:
	class CViewPort*	m_pViewPort;
	int					m_iZOrder;

public:
	int GetZOrder()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Input(float fTime);
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIObject* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

