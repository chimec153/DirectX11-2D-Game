#pragma once
#include "../Ref.h"
class CMRT :
	public CRef
{
	friend class CRenderManager;

private:
	CMRT();
	CMRT(const CMRT& mrt);
	virtual ~CMRT();

private:
	ID3D11Texture2D*			m_pTex;
	ID3D11ShaderResourceView*	m_pSRV;
	ID3D11UnorderedAccessView*	m_pUAV;
	ID3D11ShaderResourceView*	m_pPrevSRV;
	ID3D11UnorderedAccessView*	m_pPrevUAV;
	ID3D11RenderTargetView*		m_pTarget;
	ID3D11RenderTargetView*		m_pPrevTarget;
	ID3D11DepthStencilView*		m_pDepth;
	ID3D11DepthStencilView*		m_pPrevDepth;
	ID3D11ShaderResourceView*	m_pDepthSRV;
#ifdef _DEBUG
	Matrix						m_matDebugWVP;
#endif
	float						m_pClear[4];

public:
	ID3D11RenderTargetView* GetRenderTarget()	const;

public:
	void CreateRenderTarget(int iWidth, int iHeight, DXGI_FORMAT eFmt, DXGI_FORMAT eDepthfmt);
	void SetShader(int iRegister, int iType = static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	void ResetShader(int iRegister, int iType = static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));
	void SetTarget();
	void ResetTarget();
	void Clear();
#ifdef _DEBUG
	void CreateDebugMat(const Vector3& vScale, const Vector3& vTranslate);
#endif
	void SetDepthStencilView(int iRegister, int iType = static_cast<int>(SHADER_CBUFFER_TYPE::CBUFFER_PIXEL));

public:
	virtual bool Init();

};

