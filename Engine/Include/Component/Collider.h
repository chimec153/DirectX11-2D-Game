#pragma once

#include "SceneComponent.h"

class CCollider :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CCollider();
	CCollider(const CCollider& col);
	virtual ~CCollider() = 0;

protected:
#ifdef _DEBUG
	class CMesh2D*			m_pDebugMesh;
	class CMaterial*		m_pDebugMtrl;
#endif
	Vector3					m_vMin;
	Vector3					m_vMax;
	bool					m_bUI;
	bool					m_b2D;
	COLLIDER_TYPE			m_eColType;
	Vector3					m_vCross;
	std::list<int>			m_SecList;
	std::list<CCollider*>	m_CurColList;
	std::list<CCollider*>	m_PrevColList;
	PCollisionProfile		m_pProfile;
	std::list<std::function<void(CCollider*, CCollider*, float)>>		m_CallList[(int)COLLISION_STATE::MAX];
	std::list<std::function<void(CCollider*, const Vector2&, float)>>	m_MouseCallList[(int)COLLISION_STATE::MAX];
	bool					m_bCheck;
	bool					m_bMouse;
	int						m_iZOrder;

public:
	COLLIDER_TYPE GetColType()	const;
	void SetCross(const Vector3& v);
	void AddSec(int iSec);
	void AddPrevCol(CCollider* pCol);
	bool HasPrevCol(CCollider* pCol)	const;
	void DeletePrevCol(CCollider* pCol);
	void SetProfile(const std::string& strTag);
	PCollisionProfile GetProfile()	const;
	void CheckPrevSec(float fTime);
	void SetZOrder(int iZ);
	int GetZOrder()	const;
	void Clear();
	bool IsUI()	const;
	bool Is2D()	const;
	const Vector3& GetMin()	const;
	const Vector3& GetMax()	const;
	bool IsCheck()	const;
	void Check();
	void SetUI();
	void SetMouse();
	bool IsMouse()	const;
	const Vector3& GetCross()	const;
	bool HasObj(class CObj* pObj);
	class CObj* GetObj()	const;

public:
	void SetCallBack(COLLISION_STATE eStat, void(*pFunc)(CCollider*, CCollider*, float));

	template <typename T>
	void SetCallBack(COLLISION_STATE eState, T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
	{
		m_CallList[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void SetMouseCallBack(COLLISION_STATE eStat, void(*pFunc)(CCollider*, const Vector2&, float));

	template <typename T>
	void SetMouseCallBack(COLLISION_STATE eState, T* pObj, void(T::* pFunc)(CCollider*, const Vector2&, float))
	{
		m_MouseCallList[(int)eState].push_back(std::bind(pFunc, pObj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}

	void Call(COLLISION_STATE eState, CCollider* pDest, float fTime);
	void MouseCall(COLLISION_STATE eState, const Vector2& vMouse, float fTime);

public:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float fTime);
	virtual void PostUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PreRender(float fTime);
	virtual void Render(float fTime);
	virtual void PostRender(float fTime);
	virtual CCollider* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	virtual bool Collision(class CCollider* pDest);

};

