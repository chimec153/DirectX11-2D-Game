#pragma once
#include "UI\UIObject.h"
class CCheckObj :
	public CUIObject
{
	friend class CScene;
	friend class CViewPort;

private:
	CCheckObj();
	CCheckObj(const CCheckObj& obj);
	virtual ~CCheckObj();

private:
	class CCheckBox*	m_pBox;
	class CUIFont*		m_pText;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCheckObj* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

