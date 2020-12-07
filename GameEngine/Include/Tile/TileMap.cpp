#include "TileMap.h"
#include "Tile.h"
#include "../Camera/CameraManager.h"
#include "../Component/Camera.h"
#include "../Device.h"
#include "../RenderManager.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Material.h"
#include "../PathManager.h"

CTileMap::CTileMap()	:
	m_iCountX(0),
	m_iCountY(0),
	m_vTileSize(),
	m_eTileType(TILE_TYPE::RECT),
	m_pTexIdx(nullptr),
	m_pColIdx(nullptr),
	m_bAni(false)
{
}

CTileMap::CTileMap(const CTileMap& map)	:
	CSceneComponent(map)
{
	m_iCountX = map.m_iCountX;
	m_iCountY = map.m_iCountY;

	m_vTileSize = map.m_vTileSize;
	m_eTileType = map.m_eTileType;

	m_pTexIdx = new char[m_iCountX * m_iCountY];

	memcpy(m_pTexIdx, map.m_pTexIdx, m_iCountX * m_iCountY);

	m_pColIdx = new char[m_iCountX * m_iCountY];

	memcpy(m_pColIdx, map.m_pColIdx, m_iCountX * m_iCountY);
}

CTileMap::~CTileMap()
{
	SAFE_RELEASE_VECLIST(m_vecTile);
	SAFE_DELETE_ARRAY(m_pTexIdx);
	SAFE_DELETE_ARRAY(m_pColIdx);
}

void CTileMap::CreateTile(int x, int y, const Vector3& vSize, const Vector2& vStart, TILE_TYPE eType)
{
	SAFE_RELEASE_VECLIST(m_vecTile);

	m_eTileType = eType;

	m_iCountX = x;
	m_iCountY = y;

	m_vTileSize = vSize;

	SetWorldPos(vStart.x, vStart.y, 0.f);

	CMaterial* pMat = GET_SINGLE(CResourceManager)->FindMaterial("Color");

	CMaterial* pClone = pMat->Clone();

	for (int j = y - 1; j >= 0; --j)
	{
		for (int i = 0; i < x; ++i)
		{
			CTile* pTile = new CTile;

			pTile->m_pLayer = m_pLayer;
			pTile->m_pScene = m_pScene;

			pTile->m_eTileType = eType;
			pTile->SetWorldScale(vSize);
			pTile->m_iX = i;
			pTile->m_iY = j;

			Vector3 vPos = Vector3(vSize.x * i + vSize.x / 2.f,
				vSize.y * (y - j- 1) + vSize.y / 2.f, 0.f);

			pTile->SetWorldPos(vPos);
			pTile->SetPivot(0.5f, 0.5f, 0.f);

			pTile->CSceneComponent::PostUpdate(0.f);
			pTile->SetInstance();
			pTile->m_vSize = Vector2(vSize.x, vSize.y);
			pTile->SetMaterial(pClone);

			Matrix matWV = pTile->GetMatWorld();

			pTile->m_matWV = matWV;
			pTile->m_iXTexIdx = i;
			pTile->m_iYTexIdx = j;

			m_vecTile.push_back(pTile);
		}
	}

	SAFE_RELEASE(pClone);
	SAFE_RELEASE(pMat);
}

int CTileMap::GetIndex(int x, int y)	const
{
	return y * m_iCountX + x;
}

int CTileMap::GetIndex(const Vector2& vPos)	const
{
	Vector3 _vPos = Vector3(vPos.x, vPos.y, 0.f);

	_vPos -= GetWorldPos();

	if (_vPos.x < 0.f || 
		_vPos.y < 0.f)
		return -1;

	_vPos /= m_vTileSize;

	if (_vPos.x >= m_iCountX ||
		_vPos.y >= m_iCountY)
		return -1;

	return (int)_vPos.y * m_iCountX + (int)_vPos.x;
}

void CTileMap::SetTileTexture(const std::string& strKey)
{
		m_vecTile[0]->SetTexture(REGISTER_TYPE::RT_DIF, strKey);
}

void CTileMap::SetTileTexture(CTexture* pTex)
{
		m_vecTile[0]->SetTexture(REGISTER_TYPE::RT_DIF, pTex);
}

bool CTileMap::LoadTileIdx(const char* pFileName,int iWidth, int iHeight)
{
	char* pContext = nullptr;

	char* pResult = strtok_s((char*)pFileName, ",", &pContext);

	m_pTexIdx = new char[m_iCountX * m_iCountY];

	memset(m_pTexIdx, 0, m_iCountX * m_iCountY);

	m_pTexIdx[0] = atoi(pResult);

	m_vecTile[0]->m_iXTexIdx = atoi(pResult) % (int)(iWidth / 16.f) - 1;
	m_vecTile[0]->m_iYTexIdx = atoi(pResult) / (int)(iWidth / 16.f);

	for (int y = 0; y < m_iCountY; ++y)
	{
		for (int x = 0; x < m_iCountX; ++x)
		{
			if (x == 0 && y == 0)
				continue;

			char* pResult = strtok_s(nullptr, ",", &pContext);

			int idx = (m_iCountY - y - 1) * m_iCountX + x;

			m_pTexIdx[idx] = atoi(pResult);

			if (atoi(pResult) == 0)
				m_vecTile[idx]->m_bEnable = false;

			else
			{
				m_vecTile[idx]->m_iXTexIdx =atoi(pResult) % (int)(iWidth / 16.f) - 1;
				m_vecTile[idx]->m_iYTexIdx =atoi(pResult) / (int)(iWidth / 16.f);
			}
		}
	}

	return false;
}

bool CTileMap::LoadColIdx(const char* pFileName, int iWidth, int iHeight)
{
	char* pContext = nullptr;

	char* pResult = strtok_s((char*)pFileName, ",", &pContext);

	m_pColIdx = new char[m_iCountX * m_iCountY];

	memset(m_pColIdx, 0, m_iCountX * m_iCountY);

	m_pColIdx[0] = atoi(pResult);

	m_vecTile[0]->m_iXTexIdx = atoi(pResult) % (int)(iWidth / 16.f) - 1;
	m_vecTile[0]->m_iYTexIdx = atoi(pResult) / (int)(iWidth / 16.f);

	for (int y = 0; y < m_iCountY; ++y)
	{
		for (int x = 0; x < m_iCountX; ++x)
		{
			if (x == 0 && y == 0)
				continue;

			char* pResult = strtok_s(nullptr, ",", &pContext);

			int idx = (m_iCountY - y - 1) * m_iCountX + x;

			m_pColIdx[idx] = atoi(pResult);

			if (atoi(pResult) == 0)
				m_vecTile[idx]->m_bEnable = false;

			else
			{
				m_vecTile[idx]->m_iXTexIdx = atoi(pResult) % (int)(iWidth / 16.f) - 1;
				m_vecTile[idx]->m_iYTexIdx = atoi(pResult) / (int)(iWidth / 16.f);
			}
		}
	}

	return false;
}

char CTileMap::GetTileCol(const Vector2& vPos)	const
{
	int idx = GetIndex(vPos);

	if (idx < 0)
		return -1;

	return m_pColIdx[idx];
}

int CTileMap::GetCountX() const
{
	return m_iCountX;
}

int CTileMap::GetCountY() const
{
	return m_iCountY;
}

void CTileMap::SetTileAnim(bool bAni)
{
	m_bAni = bAni;
}

void CTileMap::SetTileOpacity(float fOp)
{
	CMaterial* pMtrl = m_vecTile[0]->GetMaterial();

	Vector4 vDif = pMtrl->GetDif();

	vDif.w = fOp;

	pMtrl->SetDiffuseColor(vDif);

	SAFE_RELEASE(pMtrl);
}

CTile* CTileMap::GetTile(const Vector2& vPos) const
{
	int idx = GetIndex(vPos);

	return m_vecTile[idx];
}

CTile* CTileMap::GetTile(int x, int y) const
{
	return m_vecTile[y * m_iCountX + x];
}

CTile* CTileMap::GetTile(int idx)	const
{
	return m_vecTile[idx];
}

bool CTileMap::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTileMap::Start()
{
	CSceneComponent::Start();
}

void CTileMap::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CTileMap::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	if (m_bAni)
	{

	}
/*
	size_t iSz = m_vecTile.size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecTile[i]->PostUpdate(fTime);*/
}

void CTileMap::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CTileMap::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);

	Resolution tRS = RESOLUTION;

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Vector3 vRT = pCam->GetWorldPos() +Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * pCam->GetPivot();
	Vector3 vLB = pCam->GetWorldPos() - Vector3((float)tRS.iWidth, (float)tRS.iHeight, 0.f) * pCam->GetPivot();

	SAFE_RELEASE(pCam);

	Vector3 vPos = GetWorldPos();

	vRT -= vPos;
	vLB -= vPos;

	vRT /= m_vTileSize;
	vLB /= m_vTileSize;

	int iStartX = (int)vLB.x;
	int iStartY = (int)vLB.y;

	int iEndX = (int)vRT.x;
	int iEndY = (int)vRT.y;

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;

	iEndX = iEndX >= m_iCountX ? m_iCountX - 1 : iEndX;
	iEndY = iEndY >= m_iCountY ? m_iCountY - 1 : iEndY;

	for (int x = iStartX; x <= iEndX; ++x)
	{
		for (int y = iStartY; y <= iEndY; ++y)
		{
			int idx = y * m_iCountX + x;

			if (!m_vecTile[idx]->IsEnable())
				continue;

			GET_SINGLE(CRenderManager)->AddTileInstancingData(m_vecTile[idx]);
		}
	}
}

void CTileMap::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CTileMap::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CTileMap* CTileMap::Clone()
{
	return new CTileMap(*this);
}

void CTileMap::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CTileMap::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
