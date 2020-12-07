#pragma once
#include "UIControl.h"

class CUIFont :
	public CUIControl
{
	friend class CObj;

private:
	CUIFont();
	CUIFont(const CUIFont& ui);
	virtual ~CUIFont();

private:
	ID2D1RenderTarget*		m_pTarget;
	bool					m_bShadow;
	bool					m_bAlpha;
	bool					m_bShadowAlpha;
	Vector3					m_vShadowOffset;
	UINT					m_iColor;
	UINT					m_iShadowColor;
	ID2D1SolidColorBrush*	m_pColor;
	ID2D1SolidColorBrush*	m_pShadowColor;
	TCHAR*					m_pText;
	int						m_iLength;
	TCHAR*					m_pFont;
	IDWriteTextFormat*		m_pFormat;
	IDWriteTextLayout*		m_pLayout;
	float					m_fSize;
	float					m_fOpacity;
	float					m_fShadowOpacity;
	D2D1_RECT_F				m_tRect;
	TEXT_ALIGN_HORISONTAL	m_eAlignH;
	TEXT_ALIGN_VERTICAL		m_eAlignV;

public:
	void SetText(const TCHAR* pText);
	void AddText(const TCHAR* pText);
	void SetShadowOffset(const Vector3& vOff);
	void SetOpacity(float fOpa);
	void SetShadowOpacity(float fOpa);
	void SetAlpha(bool bAlp);
	void SetShadow(bool bSha);
	void SetShadowAlpha(bool bAlp);
	void SetShadowColor(float r, float g, float b, float a);
	void SetShadowColor(BYTE r, BYTE g, BYTE b, BYTE a);
	void SetShadowColor(UINT iColor);
	void SetColor(float r, float g, float b, float a);
	void SetColor(BYTE r, BYTE g, BYTE b, BYTE a);
	void SetColor(UINT iColor);
	void SetFormat(const std::string& strKey);
	void SetFormat(const std::string& strKey, const TCHAR* pFont, int iWeight, int iStyle, int iStretch, float fSize, const TCHAR* pLocal);
	void CreateTextLayout();
	void SetRect(float l, float t, float r, float b);
	void SetAlignH(TEXT_ALIGN_HORISONTAL eAlign);
	void SetAlignV(TEXT_ALIGN_VERTICAL eAlign);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CUIFont* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

