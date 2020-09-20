#include "Obj.h"
#include "../Component/SceneComponent.h"
#include "../Component/ObjComponent.h"
#include "../Component/Mesh2DComponent.h"

CObj::CObj()	:
	m_bStart(false),
	m_pScene(nullptr)
{
	m_pRootComponent = new CMesh2DComponent;
}

CObj::CObj(const CObj& obj)
{
	*this = obj;

	m_iRef = 1;
}

CObj::~CObj()
{
	SAFE_RELEASE(m_pRootComponent);
	SAFE_RELEASE_VECLIST(m_vecObjComponent);
}

void CObj::SetRootComponent(CSceneComponent* pComponent)
{
	SAFE_RELEASE(m_pRootComponent);

	m_pRootComponent = pComponent;

	if (m_pRootComponent)
		m_pRootComponent->AddRef();
}

bool CObj::Init()
{
	return true;
}

void CObj::Start()
{
	m_bStart = true;
}

void CObj::Input(float fTime)
{
}

void CObj::Update(float fTime)
{
		m_pRootComponent->Update(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->Update(fTime);
}

void CObj::PostUpdate(float fTime)
{
		m_pRootComponent->PostUpdate(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->PostUpdate(fTime);
}

void CObj::Collision(float fTime)
{
		m_pRootComponent->Collision(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->Collision(fTime);
}

void CObj::PreRender(float fTime)
{
		m_pRootComponent->PreRender(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->PreRender(fTime);
}

void CObj::Render(float fTime)
{
		m_pRootComponent->Render(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->Render(fTime);
}

void CObj::PostRender(float fTime)
{
		m_pRootComponent->PostRender(fTime);

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->PostRender(fTime);
}

void CObj::SetInheritScale(bool bInherit)
{
	m_pRootComponent->SetInheritScale(bInherit);
}

void CObj::SetInheritRotX(bool bInherit)
{
	m_pRootComponent->SetInheritRotX(bInherit);
}

void CObj::SetInheritRotY(bool bInherit)
{
	m_pRootComponent->SetInheritRotY(bInherit);
}

void CObj::SetInheritRotZ(bool bInherit)
{
	m_pRootComponent->SetInheritRotZ(bInherit);
}

void CObj::InheritScale()
{
		m_pRootComponent->InheritScale();
}

void CObj::InheritRot()
{
		m_pRootComponent->InheritRot();
}

void CObj::InheritPos()
{
		m_pRootComponent->InheritPos();
}

void CObj::SetRelativeScale(const Vector3& v)
{
		m_pRootComponent->SetRelativeScale(v);
}

void CObj::SetRelativeScale(float x, float y, float z)
{
		m_pRootComponent->SetRelativeScale(x, y, z);
}

void CObj::SetRelativePos(const Vector3 & v)
{
		m_pRootComponent->SetRelativePos(v);
}

void CObj::SetRelativePos(float x, float y, float z)
{
		m_pRootComponent->SetRelativePos(x, y, z);
}

void CObj::SetRelativeRot(const Vector3 & v)
{
		m_pRootComponent->SetRelativeRot(v);
}

void CObj::SetRelativeRot(float x, float y, float z)
{
		m_pRootComponent->SetRelativeRot(x, y, z);
}

void CObj::SetRelativeRotX(float x)
{
		m_pRootComponent->SetRelativeRotX(x);
}

void CObj::SetRelativeRotY(float y)
{
		m_pRootComponent->SetRelativeRotY(y);
}

void CObj::SetRelativeRotZ(float z)
{
		m_pRootComponent->SetRelativeRotZ(z);
}

void CObj::AddRelativeScale(const Vector3 & v)
{
		m_pRootComponent->AddRelativeScale(v);
}

void CObj::AddRelativeScale(float x, float y, float z)
{
		m_pRootComponent->AddRelativeScale(x, y, z);
}

void CObj::AddRelativePos(const Vector3 & v)
{
		m_pRootComponent->AddRelativePos(v);
}

void CObj::AddRelativePos(float x, float y, float z)
{
		m_pRootComponent->AddRelativePos(x, y, z);
}

void CObj::AddRelativeRot(const Vector3 & v)
{
		m_pRootComponent->AddRelativeRot(v);
}

void CObj::AddRelativeRot(float x, float y, float z)
{
		m_pRootComponent->AddRelativeRot(x, y, z);
}

void CObj::AddRelativeRotX(float x)
{
		m_pRootComponent->AddRelativeRotX(x);
}

void CObj::AddRelativeRotY(float y)
{
		m_pRootComponent->AddRelativeRotY(y);
}

void CObj::AddRelativeRotZ(float z)
{
		m_pRootComponent->AddRelativeRotZ(z);
}

Vector3 CObj::GetRelativeScale() const
{
	return m_pRootComponent->GetRelativeScale();
}

Vector3 CObj::GetRelativeRot() const
{
	return m_pRootComponent->GetRelativeRot();
}

Vector3 CObj::GetRelativePos() const
{
	return m_pRootComponent->GetRelativePos();
}

Vector3 CObj::GetRelativeAxis(AXIS axis) const
{
	return m_pRootComponent->GetRelativeAxis(axis);
}

Vector3 CObj::GetWorldScale() const
{
		return m_pRootComponent->GetWorldScale();

}

Vector3 CObj::GetWorldRot() const
{
		return m_pRootComponent->GetWorldRot();

}

Vector3 CObj::GetWorldPos() const
{
		return m_pRootComponent->GetWorldPos();

}

Vector3 CObj::GetWorldAxis(AXIS axis) const
{
		return m_pRootComponent->GetWorldAxis(axis);

}

Vector3 CObj::GetPivot() const
{
		return m_pRootComponent->GetPivot();

}

Matrix CObj::GetMatScale() const
{
	return m_pRootComponent->GetMatScale();
}

Matrix CObj::GetMatRot() const
{
	return m_pRootComponent->GetMatRot();
}

Matrix CObj::GetMatPos() const
{
	return m_pRootComponent->GetMatPos();
}

Matrix CObj::GetMatWorld() const
{
	return m_pRootComponent->GetMatWorld();
}

void CObj::SetWorldScale(const Vector3 & v)
{
		m_pRootComponent->SetWorldScale(v);
}

void CObj::SetWorldScale(float x, float y, float z)
{
		m_pRootComponent->SetWorldScale(x, y, z);
}

void CObj::SetWorldPos(const Vector3 & v)
{
		m_pRootComponent->SetWorldPos(v);
}

void CObj::SetWorldPos(float x, float y, float z)
{
		m_pRootComponent->SetWorldPos(x, y, z);
}

void CObj::SetWorldRot(const Vector3 & v)
{
		m_pRootComponent->SetWorldRot(v);
}

void CObj::SetWorldRot(float x, float y, float z)
{
		m_pRootComponent->SetWorldRot(x, y, z);
}

void CObj::SetWorldRotX(float x)
{
		m_pRootComponent->SetWorldRotX(x);
}

void CObj::SetWorldRotY(float y)
{
		m_pRootComponent->SetWorldRotY(y);
}

void CObj::SetWorldRotZ(float z)
{
		m_pRootComponent->SetWorldRotZ(z);
}

void CObj::AddWorldScale(const Vector3 & v)
{
		m_pRootComponent->AddWorldScale(v);
}

void CObj::AddWorldScale(float x, float y, float z)
{
		m_pRootComponent->AddWorldScale(x, y, z);
}

void CObj::AddWorldPos(const Vector3 & v)
{
		m_pRootComponent->AddWorldPos(v);
}

void CObj::AddWorldPos(float x, float y, float z)
{
		m_pRootComponent->AddWorldPos(x, y, z);
}

void CObj::AddWorldRot(const Vector3 & v)
{
		m_pRootComponent->AddWorldRot(v);
}

void CObj::AddWorldRot(float x, float y, float z)
{
		m_pRootComponent->AddWorldRot(x, y, z);
}

void CObj::AddWorldRotX(float x)
{
		m_pRootComponent->AddWorldRotX(x);
}

void CObj::AddWorldRotY(float y)
{
		m_pRootComponent->AddWorldRotY(y);
}

void CObj::AddWorldRotZ(float z)
{
		m_pRootComponent->AddWorldRotZ(z);
}

void CObj::SetPivot(const Vector3 & v)
{
		m_pRootComponent->SetPivot(v);
}

void CObj::SetPivot(float x, float y, float z)
{
		m_pRootComponent->SetPivot(x,y,z);
}

void CObj::SetMeshSize(const Vector3 & v)
{
		m_pRootComponent->SetMeshSize(v);
}
