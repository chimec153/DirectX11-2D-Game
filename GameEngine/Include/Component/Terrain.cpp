#include "Terrain.h"
#include "../PathManager.h"
#include "../Resource/ResourceManager.h"
#include "../Object/Obj.h"
#include "Renderer.h"
#include "../Resource/ShaderManager.h"
#include "Decal.h"
#include "ColliderTerrain.h"
#include "../Input.h"
#include "../Scene/Scene.h"
#include "../Resource/Mesh.h"
#include "../Resource/Texture.h"
#include <wincodec.h>

std::unordered_map<std::string, CTerrain*> CTerrain::m_mapTerrain;
CTerrain* CTerrain::m_pMainTerrain = nullptr;

CTerrain::CTerrain()	:
	m_iCountX(0)
	, m_iCountY(0)
	, m_pHeightMapTexture(nullptr)
	, m_bEditMode(false)
	, m_pDecal(nullptr)
	, m_pBrushTexture(nullptr)
	 , m_pTerrainImage(nullptr)
{
}

CTerrain::CTerrain(const CTerrain& terrain)	:
	CSceneComponent(terrain)
	, m_iCountX(terrain.m_iCountX)
	, m_iCountY(terrain.m_iCountY)
	, m_pHeightMapTexture(terrain.m_pHeightMapTexture)
	, m_bEditMode(terrain.m_bEditMode)
	, m_pDecal(FindComByType<CDecal>(terrain.m_pDecal->GetName()))
	, m_pBrushTexture(terrain.m_pBrushTexture)
	, m_pTerrainImage(nullptr)
{
	if (m_pHeightMapTexture)
	{
		m_pHeightMapTexture->AddRef();
	}

	if (m_pBrushTexture)
	{
		m_pBrushTexture->AddRef();
	}
}

CTerrain::~CTerrain()
{
	SAFE_RELEASE(m_pHeightMapTexture);
	SAFE_RELEASE(m_pDecal);
	SAFE_DELETE(m_pTerrainImage);
	SAFE_RELEASE(m_pBrushTexture);
}

bool CTerrain::CreateHeightMap(int iWidth, int iHeight, const std::string& strName, 
	const TCHAR* pFileName, const std::string& strPathKey)
{
	m_iCountX = iWidth;
	m_iCountY = iHeight;

	TCHAR strFullPath[MAX_PATH] = {};

	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathKey);

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	TCHAR pExt[_MAX_EXT] = {};

	_wsplitpath_s(strFullPath, nullptr, 0, nullptr, 0, nullptr, 0, pExt, _MAX_EXT);

	_wcsupr_s(pExt);

	m_pTerrainImage = new ScratchImage;

	if (!lstrcmp(pExt, L".DDS"))
	{
		if (FAILED(LoadFromDDSFile(strFullPath, DDS_FLAGS_NONE, nullptr, *m_pTerrainImage)))
		{
			SAFE_DELETE(m_pTerrainImage);
			return false;
		}	
	}

	else if (!lstrcmp(pExt, L".TGA"))
	{
		if (FAILED(LoadFromTGAFile(strFullPath, nullptr, *m_pTerrainImage)))
		{
			SAFE_DELETE(m_pTerrainImage);
			return false;
		}
	}

	else
	{
		if (FAILED(LoadFromWICFile(strFullPath, WIC_FLAGS_NONE, nullptr, *m_pTerrainImage)))
		{
			SAFE_DELETE(m_pTerrainImage);
			return false;
		}
	}

	uint8_t* pPixels = m_pTerrainImage->GetPixels();
	size_t iTotalCount = static_cast<size_t>(m_iCountX) * static_cast<size_t>(m_iCountY);
	std::vector<float>	vecHeight(iTotalCount);

	for (int i = 0; i < m_iCountY; i++)
	{
		for (int j = 0; j < m_iCountX; j++)
		{
			vecHeight[i * static_cast<size_t>(m_iCountX) + j] =
				pPixels[(i * m_iCountX + j) * 4] / 60.f;
		}
	}

	m_vecVtx.resize(iTotalCount);
	m_vecPos.resize(iTotalCount);

	for (int i = 0; i < m_iCountY; i++)
	{
		for (int j = 0; j < m_iCountX; j++)
		{
			int iIdx = i * m_iCountX + j;

			VERTEX3D tVtx = {};

			tVtx.vPos = Vector3(static_cast<float>(j), vecHeight[iIdx], static_cast<float>(i));
			tVtx.vUV = Vector2(j/static_cast<float>((m_iCountX-1)), 1-i/static_cast<float>((m_iCountY-1)));

			m_vecVtx[iIdx] = tVtx;
			m_vecPos[iIdx] = tVtx.vPos;
		}
	}

	std::vector<UINT> vecIdx((static_cast<size_t>(m_iCountX) - 1) * (static_cast<size_t>(m_iCountY) - 1) * 6);

	int iTri = 0;
	for (int i = 0; i < m_iCountY-1; i++)
	{
		for (int j = 0; j < m_iCountX-1; j++)
		{
			vecIdx[iTri++] = (i + 1) * m_iCountX + j;
			vecIdx[iTri++] = (i + 1) * m_iCountX + j + 1;
			vecIdx[iTri++] = i * m_iCountX + j + 1;
			
			vecIdx[iTri++] = (i + 1) * m_iCountX + j;
			vecIdx[iTri++] = i * m_iCountX + j + 1;
			vecIdx[iTri++] = i * m_iCountX + j;
		}
	}

	ComputeNormal();
	ComputeTangent();

	if (!GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_STATIC, strName, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		&m_vecVtx[0], static_cast<int>(sizeof(VERTEX3D)), m_iCountX * m_iCountY, D3D11_USAGE_DYNAMIC,
		&vecIdx[0], 4, (m_iCountX - 1) * (m_iCountY - 1) * 6, D3D11_USAGE_IMMUTABLE, DXGI_FORMAT_R32_UINT))
		return false;

	CRenderer* pRenderer = GetRenderer();
	if (pRenderer)
	{
		pRenderer->SetMesh(strName);
		pRenderer->SetMaterial("Color");
		pRenderer->SetShader("TerrainShader");
		SAFE_RELEASE(pRenderer);
	}

	CColliderTerrain* pTerrain = m_pObj->CreateComponent<CColliderTerrain>("TerrainBody");

	pTerrain->SetInfo(m_iCountX, m_iCountY);
	pTerrain->SetPos(m_vecPos);
	pTerrain->SetCallBack<CTerrain>(COLLISION_STATE::INIT, this, &CTerrain::CollisionInit);
	pTerrain->SetCallBack<CTerrain>(COLLISION_STATE::STAY, this, &CTerrain::CollisionStay);
	pTerrain->SetCallBack<CTerrain>(COLLISION_STATE::END, this, &CTerrain::CollisionEnd);

#ifdef _DEBUG
	pTerrain->SetDebugMesh(strName);
#endif

	AddChild(pTerrain);

	SAFE_RELEASE(pTerrain);

	return true;
}

void CTerrain::SetTiling(float fTileX, float fTileY)
{
	m_tCBuffer.vSplatSize.x = fTileX;
	m_tCBuffer.vSplatSize.y = fTileY;
}

void CTerrain::ComputeNormal()
{
	m_vecFaceNormal.resize((static_cast<size_t>(m_iCountX) - 1) * (static_cast<size_t>(m_iCountY) - 1) * 2);

	for (int i = 0; i < m_iCountX - 1; i++)
	{
		for (int j = 0; j < m_iCountY - 1; j++)
		{
			int iIdx = j * (m_iCountX - 1) + i;

			int iIdx0 = (j + 1) * m_iCountX + i;
			int iIdx1 = (j + 1) * m_iCountX + i + 1;
			int iIdx2 = j * m_iCountX + i + 1;

			Vector3 vEdge1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			Vector3 vEdge2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			Vector3 vFaceNormal = vEdge1.Cross(vEdge2);

			vFaceNormal.Normalize();

			m_vecFaceNormal[static_cast<size_t>(iIdx) * 2] = vFaceNormal;

			m_vecVtx[iIdx0].vNormal += vFaceNormal;
			m_vecVtx[iIdx1].vNormal += vFaceNormal;
			m_vecVtx[iIdx2].vNormal += vFaceNormal;

			iIdx0 = (j + 1) * m_iCountX + i;
			iIdx1 = j * m_iCountX + i + 1;
			iIdx2 = j * m_iCountX + i;

			vEdge1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			vEdge2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			vEdge1.Normalize();
			vEdge2.Normalize();

			vFaceNormal = vEdge1.Cross(vEdge2);

			vFaceNormal.Normalize();

			m_vecFaceNormal[static_cast<size_t>(iIdx) * 2 + 1] = vFaceNormal;

			m_vecVtx[iIdx0].vNormal += vFaceNormal;
			m_vecVtx[iIdx1].vNormal += vFaceNormal;
			m_vecVtx[iIdx2].vNormal += vFaceNormal;
		}
	}

	for (int i = 0; i < m_iCountX; i++)
	{
		for (int j = 0; j < m_iCountY; j++)
		{
			m_vecVtx[j * static_cast<size_t>(m_iCountX) + i].vNormal.Normalize();
		}
	}
}

void CTerrain::ComputeTangent()
{
	for (int i = 0; i < m_iCountX-1; i++)
	{
		for (int j = 0; j < m_iCountY-1; j++)
		{
			int iIdx0 = (j + 1) * m_iCountY + i;
			int iIdx1 = (j + 1) * m_iCountY + i + 1;
			int iIdx2 = j * m_iCountY + i + 1;

			Vector3 vPos1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			Vector3 vPos2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			Vector2 vUV1 = m_vecVtx[iIdx1].vUV - m_vecVtx[iIdx0].vUV;
			Vector2 vUV2 = m_vecVtx[iIdx2].vUV - m_vecVtx[iIdx0].vUV;

			float fDen = 1.f / (vUV1.x * vUV2.y - vUV1.y * vUV2.x);

			int iIdx = i + j * m_iCountX;
			m_vecVtx[iIdx0].vTangent = (vUV2.y * vPos1 - vUV1.y * vPos2) * fDen;
			m_vecVtx[iIdx0].vTangent.Normalize();
			m_vecVtx[iIdx1].vTangent = m_vecVtx[iIdx0].vTangent;
			m_vecVtx[iIdx2].vTangent = m_vecVtx[iIdx0].vTangent;

			m_vecVtx[iIdx0].vBinormal = m_vecVtx[iIdx0].vNormal.Cross(m_vecVtx[iIdx0].vTangent);
			m_vecVtx[iIdx0].vBinormal.Normalize();
			m_vecVtx[iIdx1].vBinormal = m_vecVtx[iIdx1].vNormal.Cross(m_vecVtx[iIdx1].vTangent);
			m_vecVtx[iIdx1].vBinormal.Normalize();
			m_vecVtx[iIdx2].vBinormal = m_vecVtx[iIdx2].vNormal.Cross(m_vecVtx[iIdx2].vTangent);
			m_vecVtx[iIdx2].vBinormal.Normalize();

			iIdx1 = j * m_iCountY + i + 1;
			iIdx2 = j * m_iCountY + i;

			vPos1 = m_vecVtx[iIdx1].vPos - m_vecVtx[iIdx0].vPos;
			vPos2 = m_vecVtx[iIdx2].vPos - m_vecVtx[iIdx0].vPos;

			vUV1 = m_vecVtx[iIdx1].vUV - m_vecVtx[iIdx0].vUV;
			vUV2 = m_vecVtx[iIdx2].vUV - m_vecVtx[iIdx0].vUV;

			fDen = 1.f / (vUV1.x * vUV2.y - vUV1.y * vUV2.x);

			m_vecVtx[iIdx0].vTangent = (vUV2.y * vPos1 - vUV1.y * vPos2) * fDen;
			m_vecVtx[iIdx0].vTangent.Normalize();
			m_vecVtx[iIdx1].vTangent = m_vecVtx[iIdx0].vTangent;
			m_vecVtx[iIdx2].vTangent = m_vecVtx[iIdx0].vTangent;

			m_vecVtx[iIdx0].vBinormal = m_vecVtx[iIdx0].vNormal.Cross(m_vecVtx[iIdx0].vTangent);
			m_vecVtx[iIdx0].vBinormal.Normalize();
			m_vecVtx[iIdx1].vBinormal = m_vecVtx[iIdx1].vNormal.Cross(m_vecVtx[iIdx1].vTangent);
			m_vecVtx[iIdx1].vBinormal.Normalize();
			m_vecVtx[iIdx2].vBinormal = m_vecVtx[iIdx2].vNormal.Cross(m_vecVtx[iIdx2].vTangent);
			m_vecVtx[iIdx2].vBinormal.Normalize();
		}
	}
}

void CTerrain::SetSplatCount(int iCount)
{
	m_tCBuffer.iSplatCount = iCount;

	if (m_tCBuffer.iSplatCount > 1)
	{
		SetShader("TerrainArrayShader");
	}
}

int CTerrain::GetIndex(const Vector3& vPos)
{
	Vector3 vScale = GetWorldScale();
	Vector3 vTerrainPos = GetWorldPos();
	Vector3 vPivot = GetPivot();

	Vector3 vIndex = (vPos - vTerrainPos) / vScale;

	if (vIndex.x < 0.f || vIndex.z < 0.f || vIndex.x >= m_iCountX || vIndex.z >= m_iCountY)
		return -1;

	return static_cast<int>(vIndex.z) * m_iCountX + static_cast<int>(vIndex.x);
}

float CTerrain::GetHeight(int iIndex)
{
	if (iIndex < 0)
		return 0.f;

	if (m_vecPos.size() <= iIndex)
		return 0.f;

	Vector3 vScale = GetWorldScale();

	return m_vecPos[iIndex].y * vScale.y;
}

float CTerrain::GetHeight(const Vector3& vPos)
{
	int iIdx = GetIndex(vPos);

	return GetHeight(iIdx);
}

const std::vector<Vector3>& CTerrain::GetVecPos() const
{
	return m_vecPos;
}

void CTerrain::SetEditMode(bool bEdit)
{
	m_bEditMode = bEdit;

	if (m_pDecal)
	{
		m_pDecal->Enable(m_bEditMode);
	}
	else
	{
		m_pDecal = m_pObj->CreateComponent<CDecal>("TerrainDecal", m_pScene->FindLayer(DECAL_LAYER));

		m_pDecal->SetInheritScale(false);
		m_pDecal->SetWorldScale(100.f, 100.f, 100.f);
		m_pDecal->SetTexture(REGISTER_TYPE::RT_DIF, 
			"DefaultDistortionFilter",L"DefaultDistortionFilter.png");
		m_pDecal->SetAlpha(0.5f);
		m_pDecal->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_3D);

		AddChild(m_pDecal);

		GET_SINGLE(CResourceManager)->LoadTexture("DefaultDistortionFilter",
			L"DefaultDistortionFilter.png");

		m_pBrushTexture = GET_SINGLE(CResourceManager)->FindTexture("DefaultDistortionFilter");
			
	}
}

bool CTerrain::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTerrain::Start()
{
	CSceneComponent::Start();
}

void CTerrain::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CTerrain::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CTerrain::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CTerrain::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CTerrain::Render(float fTime)
{
	GET_SINGLE(CShaderManager)->UpdateCBuffer("Terrain", &m_tCBuffer);

	CSceneComponent::Render(fTime);
}

void CTerrain::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CTerrain* CTerrain::Clone()
{
	return new CTerrain(*this);
}

void CTerrain::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CTerrain::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}

void CTerrain::CollisionInit(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CTerrain::CollisionStay(CCollider* pSrc, CCollider* pDest, float fTime)
{
	if (m_bEditMode)
	{
		std::string strDest = pDest->GetName();

		if (strDest == "MouseBody")
		{
			m_pDecal->SetWorldPos(pDest->GetCross());

			if (GET_SINGLE(CInput)->IsMousePress(MOUSE_BUTTON_TYPE::LEFT))
			{
				AddHeight(pDest->GetCross(), fTime);
			}

			else if (GET_SINGLE(CInput)->IsMousePress(MOUSE_BUTTON_TYPE::RIGHT))
			{
				AddHeight(pDest->GetCross(), -fTime);
			}
		}
	}
}

void CTerrain::CollisionEnd(CCollider* pSrc, CCollider* pDest, float fTime)
{
}

void CTerrain::SpawnWindow()
{
	CSceneComponent::SpawnWindow();

	std::string strName = GetName();

	if (strName == "")
	{
		strName = "None";
	}

	if (ImGui::Begin(strName.c_str()))
	{
		if (ImGui::Checkbox("Edit Mode", &m_bEditMode))
		{
			SetEditMode(m_bEditMode);
		}

		if (ImGui::Button("Save Height Map"))
		{
			TCHAR strFullPath[MAX_PATH] = {};

			const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);

			if (pPath)
			{
				wcscpy_s(strFullPath, MAX_PATH, pPath);
			}

			wcscat_s(strFullPath, MAX_PATH, L"HeightMap.png");

			SaveToWICFile(*m_pTerrainImage->GetImages(), WIC_FLAGS_NONE, GUID_ContainerFormatPng, strFullPath);
		}
	}
	ImGui::End();
}

void CTerrain::AddHeight(const Vector3& vCross, float fTime)
{
	int iIndex = GetIndex(vCross);

	if (iIndex != -1)
	{
		int iHeight = m_pBrushTexture->GetHeigth();
		int iWidth = m_pBrushTexture->GetWidth();

		ScratchImage* pImage = m_pBrushTexture->GetImage();

		uint8_t* pPixels = pImage->GetPixels();

		Vector3 vScale = GetWorldScale();
		Vector3 vDecalScale = m_pDecal->GetWorldScale();

		int iStartIndex = GetIndex(vCross - vDecalScale * 0.5f);
		int iEndIndex = GetIndex(vCross + vDecalScale * 0.5f);

		Vector3 vStart = (vCross - vDecalScale * 0.5f - GetWorldPos()) / GetWorldScale();
		Vector3 vEnd = (vCross + vDecalScale * 0.5f - GetWorldPos()) / GetWorldScale();

		int iStartX = static_cast<int>(vStart.x);
		int iStartY = static_cast<int>(vStart.z);
		int iEndX = static_cast<int>(vEnd.x);
		int iEndY = static_cast<int>(vEnd.z);

		iStartX = iStartX < 0 ? 0 : iStartX;
		iStartY = iStartY < 0 ? 0 : iStartY;

		iEndX = iEndX >= m_iCountX ? m_iCountX - 1 : iEndX;
		iEndY = iEndY >= m_iCountY ? m_iCountY - 1 : iEndY;

		for (int i = iStartY; i <= iEndY; ++i)
		{
			for (int j = iStartX; j <= iEndX; ++j)
			{
				Vector3 vPos = Vector3(static_cast<float>(j), 0.f, static_cast<float>(i)) * vScale + GetWorldPos();
				vPos -= vCross - vDecalScale * 0.5f;

				if (vPos.x < 0.f || vPos.z < 0.f)
					continue;

				Vector3 vIndex = vPos / vDecalScale * Vector3(static_cast<float>(iWidth), 0.f, static_cast<float>(iHeight));
				size_t iVtxIndex = i * static_cast<size_t>(m_iCountX) + j;
				m_vecVtx[iVtxIndex].vPos.y +=
					pPixels[4 * (static_cast<int>(vIndex.z) * iWidth + static_cast<int>(vIndex.x))] / 255.f * fTime;

				if (m_vecVtx[iVtxIndex].vPos.y * 60.f > 255.f)
				{
					m_vecVtx[iVtxIndex].vPos.y = 255.f / 60.f;
				}
				else if (m_vecVtx[iVtxIndex].vPos.y < 0.f)
				{
					m_vecVtx[iVtxIndex].vPos.y = 0.f;
				}

				m_vecPos[iVtxIndex] = m_vecVtx[iVtxIndex].vPos;

				uint8_t* pTerrainPixels = m_pTerrainImage->GetPixels();

				int iHeight = static_cast<int>(m_vecVtx[iVtxIndex].vPos.y * 60.f);

				pTerrainPixels[iVtxIndex * 4] = iHeight;
				pTerrainPixels[iVtxIndex * 4 + 1] = iHeight;
				pTerrainPixels[iVtxIndex * 4 + 2] = iHeight;
			}
		}

		CRenderer* pRenderer = GetRenderer();

		CMesh* pMesh = pRenderer->GetMesh();

		pMesh->UpdateVertexBuffer(&m_vecVtx[0]);

		SAFE_RELEASE(pMesh);

		SAFE_RELEASE(pRenderer);

		CColliderTerrain* pTerrainBody = FindComByType<CColliderTerrain>();

		pTerrainBody->SetPos(m_vecPos);

		SAFE_RELEASE(pTerrainBody);
	}
}

void CTerrain::SetMainTerrain(const std::string& strKey)
{
	CTerrain* pTerrain = FindTerrain(strKey);

	if (!pTerrain)
		return;

	m_pMainTerrain = pTerrain;

	SAFE_RELEASE(pTerrain);
}

CTerrain* CTerrain::GetMainTerrain()
{
	if (m_pMainTerrain)
		m_pMainTerrain->AddRef();

	return m_pMainTerrain;
}

void CTerrain::AddTerrain(CTerrain* pTerrain)
{
	if (!pTerrain)
		return;

	if (!m_pMainTerrain)
		m_pMainTerrain = pTerrain;

	m_mapTerrain.insert(std::make_pair(pTerrain->GetName(), pTerrain));

	pTerrain->AddRef();
}

CTerrain* CTerrain::FindTerrain(const std::string& strKey)
{
	std::unordered_map<std::string, CTerrain*>::iterator iter = m_mapTerrain.find(strKey);

	if (iter == m_mapTerrain.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CTerrain::Destroy()
{
	SAFE_RELEASE_MAP(m_mapTerrain);
}
