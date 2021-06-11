#include "MultiRenderTarget.h"
#include "../Device.h"

CMultiRenderTarget::CMultiRenderTarget()	:
	m_pDepthView(nullptr)
	, m_pPrevDepthView(nullptr)
	, m_pDepthTex(nullptr)
	, m_pClearColor()
	, m_pDepthSRV(nullptr)
{
}

CMultiRenderTarget::~CMultiRenderTarget()
{
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pPrevDepthView);
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthSRV);
}

bool CMultiRenderTarget::CreateDepthView(int iWidth, int iHeight, DXGI_FORMAT eFmt)
{
	SAFE_RELEASE(m_pDepthTex);
	SAFE_RELEASE(m_pDepthView);
	SAFE_RELEASE(m_pDepthSRV);

	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tDesc.Height = iHeight;
	tDesc.Width = iWidth;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL| D3D11_BIND_SHADER_RESOURCE;
	tDesc.MipLevels = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(DEVICE->CreateTexture2D(&tDesc, nullptr, &m_pDepthTex)))
		return false;
	D3D11_DEPTH_STENCIL_VIEW_DESC tDSV = {};
	tDSV.Format = eFmt;
	tDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	tDSV.Texture2D.MipSlice = 0;
	if (FAILED(DEVICE->CreateDepthStencilView(m_pDepthTex, &tDSV, &m_pDepthView)))
		return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.Texture2D.MipLevels = 1;
	tSRVDesc.Texture2D.MostDetailedMip = 0;
	tSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pDepthTex, &tSRVDesc, &m_pDepthSRV)))
	{
		SAFE_RELEASE(m_pDepthTex);
		return false;
	}

	return true;
}

void CMultiRenderTarget::AddTarget(ID3D11RenderTargetView* pRTV)
{
	m_vecPrevTarget.push_back(nullptr);
	m_vecTarget.push_back(pRTV);
}

void CMultiRenderTarget::SetTarget()
{
	CONTEXT->OMGetRenderTargets(static_cast<UINT>(m_vecPrevTarget.size()), &m_vecPrevTarget[0], &m_pPrevDepthView);

	CONTEXT->OMSetRenderTargets(static_cast<UINT>(m_vecTarget.size()), &m_vecTarget[0], m_pDepthView);
}

void CMultiRenderTarget::ResetTarget()
{
	size_t iSize = m_vecPrevTarget.size();
	CONTEXT->OMSetRenderTargets(static_cast<int>(iSize), &m_vecPrevTarget[0], m_pPrevDepthView);

	for (size_t i = 0; i < iSize; i++)
	{
		SAFE_RELEASE(m_vecPrevTarget[i]);
	}
	SAFE_RELEASE(m_pPrevDepthView);
}

void CMultiRenderTarget::Clear()
{
	for (int i = 0; i < static_cast<int>(m_vecTarget.size()); i++)
	{
		CONTEXT->ClearRenderTargetView(m_vecTarget[i], m_pClearColor);
	}

	CONTEXT->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CMultiRenderTarget::SetDepthStencilView(int iRegister, int iType)
{
	CONTEXT->PSSetShaderResources(iRegister, 1, &m_pDepthSRV);
}
