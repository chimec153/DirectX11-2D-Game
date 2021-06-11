#include "Tile.h"
#include "../Resource/Material.h"
#include "TileMap.h"

CTile::CTile()	:
	m_eTileOption(TILE_OPTION::NONE),
	m_eTileType(TILE_TYPE::RECT),
	m_iIdx(0),
	m_iX(0),
	m_iY(0),
	m_fXTexIdx(0),
	m_fYTexIdx(0),
	m_vSize(),
	m_matWV(),
	m_bAnim(false),
	m_fFrameTime(0.f),
	m_fMaxTime(0.f),
	m_iFrame(0),
	m_pMap(nullptr)
	, m_bOpen(false)
	, m_bClose(false)
	, m_fH(0.f)
	, m_fG(0.f)
	, m_pParent(nullptr)
	, m_vColor(Vector4::White)
{
}

CTile::CTile(const CTile& tile)	:
	CSceneComponent(tile)
	, m_vColor(tile.m_vColor)
{
	m_eTileOption = tile.m_eTileOption;
	m_eTileType = tile.m_eTileType;

	m_iIdx = tile.m_iIdx;
	m_iX = tile.m_iX;
	m_iY = tile.m_iY;
	m_fXTexIdx = tile.m_fXTexIdx;
	m_fYTexIdx = tile.m_fYTexIdx;
	m_vSize = tile.m_vSize;
	m_matWV = tile.m_matWV;
	m_bAnim = tile.m_bAnim;
	m_pMap = tile.m_pMap;

	size_t iSz = tile.m_vecTexIdx.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecTexIdx.push_back(tile.m_vecTexIdx[i]);
	}

	m_fFrameTime = tile.m_fFrameTime;
	m_fMaxTime = tile.m_fMaxTime;
	m_iFrame = tile.m_iFrame;
}

CTile::~CTile()
{
}

const Vector2 CTile::GetTexIdx() const
{
	return Vector2((float)m_fXTexIdx, (float)m_fYTexIdx);
}

const Vector2 CTile::GetSize() const
{
	return m_vSize;
}

const Matrix& CTile::GetWV() const
{
	return m_matWV;
}

const Vector2 CTile::GetTextureSize() const
{
	return m_pMap->GetTextureSize();
}

void CTile::SetAnim(const std::vector<Vector2>& vecFrame,float fFrameTime)
{
	size_t iSz = vecFrame.size();

	for (int i = 0; i < iSz; ++i)
	{
		m_vecTexIdx.push_back(vecFrame[i]);
	}

	m_fMaxTime = fFrameTime;

	m_bAnim = true;
}

CTileMap* CTile::GetMap() const
{
	return m_pMap;
}

TILE_OPTION CTile::GetTileOption() const
{
	return m_eTileOption;
}

void CTile::SetTileOption(TILE_OPTION eOp)
{
	m_eTileOption = eOp;
}

int CTile::GetIndexX() const
{
	return m_iX;
}

int CTile::GetIndexY() const
{
	return m_iY;
}

void CTile::SetOpen(bool bOpen)
{
	m_bOpen = bOpen;
}

void CTile::SetClose(bool bClose)
{
	m_bClose =bClose;
}

bool CTile::IsOpen() const
{
	return m_bOpen;
}

bool CTile::IsClose() const
{
	return m_bClose;
}

void CTile::SetH(float fH)
{
	m_fH = fH;
}

void CTile::SetG(float fG)
{
	m_fG = fG;
}

float CTile::GetH() const
{
	return m_fH;
}

float CTile::GetG() const
{
	return m_fG;
}

CTile* CTile::GetParent() const
{
	return m_pParent;
}

void CTile::SetParent(CTile* pParent)
{
	m_pParent = pParent;
}

void CTile::SetTexIdx(const Vector2& vIdx)
{
	m_fXTexIdx = vIdx.x;
	m_fYTexIdx = vIdx.y;
}

void CTile::SetColor(const Vector4& vColor)
{
	m_vColor = vColor;
}

const Vector4& CTile::GetColor() const
{
	return m_vColor;
}

bool CTile::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTile::Start()
{
	CSceneComponent::Start();
}

void CTile::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CTile::PostUpdate(float fTime)
{
	if (m_bAnim)
	{
		m_fFrameTime += fTime;

		while (m_fFrameTime >= m_fMaxTime * 4.f)
		{
			m_fFrameTime -= m_fMaxTime * 4.f;

			++m_iFrame;

			if (m_iFrame >= m_vecTexIdx.size())
			{
				m_iFrame = 0;
			}

			m_fXTexIdx = m_vecTexIdx[m_iFrame].x;
			m_fYTexIdx = m_vecTexIdx[m_iFrame].y;
		}
	}
}

void CTile::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);
}

void CTile::PreRender(float fTime)
{
	CSceneComponent::PreRender(fTime);
}

void CTile::Render(float fTime)
{
	CSceneComponent::Render(fTime);
}

void CTile::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CTile* CTile::Clone()
{
	return new CTile(*this);
}

void CTile::Save(FILE * pFile)
{
	CSceneComponent::Save(pFile);
}

void CTile::Load(FILE * pFile)
{
	CSceneComponent::Load(pFile);
}
