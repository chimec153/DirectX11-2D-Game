#pragma once

#include "GameEngine.h"

class CRenderManager
{
private:
	std::vector<class CSceneComponent*>		m_vecComponent;
	std::vector<class CSceneComponent*>		m_vec2DComponent;
	std::vector<class CSceneComponent*>			m_vecUIComponent;
	std::unordered_map<std::string, class CRenderState*>	m_mapState;
	std::unordered_map<unsigned __int64, class CInstancing*>	m_mapInstancing;
	class CInstancing* m_TileInst;
	std::list<class CLayer*>				m_LayerList;

public:
	bool Init();
	bool AddComponent(class CSceneComponent* pComponent);
	void AddCon(class CUIControl* pCon);
	void ClearComponent();
	void AddLayer(class CLayer* pLayer);
	void ClearInst();

public:
	void Render(float fTime);
	void Render2D(float fTime);
	void Render3D(float fTime);
	void RenderUI(float fTime);

public:
	bool AddBlendDesc(const std::string& strTag, bool bEnable = true, D3D11_BLEND eSrc = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND eDest = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP eOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND eSrcAlpha = D3D11_BLEND_ONE, D3D11_BLEND eDestAlpha = D3D11_BLEND_ZERO, D3D11_BLEND_OP eOpAlpha = D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE eWrite = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& strTag, bool bAlpha = false, bool bBlend = false);

public:
	bool AddDepthStencilDesc(const std::string& strTag, bool bDepthEnable = true, D3D11_DEPTH_WRITE_MASK eWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC eDepthFunc = D3D11_COMPARISON_LESS, bool bStencilEnable = false,
		UINT8 iReadMask = D3D11_DEFAULT_STENCIL_READ_MASK, UINT8 iWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		const D3D11_DEPTH_STENCILOP_DESC& tFront = {}, const D3D11_DEPTH_STENCILOP_DESC& tBack = {});

public:
	bool AddRasterizerState(const std::string& strTag, D3D11_FILL_MODE eFillMode = D3D11_FILL_SOLID, D3D11_CULL_MODE eCullMode = D3D11_CULL_BACK,
	bool bFront = FALSE, int iBias = D3D11_DEFAULT_DEPTH_BIAS, float fBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
	float fSSDB = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, bool bDepthClip = TRUE,
	bool bScissor = FALSE, bool bMultiSample = FALSE, bool bAntiAliasLine = FALSE);
	   bool CreateRasterizerState(const std::string& strTag);

public:
	bool CreateInstancing(class CMesh* pMesh, class CMaterial* pMtrl, int iCount, int iSize);
	class CInstancing* FindInstancing(unsigned __int64 iKey);
	void AddTileInstancingData(class CTile* pTile);
	void RenderInstancing(float fTime);
	void RenderTileInstancing(float fTime);

public:
	void SetState(const std::string& strTag);
	void ResetState(const std::string& strTag);
	
public:
	class CRenderState* FindState(const std::string& strTag);

private:
	static bool SortY(class CSceneComponent* pCom1, class CSceneComponent* pCom2);
	static bool SortZ(class CSceneComponent* pSrc, class CSceneComponent* pDst);

public:
	static unsigned int GetHashKey(const std::string& strKey);

	DECLARE_SINGLE(CRenderManager)
};

