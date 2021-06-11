#include "BlendState.h"
#include "../Device.h"

CBlendState::CBlendState()	:
	m_iSampleMask(0xffffffff),
	m_iPrevSampleMask(0xffffffff)
{
	memset(m_pBlendFactor, 0, sizeof(float) * 4);
	memset(m_pPrevBlendFactor, 0, sizeof(float) * 4);
}

CBlendState::~CBlendState()
{
}

void CBlendState::SetState()
{
	CONTEXT->OMGetBlendState((ID3D11BlendState * *)&m_pPrevState, m_pPrevBlendFactor, &m_iPrevSampleMask);

	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pState, m_pBlendFactor, m_iSampleMask);
}

void CBlendState::ResetState()
{
	CONTEXT->OMSetBlendState((ID3D11BlendState*)m_pPrevState, m_pPrevBlendFactor, m_iPrevSampleMask);

	SAFE_RELEASE(m_pPrevState);
}

void CBlendState::SetBlendFactor(float r, float g, float b, float a)
{
	m_pBlendFactor[0] = r;
	m_pBlendFactor[1] = g;
	m_pBlendFactor[2] = b;
	m_pBlendFactor[3] = a;
}

bool CBlendState::AddBlendDesc(bool bEnable, D3D11_BLEND eSrc, 
	D3D11_BLEND eDest, D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eOpAlpha,
	UINT8 iWrite)
{
	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};

	tDesc.BlendEnable = bEnable;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eOpAlpha;
	tDesc.RenderTargetWriteMask = iWrite;

	m_vecDesc.push_back(tDesc);

	return true;
}

bool CBlendState::CreateBlendState(bool bAlpha, bool bBlend)
{
	D3D11_BLEND_DESC tBlendDesc = {};

	tBlendDesc.AlphaToCoverageEnable = bAlpha;
	tBlendDesc.IndependentBlendEnable = bBlend;

	memcpy(tBlendDesc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC) * m_vecDesc.size());

	if (FAILED(DEVICE->CreateBlendState(&tBlendDesc, (ID3D11BlendState * *)& m_pState)))
		return false;

	return true;
}
