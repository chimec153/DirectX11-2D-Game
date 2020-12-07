#include "RasterizerState.h"
#include "../Device.h"

CRasterizerState::CRasterizerState()
{
}

CRasterizerState::~CRasterizerState()
{
}

bool CRasterizerState::AddRasterizerState(D3D11_FILL_MODE eFillMode, D3D11_CULL_MODE eCullMode, 
	bool bFront, int iBias, float fBiasClamp, float fSSDB, bool bDepthClip, 
	bool bScissor, bool bMultiSample, bool bAntiAliasLine)
{
	D3D11_RASTERIZER_DESC tDesc = {};

	tDesc.FillMode = eFillMode;
	tDesc.CullMode = eCullMode;
	tDesc.FrontCounterClockwise = bFront;
	tDesc.DepthBias = iBias;
	tDesc.DepthBiasClamp = fBiasClamp;
	tDesc.SlopeScaledDepthBias = fSSDB;
	tDesc.DepthClipEnable = bDepthClip;
	tDesc.ScissorEnable = bScissor;
	tDesc.MultisampleEnable = bMultiSample;
	tDesc.AntialiasedLineEnable = bAntiAliasLine;

	m_vecDesc.push_back(tDesc);

	return true;
}

bool CRasterizerState::CreateRasterizerState()
{
	if(FAILED(DEVICE->CreateRasterizerState(&m_vecDesc[0],(ID3D11RasterizerState**)&m_pState)))
		return false;

	return true;
}

void CRasterizerState::SetState()
{
	CONTEXT->RSGetState((ID3D11RasterizerState * *)& m_pPrevState);

	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pState);
}

void CRasterizerState::ResetState()
{
	CONTEXT->RSSetState((ID3D11RasterizerState*)m_pPrevState);

	SAFE_RELEASE(m_pPrevState);
}
