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

bool CBlendState::AddBlendDesc(bool bEnable, D3D11_BLEND eSrc, 
	D3D11_BLEND eDest, D3D11_BLEND_OP eOp, D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eOpAlpha, D3D11_COLOR_WRITE_ENABLE eWrite)
{
	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};

	tDesc.BlendEnable = true;
	tDesc.SrcBlend = eSrc;
	tDesc.DestBlend = eDest;
	tDesc.BlendOp = eOp;
	tDesc.SrcBlendAlpha = eSrcAlpha;
	tDesc.DestBlendAlpha = eDestAlpha;
	tDesc.BlendOpAlpha = eOpAlpha;
	tDesc.RenderTargetWriteMask = eWrite;

	m_vecDesc.push_back(tDesc);

	return true;
}

bool CBlendState::CreateBlendState(bool bAlpha, bool bBlend)
{
	D3D11_BLEND_DESC tBlendDesc = {};

	tBlendDesc.AlphaToCoverageEnable = false;
	tBlendDesc.IndependentBlendEnable = false;

	memcpy(tBlendDesc.RenderTarget, &m_vecDesc[0], sizeof(D3D11_RENDER_TARGET_BLEND_DESC));

	if (FAILED(DEVICE->CreateBlendState(&tBlendDesc, (ID3D11BlendState * *)& m_pState)))
		return false;

	return true;
}
