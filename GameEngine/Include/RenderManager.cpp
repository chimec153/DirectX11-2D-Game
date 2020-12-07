#include "RenderManager.h"
#include "Component/SceneComponent.h"
#include "Render/BlendState.h"
#include "Render/DepthStencilState.h"
#include "Render/RasterizerState.h"
#include "UI/UIControl.h"
#include "Resource/Material.h"
#include "Resource/Mesh.h"
#include "Instancing.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Tile/Tile.h"
#include "Resource/ResourceManager.h"
#include "Component/SpriteComponent.h"
#include "Resource/Texture.h"
#include "Layer.h"
#include "Scene/SceneManager.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	: m_TileInst(nullptr)
{

}

CRenderManager::~CRenderManager()
{
	SAFE_DELETE_MAP(m_mapState);
	SAFE_DELETE_MAP(m_mapInstancing);
	SAFE_DELETE(m_TileInst);
}

bool CRenderManager::Init()
{
	AddBlendDesc("AlphaBlend");
	CreateBlendState("AlphaBlend");

	AddRasterizerState("NoCullBack", D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateRasterizerState("NoCullBack");

	AddDepthStencilDesc("DepthNoWrite", false);

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

	AddDepthStencilDesc("Silhouette", true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tFrt, tBck);

	AddDepthStencilDesc("Character", true, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
		true, 0xff, 0xff, tFrt, tBck);

	CDepthStencilState* pState = (CDepthStencilState*)FindState("Silhouette");

	pState->SetStencilRef(0x1);

	pState = (CDepthStencilState*)FindState("Character");

	pState->SetStencilRef(0);

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

	return true;
}

bool CRenderManager::AddComponent(CSceneComponent* pComponent)
{
	SCENE_COMPONENT_TYPE eType = pComponent->GetSceneComponentType();

	if (eType == SCENE_COMPONENT_TYPE::SCT_3D)
	{
		if (pComponent->IsInstanced())
		{
			CMaterial* pMtrl = pComponent->GetMaterial();
			CMesh* pMesh = pComponent->GetMesh();

			unsigned int iMtrlKey = GetHashKey(pMtrl->GetName());
			unsigned int iMeshKey = GetHashKey(pMesh->GetName());

			unsigned __int64 iKey = iMtrlKey;

			iKey <<= 32;

			iKey |= iMeshKey;

			CInstancing* pInst = FindInstancing(iKey);

			if (!pInst)
			{
				pInst = new CInstancing;

				pInst->Init(pMesh, pMtrl, 1000, sizeof(InstancedData));

				CLayer* pLayer = pComponent->GetLayer();

				pLayer->AddInst(pInst);

				m_mapInstancing.insert(std::make_pair(iKey, pInst));
			}

			SAFE_RELEASE(pMesh);
			SAFE_RELEASE(pMtrl);

			InstancedData tData = {};

			CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

			tData.matWVP = pComponent->GetMatWorld() * pCam->GetViewMat() * pCam->GetProjMat();

			SAFE_RELEASE(pCam);

			tData.vPivot = pComponent->GetPivot();
			tData.vSize = pComponent->GetMeshSize();

			pInst->AddInstanceData(&tData);
		}

		else
		{
			CLayer* pLayer = pComponent->GetLayer();

			pLayer->AddCom(pComponent);

			//m_vecComponent.push_back(pComponent);
		}
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

				CMaterial* pClone = pMtrl->Clone();

				pClone->SetShader("Inst");

				if (eType == SCENECOMPONENT_CLASS_TYPE::SCT_SPRITE)
				{
					CTexture* pTex = ((CSpriteComponent*)pComponent)->GetTexture();

					pClone->SetTexture(REGISTER_TYPE::RT_DIF, pTex);

					SAFE_RELEASE(pTex);

					pInst->Init(pMesh, pClone, 10000, sizeof(InstancedData2D));
				}

				else if (eType == SCENECOMPONENT_CLASS_TYPE::COLLIDEROBB2D ||
					eType == SCENECOMPONENT_CLASS_TYPE::COLLIDERPIXEL ||
					eType == SCENECOMPONENT_CLASS_TYPE::COLLIDER_POINT ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERRECT ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERLINE ||
					eType == SCENECOMPONENT_CLASS_TYPE::SCT_COLLIDERCIRCLE)
				{
					pClone->SetShader("InstCollider");
					pInst->Init(pMesh, pClone, 10000, sizeof(INSTCOLLIDER2D));
				}

				else
					pInst->Init(pMesh, pClone, 10000, sizeof(InstancedData2D));

				CLayer* pLayer = pComponent->GetLayer();

				pLayer->AddInst(pInst);

				pInst->SetLayer(pLayer);

				SAFE_RELEASE(pClone);

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

			pLayer->AddCom(pComponent);
			//m_vec2DComponent.push_back(pComponent);

		}
	}

	else
	{
		CLayer* pLayer = pComponent->GetLayer();

		pLayer->AddCom(pComponent);
		//m_vecUIComponent.push_back(pComponent);

	}

	return true;
}

void CRenderManager::AddCon(CUIControl* pCon)
{
	m_vecUIComponent.push_back(pCon);
}

void CRenderManager::ClearComponent()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Clear();
	}

	m_vec2DComponent.clear();
	m_vecComponent.clear();
	m_vecUIComponent.clear();
}

void CRenderManager::AddLayer(CLayer* pLayer)
{
	m_LayerList.push_back(pLayer);
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

void CRenderManager::Render(float fTime)
{
	ResetState("AlphaBlend");

	SetState("AlphaBlend");
	SetState("DepthNoWrite");
	SetState("NoCullBack");

	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render(fTime);
		(*iter)->Clear();
	}

	m_LayerList.clear();
	
/*
	RenderTileInstancing(fTime);
	Render2D(fTime);
	Render3D(fTime);
	RenderUI(fTime);*/

	ResetState("NoCullBack");
	ResetState("DepthNoWrite");
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
	size_t iSize = m_vecComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecComponent[i]->Render(fTime);
}

void CRenderManager::RenderUI(float fTime)
{
	std::sort(m_vecUIComponent.begin(), m_vecUIComponent.end(), CRenderManager::SortZ);

	size_t iSize = m_vecUIComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecUIComponent[i]->Render(fTime);
}

bool CRenderManager::AddBlendDesc(const std::string& strTag, bool bEnable, 
	D3D11_BLEND eSrc, D3D11_BLEND eDest, D3D11_BLEND_OP eOp, 
	D3D11_BLEND eSrcAlpha, D3D11_BLEND eDestAlpha, D3D11_BLEND_OP eOpAlpha, 
	D3D11_COLOR_WRITE_ENABLE eWrite)
{
	CBlendState* pState = (CBlendState*)FindState(strTag);

	if (!pState)
	{
		pState = new CBlendState;

		pState->SetTag(strTag);

		m_mapState.insert(std::make_pair(strTag, pState));
	}

	if (!pState->AddBlendDesc(bEnable, eSrc, eDest, eOp, eSrcAlpha, eDestAlpha, eOpAlpha, eWrite))
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_DELETE(pState);
		return false;
	}

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

		SAFE_DELETE(pState);
		return false;
	}

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

		pState->SetTag(strTag);
		
		m_mapState.insert(std::make_pair(strTag, pState));
	}

	if (!pState->AddDepthStencilDesc(bDepthEnable, eWriteMask, eDepthFunc, bStencilEnable, iReadMask, iWriteMask, tFront, tBack))
	{
		std::unordered_map<std::string, CRenderState*>::iterator iter = m_mapState.find(strTag);
		m_mapState.erase(iter);

		SAFE_DELETE(pState);
		return false;
	}

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

	pState->SetTag(strTag);
	
	if (!pState->AddRasterizerState(eFillMode, eCullMode, bFront, iBias, fBiasClamp,
		fSSDB, bDepthClip, bScissor, bMultiSample, bAntiAliasLine))
	{
		SAFE_DELETE(pState);
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

		SAFE_DELETE(pState);
		return false;
	}

	return true;
}

bool CRenderManager::CreateInstancing(CMesh* pMesh, CMaterial* pMtrl, int iCount, int iSize)
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

	if (!pInst->Init(pMesh, pMtrl, iCount, iSize))
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

		CMaterial* pTileMtrl = pTile->GetMaterial();

		CMaterial* pClone = pTileMtrl->Clone();

		pClone->SetShader("Inst");

		CMesh* pMesh = (CMesh*)GET_SINGLE(CResourceManager)->GetDefaultMesh();

		if (!pTileInst->Init(pMesh, pClone, 10000, sizeof(InstancedData2D)))
		{
			SAFE_DELETE(pTileInst);
			SAFE_RELEASE(pClone);
			SAFE_RELEASE(pMesh);

			SAFE_RELEASE(pTileMtrl);
			return;
		}

		pLayer->SetTileInst(pTileInst);

		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		pScene->SortInstText();

		if (pLayer->GetName() == "1_FG1")
		{
			pTileInst->AddState("ForeGround");
		}

		SAFE_RELEASE(pTileMtrl);
		SAFE_RELEASE(pClone);
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
}

void CRenderManager::ResetState(const std::string& strTag)
{
	CRenderState* pState = FindState(strTag);

	if (!pState)
		return;

	pState->ResetState();
}

CRenderState* CRenderManager::FindState(const std::string& strTag)
{
	std::unordered_map<std::string, CRenderState*>::iterator  iter = m_mapState.find(strTag);

	if (iter == m_mapState.end())
		return nullptr;
	
	return iter->second;
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
