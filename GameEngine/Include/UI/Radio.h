#pragma once
#include "UIButton.h"
class CRadio :
	public CUIButton
{
	friend class CObj;

private:
	CRadio();
	CRadio(const CRadio& ui);
	virtual ~CRadio();

private:
	bool					m_bSel;
	class CTexture*			m_pSelTex;
	std::vector<CRadio*>	m_vecRadio;

public:
	void SetSelTexture(const std::string& strKey);
	void SetSelTexture(class CTexture* pTex);
	void AddGroup(class CRadio* pRadio);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CRadio* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void Select(float fTime);

private:
	void DeSel();
};

