#include "SceneComponent.h"
#include "Transform.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../RenderManager.h"
#include "../Render/RenderState.h"

CSceneComponent::CSceneComponent()	:
	m_pParent(nullptr),
	m_eSceneComponentType(SCENE_COMPONENT_TYPE::SCT_2D),
	m_eSceneComponentClassType(SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D),
	m_pMaterial(nullptr),
	m_pMesh(nullptr),
	m_bInst(false)
{
	m_eType = COMPONENT_TYPE::CT_SCENE;

	m_pTransform = new CTransform;

	m_pTransform->m_pOwner = this;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com)
{
	if (com.m_pTransform)
	{
		m_pTransform = com.m_pTransform->Clone();
		m_pTransform->m_pScene = m_pScene;
		m_pTransform->m_pOwner = this;
	}

	else
		m_pTransform = nullptr;


	m_pParent = nullptr;

	m_eSceneComponentClassType = com.m_eSceneComponentClassType;
	m_eSceneComponentType = com.m_eSceneComponentType;
	
	m_vecChild.clear();

	size_t iSize = com.m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CSceneComponent* pComponent = com.m_vecChild[i]->Clone();

		pComponent->m_pScene = m_pScene;
		pComponent->m_pObj = m_pObj;
		pComponent->m_pParent = this;
		pComponent->m_pTransform->m_pParent = m_pTransform;
		pComponent->m_pTransform->m_pOwner = pComponent;

		m_vecChild.push_back(pComponent);
		m_pTransform->m_vecChild.push_back(pComponent->m_pTransform);

		pComponent->InheritPos();
		pComponent->InheritRot();
		pComponent->InheritScale();
	}

	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();

	if (com.m_pMaterial)
		m_pMaterial = com.m_pMaterial->Clone();

	else
		m_pMaterial = nullptr;

	m_bInst = com.m_bInst;
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_pTransform);
	SAFE_RELEASE_VECLIST(m_vecChild);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMaterial);
}

bool CSceneComponent::AddChild(CSceneComponent* pChild)
{
	pChild->m_pParent = this;
	pChild->m_pTransform->m_pParent = m_pTransform;

	pChild->AddRef();

	m_vecChild.push_back(pChild);
	m_pTransform->m_vecChild.push_back(pChild->m_pTransform);

	pChild->InheritPos();
	pChild->InheritRot();
	pChild->InheritScale();

	return true;
}

bool CSceneComponent::DeleteChild(CSceneComponent* pChild)
{
	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecChild[i] == pChild)
		{
			Detach(pChild);

			std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin() + i;

			m_vecChild.erase(iter);

			std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin() + i;

			m_pTransform->m_vecChild.erase(iterTr);

			break;
		}
	}

	return true;
}

bool CSceneComponent::Detach(CSceneComponent* pChild)
{
	pChild->m_pParent = nullptr;

	pChild->m_pTransform->m_pParent = nullptr;

	pChild->Release();

	pChild->SetRelativePos(pChild->GetRelativePos());
	pChild->SetRelativeRot(pChild->GetRelativeRot());
	pChild->SetRelativeScale(pChild->GetRelativeScale());

	return true;
}

SCENE_COMPONENT_TYPE CSceneComponent::GetSceneComponentType() const
{
	return m_eSceneComponentType;
}

CSceneComponent* CSceneComponent::FindComponent(const std::string& strTag)
{
	if (GetName() == strTag)
	{
		AddRef();

		return this;
	}

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CSceneComponent* pCom = m_vecChild[i]->FindComponent(strTag);

		if (pCom)
			return pCom;
	}

	return nullptr;
}


SCENECOMPONENT_CLASS_TYPE CSceneComponent::GetSceneComponentClassType() const
{
	return m_eSceneComponentClassType;
}

void CSceneComponent::SetScene(CScene* pScene)
{
	m_pScene = pScene;

	m_pTransform->SetScene(pScene);

	size_t iSz = m_vecChild.size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecChild[i]->SetScene(pScene);
}

void CSceneComponent::SetSceneComType(SCENE_COMPONENT_TYPE eType)
{
	m_eSceneComponentType = eType;
}

CSceneComponent* CSceneComponent::GetParent() const
{
	return m_pParent;
}

void CSceneComponent::SetObj(CObj* pObj)
{
	m_pObj = pObj;

	size_t iSz = m_vecChild.size();

	for (size_t i = 0; i < iSz; ++i)
		m_vecChild[i]->SetObj(pObj);
}

void CSceneComponent::AddRenderState(const std::string& strKey)
{
	CRenderState* pState = GET_SINGLE(CRenderManager)->FindState(strKey);

	m_vecRenderState.push_back(pState);
}

CRenderState* CSceneComponent::FindRenderState(const std::string& strKey)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecRenderState[i]->GetTag() == strKey)
		{
			return m_vecRenderState[i];
		}
	}

	return nullptr;
}

void CSceneComponent::DeleteRenderState(const std::string& strKey)
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecRenderState[i]->GetTag() == strKey)
		{
			std::vector<CRenderState*>::iterator iter = m_vecRenderState.begin() + i;

			m_vecRenderState.erase(iter);

			return;
		}
	}
}

void CSceneComponent::SetState()
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->SetState();
	}
}

void CSceneComponent::ResetState()
{
	size_t iSz = m_vecRenderState.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecRenderState[i]->ResetState();
	}
}

CMesh* CSceneComponent::GetMesh() const
{
	if (m_pMesh)
		m_pMesh->AddRef();

	return m_pMesh;
}

void CSceneComponent::SetMesh(const std::string& strName)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = GET_SINGLE(CResourceManager)->FindMesh(strName);

	if (m_pMesh)
	{
		CMaterial* pMaterial = m_pMesh->GetMaterial();
		CMaterial* pClone = pMaterial->Clone();

		SAFE_RELEASE(pMaterial);

		m_pMaterial = pClone;

		SAFE_RELEASE(pClone);
	}
}

void CSceneComponent::SetMesh(CMesh* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
	{
		m_pMesh->AddRef();

		CMaterial* pMaterial = m_pMesh->GetMaterial();
		CMaterial* pClone = pMaterial->Clone();

		SAFE_RELEASE(pMaterial);

		SetMaterial(pClone);

		SAFE_RELEASE(pClone);
	}
}

CMaterial* CSceneComponent::GetMaterial() const
{
	if (m_pMaterial)
		m_pMaterial->AddRef();

	return m_pMaterial;
}

void CSceneComponent::SetMaterial(CMaterial* pMaterial)
{
	SAFE_RELEASE(m_pMaterial);

	m_pMaterial = pMaterial;

	if (m_pMaterial)
		m_pMaterial->AddRef();
}

bool CSceneComponent::IsInstanced() const
{
	return m_bInst;
}

void CSceneComponent::SetInstance(bool bInst)
{
	m_bInst = bInst;
}

void CSceneComponent::SetLayer(CLayer* pLayer)
{
	m_pLayer = pLayer;

	size_t iSz = m_vecChild.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		m_vecChild[i]->SetLayer(pLayer);
	}
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, const std::string& strTag, int iRegister, int iCount, unsigned int iType)
{
	m_pMaterial->SetTexture(eType, strTag, iRegister, iCount, iType);
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, CTexture* pTex, int iRegister, int iCount, unsigned int iType)
{
	m_pMaterial->SetTexture(eType, pTex, iRegister, iCount, iType);
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFileName, const std::string& strPathName, int iRegister, int iCount, unsigned int iType)
{
	m_pMaterial->SetTexture(eType, strTag, pFileName, strPathName, iRegister, iCount, iType);
}

void CSceneComponent::SetTextureFromFullPath(REGISTER_TYPE eType, const std::string& strTag, const TCHAR* pFullPath, int iRegister, int iCount, unsigned int iType)
{
	m_pMaterial->SetTextureFromFullPath(eType, strTag, pFullPath, iRegister, iCount, iType);
}

bool CSceneComponent::Init()
{
	m_pTransform->m_pScene = m_pScene;

	return true;
}

void CSceneComponent::Start()
{
	m_bStart = true;

	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();
}

void CSceneComponent::Update(float fTime)
{
	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();
	std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			m_pTransform->m_vecChild.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->Update(fTime);
		}

		++iter;
		++iterTr;
	}

	m_pTransform->Update(fTime);
}

void CSceneComponent::PostUpdate(float fTime)
{
	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();
	std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			m_pTransform->m_vecChild.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PostUpdate(fTime);
		}

		++iter;
		++iterTr;
	}

	m_pTransform->PostUpdate(fTime);
}

void CSceneComponent::Collision(float fTime)
{
	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();
	std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			m_pTransform->m_vecChild.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->Collision(fTime);
		}

		++iter;
		++iterTr;
	}
}

void CSceneComponent::PreRender(float fTime)
{
	GET_SINGLE(CRenderManager)->AddComponent(this);

	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();
	std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			m_pTransform->m_vecChild.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PreRender(fTime);
		}

		++iter;
		++iterTr;
	}
}

void CSceneComponent::Render(float fTime)
{
	m_pTransform->SetTransform();

	if(m_pMaterial)
		m_pMaterial->SetMaterial();
}

void CSceneComponent::PostRender(float fTime)
{
	std::vector<CSceneComponent*>::iterator iter = m_vecChild.begin();
	std::vector<CSceneComponent*>::iterator iterEnd = m_vecChild.end();
	std::vector<CTransform*>::iterator iterTr = m_pTransform->m_vecChild.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_vecChild.erase(iter);
			iterEnd = m_vecChild.end();
			m_pTransform->m_vecChild.erase(iterTr);
			continue;
		}

		else if ((*iter)->IsEnable())
		{
			(*iter)->PostRender(fTime);
		}

		++iter;
		++iterTr;
	}
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	m_pTransform->Save(pFile);

	fwrite(&m_eSceneComponentType, 4, 1, pFile);
	fwrite(&m_eSceneComponentClassType, 4, 1, pFile);


}

void CSceneComponent::Load(FILE* pFile)
{
	CComponent::Load(pFile);

	if (m_pParent)
		m_pTransform->m_pParent = m_pParent->m_pTransform;

	m_pTransform->m_pOwner = this;
	m_pTransform->m_pScene = m_pScene;

	m_pTransform->Load(pFile);

	fread(&m_eSceneComponentType, 4, 1, pFile);
	fread(&m_eSceneComponentClassType, 4, 1, pFile);

	CMesh* pMesh = (CMesh*)GET_SINGLE(CResourceManager)->GetDefaultMesh();

	SetMesh(pMesh);

	SAFE_RELEASE(pMesh);
}

void CSceneComponent::SetInheritScale(bool bInherit)
{
	m_pTransform->SetInheritScale(bInherit);
}

void CSceneComponent::SetInheritRotX(bool bInherit)
{
	m_pTransform->SetInheritRotX(bInherit);
}

void CSceneComponent::SetInheritRotY(bool bInherit)
{
	m_pTransform->SetInheritRotY(bInherit);
}

void CSceneComponent::SetInheritRotZ(bool bInherit)
{
	m_pTransform->SetInheritRotZ(bInherit);
}

void CSceneComponent::InheritScale()
{
	m_pTransform->InheritScale();
}

void CSceneComponent::InheritRot()
{
	m_pTransform->InheritRot();
}

void CSceneComponent::InheritPos()
{
	m_pTransform->InheritPos();
}

void CSceneComponent::SetRelativeScale(const Vector3& v)
{
	m_pTransform->SetRelativeScale(v);
}

void CSceneComponent::SetRelativeScale(float x, float y, float z)
{
	m_pTransform->SetRelativeScale(x, y, z);
}

void CSceneComponent::SetRelativePos(const Vector3 & v)
{
	m_pTransform->SetRelativePos(v);
}

void CSceneComponent::SetRelativePos(float x, float y, float z)
{
	m_pTransform->SetRelativePos(x, y, z);
}

void CSceneComponent::SetRelativeRot(const Vector3 & v)
{
	m_pTransform->SetRelativeRot(v);
}

void CSceneComponent::SetRelativeRot(float x, float y, float z)
{
	m_pTransform->SetRelativeRot(x, y, z);
}

void CSceneComponent::SetRelativeRotX(float x)
{
	m_pTransform->SetRelativeRotX(x);
}

void CSceneComponent::SetRelativeRotY(float y)
{
	m_pTransform->SetRelativeRotY(y);
}

void CSceneComponent::SetRelativeRotZ(float z)
{
	m_pTransform->SetRelativeRotZ(z);
}

void CSceneComponent::AddRelativeScale(const Vector3 & v)
{
	m_pTransform->AddRelativeScale(v);
}

void CSceneComponent::AddRelativeScale(float x, float y, float z)
{
	m_pTransform->AddRelativeScale(x, y, z);
}

void CSceneComponent::AddRelativePos(const Vector3 & v)
{
	m_pTransform->AddRelativePos(v);
}

void CSceneComponent::AddRelativePos(float x, float y, float z)
{
	m_pTransform->AddRelativePos(x, y, z);
}

void CSceneComponent::AddRelativeRot(const Vector3 & v)
{
	m_pTransform->AddRelativeRot(v);
}

void CSceneComponent::AddRelativeRot(float x, float y, float z)
{
	m_pTransform->AddRelativeRot(x, y, z);
}

void CSceneComponent::AddRelativeRotX(float x)
{
	m_pTransform->AddRelativeRotX(x);
}

void CSceneComponent::AddRelativeRotY(float y)
{
	m_pTransform->AddRelativeRotY(y);
}

void CSceneComponent::AddRelativeRotZ(float z)
{
	m_pTransform->AddRelativeRotZ(z);
}

Vector3 CSceneComponent::GetVelocityScale() const
{
	return m_pTransform->GetVelocityScale();
}

Vector3 CSceneComponent::GetVelocityRot() const
{
	return m_pTransform->GetVelocityRot();
}

Vector3 CSceneComponent::GetVelocity() const
{
	return m_pTransform->GetVelocity();
}

float CSceneComponent::GetVelocityAmt() const
{
	return m_pTransform->GetVelocityAmt();
}

Vector3 CSceneComponent::GetRelativeScale() const
{
	return m_pTransform->GetRelativeScale();
}

Vector3 CSceneComponent::GetRelativeRot() const
{
	return m_pTransform->GetRelativeRot();
}

Vector3 CSceneComponent::GetRelativePos() const
{
	return m_pTransform->GetRelativePos();
}

Vector3 CSceneComponent::GetRelativeAxis(WORLD_AXIS axis) const
{
	return m_pTransform->GetRelativeAxis(axis);
}

Vector3 CSceneComponent::GetWorldScale() const
{
	return m_pTransform->GetWorldScale();
}

Vector3 CSceneComponent::GetWorldRot() const
{
	return m_pTransform->GetWorldRot();
}

Vector3 CSceneComponent::GetWorldPos() const
{
	return m_pTransform->GetWorldPos();
}

Vector3 CSceneComponent::GetWorldAxis(WORLD_AXIS axis) const
{
	return m_pTransform->GetWorldAxis(axis);
}

Vector3 CSceneComponent::GetPivot() const
{
	return m_pTransform->GetPivot();
}

Vector3 CSceneComponent::GetMeshSize() const
{
	return m_pTransform->GetMeshSize();
}

Matrix CSceneComponent::GetMatScale() const
{
	return m_pTransform->GetMatScale();
}

Matrix CSceneComponent::GetMatRot() const
{
	return m_pTransform->GetMatRot();
}

Matrix CSceneComponent::GetMatPos() const
{
	return m_pTransform->GetMatPos();
}

Matrix CSceneComponent::GetMatWorld() const
{
	return m_pTransform->GetMatWorld();
}

void CSceneComponent::SetWorldScale(const Vector3& v)
{
	m_pTransform->SetWorldScale(v);
}

void CSceneComponent::SetWorldScale(float x, float y, float z)
{
	m_pTransform->SetWorldScale(x, y, z);
}

void CSceneComponent::SetWorldPos(const Vector3 & v)
{
	m_pTransform->SetWorldPos(v);
}

void CSceneComponent::SetWorldPos(float x, float y, float z)
{
	m_pTransform->SetWorldPos(x, y, z);
}

void CSceneComponent::SetWorldRot(const Vector3 & v)
{
	m_pTransform->SetWorldRot(v);
}

void CSceneComponent::SetWorldRot(float x, float y, float z)
{
	m_pTransform->SetWorldRot(x, y, z);
}

void CSceneComponent::SetWorldRotX(float x)
{
	m_pTransform->SetWorldRotX(x);
}

void CSceneComponent::SetWorldRotY(float y)
{
	m_pTransform->SetWorldRotY(y);
}

void CSceneComponent::SetWorldRotZ(float z)
{
	m_pTransform->SetWorldRotZ(z);
}

void CSceneComponent::AddWorldScale(const Vector3 & v)
{
	m_pTransform->AddWorldScale(v);
}

void CSceneComponent::AddWorldScale(float x, float y, float z)
{
	m_pTransform->AddWorldScale(x, y, z);
}

void CSceneComponent::AddWorldPos(const Vector3 & v)
{
	m_pTransform->AddWorldPos(v);
}

void CSceneComponent::AddWorldPos(float x, float y, float z)
{
	m_pTransform->AddWorldPos(x, y, z);
}

void CSceneComponent::AddWorldRot(const Vector3 & v)
{
	m_pTransform->AddWorldRot(v);
}

void CSceneComponent::AddWorldRot(float x, float y, float z)
{
	m_pTransform->AddWorldRot(x, y, z);
}

void CSceneComponent::AddWorldRotX(float x)
{
	m_pTransform->AddWorldRotX(x);
}

void CSceneComponent::AddWorldRotY(float y)
{
	m_pTransform->AddWorldRotY(y);
}

void CSceneComponent::AddWorldRotZ(float z)
{
	m_pTransform->AddWorldRotZ(z);
}

void CSceneComponent::SetPivot(const Vector3 & v)
{
	m_pTransform->SetPivot(v);
}

void CSceneComponent::SetPivot(float x, float y, float z)
{
	m_pTransform->SetPivot(x, y, z);
}

void CSceneComponent::SetMeshSize(const Vector3 & v)
{
	m_pTransform->SetMeshSize(v);
}

void CSceneComponent::GetAllComponentName(std::vector<Hierarchy>& vecHierarchy)
{
	Hierarchy tHier;

	tHier.strName = GetName();

	if (m_pParent)
		tHier.strParent = m_pParent->GetName();

	vecHierarchy.push_back(tHier);

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->GetAllComponentName(vecHierarchy);
}

void CSceneComponent::GetAllComponent(std::vector<CSceneComponent*>& vecCom)
{
	vecCom.push_back(this);

	size_t iSize = m_vecChild.size();

	for (size_t i = 0; i < iSize; ++i)
		m_vecChild[i]->GetAllComponent(vecCom);
}
