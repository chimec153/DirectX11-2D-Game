#pragma once

#include "GameEngine.h"

class CFontManager
{
private:
	std::unordered_map<UINT, ID2D1SolidColorBrush*>		m_mapBrush;
	std::unordered_map<std::string, IDWriteTextFormat*>	m_mapFormat;
	IDWriteFactory*										m_pFactory;

public:
	bool Init();
	bool CreateFormat(const std::string& strKey, const TCHAR* pFont, int iWeight,
		int iStyle, int iStretch, float fFontSize, const TCHAR* pLocal);
	IDWriteTextLayout* CreateLayout(const TCHAR* pText, unsigned int iLength, 
		IDWriteTextFormat* pFmt, float fWidth, float fHeight);
	ID2D1SolidColorBrush* CreateBrush(BYTE r, BYTE g, BYTE b, BYTE a);
	ID2D1SolidColorBrush* CreateBrush(float r, float g, float b, float a);
	ID2D1SolidColorBrush* CreateBrush(UINT iColor);
	UINT ConvertColor(float r, float g, float b, float a);

public:
	ID2D1SolidColorBrush* FindBrush(UINT iColor);
	IDWriteTextFormat* FindFormat(const std::string& strKey);

	DECLARE_SINGLE(CFontManager)
};

