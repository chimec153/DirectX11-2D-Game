#include "FontManager.h"
#include "Device.h"

DEFINITION_SINGLE(CFontManager)

CFontManager::CFontManager()	:
	m_pFactory(nullptr)
{

}

CFontManager::~CFontManager()
{
	SAFE_RELEASE(m_pFactory);
	SAFE_RELEASE_MAP(m_mapBrush);
	SAFE_RELEASE_MAP(m_mapFormat);
}

bool CFontManager::Init()
{
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), (IUnknown**)&m_pFactory)))
		return false;

	CreateFormat("Normal", TEXT("±Ã¼­"), 700, 0, 5, 20, TEXT("ko"));
	CreateFormat("Normal1", TEXT("±Ã¼­"), 600, 0, 5, 20, TEXT("ko"));
	CreateFormat("Normal2", TEXT("±Ã¼­"), 500, 0, 5, 12, TEXT("ko"));

	return true;
}

bool CFontManager::CreateFormat(const std::string& strKey, const TCHAR* pFont, int iWeight, int iStyle, int iStretch, float fFontSize, const TCHAR* pLocal)
{
	IDWriteTextFormat* pFmt = FindFormat(strKey);

	if (pFmt)
		return false;

	if (FAILED(m_pFactory->CreateTextFormat(pFont,nullptr, (DWRITE_FONT_WEIGHT)iWeight, 
		(DWRITE_FONT_STYLE)iStyle, (DWRITE_FONT_STRETCH)iStretch, fFontSize, pLocal, &pFmt)))
		return false;

	m_mapFormat.insert(std::make_pair(strKey, pFmt));

	return true;
}

IDWriteTextLayout* CFontManager::CreateLayout(const TCHAR* pText, 
	unsigned int iLength, IDWriteTextFormat* pFmt, float fWidth, float fHeight)
{
	IDWriteTextLayout* pLayout = nullptr;

	if (FAILED(m_pFactory->CreateTextLayout(pText, iLength, pFmt, fWidth, fHeight, &pLayout)))
		return nullptr;

	return pLayout;
}

ID2D1SolidColorBrush* CFontManager::CreateBrush(BYTE r, BYTE g, BYTE b, BYTE a)
{
	return CreateBrush(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

ID2D1SolidColorBrush* CFontManager::CreateBrush(float r, float g, float b, float a)
{
	UINT iColor = ConvertColor(r, g, b, a);

	ID2D1SolidColorBrush* pBrush = FindBrush(iColor);

	if (pBrush)
		return pBrush;

	if(FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1_COLOR_F(D2D1::ColorF(r, g, b, a)), &pBrush)))
		return nullptr;

	m_mapBrush.insert(std::make_pair(iColor, pBrush));

	return pBrush;
}

ID2D1SolidColorBrush* CFontManager::CreateBrush(UINT iColor)
{
	ID2D1SolidColorBrush* pBrush = FindBrush(iColor);

	if (pBrush)
		return pBrush;

	float b = (iColor & 0xff) / 255.f;
	float g = ((iColor >> 8) & 0xff) / 255.f;
	float r = ((iColor >> 16) & 0xff) / 255.f;
	float a = ((iColor >> 24) & 0xff) / 255.f;

	if (FAILED(RENDERTARGET2D->CreateSolidColorBrush(D2D1_COLOR_F(D2D1::ColorF(r, g, b, a)), &pBrush)))
		return nullptr;

	m_mapBrush.insert(std::make_pair(iColor, pBrush));

	return pBrush;
}

UINT CFontManager::ConvertColor(float r, float g, float b, float a)
{
	UINT iColor;

	iColor = (int)(a * 255.f);

	iColor = (iColor << 8) | (int)(r * 255.f);
	iColor = (iColor << 8) | (int)(g * 255.f);
	iColor = (iColor << 8) | (int)(b * 255.f);

	return iColor;
}

ID2D1SolidColorBrush* CFontManager::FindBrush(UINT iColor)
{
	std::unordered_map<UINT, ID2D1SolidColorBrush*>::iterator iter = m_mapBrush.find(iColor);

	if (iter == m_mapBrush.end())
		return nullptr;

	return iter->second;
}

IDWriteTextFormat* CFontManager::FindFormat(const std::string& strKey)
{
	std::unordered_map<std::string, IDWriteTextFormat*>::iterator iter = m_mapFormat.find(strKey);

	if (iter == m_mapFormat.end())
		return nullptr;

	return iter->second;
}
