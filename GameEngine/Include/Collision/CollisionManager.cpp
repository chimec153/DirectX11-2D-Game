#include "CollisionManager.h"
#include "../Device.h"
#include "CollisionSection.h"
#include "../Component/Collider.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{

}

CCollisionManager::~CCollisionManager()
{
	SAFE_DELETE_VECLIST(m_vecChannel);
	SAFE_DELETE_MAP(m_mapProfile);

	size_t iSz = m_vecSec.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		SAFE_DELETE_VECLIST(m_vecSec[i]->vecSec);
		SAFE_DELETE(m_vecSec[i]);
	}
}

bool CCollisionManager::Init()
{
	m_vecSec.resize((int)SECTION_TYPE::END);

	m_vecSec[(int)SECTION_TYPE::_2D] = CreateSection(1, 1, 1,
		Vector3(10000.f, 10000.f, 1.f), Vector3(10000.f, 10000.f, 1.f) / 2.f);
	m_vecSec[(int)SECTION_TYPE::_2D]->eType = SECTION_TYPE::END;

	m_vecSec[(int)SECTION_TYPE::_3D] = CreateSection(1, 1, 1,
		Vector3(10000.f, 10000.f, 1.f), Vector3(10000.f, 10000.f, 1.f) / 2.f);
	m_vecSec[(int)SECTION_TYPE::_3D]->eType = SECTION_TYPE::END;

	m_vecSec[(int)SECTION_TYPE::UI] = CreateSection(1, 1, 1,
		Vector3((float)RESOLUTION.iWidth, (float)RESOLUTION.iHeight, 1.f),
		Vector3((float)RESOLUTION.iWidth, (float)RESOLUTION.iHeight, 1.f) / 2.f);
	m_vecSec[(int)SECTION_TYPE::UI]->eType = SECTION_TYPE::END;

	CreateChannel("Base", COLLISION_CHANNEL::BASE, COLLISION_TYPE::BLK);
	CreateChannel("Player", COLLISION_CHANNEL::PLAYER, COLLISION_TYPE::BLK);
	CreateChannel("Monster", COLLISION_CHANNEL::MONSTER, COLLISION_TYPE::BLK);
	CreateChannel("NPC", COLLISION_CHANNEL::NPC, COLLISION_TYPE::BLK);
	CreateChannel("Custom1", COLLISION_CHANNEL::CUSTOM1, COLLISION_TYPE::BLK);

	CreateProfile("Base", COLLISION_CHANNEL::BASE, COLLISION_TYPE::BLK);
	CreateProfile("Player", COLLISION_CHANNEL::PLAYER, COLLISION_TYPE::BLK);
	CreateProfile("Monster", COLLISION_CHANNEL::MONSTER, COLLISION_TYPE::BLK);
	CreateProfile("NPC", COLLISION_CHANNEL::NPC, COLLISION_TYPE::BLK);
	CreateProfile("Custom1", COLLISION_CHANNEL::CUSTOM1, COLLISION_TYPE::BLK);

	SetChannelState("Player", COLLISION_CHANNEL::MONSTER, COLLISION_TYPE::IGN);
	SetChannelState("Player", COLLISION_CHANNEL::PLAYER, COLLISION_TYPE::IGN);

	return true;
}


PSectionInfo CCollisionManager::CreateSection(int x, int y, int z, const Vector3& vSize, const Vector3& vCenter)
{
	PSectionInfo pInfo = new SectionInfo;

	pInfo->ixCnt = x;
	pInfo->iyCnt = y;
	pInfo->izCnt = z;
	pInfo->vSize = vSize;
	pInfo->vCenter = vCenter;

	Vector3 vSecSize = Vector3(vSize.x / x, vSize.y / y, vSize.z / z);

	for (int Z = 0; Z < z; ++Z)
	{
		for (int Y = 0; Y < y; ++Y)
		{
			for (int X = 0; X < x; ++X)
			{
				CCollisionSection* pSec = new CCollisionSection;

				pSec->Init(X, Y, Z, Z * x * y + Y * x + X, vSecSize,
					vCenter - vSize / 2.f + Vector3(X * vSecSize.x, Y * vSecSize.y, Z * vSecSize.z) + vSecSize / 2.f);

				pInfo->vecSec.push_back(pSec);
			}
		}
	}

	return pInfo;
}


void CCollisionManager::AddCollider(CCollider* pCol)
{
	if (pCol->IsMouse())
	{
		AddCollider(pCol, SECTION_TYPE::UI);
		AddCollider(pCol, SECTION_TYPE::_2D);
	}

	else if (pCol->IsUI())
	{
		AddCollider(pCol, SECTION_TYPE::UI);
	}

	else if (pCol->Is2D())
	{
		AddCollider(pCol, SECTION_TYPE::_2D);
	}
}

void CCollisionManager::AddCollider(CCollider* pCol, SECTION_TYPE eType)
{
	PSectionInfo pInfo = m_vecSec[(int)eType];

	int iStartX, iStartY, iStartZ;
	int iEndX, iEndY, iEndZ;

	Vector3 vMin = pCol->GetMin();
	Vector3 vMax = pCol->GetMax();

	if (!pCol->IsUI() || pCol->IsMouse())
	{
		vMin -= pInfo->vCenter;
		vMax -= pInfo->vCenter;
	}

	iStartX = (int)(vMin.x / pInfo->vSize.x / pInfo->ixCnt);
	iStartY = (int)(vMin.y / pInfo->vSize.y / pInfo->iyCnt);
	iStartZ = (int)(vMin.z / pInfo->vSize.z / pInfo->izCnt);

	iEndX = (int)(vMax.x / pInfo->vSize.x / pInfo->ixCnt);
	iEndY = (int)(vMax.y / pInfo->vSize.y / pInfo->iyCnt);
	iEndZ = (int)(vMax.z / pInfo->vSize.z / pInfo->izCnt);

	iStartX = iStartX < 0 ? 0 : iStartX;
	iStartY = iStartY < 0 ? 0 : iStartY;
	iStartZ = iStartZ < 0 ? 0 : iStartZ;

	iEndX = iEndX >= pInfo->ixCnt ? pInfo->ixCnt - 1 : iEndX;
	iEndY = iEndY >= pInfo->iyCnt ? pInfo->iyCnt - 1 : iEndY;
	iEndZ = iEndZ >= pInfo->izCnt ? pInfo->izCnt - 1 : iEndZ;

	for (int z = iStartZ; z <= iEndZ; ++z)
	{
		for (int y = iStartY; y <= iEndY; ++y)
		{
			for (int x = iStartX; x <= iEndX; ++x)
			{
				int idx = z * pInfo->iyCnt * pInfo->ixCnt + y * pInfo->ixCnt + x;

				pInfo->vecSec[idx]->AddCollider(pCol);
			}
		}
	}
}

void CCollisionManager::Collsion(float fTime)
{
	size_t iUISize = m_vecSec[(int)SECTION_TYPE::UI]->vecSec.size();

	for (size_t i = 0; i < iUISize; ++i)
	{
		m_vecSec[(int)SECTION_TYPE::UI]->vecSec[i]->m_ColList.sort(&CCollisionManager::SortZ);

		m_vecSec[(int)SECTION_TYPE::UI]->vecSec[i]->Collision(fTime);
	}

	for (int i = 0; i < (int)SECTION_TYPE::UI; ++i)
	{
		size_t iSz = m_vecSec[i]->vecSec.size();

		for (size_t j = 0; j < iSz; ++j)
		{
			m_vecSec[i]->vecSec[j]->Collision(fTime);
		}
	}

	for (int i = 0; i < (int)SECTION_TYPE::END; ++i)
	{
		size_t iSz = m_vecSec[i]->vecSec.size();

		for (size_t j = 0; j < iSz; ++j)
		{
			m_vecSec[i]->vecSec[j]->Clear();
		}
	}
}

void CCollisionManager::SetCenter(const Vector3& v)
{
	for (int i = 0; i < (int)SECTION_TYPE::END; ++i)
	{
		m_vecSec[i]->vCenter = v;

		size_t iSz = m_vecSec[i]->vecSec.size();

		for (size_t j = 0; j < iSz; ++j)
		{
			Vector3 vMin = m_vecSec[i]->vCenter - m_vecSec[i]->vSize / 2.f
				+ Vector3((j % (m_vecSec[i]->ixCnt)) * m_vecSec[i]->vSize.x,
				((j % (m_vecSec[i]->ixCnt * m_vecSec[i]->iyCnt)) / m_vecSec[i]->ixCnt) * m_vecSec[i]->vSize.y,
					(j / (m_vecSec[i]->ixCnt * m_vecSec[i]->iyCnt)) * m_vecSec[i]->vSize.z);

			Vector3 vMax = vMin + m_vecSec[i]->vecSec[j]->m_vSize;

			m_vecSec[i]->vecSec[j]->SetMinMax(vMin, vMax);
		}
	}
}

void CCollisionManager::ClearCollider(CCollider* pCol)
{
	if (pCol->Is2D())
	{
		size_t iSz = m_vecSec[(int)SECTION_TYPE::_2D]->vecSec.size();

		for (size_t i = 0; i < iSz; ++i)
		{
			if (m_vecSec[(int)SECTION_TYPE::_2D]->vecSec[i]->EraseCollider(pCol))
				return;
		}
	}

	else if (pCol->IsUI())
	{
		size_t iSz = m_vecSec[(int)SECTION_TYPE::UI]->vecSec.size();

		for (size_t i = 0; i < iSz; ++i)
		{
			if (m_vecSec[(int)SECTION_TYPE::UI]->vecSec[i]->EraseCollider(pCol))
				return;
		}
	}

	else
	{
		size_t iSz = m_vecSec[(int)SECTION_TYPE::_3D]->vecSec.size();

		for (size_t i = 0; i < iSz; ++i)
		{
			if (m_vecSec[(int)SECTION_TYPE::_3D]->vecSec[i]->EraseCollider(pCol))
				return;
		}
	}
}


bool CCollisionManager::SortZ(CCollider* pSrc, CCollider* pDst)
{
	return pSrc->GetZOrder() < pDst->GetZOrder();
}


bool CCollisionManager::CreateChannel(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType)
{
	PCollisionChannel pChannel = new CollisionChannel;

	pChannel->eChannel = COLLISION_CHANNEL(m_vecChannel.size());
	pChannel->eType = eType;
	pChannel->strTag = strTag;

	m_vecChannel.push_back(pChannel);

	std::unordered_map<std::string, PCollisionProfile>::iterator iter = m_mapProfile.begin();
	std::unordered_map<std::string, PCollisionProfile>::iterator iterEnd = m_mapProfile.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->vecChannel.push_back(*pChannel);
	}

	return true;
}

bool CCollisionManager::CreateProfile(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType)
{
	PCollisionProfile pPro = FindProfile(strTag);

	if (pPro)
		return false;

	pPro = new CollisionProfile;

	pPro->eChannel = eChannel;
	pPro->strTag = strTag;	

	size_t iSize = m_vecChannel.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CollisionChannel tChan = *m_vecChannel[i];
		tChan.eType = eType;
		pPro->vecChannel.push_back(*m_vecChannel[i]);
	}

	m_mapProfile.insert(std::make_pair(strTag, pPro));

	return true;
}

void CCollisionManager::SetChannelState(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType)
{
	PCollisionProfile pPro = FindProfile(strTag);

	if (!pPro)
		return;

	pPro->vecChannel[(int)eChannel].eType = eType;
}

PCollisionProfile CCollisionManager::FindProfile(const std::string& strTag)
{
	std::unordered_map<std::string, PCollisionProfile>::iterator iter = m_mapProfile.find(strTag);

	if (iter == m_mapProfile.end())
		return nullptr;

	return iter->second;
}
