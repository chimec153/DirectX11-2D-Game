#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice()	:
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pTargetView(nullptr),
	m_pDepthView(nullptr)
{
	memset(m_pColor, 0, sizeof(float) * 4);

	m_pColor[1] = 0.1f;
	m_pColor[2] = 0.1f;
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthView);

	if (m_pContext)
		m_pContext->ClearState();

	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);
}

bool CDevice::Init(HWND hWnd, int iWidth, int iHeight, bool bWindowed)
{
	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	unsigned int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	iFlag |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.Height = iHeight;
	tDesc.BufferDesc.Width = iWidth;
	tDesc.BufferDesc.RefreshRate.Numerator = 1;
	tDesc.BufferDesc.RefreshRate.Denominator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferCount = 1;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.OutputWindow = hWnd;
	tDesc.Windowed = bWindowed;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL eLevel1 = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
		0, iFlag, &eLevel, 1, D3D11_SDK_VERSION, &tDesc,
		&m_pSwapChain, &m_pDevice, &eLevel1, &m_pContext)))
		return false;

	D3D11_FEATURE_DATA_THREADING tThread = {};

	m_pDevice->CheckFeatureSupport(D3D11_FEATURE_THREADING, &tThread, sizeof(tThread));

	ID3D11Texture2D* pBackBuffer = nullptr;

	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);

	m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTargetView);

	SAFE_RELEASE(pBackBuffer);

	D3D11_TEXTURE2D_DESC tDepth = {};

	tDepth.Width = m_tRS.iWidth;
	tDepth.Height = m_tRS.iHeight;
	tDepth.MipLevels = 1;
	tDepth.ArraySize = 1;
	tDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tDepth.SampleDesc.Count = 1;
	tDepth.SampleDesc.Quality = 0;
	tDepth.Usage = D3D11_USAGE_DEFAULT;
	tDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D* pDepthBuffer = nullptr;

	m_pDevice->CreateTexture2D(&tDepth, nullptr, &pDepthBuffer);

	m_pDevice->CreateDepthStencilView(pDepthBuffer, nullptr, &m_pDepthView);

	SAFE_RELEASE(pDepthBuffer);

	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthView);

	D3D11_VIEWPORT tView = {};

	tView.Width = (float)m_tRS.iWidth;
	tView.Height = (float)m_tRS.iHeight;
	tView.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &tView);

	return true;
}

void CDevice::ClearState()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_pColor);

	m_pContext->ClearDepthStencilView(m_pDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Render()
{
	m_pSwapChain->Present(0, 0);
}
