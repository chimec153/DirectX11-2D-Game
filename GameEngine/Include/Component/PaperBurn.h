#pragma once
#include "Component.h"
class CPaperBurn :
	public CComponent
{
	friend class CObj;

private:
	CPaperBurn();
	CPaperBurn(const CPaperBurn& burn);
	virtual ~CPaperBurn();

private:
	PAPERCBUFFER	m_tCBuffer;
	bool			m_bReverse;

public:
	void SetInFilter(float f);
	void SetMidFilter(float f);
	void SetOutFilter(float f);
	void SetInColor(float r, float g, float b, float a);
	void SetOutColor(float r, float g, float b, float a);
	void Reset();

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CPaperBurn* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

