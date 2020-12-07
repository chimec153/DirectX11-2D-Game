#pragma once

#include "../GameEngine.h"

enum class SECTION_TYPE
{
	_2D,
	_3D,
	UI,
	END
};

typedef struct _tagSectionInfo
{
	std::vector<class CCollisionSection*>	vecSec;
	int		ixCnt;
	int		iyCnt;
	int		izCnt;
	Vector3	vSize;
	Vector3	vCenter;
	SECTION_TYPE	eType;
	bool	bUI;

	_tagSectionInfo() :
		ixCnt(0),
		iyCnt(0),
		izCnt(0),
		vSize(),
		vCenter(),
		bUI(false),
		eType(SECTION_TYPE::_2D)
	{
	}
}SectionInfo, * PSectionInfo;

class CCollisionManager
{
private:
	std::vector<PCollisionChannel>						m_vecChannel;
	std::unordered_map<std::string, PCollisionProfile>	m_mapProfile;

private:
	std::vector<PSectionInfo>	m_vecSec;

public:
	bool Init();
	bool CreateChannel(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType);
	bool CreateProfile(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType);
	void SetChannelState(const std::string& strTag, COLLISION_CHANNEL eChannel, COLLISION_TYPE eType);

public:
	PSectionInfo CreateSection(int x, int y, int z, const Vector3& vSize, const Vector3& vCen);
	void AddCollider(class CCollider* pCol);
	void AddCollider(class CCollider* pCol, SECTION_TYPE eType);
	void Collsion(float fTime);
	void SetCenter(const Vector3& v);
	void ClearCollider(class CCollider* pCol);

public:
	PCollisionProfile FindProfile(const std::string& strTag);

	DECLARE_SINGLE(CCollisionManager)

private:
	static bool SortZ(class CCollider* pSrc, class CCollider* pDst);
};
