#pragma once

#include "../GameEngine.h"

class CCollisionSection
{
	friend class CCollisionManager;

private:
	CCollisionSection();
	~CCollisionSection();

private:
	int								m_ixIdx;
	int								m_iyIdx;
	int								m_izIdx;
	int								m_iIdx;
	Vector3							m_vMax;
	Vector3							m_vMin;
	Vector3							m_vSize;
	std::list<class CCollider*>		m_ColList;
	bool							m_bMouse;

public:
	bool Init(int x, int y, int z, int idx, Vector3 vSize, Vector3 v);
	void SetMinMax(const Vector3& vMin, const Vector3& vMax);
	void Collision(float fTime);
	void AddCollider(class CCollider* pCol);
	void Clear();
	bool EraseCollider(class CCollider* pCol);

};

