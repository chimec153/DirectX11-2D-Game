#pragma once
#include "UIButton.h"
class CCheckBox :
	public CUIButton
{
	friend class CObj;

private:
	CCheckBox();
	CCheckBox(const CCheckBox& ui);
	virtual ~CCheckBox();

private:
	class CTexture* m_pCheckTex;
	bool			m_bChk;

public:
	void SetChkTex(const std::string& strKey);
	void SetChkTex(class CTexture* pTex);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCheckBox* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void Check(float fTime);

};

