#include "Tile.h"
#include "../Resource/Material.h"

CTile::CTile()	:
	m_eTileOption(TILE_OPTION::NONE),
	m_eTileType(TILE_TYPE::RECT),
	m_iIdx(0),
	m_iX(0),
	m_iY(0),
	m_iXTexIdx(0),
	m_iYTexIdx(0),
	m_vSize(),
	m_matWV(),
	m_bAnim(false),
	m_fFrameTime(0.f),
	m_fMaxTime(0.f),
	m_iFrame(0)
{
}

CTile::CTile(const CTile& tile)	:
	CSceneComponent(tile)
{
	m_eTileOption = tile.m_eTileOption;
	m_eTileType = tile.m_eTileType;

	m_iIdx = tile.m_iIdx;
	m_iX = tile.m_iX;
	m_iY = tile.m_iY;
	m_iXTexIdx = tile.m_iXTexIdx;
	m_iYTexIdx = tile.m_iYTexIdx;
	m_vSize = tile.m_vSize;
	m_matWV = tile.m_matWV;
	m_bAnim = tile.m_bAnim;

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
	return Vector2((float)m_iXTexIdx, (float)m_iYTexIdx);
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
	return m_pMaterial->GetTextureSize();
}

void CTile::SetAnim(const Vector2& iStart, int iCount, float fMaxTime)
{
	for (int i = 0; i < iCount; ++i)
	{
		Vector2 vFrm = iStart + Vector2((float)i, 0.f);

		m_vecTexIdx.push_back(vFrm);
	}

	m_fMaxTime = fMaxTime;

	m_bAnim = true;
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

		while (m_fFrameTime >= m_fMaxTime)
		{
			m_fFrameTime -= m_fMaxTime;

			++m_iFrame;

			if (m_iFrame >= m_vecTexIdx.size())
			{
				m_iFrame = 0;
			}

			m_iXTexIdx = (int)(m_vecTexIdx[m_iFrame].x);
			m_iYTexIdx = (int)(m_vecTexIdx[m_iFrame].y);
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
