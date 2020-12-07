#pragma once
#include "UIControl.h"

enum class BAR_DIR
{
	L,
	T,
	R,
	B,
	END
};

class CBar :
	public CUIControl
{
	friend class CObj;

private:
	CBar();
	CBar(const CBar& ui);
	virtual ~CBar();

private:
	float				m_fPercent;
	BARCBUFFER			m_tCBuffer;
	BAR_DIR				m_eDir;

public:
	float GetPercent()	const;
	void SetPercent(float f);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CBar* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

};

