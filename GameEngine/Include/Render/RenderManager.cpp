#include "RenderManager.h"
#include "../Component/SceneComponent.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "../UI/UIControl.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "../Instancing.h"
#include "../Camera/CameraManager.h"
#include "../Component/Camera.h"
#include "../Tile/Tile.h"
#include "../Resource/ResourceManager.h"
#include "../Component/SpriteComponent.h"
#include "../Resource/Texture.h"
#include "../Layer.h"
#include "../Scene/SceneManager.h"
#include "../Component/Renderer.h"
#include "../Tile/TileMap.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/GraphicShader.h"
#include "PostProcess.h"
#include "MRT.h"
#include "../Resource/Mesh2D.h"
#include "MultiRenderTarget.h"
#include "../Component/Light.h"
#include "RenderInstance.h"
#include "../Resource/Animation.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	: 
	m_TileInst(nullptr)
	, m_pFullShader(nullptr)
	, m_pLightAccShader(nullptr)
	, m_pLightCombine(nullptr)
#ifdef _DEBUG
	, m_pDebugShader(nullptr)
#endif
	, m_pMesh(nullptr)
	, m_pAlphaBlend(nullptr)
	, m_pShadowShader(nullptr)
{

}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE_MAP(m_mapInstancing);
	SAFE_DELETE(m_TileInst);
	Safe_Release_Map(m_mapState);
	SAFE_RELEASE(m_pFullShader);
	SAFE_RELEASE(m_pShadowShader);
	Safe_Release_Map(m_mapMRT);
	SAFE_RELEASE(m_pMesh);
#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugShader);
#endif
	SAFE_RELEASE(m_pLightAccShader);
	SAFE_RELEASE(m_pLightCombine);
	SAFE_RELEASE(m_pAlphaBlend);
	Safe_Release_Map(m_mapMultiRenderTarget);
	for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); ++i)
	{
		Safe_Release_Map(m_mapInstance3D[i]);
	}
}

bool CRenderManager::Init()
{
	AddBlendDesc(ALPHA_BLEND);
	CreateBlendState(ALPHA_BLEND);

	m_pAlphaBlend = FindState(ALPHA_BLEND);

	AddBlendDesc("DecalBlend", true, 
		D3D11_BLEND_SRC_ALPHA, 
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE,
		D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD, 
		D3D11_COLOR_WRITE_ENABLE_RED |
		D3D11_COLOR_WRITE_ENABLE_GREEN |
		D3D11_COLOR_WRITE_ENABLE_BLUE);
	AddBlendDesc("DecalBlend", true,
		D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND_INV_SRC_ALPHA,
		D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE,
		D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_RED |
		D3D11_COLOR_WRITE_ENABLE_GREEN |
		D3D11_COLOR_WRITE_ENABLE_BLUE);
	AddBlendDesc("DecalBlend", false);
	CreateBlendState("DecalBlend",false, true);

	AddBlendDesc(LIGHT_BLEND, true, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO);
	CreateBlendState(LIGHT_BLEND);

	AddRasterizerState(CULL_NONE, D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateRasterizerState(CULL_NONE);

	AddRasterizerState(CULL_FRONT, D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateRasterizerState(CULL_FRONT);

	AddRasterizerState(WIRE_FRAME, D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
	CreateRasterizerState(WIRE_FRAME);

	AddRasterizerState("Shadow", D3D11_FILL_SOLID, D3D11_CULL_BACK, false, 1000, 0.f, 2.f);
	CreateRasterizerState("Shadow");

	AddDepthStencilDesc("DepthNoWrite", false);
	AddDepthStencilDesc(DEPTH_READ, true, D3D11_DEPTH_WRITE_MASK_ZERO);
	AddDepthStencilDesc("Default");

	D3D11_DEPTH_STENCILOP_DESC tFrt = {};
	D3D11_DEPTH_STENCILOP_DESC tBck = {};

	tFrt.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	tFrt.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tFrt.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tFrt.StencilFunc = D3D11_COMPARISON_EQUAL;

	tBck.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	tBck.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tBck.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tBck.StencilFunc = D3D11_COMPARISON_EQUAL;

	AddDepthStencilDesc("Silhouette", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS,
		true, 0xff, 0xff, tFrt, tBck);

	AddDepthStencilDesc("Character", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_ALWAYS,
		true, 0xff, 0xff, tFrt, tBck);

	tFrt.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;

	AddDepthStencilDesc("Colossus", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tFrt, tBck);

	CDepthStencilState* pState = (CDepthStencilState*)FindState("Silhouette");

	pState->SetStencilRef(0x1);

	SAFE_RELEASE(pState);

	pState = (CDepthStencilState*)FindState("Character");

	pState->SetStencilRef(0);

	SAFE_RELEASE(pState);

	pState = (CDepthStencilState*)FindState("Colossus");

	pState->SetStencilRef(0x1);

	SAFE_RELEASE(pState);

	tFrt.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	tFrt.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tFrt.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tFrt.StencilFunc = D3D11_COMPARISON_ALWAYS;

	tBck.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	tBck.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	tBck.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	tBck.StencilFunc = D3D11_COMPARISON_ALWAYS;

	AddDepthStencilDesc("ForeGround", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tFrt, tBck);

	pState = (CDepthStencilState*)FindState("ForeGround");

	pState->SetStencilRef(0x1);

	SAFE_RELEASE(pState);

	AddDepthStencilDesc("DepthWriteForeGround", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tFrt, tBck);

	pState = (CDepthStencilState*)FindState("DepthWriteForeGround");

	pState->SetStencilRef(0x1);

	SAFE_RELEASE(pState);

	CMRT* pMRT = CreateMRT("Scene");

	Resolution tRS = RESOLUTION;

	pMRT->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	CMRT* pAlbedo = CreateMRT("Albedo");
	CMRT* pGBuffer1 = CreateMRT("GBuffer1");
	CMRT* pGBuffer2 = CreateMRT("GBuffer2");
	CMRT* pGBuffer3 = CreateMRT("GBuffer3");
	CMRT* pGBuffer4 = CreateMRT("GBuffer4");
	CMRT* pGBuffer5 = CreateMRT("GBuffer5");
	pAlbedo->CreateRenderTarget(tRS.iWidth, tRS.iHeight, 
		DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pGBuffer1->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pGBuffer2->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pGBuffer3->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pGBuffer4->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pGBuffer5->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);

	if (!CreateMultiTargetView("Albedo", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	AddTarget("Albedo", "Albedo");
	AddTarget("Albedo", "GBuffer1");
	AddTarget("Albedo", "GBuffer2");
	AddTarget("Albedo", "GBuffer3");
	AddTarget("Albedo", "GBuffer4");
	AddTarget("Albedo", "GBuffer5");

	CMRT* pLightDif = CreateMRT("LightDif");
	CMRT* pLightSpc = CreateMRT("LightSpc");
	CMRT* pLightEmv = CreateMRT("LightEmv");

	pLightDif->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pLightSpc->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pLightEmv->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);

	if (!CreateMultiTargetView("LightAcc", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	AddTarget("LightAcc", "LightDif");
	AddTarget("LightAcc", "LightSpc");
	AddTarget("LightAcc", "LightEmv");

	if (!CreateMultiTargetView("Decal", tRS.iWidth, tRS.iHeight, DXGI_FORMAT_D24_UNORM_S8_UINT))
		return false;

	AddTarget("Decal", "Albedo");
	AddTarget("Decal", "GBuffer1");
	AddTarget("Decal", "GBuffer3");

	CMRT* pShadow = CreateMRT("Shadow");

	pShadow->CreateRenderTarget(tRS.iWidth, tRS.iHeight, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D24_UNORM_S8_UINT);

#ifdef _DEBUG
	pMRT->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(0.f, 0.f, 0.f));
	pAlbedo->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 0.f, 0.f));
	pGBuffer1->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 100.f, 0.f));
	pGBuffer2->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 200.f, 0.f));
	pGBuffer3->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 300.f, 0.f));
	pGBuffer4->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 400.f, 0.f));
	pGBuffer5->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(100.f, 500.f, 0.f));
	pLightDif->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(200.f, 100.f, 0.f));
	pLightSpc->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(200.f, 200.f, 0.f));
	pLightEmv->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(200.f, 300.f, 0.f));
	pShadow->CreateDebugMat(Vector3(100.f, 100.f, 0.f), Vector3(300.f, 100.f, 0.f));
#endif

	SAFE_RELEASE(pMRT);
	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pGBuffer1);
	SAFE_RELEASE(pGBuffer2);
	SAFE_RELEASE(pGBuffer3);
	SAFE_RELEASE(pGBuffer4);
	SAFE_RELEASE(pGBuffer5);
	SAFE_RELEASE(pLightDif);
	SAFE_RELEASE(pLightSpc);
	SAFE_RELEASE(pLightEmv);
	SAFE_RELEASE(pShadow);

	m_pFullShader = static_cast<CGraphicShader*>(GET_SINGLE(CShaderManager)->FindShader("Full"));
	m_pMesh = GET_SINGLE(CResourceManager)->GetDefaultMesh();

#ifdef _DEBUG
	m_pDebugShader = static_cast<CGraphicShader*>(GET_SINGLE(CShaderManager)->FindShader("Debug"));
#endif

	m_pLightAccShader = static_cast<CGraphicShader*>(GET_SINGLE(CShaderManager)->FindShader("LightAcc"));
	m_pLightCombine = static_cast<CGraphicShader*>(GET_SINGLE(CShaderManager)->FindShader("LightCombine"));

	m_pShadowShader = GET_SINGLE(CShaderManager)->FindShader("ShadowShader");

	return true;
}

bool CRenderManager::AddComponent(CSceneComponent* pComponent)
{
	SCENE_COMPONENT_TYPE eType = pComponent->GetSceneComponentType();

	if (pComponent->IsPostProcess())
	{
		m_PostProcessList.push_back(pComponent->GetPostProcess());
	}
	else if (eType == SCENE_COMPONENT_TYPE::SCT_3D)
	{
		pComponent->SetInstance(true);

		CMaterial* pMtrl = pComponent->GetMaterial();
		CMesh* pMesh = pComponent->GetMesh();
		CShader* pShader = pComponent->GetShader();
		CLayer* pLayer = pComponent->GetLayer();
		int iZOrder = -1;
		if (pLayer)
		{
			iZOrder = pLayer->GetZOrder();
		}
		else
		{
			assert(false);
		}

		CRenderInstance* pInst = FindInstance3D(pMesh, pShader, pMtrl, iZOrder);

		if (!pInst)
		{
			CShader* pInstShader = GET_SINGLE(CShaderManager)->FindShader("Inst3D");

			pInst = new CRenderInstance;

			int iBoneCount = 0;

			if (pMesh)
			{
				if (pMesh->GetMeshType() == MESH_TYPE::MT_ANIMATION)
				{
					CRenderer* pRenderer = pComponent->GetRenderer();
					if (pRenderer)
					{
						CAnimation* pAni = pRenderer->GetAnimation();
						if (pAni)
						{
							CSkeleton* pSkeleton = pAni->GetSkeleton();
							if (pSkeleton)
							{
								iBoneCount = static_cast<int>(pSkeleton->GetBoneCount());
							}
						}

						SAFE_RELEASE(pAni);

					}
					SAFE_RELEASE(pRenderer);
				}
			}

			pInst->Init(pMesh, pInstShader, sizeof(MATRIXIES), 1000, iBoneCount);

			SAFE_RELEASE(pInstShader);

			size_t iKey = GetKey(pMesh, pShader, pMtrl);

			pInst->AddRef();

			m_mapInstance3D[iZOrder].insert(std::make_pair(iKey, pInst));
		}

		SAFE_RELEASE(pMesh);
		SAFE_RELEASE(pMtrl);
		SAFE_RELEASE(pShader);

		MATRIXIES tMat = {};

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();
		const Matrix& matWorld = pComponent->GetMatWorld();

		if (pCam)
		{
			tMat.matWV = matWorld * pCam->GetViewMat();
			tMat.matWVP = tMat.matWV * pCam->GetProjMat();

			tMat.matWV.Transpose();
			tMat.matWVP.Transpose();
		}

		Light* pLight = Light::GetMainLight();

		if (pLight)
		{
			tMat.matLightWVP = matWorld * pLight->GetVP();
			tMat.matLightWVP.Transpose();
		}

		SAFE_RELEASE(pLight);

		SAFE_RELEASE(pCam);

		int iCount = pInst->GetCount();
		pComponent->SetInstIndex(iCount);
		pComponent->SetInstancing(pInst);
		pInst->AddData(&tMat);
		pInst->AddComponent(pComponent);

		SAFE_RELEASE(pInst);
	}

	else if (eType == SCENE_COMPONENT_TYPE::SCT_2D)
	{
		if (pComponent->IsInstanced())
		{
			CMaterial* pMtrl = pComponent->GetMaterial();
			CMesh* pMesh = pComponent->GetMesh();

			if (!pMtrl || !pMesh)
				return false;

			unsigned int iMtrlKey = GetHashKey(pMtrl->GetName());
			unsigned int iMeshKey = GetHashKey(pMesh->GetName());

			unsigned __int64 iKey = iMtrlKey;

			iKey <<= 32;

			iKey |= iMeshKey;

			CInstancing* pInst = FindInstancing(iKey);

			SCENECOMPONENT_CLASS_TYPE eType = pComponent->GetSceneComponentClassType();

			if (!pInst)
			{
				pInst = new CInstancing;

				if (eType == SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE)
				{
					CShader* pShader = GET_SINGLE(CShaderManager)->FindShader("Inst");

					pInst->Init(pMesh, pMtrl, pShader, 10000, sizeof(InstancedData2D));

					SAFE_RELEASE(pShader);
				}

				else if (eType == SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D ||
					eType == SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL ||
					eType == SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERRECT ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERLINE ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERCIRCLE)
				{
					CShader* pShader = GET_SINGLE(CShaderManager)->FindShader("InstCollider");

					pInst->Init(pMesh, pMtrl, pShader, 10000, sizeof(INSTCOLLIDER2D));

					SAFE_RELEASE(pShader);
				}

				else
				{
					CShader* pShader = pComponent->GetShader();

					pInst->Init(pMesh, pMtrl, pShader, 10000, sizeof(InstancedData2D));

					SAFE_RELEASE(pShader);
				}

				CRenderer* pRenderer = pComponent->GetRenderer();

				std::vector<CRenderState*> vecState = pRenderer->GetVecRenderState();

				size_t iSz = vecState.size();

				for (size_t i = 0; i < iSz; ++i)
				{
					pInst->AddState(vecState[i]);
				}

				SAFE_RELEASE(pRenderer);

				CLayer* pLayer = pComponent->GetLayer();

				pLayer->AddInst(pInst);

				pInst->SetLayer(pLayer);

				m_mapInstancing.insert(std::make_pair(iKey, pInst));
			}

			CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

			if (eType == SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE)
			{
				InstancedData2D tData = {};

				tData.matWVP = pComponent->GetMatWorld() * pCam->GetViewMat() * pCam->GetProjMat();

				tData.matWVP.Transpose();

				tData.vPivot = pComponent->GetPivot();
				tData.vSize = pComponent->GetMeshSize();

				Frame tFrame = ((CSpriteComponent*)pComponent)->GetFrame();

				tData.vTexSize = ((CSpriteComponent*)pComponent)->GetTextureSize();
				tData.vStart = tFrame.vStart;
				tData.vEnd = tFrame.vEnd;

				pInst->AddInstanceData(&tData);
				pInst->AddCom(pComponent);
			}

			else if (eType == SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D ||
				eType == SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL ||
				eType == SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT ||
				eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERRECT ||
				eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERLINE ||
				eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERCIRCLE)
			{
				INSTCOLLIDER2D tData = {};

				tData.matWVP = pComponent->GetMatWorld() * pCam->GetViewMat() * pCam->GetProjMat();
				tData.matWVP.Transpose();
				tData.vMeshSize = pComponent->GetMeshSize();
				tData.vPivot = pComponent->GetPivot();
				tData.vColor = pMtrl->GetDif();

				pInst->AddInstanceData(&tData);
				pInst->AddCom(pComponent);
			}

			else
			{
				InstancedData2D tData = {};

				tData.matWVP = pComponent->GetMatWorld() * pCam->GetViewMat() * pCam->GetProjMat();

				tData.matWVP.Transpose();

				tData.vPivot = pComponent->GetPivot();
				tData.vSize = pComponent->GetMeshSize();

				size_t iCnt = pMtrl->GetTextureCount();

				if (iCnt > 0)
				{
					tData.vTexSize = pMtrl->GetTextureSize();
				}

				else
				{
					tData.vTexSize = Vector2(1.f, 1.f);
				}

				tData.vStart = Vector2(0.f, 0.f);
				tData.vEnd = Vector2(1.f, 1.f);

				pInst->AddInstanceData(&tData);
				pInst->AddCom(pComponent);
			}


			SAFE_RELEASE(pCam);

			SAFE_RELEASE(pMesh);
			SAFE_RELEASE(pMtrl);
		}

		else
		{

			CLayer* pLayer = pComponent->GetLayer();

			if (pLayer)
				pLayer->AddCom(pComponent);
		}
	}

	else
	{
		CLayer* pLayer = pComponent->GetLayer();

		if (pLayer)
			pLayer->AddCom(pComponent);

	}

	return true;
}

void CRenderManager::AddCon(CUIControl* pCon)
{
	m_vecUIComponent.push_back(pCon);
}

void CRenderManager::ClearComponent()
{
	for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); i++)
	{
		std::list<CLayer*>::iterator iter = m_LayerList[i].begin();
		std::list<CLayer*>::iterator iterEnd = m_LayerList[i].end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Clear();
		}

		m_LayerList[i].clear();
	}

	m_vec2DComponent.clear();
	m_vecComponent.clear();
	m_vecUIComponent.clear();

	std::unordered_map<std::string, CMRT*>::iterator iter = m_mapMRT.begin();
	std::unordered_map<std::string, CMRT*>::iterator iterEnd = m_mapMRT.end();

	for(;iter!=iterEnd;++iter)
	{
		iter->second->Clear();
	}

	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iterM = m_mapMultiRenderTarget.begin();
	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iterMEnd = m_mapMultiRenderTarget.end();

	for (; iterM != iterMEnd; ++iterM)
	{
		iterM->second->Clear();
	}
}

void CRenderManager::AddLayer(CLayer* pLayer)
{
	int iZOrder =pLayer->GetZOrder();

	m_LayerList[iZOrder].push_back(pLayer);
}

void CRenderManager::ClearInst()
{
	std::unordered_map<unsigned __int64, CInstancing*>::iterator iter = m_mapInstancing.begin();
	std::unordered_map<unsigned __int64, CInstancing*>::iterator iterEnd = m_mapInstancing.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Clear();
	}

	if(m_TileInst)
		m_TileInst->Clear();

	SAFE_DELETE_MAP(m_mapInstancing);
	SAFE_DELETE(m_TileInst);
}

void CRenderManager::PreRender(float fTime)
{
	for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); ++i)
	{
		std::unordered_map<size_t, CRenderInstance*>::iterator iter = m_mapInstance3D[i].begin();
		std::unordered_map<size_t, CRenderInstance*>::iterator iterEnd = m_mapInstance3D[i].end();

		for (; iter != iterEnd; ++iter)
		{
			if (iter->second->GetCount() < 7)
			{
				const std::list<CSceneComponent*>& ComponentList = 
					iter->second->GetComponentList();

				std::list<CSceneComponent*>::const_iterator iterC = ComponentList.begin();
				std::list<CSceneComponent*>::const_iterator iterCEnd = ComponentList.end();

				for (; iterC != iterCEnd; ++iterC)
				{
					(*iterC)->SetInstance(false);
					(*m_LayerList[i].begin())->AddCom(*iterC);
				}

				iter->second->Clear();
			}
		}
	}

	(*m_LayerList[static_cast<int>(LAYER_TYPE::TERRAIN)].begin())->CameraZSort();
	(*m_LayerList[static_cast<int>(LAYER_TYPE::CUBEMAP)].begin())->CameraZSort();
	(*m_LayerList[static_cast<int>(LAYER_TYPE::ALBEDO)].begin())->CameraZSort();
	(*m_LayerList[static_cast<int>(LAYER_TYPE::DECAL)].begin())->CameraZSort(true);
	(*m_LayerList[static_cast<int>(LAYER_TYPE::PARTICLE)].begin())->CameraZSort(true);
	(*m_LayerList[static_cast<int>(LAYER_TYPE::COLLIDER)].begin())->CameraZSort();
	(*m_LayerList[static_cast<int>(LAYER_TYPE::ALPHA)].begin())->CameraZSort(true);
}

void CRenderManager::Render(float fTime)
{
	Render3D(fTime);


#ifdef _DEBUG
	SetState("DepthNoWrite");
	std::unordered_map<std::string, CMRT*>::iterator iterM = m_mapMRT.begin();
	std::unordered_map<std::string, CMRT*>::iterator iterMEnd = m_mapMRT.end();

	m_pDebugShader->SetShader();

	for (; iterM != iterMEnd; ++iterM)
	{
		iterM->second->SetShader(0);

		m_pMesh->Render(fTime);
		iterM->second->ResetShader(0);
	}
	ResetState("DepthNoWrite");

#endif

	m_PostProcessList.clear();
}

void CRenderManager::RenderTarget(float fTime)
{
	//SetState("DepthNoWrite");

	SetShader("Scene", 98);

	m_pFullShader->SetShader();
	m_pMesh->Render(fTime);

	ResetShader("Scene", 98);
	//ResetState("DepthNoWrite");
}

void CRenderManager::PostRender(float fTime)
{
	SetShader("Scene", 98);

	std::list<CPostProcess*>::iterator iter = m_PostProcessList.begin();
	std::list<CPostProcess*>::iterator iterEnd = m_PostProcessList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	ResetShader("Scene", 98);
}

void CRenderManager::Render2D(float fTime)
{
	std::sort(m_vec2DComponent.begin(), m_vec2DComponent.end(), CRenderManager::SortY);


	size_t iSize = m_vec2DComponent.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vec2DComponent[i]->Render(fTime);
	}
		
}

void CRenderManager::Render3D(float fTime)
{
	RenderShadow(fTime);
	RenderCubemap(fTime);
	RenderTerrain(fTime);
	RenderAlbedo(fTime);
	RenderDecal(fTime);
	RenderLightAcc(fTime);
	RenderLightCombine(fTime);
	RenderScene(fTime);
	RenderParticle(fTime);
	RenderCollider(fTime);
	RenderAlpha(fTime);
	RenderUILayer(fTime);
}

void CRenderManager::RenderUI(float fTime)
{
	std::sort(m_vecUIComponent.begin(), m_vecUIComponent.end(), CRenderManager::SortZ);

	size_t iSize = m_vecUIComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecUIComponent[i]->Render(fTime);
}

void CRenderManager::RenderDecal(float fTime)
{
	
	CMultiRenderTarget* pDecal = FindMultiRenderTarget("Decal");
	CMRT* pGBuffer2 = FindMRT("GBuffer2");

	pGBuffer2->SetShader(6);
	SetShader("GBuffer4", 14);
	SetShader("GBuffer5", 15);
	pDecal->SetTarget();

	SetState("DecalBlend");
	SetState(NO_DEPTH);

	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::DECAL)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::DECAL)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	ResetState(NO_DEPTH);
	ResetState("DecalBlend");

	pDecal->ResetTarget();
	pGBuffer2->ResetShader(6);
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->PSSetShaderResources(14, 1, &pSRV);
	CONTEXT->PSSetShaderResources(15, 1, &pSRV);

	SAFE_RELEASE(pGBuffer2);
	SAFE_RELEASE(pDecal);
}

void CRenderManager::RenderShadow(float fTime)
{
	CMRT* pShadow = FindMRT("Shadow");
	CRenderState* pState = FindState("Shadow");

	Light* pLight = Light::GetMainLight();

	pLight->SetShader();

	SAFE_RELEASE(pLight);

	m_pShadowShader->SetShader();

	pShadow->SetTarget();
	pState->SetState();

	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::TERRAIN)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::TERRAIN)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderShadow(fTime);
	}

	std::unordered_map<size_t, CRenderInstance*>::iterator iterM =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::TERRAIN)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterMEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::TERRAIN)].end();

	for (; iterM != iterMEnd; ++iterM)
	{
		iterM->second->RenderShadow(fTime);
	}

	iter = m_LayerList[static_cast<int>(LAYER_TYPE::ALBEDO)].begin();
	iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::ALBEDO)].end();
	m_pShadowShader->SetShader();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderShadow(fTime);
	}

	iterM =	m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALBEDO)].begin();
	iterMEnd = m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALBEDO)].end();

	for (; iterM != iterMEnd; ++iterM)
	{
		iterM->second->RenderShadow(fTime);
	}

	iter = m_LayerList[static_cast<int>(LAYER_TYPE::ALPHA)].begin();
	iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::ALPHA)].end();
	m_pShadowShader->SetShader();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->RenderShadow(fTime);
	}

	iterM = m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALPHA)].begin();
	iterMEnd = m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALPHA)].end();

	for (; iterM != iterMEnd; ++iterM)
	{
		iterM->second->RenderShadow(fTime);
	}

	pState->ResetState();
	pShadow->ResetTarget();

	SAFE_RELEASE(pState);
	SAFE_RELEASE(pShadow);
}

void CRenderManager::RenderTerrain(float fTime)
{
	ClearMultiTarget("Albedo");
	SetMultiTarget("Albedo");
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::TERRAIN)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::TERRAIN)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	std::unordered_map<size_t, CRenderInstance*>::iterator iterI = 
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::TERRAIN)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd = 
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::TERRAIN)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
	ResetMultiTarget("Albedo");
}

void CRenderManager::RenderCubemap(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::CUBEMAP)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::CUBEMAP)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	std::unordered_map<size_t, CRenderInstance*>::iterator iterI =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::CUBEMAP)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::CUBEMAP)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
}

void CRenderManager::RenderAlbedo(float fTime)
{
	SetMultiTarget("Albedo");

	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::ALBEDO)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::ALBEDO)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}

	std::unordered_map<size_t, CRenderInstance*>::iterator iterI = m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALBEDO)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd = m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALBEDO)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}

	ResetMultiTarget("Albedo");
}

void CRenderManager::RenderParticle(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::PARTICLE)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::PARTICLE)].end();

	m_pAlphaBlend->SetState();

	CMRT* pMRT = FindMRT("GBuffer2");

	pMRT->SetShader(6);
	SetState(NO_DEPTH);

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
	std::unordered_map<size_t, CRenderInstance*>::iterator iterI =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::PARTICLE)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::PARTICLE)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
	ResetState(NO_DEPTH);

	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->PSSetShaderResources(6, 1, &pSRV);	

	SAFE_RELEASE(pMRT);

	m_pAlphaBlend->ResetState();
}

void CRenderManager::RenderCollider(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::COLLIDER)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::COLLIDER)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
	std::unordered_map<size_t, CRenderInstance*>::iterator iterI =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::COLLIDER)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::COLLIDER)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
}

void CRenderManager::RenderAlpha(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::ALPHA)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::ALPHA)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
	std::unordered_map<size_t, CRenderInstance*>::iterator iterI =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALPHA)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::ALPHA)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
}

void CRenderManager::RenderUILayer(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList[static_cast<int>(LAYER_TYPE::UI)].begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList[static_cast<int>(LAYER_TYPE::UI)].end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
	}
	std::unordered_map<size_t, CRenderInstance*>::iterator iterI =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::UI)].begin();
	std::unordered_map<size_t, CRenderInstance*>::iterator iterIEnd =
		m_mapInstance3D[static_cast<int>(LAYER_TYPE::UI)].end();

	for (; iterI != iterIEnd; ++iterI)
	{
		iterI->second->Render(fTime);
	}
}

void CRenderManager::RenderLightAcc(float fTime)
{
	CMRT* pGBuffer1 = FindMRT("GBuffer1");
	CMRT* pGBuffer2 = FindMRT("GBuffer2");
	CMRT* pGBuffer3 = FindMRT("GBuffer3");
	CMRT* pShadow = FindMRT("Shadow");

	pGBuffer1->SetShader(5);
	pGBuffer2->SetShader(6);
	pGBuffer3->SetShader(7);
	pShadow->SetDepthStencilView(8);

	SetState(NO_DEPTH);
	SetState(LIGHT_BLEND);

	CMultiRenderTarget* pTarget = FindMultiRenderTarget("LightAcc");

	pTarget->Clear();
	pTarget->SetTarget();

	m_pLightAccShader->SetShader();

	Light::RenderAll(fTime);

	pTarget->ResetTarget();

	ResetState(NO_DEPTH);
	ResetState(LIGHT_BLEND);

	pGBuffer1->ResetShader(5);
	pGBuffer2->ResetShader(6);
	pGBuffer3->ResetShader(7);
	pShadow->ResetShader(8);

	SAFE_RELEASE(pTarget);
	SAFE_RELEASE(pGBuffer1);
	SAFE_RELEASE(pGBuffer2);
	SAFE_RELEASE(pGBuffer3);
	SAFE_RELEASE(pShadow);
}

void CRenderManager::RenderLightCombine(float fTime)
{
	CMRT* pMRT = FindMRT("Scene");

	//pMRT->Clear();
	pMRT->SetTarget();

	CMRT* pAlbedo = FindMRT("Albedo");
	CMRT* pLightDif = FindMRT("LightDif");
	CMRT* pLightSpc = FindMRT("LightSpc");
	CMRT* pLightEmv = FindMRT("LightEmv");

	pAlbedo->SetShader(4);
	pLightDif->SetShader(11);
	pLightSpc->SetShader(12);
	pLightEmv->SetShader(13);

	m_pLightCombine->SetShader();

	SetState(NO_DEPTH);

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	UINT iOffset = 0;
	UINT iStride = 0;
	ID3D11Buffer* pBuffer = nullptr;
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iStride, &iOffset);
	CONTEXT->Draw(4, 0);

	ResetState(NO_DEPTH);

	pAlbedo->ResetShader(4);
	pLightDif->ResetShader(11);
	pLightSpc->ResetShader(12);
	pLightEmv->ResetShader(13);

	pMRT->ResetTarget();
	SAFE_RELEASE(pMRT);
	SAFE_RELEASE(pAlbedo);
	SAFE_RELEASE(pLightDif);
	SAFE_RELEASE(pLightSpc);
	SAFE_RELEASE(pLightEmv);
}

void CRenderManager::RenderScene(float fTime)
{
	CMRT* pMRT = FindMRT("Scene");

	pMRT->SetShader(98);

	m_pFullShader->SetShader();

	SetState(NO_DEPTH);

	
	m_pAlphaBlend->SetState();

	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	ID3D11Buffer* pBuffer = nullptr;
	UINT iOffset = 0;
	UINT iStride = 0;
	CONTEXT->IASetVertexBuffers(0, 1, &pBuffer, &iOffset, &iStride);
	CONTEXT->Draw(4, 0);

	m_pAlphaBlend->ResetState();

	ResetState(NO_DEPTH);

	pMRT->ResetShader(98);
	SAFE_RELEASE(pMRT);
}

bool CRenderManager::AddBlendDesc(const std::string& strTag, bool bEnable, 
	D3D11_BLEND eSrc, D3D11_BLEND eDest, D3D11_BLEND_OP eOp, 
	D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eOpAlpha, 
	UINT8 iWrite)
{
	CBlendState* pState = (CBlendState*)FindState(strTag);

	if (!pState)
	{
		pState = new CBlendState;

		pState->SetName(strTag);

		m_mapState.insert(std::make_pair(strTag, pState));

		pState->AddRef();
	}

	if (!pState->AddBlendDesc(bEnable, eSrc, eDest, eOp, eSrcAlpha, eDestAlpha, eOpAlpha, iWrite))
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_RELEASE(pState);
		return false;
	}

	SAFE_RELEASE(pState);

	return true;
}

bool CRenderManager::CreateBlendState(const std::string& strTag, bool bAlpha, bool bBlend)
{
	CBlendState* pState = (CBlendState*)FindState(strTag);

	if (!pState)
		return false;

	if (!pState->CreateBlendState(bAlpha, bBlend))
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_RELEASE(pState);
		return false;
	}

	SAFE_RELEASE(pState);

	return true;
}

bool CRenderManager::AddDepthStencilDesc(const std::string& strTag, bool bDepthEnable, 
	D3D11_DEPTH_WRITE_MASK eWriteMask, D3D11_COMPARISON_FUNC eDepthFunc, bool bStencilEnable, 
	UINT8 iReadMask, UINT8 iWriteMask, const D3D11_DEPTH_STENCILOP_DESC& tFront, const D3D11_DEPTH_STENCILOP_DESC& tBack)
{
	CDepthStencilState* pState = (CDepthStencilState*)FindState(strTag);

	if (!pState)
	{
		pState = new CDepthStencilState;

		pState->AddRef();

		pState->SetName(strTag);
		
		m_mapState.insert(std::make_pair(strTag, pState));
	}

	if (!pState->AddDepthStencilDesc(bDepthEnable, eWriteMask, eDepthFunc, bStencilEnable, iReadMask, iWriteMask, tFront, tBack))
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_RELEASE(pState);
		return false;
	}

	SAFE_RELEASE(pState);

	return true;
}

bool CRenderManager::AddRasterizerState(const std::string& strTag, D3D11_FILL_MODE eFillMode, 
	D3D11_CULL_MODE eCullMode, bool bFront, int iBias, float fBiasClamp, float fSSDB, 
	bool bDepthClip, bool bScissor, bool bMultiSample, bool bAntiAliasLine)
{
	CRasterizerState* pState = (CRasterizerState*)FindState(strTag);

	if (pState)
		return false;

	pState = new CRasterizerState;

	pState->SetName(strTag);
	
	if (!pState->AddRasterizerState(eFillMode, eCullMode, bFront, iBias, fBiasClamp,
		fSSDB, bDepthClip, bScissor, bMultiSample, bAntiAliasLine))
	{
		SAFE_RELEASE(pState);
		return false;
	}

	m_mapState.insert(std::make_pair(strTag, pState));

	return true;
}

bool CRenderManager::CreateRasterizerState(const std::string& strTag)
{
	CRasterizerState* pState = (CRasterizerState*)FindState(strTag);

	if (!pState)
		return false;

	if (!pState->CreateRasterizerState())
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_RELEASE(pState);
		return false;
	}

	SAFE_RELEASE(pState);

	return true;
}

bool CRenderManager::CreateInstancing(CMesh* pMesh, CMaterial* pMtrl, CShader* pShader, int iCount, int iSize)
{
	unsigned int iMtrlKey = GetHashKey(pMtrl->GetName());
	unsigned int iMeshKey = GetHashKey(pMesh->GetName());

	unsigned __int64 iKey = iMtrlKey;

	iKey <<= 32;

	iKey |= iMeshKey;

	CInstancing* pInst = FindInstancing(iKey);

	if (pInst)
		return false;

	pInst = new CInstancing;

	if (!pInst->Init(pMesh, pMtrl, pShader, iCount, iSize))
	{
		SAFE_DELETE(pInst);
		return false;
	}

	m_mapInstancing.insert(std::make_pair(iKey, pInst));

	return true;
}

CInstancing* CRenderManager::FindInstancing(unsigned __int64 iKey)
{
	std::unordered_map<unsigned __int64, class CInstancing*>::iterator iter = m_mapInstancing.find(iKey);

	if (iter == m_mapInstancing.end())
		return nullptr;

	return iter->second;
}

void CRenderManager::AddTileInstancingData(CTile* pTile)
{
	CLayer* pLayer = pTile->GetLayer();

	CInstancing* pTileInst = pLayer->GetTileInst();

	if (!pTileInst)
	{
		pTileInst = new CInstancing;

		CMaterial* pTileMtrl = pTile->GetMap()->GetMaterial();

		CShader* pShader = GET_SINGLE(CShaderManager)->FindShader("Inst");

		CMesh* pMesh = (CMesh*)GET_SINGLE(CResourceManager)->GetDefaultMesh();

		if (!pTileInst->Init(pMesh, pTileMtrl, pShader,10000, sizeof(InstancedData2D)))
		{
			SAFE_DELETE(pTileInst);
			SAFE_RELEASE(pMesh);
			SAFE_RELEASE(pTileMtrl);
			return;
		}

		SAFE_RELEASE(pShader);

		pLayer->SetTileInst(pTileInst);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		pScene->SortInstText();

		CRenderer* pRenderer = pTile->GetMap()->GetRenderer();

		std::vector<CRenderState*> vecState = pRenderer->GetVecRenderState();

		size_t iSz = vecState.size();

		for (size_t i = 0; i < iSz; ++i)
		{
			pTileInst->AddState(vecState[i]);
		}

		SAFE_RELEASE(pRenderer);
		SAFE_RELEASE(pTileMtrl);
		SAFE_RELEASE(pMesh);
	}

	InstancedData2D tData = {};

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matWVP = pTile->GetWV() * pCam->GetViewMat() * pCam->GetProjMat();

	matWVP.Transpose();

	SAFE_RELEASE(pCam);

	tData.matWVP = matWVP;
	tData.vPivot = pTile->GetPivot();
	tData.vSize = pTile->GetMeshSize();
	tData.vTexSize = pTile->GetTextureSize();

	Vector2 vIdx = pTile->GetTexIdx();
	Vector2 vSize = pTile->GetSize();
	tData.vStart = vIdx * vSize;
	tData.vEnd = (vIdx + 1) * vSize;
	tData.vColor = pTile->GetColor();

	pTileInst->AddInstanceData(&tData);

	pTileInst->AddCom(pTile);
}

void CRenderManager::RenderInstancing(float fTime)
{
	std::unordered_map<unsigned __int64, CInstancing*>::iterator iter = m_mapInstancing.begin();
	std::unordered_map<unsigned __int64, CInstancing*>::iterator iterEnd = m_mapInstancing.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(fTime);
		iter->second->Render(fTime);
		iter->second->Clear();
	}
}

void CRenderManager::RenderTileInstancing(float fTime)
{
	if (m_TileInst)
	{
		m_TileInst->Update(fTime);
		m_TileInst->Render(fTime);
		m_TileInst->Clear();
	}
}

void CRenderManager::SetState(const std::string& strTag)
{
	CRenderState* pState = FindState(strTag);

	if (!pState)
		return;

	pState->SetState();

	SAFE_RELEASE(pState);
}

void CRenderManager::ResetState(const std::string& strTag)
{
	CRenderState* pState = FindState(strTag);

	if (!pState)
		return;

	pState->ResetState();

	SAFE_RELEASE(pState);
}

CRenderState* CRenderManager::FindState(const std::string& strTag)
{
	std::unordered_map<std::string, CRenderState*>::iterator  iter = m_mapState.find(strTag);

	if (iter == m_mapState.end())
		return nullptr;

	iter->second->AddRef();
	
	return iter->second;
}

CMRT* CRenderManager::FindMRT(const std::string& strKey)
{
	std::unordered_map<std::string, CMRT*>::iterator iter = m_mapMRT.find(strKey);

	if (iter == m_mapMRT.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

CMRT* CRenderManager::CreateMRT(const std::string& strKey)
{
	CMRT* pMRT = new CMRT;

	if (!pMRT->Init())
		return nullptr;

	pMRT->AddRef();

	m_mapMRT.insert(std::make_pair(strKey, pMRT));

	return pMRT;
}

void CRenderManager::ClearTarget(const std::string& strKey)
{
	CMRT* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->Clear();

	SAFE_RELEASE(pMRT);
}

void CRenderManager::SetTarget(const std::string& strKey)
{
	CMRT* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->SetTarget();

	SAFE_RELEASE(pMRT);
}

void CRenderManager::ResetTarget(const std::string& strKey)
{
	CMRT* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->ResetTarget();

	SAFE_RELEASE(pMRT);
}

void CRenderManager::SetShader(const std::string& strKey, int iRegister,int iType)
{
	CMRT* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->SetShader(iRegister,iType);

	SAFE_RELEASE(pMRT);
}

void CRenderManager::ResetShader(const std::string& strKey, int iRegister, int iType)
{
	CMRT* pMRT = FindMRT(strKey);

	if (!pMRT)
		return;

	pMRT->ResetShader(iRegister,iType);

	SAFE_RELEASE(pMRT);
}

bool CRenderManager::CreateMultiTargetView(const std::string& strKey, int iWidth, int iHeight, DXGI_FORMAT eFmt)
{
	CMultiRenderTarget* pMRT = FindMultiRenderTarget(strKey);

	if (pMRT)
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	pMRT = new CMultiRenderTarget;

	if (!pMRT->CreateDepthView(iWidth, iHeight, eFmt))
	{
		SAFE_RELEASE(pMRT);
		return false;
	}

	m_mapMultiRenderTarget.insert(std::make_pair(strKey, pMRT));

	return true;
}

CMultiRenderTarget* CRenderManager::FindMultiRenderTarget(const std::string& strKey)
{
	std::unordered_map<std::string, CMultiRenderTarget*>::iterator iter = m_mapMultiRenderTarget.find(strKey);

	if (iter == m_mapMultiRenderTarget.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CRenderManager::SetMultiTarget(const std::string& strKey)
{
	CMultiRenderTarget* pMRT = FindMultiRenderTarget(strKey);

	if (pMRT)
	{
		pMRT->SetTarget();
		pMRT->Release();
	}
}

void CRenderManager::ResetMultiTarget(const std::string& strKey)
{
	CMultiRenderTarget* pMRT = FindMultiRenderTarget(strKey);

	if (pMRT)
	{
		pMRT->ResetTarget();
		pMRT->Release();
	}
}

void CRenderManager::AddTarget(const std::string& strKey, const std::string& strMRT)
{
	CMultiRenderTarget* pMRT =FindMultiRenderTarget(strKey);
	
	if (!pMRT)
		return;

	CMRT* _pMRT = FindMRT(strMRT);

	if (!_pMRT)
	{
		SAFE_RELEASE(pMRT);
		return;
	}

	pMRT->AddTarget( _pMRT->GetRenderTarget());
	SAFE_RELEASE(_pMRT);
	SAFE_RELEASE(pMRT);
}

void CRenderManager::ClearMultiTarget(const std::string& strKey)
{
	CMultiRenderTarget* pMRT =  FindMultiRenderTarget(strKey);

	if (!pMRT)
		return;

	pMRT->Clear();
	SAFE_RELEASE(pMRT);
		
}

bool CRenderManager::SortY(CSceneComponent* pCom1, CSceneComponent* pCom2)
{
	return pCom1->GetWorldPos().y < pCom2->GetWorldPos().y;
}

bool CRenderManager::SortZ(CSceneComponent* pSrc, CSceneComponent* pDst)
{
	return ((CUIControl*)pSrc)->GetZOrder() > ((CUIControl*)pDst)->GetZOrder();
}

unsigned int CRenderManager::GetHashKey(const std::string& strKey)
{
	size_t iSz = strKey.length();

	if (iSz <= 0)
		return 0;

	unsigned int iKey = 0;

	for (int i = 0; i < iSz; ++i)
	{
		iKey += 7;

		iKey <<= 3;

		iKey *= strKey[i];
	}

	return iKey;
}

CRenderInstance* CRenderManager::FindInstance3D(CMesh* pMesh,
	CShader* pShader, CMaterial* pMtrl, int iLayer)
{
	size_t iKey = GetKey(pMesh, pShader, pMtrl);

	std::unordered_map<size_t, CRenderInstance*>::iterator iter = m_mapInstance3D[iLayer].find(iKey);

	if (iter == m_mapInstance3D[iLayer].end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

size_t CRenderManager::GetKey(CMesh* pMesh, CShader* pShader, CMaterial* pMtrl) const
{
	std::hash<std::string> hs;

	size_t iMesh = 0;

	if (pMesh)
	{
		iMesh = hs(pMesh->GetName());
	}

	size_t iShader = 0;

	if (pShader)
	{
		iShader = hs(pShader->GetName());
	}

	size_t iMtrl = 0;
	if (pMtrl)
	{
		iMtrl = hs(pMtrl->GetName());
	}

	return iMesh + iShader + iMtrl;
}
