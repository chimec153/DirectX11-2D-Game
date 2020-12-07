#pragma once
#include "UIControl.h"
class CUISprite :
	public CUIControl
{
	friend class CObj;

private:
	CUISprite();
	CUISprite(const CUISprite& ui);
	virtual ~CUISprite();

private:
	float			m_fTime;
	float			m_fFMT;
	int				m_iFrm;
	int				m_iMF;
	SpriteCBuffer	m_tCBuffer;

public:
	void SetFrm(int iFrm);
	void SetTime(float fTime);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUISprite* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

