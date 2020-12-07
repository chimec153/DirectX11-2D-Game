#include "DepthStencilState.h"

CDepthStencilState::CDepthStencilState()	:
	m_iStencilRef(0xffffffff),
	m_iPrevStencilRef(0xffffffff)
{
}

CDepthStencilState::~CDepthStencilState()
{
}

void CDepthStencilState::SetStencilRef(UINT iRef)
{
	m_iStencilRef = iRef;
}

void CDepthStencilState::SetState()
{
	CONTEXT->OMGetDepthStencilState((ID3D11DepthStencilState * *)& m_pPrevState, &m_iPrevStencilRef);

	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pState, m_iStencilRef);
}

void CDepthStencilState::ResetState()
{
	CONTEXT->OMSetDepthStencilState((ID3D11DepthStencilState*)m_pPrevState, m_iStencilRef);

	SAFE_RELEASE(m_pPrevState);
}

bool CDepthStencilState::AddDepthStencilDesc(bool bDepthEnable, D3D11_DEPTH_WRITE_MASK eWriteMask, 
	D3D11_COMPARISON_FUNC eDepthFunc, bool bStencilEnable, UINT8 iReadMask, UINT8 iWriteMask, 
	const D3D11_DEPTH_STENCILOP_DESC& tFront, const D3D11_DEPTH_STENCILOP_DESC& tBack)
{
	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	tDesc.DepthEnable = bDepthEnable;
	tDesc.DepthWriteMask = eWriteMask;
	tDesc.DepthFunc = eDepthFunc;
	tDesc.StencilEnable = bStencilEnable;
	tDesc.StencilReadMask = iReadMask;
	tDesc.StencilWriteMask = iWriteMask;
	tDesc.FrontFace = tFront;
	tDesc.BackFace = tBack;

	if (FAILED(DEVICE->CreateDepthStencilState(&tDesc, (ID3D11DepthStencilState * *)& m_pState)))
		return false;

	return true;
}