#pragma once
#include "RenderState.h"
class CRasterizerState :
	public CRenderState
{
	friend class CRenderManager;

private:
	CRasterizerState();
	~CRasterizerState();

private:
	std::vector<D3D11_RASTERIZER_DESC>	m_vecDesc;
	
public:
	bool AddRasterizerState(D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID, D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK, 
		bool bFront = FALSE, int iBias = D3D11_DEFAULT_DEPTH_BIAS, float fBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP, 
		float fSSDB = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, bool bDepthClip = TRUE, 
		bool bScissor = FALSE, bool bMultiSample = FALSE, bool bAntiAliasLine = FALSE);
	bool CreateRasterizerState();

public:
	virtual void SetState();
	virtual void ResetState();
};

