#include "Collider.h"
#include "../Resource/Mesh2D.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Scene/SceneCollision.h"
#include "../Scene/Scene.h"
#include "../Collision/CollisionManager.h"
#include "../Scene/SceneManager.h"

CCollider::CCollider() :
	m_vMin(),
	m_vMax(),
	m_bUI(false),
	m_b2D(false),
	m_eColType(COLLIDER_TYPE::CT_RECT),
	m_vCross(),
	m_pProfile(nullptr),
	m_bCheck(false),
	m_bMouse(false),
	m_iZOrder(0)
{
#ifdef _DEBUG
	m_pDebugMesh = nullptr;
	m_pDebugMtrl = nullptr;
#endif
}

CCollider::CCollider(const CCollider& col)	:
	CSceneComponent(col)
{
	m_bUI = col.m_bUI;
	m_b2D = col.m_b2D;
	m_eColType = col.m_eColType;
	m_vCross = col.m_vCross;
	m_pProfile = col.m_pProfile;
	m_bCheck = col.m_bCheck;
	m_bMouse = col.m_bMouse;

#ifdef _DEBUG
	m_pDebugMesh = col.m_pDebugMesh;

	if (m_pDebugMesh)
		m_pDebugMesh->AddRef();

	if (col.m_pDebugMtrl)
		m_pDebugMtrl = col.m_pDebugMtrl->Clone();

	else
		m_pDebugMtrl = nullptr;
#endif
}

CCollider::~CCollider()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_pDebugMesh);
	SAFE_RELEASE(m_pDebugMtrl);
#endif

	std::list<CCollider*>::iterator iter = m_PrevColList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevColList.end();

	for (; iter != iterEnd;)
	{
		(*iter)->Call(COLLISION_STATE::END, this, 0.f);
		(*iter)->DeletePrevCol(this);
		Call(COLLISION_STATE::END, (*iter), 0.f);

		iter = m_PrevColList.erase(iter);
		iterEnd = m_PrevColList.end();
	}

	GET_SINGLE(CCollisionManager)->ClearCollider(this);
}

COLLIDER_TYPE CCollider::GetColType() const
{
	return m_eColType;
}

void CCollider::SetCross(const Vector3& v)
{
	m_vCross = v;
}

void CCollider::AddSec(int iSec)
{
	m_SecList.push_back(iSec);
}

void CCollider::AddPrevCol(CCollider* pCol)
{
	m_PrevColList.push_back(pCol);
}

bool CCollider::HasPrevCol(CCollider* pCol) const
{
	std::list<CCollider*>::const_iterator iter = m_PrevColList.begin();
	std::list<CCollider*>::const_iterator iterEnd = m_PrevColList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
			return true;
	}

	return false;
}

void CCollider::DeletePrevCol(CCollider* pCol)
{
	std::list<CCollider*>::const_iterator iter = m_PrevColList.begin();
	std::list<CCollider*>::const_iterator iterEnd = m_PrevColList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == pCol)
		{
			m_PrevColList.erase(iter);
			return;
		}
	}
}

void CCollider::SetProfile(const std::string& strTag)
{
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile(strTag);
}

PCollisionProfile CCollider::GetProfile() const
{
	return m_pProfile;
}

void CCollider::CheckPrevSec(float fTime)
{
	std::list<CCollider*>::iterator iter = m_PrevColList.begin();
	std::list<CCollider*>::iterator iterEnd = m_PrevColList.end();

	for (; iter != iterEnd; )
	{
		std::list<int>::iterator iterSec = m_SecList.begin();
		std::list<int>::iterator iterSecEnd = m_SecList.end();

		bool bCheck = false;

		for (; iterSec != iterSecEnd; ++iterSec)
		{
			std::list<int>::iterator iterSec1 = (*iter)->m_SecList.begin();
			std::list<int>::iterator iterSec1End = (*iter)->m_SecList.end();

			for (; iterSec1 != iterSec1End; ++iterSec1)
			{
				if ((*iterSec) == (*iterSec1))
				{
					bCheck = true;
					break;
				}
			}

			if (bCheck)
				break;
		}

		if (!bCheck)
		{
			Call(COLLISION_STATE::END, (*iter), fTime);
			(*iter)->Call(COLLISION_STATE::END, this, fTime);
			(*iter)->DeletePrevCol(this);
			iter = m_PrevColList.erase(iter);
			iterEnd = m_PrevColList.end();
			continue;
		}

		++iter;
	}
}

void CCollider::SetZOrder(int iZ)
{
	m_iZOrder = iZ;
}

int CCollider::GetZOrder() const
{
	return m_iZOrder;
}

void CCollider::Clear()
{
	m_CurColList.clear();
	m_SecList.clear();
	m_bCheck = false;
}

bool CCollider::IsUI() const
{
	return m_bUI;
}

bool CCollider::Is2D() const
{
	return m_b2D;
}

const Vector3& CCollider::GetMin() const
{
	return m_vMin;
}

const Vector3& CCollider::GetMax() const
{
	return m_vMax;
}

bool CCollider::IsCheck() const
{
	return m_bCheck;
}

void CCollider::Check()
{
	m_bCheck = true;
}

void CCollider::SetUI()
{
	m_bUI = true;
}

void CCollider::SetMouse()
{
	m_bMouse = true;
}

bool CCollider::IsMouse() const
{
	return m_bMouse;
}

const Vector3& CCollider::GetCross() const
{
	return m_vCross;
}

bool CCollider::HasObj(CObj* pObj)
{
	return m_pObj == pObj;
}

CObj* CCollider::GetObj() const
{
	return m_pObj;
}

void CCollider::SetCallBack(COLLISION_STATE eStat, void(*pFunc)(CCollider*, CCollider*, float))
{
	m_CallList[(int)eStat].push_back(std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CCollider::SetMouseCallBack(COLLISION_STATE eStat, void(*pFunc)(CCollider*, const Vector2&, float))
{
	m_MouseCallList[(int)eStat].push_back(std::bind(pFunc, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void CCollider::Call(COLLISION_STATE eState,  CCollider* pDest, float fTime)
{
	std::list<std::function<void(CCollider*, CCollider*, float)>>::iterator iter = m_CallList[(int)eState].begin();
	std::list<std::function<void(CCollider*, CCollider*, float)>>::iterator iterEnd = m_CallList[(int)eState].end();

	for (; iter != iterEnd; ++iter)
		(*iter)(this, pDest, fTime);
}

void CCollider::MouseCall(COLLISION_STATE eState, const Vector2& vMouse, float fTime)
{
	std::list<std::function<void(CCollider*, const Vector2&, float)>>::iterator iter = m_MouseCallList[(int)eState].begin();
	std::list<std::function<void(CCollider*, const Vector2&, float)>>::iterator iterEnd = m_MouseCallList[(int)eState].end();

	for (; iter != iterEnd; ++iter)
		(*iter)(this, vMouse, fTime);
}

bool CCollider::Init()
{
	if (!CSceneComponent::Init())
		return false;

#ifdef _DEBUG
	CMaterial* pMtrl = GET_SINGLE(CResourceManager)->FindMaterial("Collider");

	m_pMaterial = pMtrl->Clone();

	SAFE_RELEASE(pMtrl);
#endif
	m_pProfile = GET_SINGLE(CCollisionManager)->FindProfile("Base");

	return true;
}

void CCollider::Start()
{
	CSceneComponent::Start();
}

void CCollider::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CCollider::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);
}

void CCollider::Collision(float fTime)
{
	CSceneComponent::Collision(fTime);

	GET_SINGLE(CCollisionManager)->AddCollider(this);
}

void CCollider::PreRender(float fTime)
{
#ifdef _DEBUG

	if (m_PrevColList.empty())
		m_pMaterial->SetDiffuseColor(0.f, 1.f, 0.f, 1.f);

	else
		m_pMaterial->SetDiffuseColor(1.f, 0.f, 0.f, 1.f);
#endif

	CSceneComponent::PreRender(fTime);
}

void CCollider::Render(float fTime)
{
	CSceneComponent::Render(fTime);

#ifdef _DEBUG

	m_pMaterial->SetMaterial();

	if (m_pMesh)
		m_pMesh->Render(fTime);
#endif
}

void CCollider::PostRender(float fTime)
{
	CSceneComponent::PostRender(fTime);
}

CCollider* CCollider::Clone()
{
	return nullptr;
}

void CCollider::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CCollider::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}

bool CCollider::Collision(CCollider* pDest)
{

	return false;
}
