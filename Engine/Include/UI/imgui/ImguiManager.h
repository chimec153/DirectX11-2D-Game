#pragma once

#include "../../GameEngine.h"

class CImguiManager
{
public:
	bool Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void PreRender(float fTime);
	void Render(float fTime);


	DECLARE_SINGLE(CImguiManager);
};

