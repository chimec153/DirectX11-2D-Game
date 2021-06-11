#include "MRT.h"
#include "../Resource/Texture.h"
#include "../Device.h"
#include "../Resource/ShaderManager.h"

CMRT::CMRT() :
	m_pTex(nullptr)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_pPrevSRV(nullptr)
	, m_pPrevUAV(nullptr)
	, m_pTarget(nullptr)
	, m_pPrevTarget(nullptr)
	, m_pDepth(nullptr)
	, m_pPrevDepth(nullptr)
	, m_pDepthSRV(nullptr)
	, m_pClear()
{
	m_pClear[0] = 0.f;
	m_pClear[2] = 0.f;

}

CMRT::CMRT(const CMRT& mrt) :
	CRef(mrt)
	, m_pTex(mrt.m_pTex)
{
	if (m_pTex)
	{
		m_pTex->AddRef();
	}
}

CMRT::~CMRT()
{
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pPrevSRV);
	SAFE_RELEASE(m_pPrevUAV);
	SAFE_RELEASE(m_pTarget);
	SAFE_RELEASE(m_pPrevTarget);
	SAFE_RELEASE(m_pDepth);
	SAFE_RELEASE(m_pPrevDepth);
	SAFE_RELEASE(m_pDepthSRV);
}

ID3D11RenderTargetView* CMRT::GetRenderTarget() const
{
	return m_pTarget;
}

void CMRT::CreateRenderTarget(int iWidth, int iHeight, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthfmt)
{
	D3D11_TEXTURE2D_DESC tTexDesc = {};

	tTexDesc.Usage = D3D11_USAGE_DEFAULT;
	tTexDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tTexDesc.ArraySize = 1;
	tTexDesc.Format = eFmt;
	tTexDesc.Height = iHeight;
	tTexDesc.Width = iWidth;
	tTexDesc.MipLevels = 0;
	tTexDesc.SampleDesc.Count = 1;
	tTexDesc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&tTexDesc, nullptr, &m_pTex)))
		return;

	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	tSRVDesc.Format = eFmt;
	tSRVDesc.Texture2D.MipLevels = 1;
	tSRVDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(DEVICE->CreateShaderResourceView(m_pTex, &tSRVDesc, &m_pSRV)))
		return;

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};

	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	tUAVDesc.Texture2D.MipSlice = 0;
	tUAVDesc.Format = eFmt;

	if (FAILED(DEVICE->CreateUnorderedAccessView(m_pTex, &tUAVDesc, &m_pUAV)))
		return;

	D3D11_RENDER_TARGET_VIEW_DESC tTargetDesc = {};

	tTargetDesc.Texture2D.MipSlice = 0;
	tTargetDesc.Format = eFmt;
	tTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	if (FAILED(DEVICE->CreateRenderTargetView(m_pTex, &tTargetDesc, &m_pTarget)))
		return;

	ID3D11Texture2D* pDepthTex = nullptr;

	D3D11_TEXTURE2D_DESC tDepthTexDesc = {};

	tDepthTexDesc.ArraySize = 1;
	tDepthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tDepthTexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tDepthTexDesc.Height = iHeight;
	tDepthTexDesc.Width = iWidth;
	tDepthTexDesc.MipLevels = 0;
	tDepthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	tDepthTexDesc.SampleDesc.Count = 1;
	tDepthTexDesc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&tDepthTexDesc, nullptr, &pDepthTex)))
		return;

	D3D11_DEPTH_STENCIL_VIEW_DESC tDepthDesc = {};

	tDepthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	tDepthDesc.Texture2D.MipSlice = 0;
	tDepthDesc.Format = eDepthfmt;

	if (FAILED(DEVICE->CreateDepthStencilView(pDepthTex, &tDepthDesc, &m_pDepth)))
		return;

	D3D11_SHADER_RESOURCE_VIEW_DESC tDepthSRVDesc = {};

	tDepthSRVDesc.Texture2D.MipLevels = 1;
	tDepthSRVDesc.Texture2D.MostDetailedMip = 0;
	tDepthSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	tDepthSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	if (FAILED(DEVICE->CreateShaderResourceView(pDepthTex, &tDepthSRVDesc, &m_pDepthSRV)))
	{
		SAFE_RELEASE(pDepthTex);
		return;
	}
		

	SAFE_RELEASE(pDepthTex);
}

void CMRT::SetShader(int iRegister, int iType)
{
	if(iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
	CONTEXT->VSSetShaderResources(iRegister, 1, &m_pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
	CONTEXT->HSSetShaderResources(iRegister, 1, &m_pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
	CONTEXT->DSSetShaderResources(iRegister, 1, &m_pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
	CONTEXT->GSSetShaderResources(iRegister, 1, &m_pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
	CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
	CONTEXT->CSSetShaderResources(iRegister, 1, &m_pSRV);

#ifdef _DEBUG
	TransformCBuffer tCBuffer = {};

	tCBuffer.matWVP = m_matDebugWVP;

	GET_SINGLE(CShaderManager)->UpdateCBuffer("Transform", &tCBuffer);
#endif
}

void CMRT::ResetShader(int iRegister, int iType)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_VERTEX))
	CONTEXT->VSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_HULL))
	CONTEXT->HSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_DOMAIN))
	CONTEXT->DSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_GEOMETRY))
	CONTEXT->GSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL))
	CONTEXT->PSSetShaderResources(iRegister, 1, &pSRV);
	if (iType & static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_COMPUTE))
	CONTEXT->CSSetShaderResources(iRegister, 1, &pSRV);
}

void CMRT::SetTarget()
{
	CONTEXT->OMGetRenderTargets(1, &m_pPrevTarget, &m_pPrevDepth);

	CONTEXT->OMSetRenderTargets(1, &m_pTarget, m_pDepth);
}

void CMRT::ResetTarget()
{
	CONTEXT->OMSetRenderTargets(1, &m_pPrevTarget, m_pPrevDepth);

	SAFE_RELEASE(m_pPrevTarget);
	SAFE_RELEASE(m_pPrevDepth);
}

void CMRT::Clear()
{
	CONTEXT->ClearRenderTargetView(m_pTarget, m_pClear);

	CONTEXT->ClearDepthStencilView(m_pDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

#ifdef _DEBUG
void CMRT::CreateDebugMat(const Vector3& vScale, const Vector3& vTranslate)
{
	Resolution tRS = RESOLUTION;

	m_matDebugWVP = Matrix::StaticScale(vScale) * Matrix::StaticTranslation(vTranslate);
	m_matDebugWVP *= DirectX::XMMatrixOrthographicOffCenterLH(0.f, static_cast<float>(tRS.iWidth), 0.f, static_cast<float>(tRS.iHeight), 0.f, 1000.f);

	m_matDebugWVP.Transpose();
}
#endif
void CMRT::SetDepthStencilView(int iRegister, int iType)
{
	CONTEXT->PSSetShaderResources(iRegister, 1, &m_pDepthSRV);
}

bool CMRT::Init()
{


	return true;
}