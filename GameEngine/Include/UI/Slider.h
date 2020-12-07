#pragma once
#include "UIControl.h"
class CSlider :
	public CUIControl
{
	friend class CObj;

private:
	CSlider();
	CSlider(const CSlider& ui);
	virtual ~CSlider();

private:
	class CUIButton*		m_pTopBtn;
	class CUIButton*		m_pBotBtn;
	class CUIButton*		m_pBackBtn;
	class CUIButton*		m_pSlider;
	float					m_fPercent;

public:
	void SetMax(float fMax);
	float GetPercent()	const;

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CSlider* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void Up(float fTime);
	void Down(float fTime);
	void Jump(float fTime);

};

