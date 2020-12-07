#pragma once

#include "RenderState.h"
#include "../Device.h"

class CDepthStencilState :
	public CRenderState
{
	friend class CRenderManager;

private:
	CDepthStencilState();
	virtual ~CDepthStencilState();

private:
	std::vector<D3D11_DEPTH_STENCIL_DESC>	m_vecDesc;
	UINT									m_iStencilRef;
	UINT									m_iPrevStencilRef;

public:
	void SetStencilRef(UINT iRef);

public:
	virtual void SetState();
	virtual void ResetState();

public:
	bool AddDepthStencilDesc(bool bDepthEnable = true, D3D11_DEPTH_WRITE_MASK eWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS, bool bStencilEnable = false,
		UINT8 iReadMask = D3D11_DEFAULT_STENCIL_READ_MASK, UINT8 iWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		const D3D11_DEPTH_STENCILOP_DESC& tFront = {}, const D3D11_DEPTH_STENCILOP_DESC& tBack = {});
};

