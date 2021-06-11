#pragma once
#include "../Ref.h"
class CMultiRenderTarget :
	public CRef
{
	friend class CRenderManager;

private:
	CMultiRenderTarget();
	virtual ~CMultiRenderTarget();

private:
	ID3D11DepthStencilView*					m_pDepthView;
	ID3D11DepthStencilView*					m_pPrevDepthView;
	std::vector<ID3D11RenderTargetView*>	m_vecTarget;
	std::vector<ID3D11RenderTargetView*>	m_vecPrevTarget;
	ID3D11Texture2D*						m_pDepthTex;
	float									m_pClearColor[4];
	ID3D11ShaderResourceView*				m_pDepthSRV;

public:
	bool CreateDepthView(int iWidth, int iHeight, DXGI_FORMAT eFmt);
	void AddTarget(ID3D11RenderTargetView* pRTV);
	void SetTarget();
	void ResetTarget();
	void Clear();
	void SetDepthStencilView(int iRegister ,int iType = static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
};

