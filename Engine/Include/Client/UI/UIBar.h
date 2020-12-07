#pragma once
#include "Object\Obj.h"
class CUIBar :
	public CObj
{
	friend class CScene;
	friend class CViewPort;

private:
	CUIBar();
	CUIBar(const CUIBar& ui);
	virtual ~CUIBar();

private:
	class CBar*		m_pBar;
	class CUIImage* m_pImg;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIBar* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

