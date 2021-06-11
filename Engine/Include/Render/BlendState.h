#pragma once
#include "RenderState.h"
class CBlendState :
	public CRenderState
{
	friend class CRenderManager;

private:
	CBlendState();
	virtual ~CBlendState();

private:
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC>	m_vecDesc;
	UINT										m_iSampleMask;
	UINT										m_iPrevSampleMask;
	float										m_pBlendFactor[4];
	float										m_pPrevBlendFactor[4];

public:
	virtual void SetState();
	virtual void ResetState();
	void SetBlendFactor(float r, float g, float b, float a);

public:
	bool AddBlendDesc(bool bEnable = true, D3D11_BLEND eSrc = D3D11_BLEND_SRC_ALPHA, D3D11_BLEND eDest = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP eOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcAlpha = D3D11_BLEND_ONE, D3D11_BLEND eDestAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP eOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 iWrite = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(bool bAlpha = false, bool bBlend = false);
};

