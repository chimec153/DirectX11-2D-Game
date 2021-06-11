#include "CollisionSection.h"
#include "../Component/Collider.h"

CCollisionSection::CCollisionSection()	:
	m_ixIdx(0),
	m_iyIdx(0),
	m_izIdx(0),
	m_iIdx(0),
	m_vMax(),
	m_vMin(),
	m_vSize()
{
}

CCollisionSection::~CCollisionSection()
{
}

bool CCollisionSection::Init(int x, int y, int z, int idx, Vector3 vSize, Vector3 vCenter)
{
	m_ixIdx = x;
	m_iyIdx = y;
	m_izIdx = z;
	m_iIdx = idx;
	m_vSize = vSize;

	m_vMin = vCenter - m_vSize / 2.f;
	m_vMax = m_vMin + m_vSize;

	return true;
}

void CCollisionSection::SetMinMax(const Vector3& vMin, const Vector3& vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;

	m_vSize = m_vMax - m_vMin;
}

void CCollisionSection::Collision(float fTime)
{
	std::list<CCollider*>::iterator iterCol = m_ColList.begin();
	std::list<CCollider*>::iterator iterColEnd = m_ColList.end();

	for (; iterCol != iterColEnd; ++iterCol)
	{
		if ((*iterCol)->IsCheck())
		{
			continue;
		}

		(*iterCol)->Check();
		(*iterCol)->CheckPrevSec(fTime);
	}

	if (m_ColList.size() < 2)
		return;

	std::list<CCollider*>::iterator iter = m_ColList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColList.end();

	std::list<CCollider*>::iterator iter1;
	std::list<CCollider*>::iterator iter1End = iterEnd;

	--iterEnd;

	for (; iter != iterEnd; ++iter)
	{
		iter1 = iter;
		iter1++;

		for (; iter1 != iter1End; ++iter1)
		{
			if ((*iter)->HasObj((*iter1)->GetObj()))
			{
				continue;
			}

			PCollisionProfile pSrcProfile = (*iter)->GetProfile();
			PCollisionProfile pDestProfile = (*iter1)->GetProfile();

			bool bSrcIgn = pSrcProfile->vecChannel[(int)pDestProfile->eChannel].eType == COLLISION_TYPE::IGN;
			bool bDestIgn = pDestProfile->vecChannel[(int)pSrcProfile->eChannel].eType == COLLISION_TYPE::IGN;

			if (bSrcIgn && bDestIgn)
			{
				continue;
			}

			if ((*iter)->Collision((*iter1)))
			{
				if ((*iter)->IsMouse() ||
					(*iter1)->IsMouse())
				{
					if (m_bMouse)
					{
						continue;
					}

					else
					{
						m_bMouse = true;
					}
				}

				if (!(*iter)->HasPrevCol((*iter1)))
				{
					(*iter)->AddPrevCol((*iter1));
					(*iter1)->AddPrevCol((*iter));

					if (!bSrcIgn)
					{
						(*iter)->Call(COLLISION_STATE::INIT, (*iter1), fTime);
					}

					if (!bDestIgn)
					{
						(*iter1)->Call(COLLISION_STATE::INIT, (*iter), fTime);
					}
				}

				else
				{
					if (!bSrcIgn)
					{
						(*iter)->Call(COLLISION_STATE::STAY, (*iter1), fTime);
					}

					if (!bDestIgn)
					{
						(*iter1)->Call(COLLISION_STATE::STAY, (*iter), fTime);
					}
				}
			}

			else
			{
				if ((*iter)->HasPrevCol((*iter1)))
				{
					(*iter)->DeletePrevCol((*iter1));
					(*iter1)->DeletePrevCol((*iter));

					if (!bSrcIgn)
					{
						(*iter)->Call(COLLISION_STATE::END, (*iter1), fTime);
					}

					if (!bDestIgn)
					{
						(*iter1)->Call(COLLISION_STATE::END, (*iter), fTime);
					}
				}
			}
		}
	}
}

void CCollisionSection::AddCollider(CCollider* pCol)
{
	pCol->AddSec(m_iIdx);
	m_ColList.push_back(pCol);
}

void CCollisionSection::Clear()
{
	m_bMouse = false;
	std::list<CCollider*>::iterator iter = m_ColList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Clear();
	}

	m_ColList.clear();
}

bool CCollisionSection::EraseCollider(CCollider* pCol)
{
	std::list<CCollider*>::iterator iter = m_ColList.begin();
	std::list<CCollider*>::iterator iterEnd = m_ColList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
		{
			m_ColList.erase(iter);
			return true;
		}
	}
	return false;
}
