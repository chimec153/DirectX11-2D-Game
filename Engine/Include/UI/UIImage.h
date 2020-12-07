#pragma once
#include "UIControl.h"
class CUIImage :
	public CUIControl
{
	friend class CObj;

protected:
	CUIImage();
	CUIImage(const CUIImage& ui);
	virtual ~CUIImage();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIImage* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

