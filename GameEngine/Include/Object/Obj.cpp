#include "Obj.h"
#include "../Component/SceneComponent.h"
#include "../Component/ObjComponent.h"
#include "../Component/Mesh2DComponent.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/Transform.h"
#include "../InputObj.h"
#include "../Component/Camera.h"

CObj::CObj()	:
	m_bStart(false),
	m_pScene(nullptr),
	m_pInput(nullptr),
	m_pLayer(nullptr),
	m_iObjClassType(0)
{
	m_pRootComponent = new CMesh2DComponent;
	m_pInput = new CInputObj;
}

CObj::CObj(const CObj& obj)	:
	CRef(obj)
{
	m_pScene = obj.m_pScene;

	if (obj.m_pRootComponent)
	{
		m_pRootComponent = obj.m_pRootComponent->Clone();

		m_pRootComponent->SetObj(this);
		m_pRootComponent->m_pScene = m_pScene;
	}

	m_vecObjComponent.clear();

	size_t iSize = obj.m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CObjComponent* pCom = new CObjComponent;

		*pCom = *obj.m_vecObjComponent[i];

		pCom->m_pObj = this;
		pCom->m_pScene = m_pScene;

		m_vecObjComponent.push_back(pCom);
	}

	m_iObjClassType = obj.m_iObjClassType;

	m_pInput = new CInputObj;
	m_bStart = false;
}

CObj::~CObj()
{
	SAFE_RELEASE(m_pRootComponent);
	SAFE_RELEASE_VECLIST(m_vecObjComponent);
	SAFE_DELETE(m_pInput);

	if (m_EditorDelete)
		m_EditorDelete(GetName());
}

bool CObj::IsStart() const
{
	return m_bStart;
}

void CObj::SetScene(CScene* pScene)
{
	m_pRootComponent->SetScene(pScene);

	size_t iSz = m_vecObjComponent.size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecObjComponent[i]->SetScene(pScene);
}

CScene* CObj::GetScene() const
{
	return m_pScene;
}

CLayer* CObj::GetLayer() const
{
	return m_pLayer;
}

void CObj::SetLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;

	m_pRootComponent->SetLayer(pLayer);

	size_t iSz = m_vecObjComponent.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecObjComponent[i]->m_pLayer = pLayer;
	}
}

int CObj::GetClassType() const
{
	return m_iObjClassType;
}


void CObj::SetRootComponent(CSceneComponent* pComponent)
{
	SAFE_RELEASE(m_pRootComponent);

	m_pRootComponent = pComponent;

	if (m_pRootComponent)
		m_pRootComponent->AddRef();
}

CSceneComponent* CObj::FindSceneComponent(const std::string& strTag)
{
	return m_pRootComponent->FindComponent(strTag);
}

void CObj::Notify(const std::string& strTag)
{
}

bool CObj::Init()
{
	m_pRootComponent->m_pScene = m_pScene;

	if (!m_pRootComponent->Init())
		return false;

	return true;
}

void CObj::Start()
{
	m_bStart = true;

	m_pRootComponent->Start();

	size_t iSize = m_vecObjComponent.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->Start();
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
	if(m_pRootComponent->IsEnable())
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

CObj* CObj::Clone()
{
	return new CObj(*this);
}

void CObj::Save(FILE* pFile)
{
	CRef::Save(pFile);

	int iSize = (int)m_vecObjComponent.size();

	fwrite(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
		m_vecObjComponent[i]->Save(pFile);

	std::vector<CSceneComponent*> vecCom;

	m_pRootComponent->GetAllComponent(vecCom);

	iSize = (int)vecCom.size();

	fwrite(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		SCENECOMPONENT_CLASS_TYPE eType = vecCom[i]->GetSceneComponentClassType();

		fwrite(&eType, 4, 1, pFile);
	}

	std::vector<Hierarchy> vecHier;

	m_pRootComponent->GetAllComponentName(vecHier);

	for (int i = 0; i < iSize; ++i)
	{
		int iLength = (int)vecHier[i].strName.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(vecHier[i].strName.c_str(), 1, iLength, pFile);

		iLength = (int)vecHier[i].strParent.length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(vecHier[i].strParent.c_str(), 1, iLength, pFile);
	}		

	for (int i = 0; i < iSize; ++i)
		vecCom[i]->Save(pFile);
}

void CObj::Load(FILE* pFile)
{
	CRef::Load(pFile);

	int iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		CObjComponent* pCom = new CObjComponent;

		pCom->m_pScene = m_pScene;
		pCom->m_pObj = this;

		pCom->Load(pFile);

		m_vecObjComponent.push_back(pCom);
	}

	std::vector<CSceneComponent*> vecCom;

	iSize = 0;

	fread(&iSize, 4, 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		CSceneComponent* pCom = nullptr;

		SCENECOMPONENT_CLASS_TYPE eType = SCENECOMPONENT_CLASS_TYPE();

		fread(&eType, 4, 1, pFile);

		switch (eType)
		{
		case SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D:
			pCom = new CMesh2DComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_STATICMESH:
			pCom = new CStaticMeshComponent;
			break;
		case SCENECOMPONENT_CLASS_TYPE::SCT_CAMERA:
			pCom = new CCamera;
			break;
		}

		pCom->m_pScene = m_pScene;
		pCom->m_pObj = this;

		vecCom.push_back(pCom);
	}

	std::vector<Hierarchy> vecHier;

	for (int i = 0; i < iSize; ++i)
	{
		Hierarchy tHier;

		int iLength = 0;
		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		tHier.strName = strTag;

		iLength = 0;

		memset(strTag, 0, 256);

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		tHier.strParent = strTag;

		vecHier.push_back(tHier);
	}

	for (int i = 0; i < iSize; ++i)
	{
		if (vecHier[i].strParent.empty())
		{
			SAFE_RELEASE(m_pRootComponent);

			m_pRootComponent = vecCom[i];

			vecCom[i]->Load(pFile);

			continue;
		}

		for (int j = 0; j < iSize; ++j)
		{
			if (vecHier[j].strName == vecHier[i].strParent)
			{
				vecCom[i]->m_pParent = vecCom[j];
				vecCom[j]->AddChild(vecCom[i]);

				vecCom[i]->Load(pFile);

				break;
			}
		}
	}

	m_bStart = false;
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

Vector3 CObj::GetVelocityScale() const
{
	return m_pRootComponent->GetVelocityScale();
}

Vector3 CObj::GetVelocityRot() const
{
	return m_pRootComponent->GetVelocityRot();
}

Vector3 CObj::GetVelocity() const
{
	return m_pRootComponent->GetVelocity();
}

float CObj::GetVelocityAmt() const
{
	return m_pRootComponent->GetVelocityAmt();
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

Vector3 CObj::GetRelativeAxis(WORLD_AXIS axis) const
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

Vector3 CObj::GetWorldAxis(WORLD_AXIS axis) const
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

void CObj::GetAllComponentName(std::vector<Hierarchy>& vecHierarchy)
{
	if(m_pRootComponent)
		m_pRootComponent->GetAllComponentName(vecHierarchy);
}

void CObj::GetAllComponent(std::vector<CSceneComponent*>& vecCom)
{
	if (m_pRootComponent)
	m_pRootComponent->GetAllComponent(vecCom);
}
