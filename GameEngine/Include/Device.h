#pragma once

#include "GameEngine.h"

class CDevice
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView* m_pTargetView;
	ID3D11DepthStencilView* m_pDepthView;
	Resolution				m_tRS;
	float					m_pColor[4];
	HWND					m_hWnd;
	ID2D1Factory*			m_p2DFactory;
	ID2D1RenderTarget*		m_p2DTarget;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	Resolution GetResolution()	const;
	Vector2 GetRatio()	const;
	ID2D1Factory* GetFactory2D()	const;
	ID2D1RenderTarget* GetRenderTarget2D()	const;

public:
	bool Init(HWND hWnd, int iWidth, int iHeight, bool bWindowed);
	void ClearState();
	void Render();


	DECLARE_SINGLE(CDevice)
};

