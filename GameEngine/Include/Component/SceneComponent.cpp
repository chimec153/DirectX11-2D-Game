#include "SceneComponent.h"
#include "Transform.h"
#include "../Resource/Material.h"
#include "../Resource/Mesh.h"
#include "../Resource/ResourceManager.h"
#include "../Render/RenderManager.h"
#include "../Render/RenderState.h"
#include "Renderer.h"
#include "../PathManager.h"
#include "../Layer.h"
#include "../Scene/Scene.h"
#include "../Render/PostProcess.h"
#include "../Camera/CameraManager.h"
#include "Camera.h"

CSceneComponent::CSceneComponent()	:
	m_pParent(nullptr),
	m_eSceneComponentType(SCENE_COMPONENT_TYPE::SCT_2D),
	m_eSceneComponentClassType(SCENECOMPONENT_CLASS_TYPE::SCT_MESH2D),
	m_bInst(false)
	, m_bPostProcess(false)
	, m_pPostProcess(nullptr)
	, m_pInstancing(nullptr)
	, m_bInFrustum(false)
	, m_bCulling(true)
{
	m_eType = COMPONENT_TYPE::CT_SCENE;

	m_pTransform = new CTransform;

	m_pTransform->m_pOwner = this;

	m_pRenderer = new CRenderer;

	m_pRenderer->m_pOwner = this;
}

CSceneComponent::CSceneComponent(const CSceneComponent& com)	:
	CComponent(com)
	, m_bPostProcess(com.m_bPostProcess)
	, m_pPostProcess(com.m_pPostProcess)
	, m_pInstancing(nullptr)
	, m_bInFrustum(com.m_bInFrustum)
	, m_bCulling(com.m_bCulling)
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
	
	m_ChildList.clear();

	std::list<CSceneComponent*>::const_iterator iter = com.m_ChildList.begin();
	std::list<CSceneComponent*>::const_iterator iterEnd = com.m_ChildList.end();

	for (;iter!=iterEnd;++iter)
	{
		CSceneComponent* pComponent = (*iter)->Clone();

		pComponent->m_pScene = m_pScene;
		pComponent->m_pObj = m_pObj;
		pComponent->m_pParent = this;
		pComponent->m_pTransform->m_pParent = m_pTransform;
		pComponent->m_pTransform->m_pOwner = pComponent;
		pComponent->m_pLayer = (*iter)->m_pLayer;

		m_ChildList.push_back(pComponent);
		if (m_pTransform)
		{
			m_pTransform->m_ChildList.push_back(pComponent->m_pTransform);
		}

		pComponent->InheritPos();
		pComponent->InheritRot();
		pComponent->InheritScale();
	}

	m_bInst = com.m_bInst;

	if (com.m_pRenderer)
		m_pRenderer = com.m_pRenderer->Clone();

	else
		m_pRenderer = nullptr;

	if (m_pPostProcess)
	{
		m_pPostProcess = m_pPostProcess->Clone();
	}
}

CSceneComponent::~CSceneComponent()
{
	SAFE_DELETE(m_pTransform);
	SAFE_RELEASE_VECLIST(m_ChildList);
	SAFE_RELEASE(m_pRenderer);
	SAFE_RELEASE(m_pPostProcess);
}

bool CSceneComponent::AddChild(CSceneComponent* pChild)
{
	pChild->m_pParent = this;
	pChild->m_pTransform->m_pParent = m_pTransform;

	pChild->AddRef();

	m_ChildList.push_back(pChild);
	m_pTransform->m_ChildList.push_back(pChild->m_pTransform);

	pChild->InheritPos();
	pChild->InheritRot();
	pChild->InheritScale();

	return true;
}

bool CSceneComponent::DeleteChild(CSceneComponent* pChild)
{
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (;iter!=iterEnd;++iter,++iterTr)
	{
		if (*iter == pChild)
		{
			Detach(pChild);

			m_ChildList.erase(iter);

			m_pTransform->m_ChildList.erase(iterTr);

			break;
		}
	}

	return true;
}

bool CSceneComponent::Detach(CSceneComponent* pChild)
{
	pChild->m_pParent = nullptr;

	pChild->m_pTransform->m_pParent = nullptr;

	pChild->SetRelativePos(pChild->GetRelativePos());
	pChild->SetRelativeRot(pChild->GetRelativeRot());
	pChild->SetRelativeScale(pChild->GetRelativeScale());

	pChild->Release();

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

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (;iter!=iterEnd;++iter)
	{
		CSceneComponent* pCom = (*iter)->FindComponent(strTag);

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

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetScene(pScene);
	}
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

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->SetObj(pObj);
}

void CSceneComponent::AddRenderState(const std::string& strKey)
{
	if (m_pRenderer)
		m_pRenderer->AddRenderState(strKey);
}

CRenderState* CSceneComponent::FindRenderState(const std::string& strKey)
{
	if (m_pRenderer)
	{
		return m_pRenderer->FindState(strKey);
	}

	return nullptr;
}

void CSceneComponent::DeleteRenderState(const std::string& strKey)
{
	if (m_pRenderer)
	{
		m_pRenderer->DeleteRenderState(strKey);
	}
}

void CSceneComponent::SetState()
{
	if (m_pRenderer)
	{
		m_pRenderer->SetState();
	}
}

void CSceneComponent::ResetState()
{
	if (m_pRenderer)
	{
		m_pRenderer->ResetState();
	}
}

CMesh* CSceneComponent::GetMesh() const
{
	if (m_pRenderer)
		return m_pRenderer->GetMesh();

	else
	{
		return nullptr;
	}
}

void CSceneComponent::SetMesh(const std::string& strName)
{
	if (m_pRenderer)
		m_pRenderer->SetMesh(strName);
}

void CSceneComponent::SetMesh(CMesh* pMesh)
{
	if (m_pRenderer)
		m_pRenderer->SetMesh(pMesh);
}

CMaterial* CSceneComponent::GetMaterial(size_t iContainer, size_t iSubset) const
{
	if (m_pRenderer)
		return m_pRenderer->GetMaterial(iContainer, iSubset);

	else
	{
		return nullptr;
	}
}

void CSceneComponent::SetMaterial(CMaterial* pMaterial, size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
		m_pRenderer->SetMaterial(pMaterial, iContainer, iSubset);
}

void CSceneComponent::SetMaterial(const std::string& strMtrl, size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
		m_pRenderer->SetMaterial(strMtrl,iContainer, iSubset);
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
	if(!m_pLayer)
		m_pLayer = pLayer;


	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetLayer(pLayer);
	}
}

CRenderer* CSceneComponent::GetRenderer() const
{
	if (m_pRenderer)
		m_pRenderer->AddRef();

	return m_pRenderer;
}

CTransform* CSceneComponent::GetTransform() const
{
	return m_pTransform;
}

void CSceneComponent::SetShader(const std::string& strShader, size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetShader(strShader,iContainer, iSubset);
	}
}

void CSceneComponent::SetShader(CShader* pShader, size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetShader(pShader, iContainer, iSubset);
	}
}

CShader* CSceneComponent::GetShader(size_t iContainer, size_t iSubset) const
{
	if (m_pRenderer)
		return m_pRenderer->GetShader(iContainer, iSubset);

	return nullptr;
}

bool CSceneComponent::IsPostProcess() const
{
	return m_bPostProcess;
}

CPostProcess* CSceneComponent::GetPostProcess() const
{
	return m_pPostProcess;
}

void CSceneComponent::SetInstIndex(int iIndex)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetInstCount(iIndex);
	}
}

void CSceneComponent::SetInstancing(CRenderInstance* pInst)
{
	m_pInstancing = pInst;
}

CRenderInstance* CSceneComponent::GetInstancing() const
{
	return m_pInstancing;
}

bool CSceneComponent::IsInFrustum() const
{
	return m_bInFrustum;
}

void CSceneComponent::SetCulling(bool bCull)
{
	m_bCulling = bCull;
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, const std::string& strTag, 
	int iRegister, int iCount, unsigned int iType,
	size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetTexture(eType, strTag, iRegister, iCount, iType,
			iContainer, iSubset);
	}
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, CTexture* pTex, 
	int iRegister, int iCount, unsigned int iType,
	size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetTexture(eType, pTex, iRegister, iCount, iType,
			iContainer, iSubset);
	}
}

void CSceneComponent::SetTexture(REGISTER_TYPE eType, const std::string& strTag, 
	const TCHAR* pFileName, const std::string& strPathName, int iRegister, 
	int iCount, unsigned int iType,
	size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetTexture(eType, strTag, pFileName, strPathName, iRegister, 
			iCount, iType,
			iContainer, iSubset);
	}
}

void CSceneComponent::SetTextureFromFullPath(REGISTER_TYPE eType, 
	const std::string& strTag, const TCHAR* pFullPath, int iRegister, 
	int iCount, unsigned int iType,
	size_t iContainer, size_t iSubset)
{
	if (m_pRenderer)
	{
		m_pRenderer->SetTexture(eType, strTag, iRegister, iCount, iType,
			iContainer, iSubset);
	}
}

bool CSceneComponent::Init()
{
	m_pTransform->m_pScene = m_pScene;
	m_pRenderer->SetName(GetName());

	return true;
}

bool CSceneComponent::Init(const char* pFileName, const std::string& strPathKey)
{
	char pFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindMultibytePath(strPathKey);

	if (pPath)
		strcpy_s(pFullPath, pPath);

	strcat_s(pFullPath, pFileName);

	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rt");

	if (pFile)
	{
		Init(pFile);

		fclose(pFile);
	}

	return true;
}

bool CSceneComponent::Init(FILE* pFile)
{
	std::vector<VertexColor> vecVertex;
	std::vector<Vector2>	vecUV;
	std::vector<unsigned short>	vecIdx;
	std::vector<VertexColor> vecVertexUV;

	int iVt = 0;

	while (true)
	{
		char cLine[1024] = {};

		fgets(cLine, 1024, pFile);

		if (strcmp(cLine, "") == 0)
		{
			break;
		}

		char* pContext = nullptr;

		char* pResult = strtok_s(cLine, " ", &pContext);

		if (strcmp(pResult, "v") == 0)
		{
			VertexColor tVertex;

			pResult = strtok_s(nullptr, " ", &pContext);

			tVertex.vPos.x = (float)atof(pResult);

			pResult = strtok_s(nullptr, " ", &pContext);

			tVertex.vPos.y = (float)atof(pResult);

			pResult = strtok_s(nullptr, " ", &pContext);

			tVertex.vPos.z = (float)atof(pResult);

			vecVertex.push_back(tVertex);
		}

		else if (strcmp(pResult, "vt") == 0)
		{
			pResult = strtok_s(nullptr, " ", &pContext);

			float fU = (float)atof(pResult);

			pResult = strtok_s(nullptr, " ", &pContext);

			float fV = (float)atof(pResult);

			vecUV.push_back(Vector2(fU, fV));
		}
		else if (strcmp(pResult, "f") == 0)
		{
			int vecVt[3] = {};
			int vecTex[3] = {};

			for (int i = 0; i < 3; ++i)
			{
				pResult = strtok_s(nullptr, " ", &pContext);

				vecVt[i] = (int)atof(pResult);

				char* pResult2 = strtok_s(nullptr, "/", &pResult);

				if (strcmp(pResult, "") == 0)
				{
					vecIdx.push_back(vecVt[i] - 1);
					continue;
				}

				else
				{
					vecTex[i] = (int)atof(pResult);

					bool bCheck = true;

					size_t iSz = vecVertexUV.size();

					for (size_t j = 0; j < iSz; ++j)
					{
						if (vecVertexUV[j].vPos == vecVertex[vecVt[i] - 1].vPos &&
							vecVertexUV[j].vUV == vecUV[vecTex[i] - 1])
						{
							bCheck = false;
							vecIdx.push_back((int)j);
							break;
						}
					}

					if (bCheck)
					{
						vecIdx.push_back((int)vecVertexUV.size());

						VertexColor vColor = {};

						vColor.vPos = vecVertex[vecVt[i] - 1].vPos;
						vColor.vUV = vecUV[vecTex[i] - 1];

						vecVertexUV.push_back(vColor);
					}
				}
			}
		}
	}

	if (!vecVertexUV.empty())
	{
		GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_2D, GetName(),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (void*)&vecVertexUV[0], (int)sizeof(VertexColor),
			(int)vecVertexUV.size(), D3D11_USAGE_DEFAULT, (void*)&vecIdx[0], 2, (int)vecIdx.size(), D3D11_USAGE_DEFAULT,
			DXGI_FORMAT_R16_UINT);
	}

	else
	{
		GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_2D, GetName(),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, (void*)&vecVertex[0], (int)sizeof(VertexColor),
			(int)vecVertex.size(), D3D11_USAGE_DEFAULT, (void*)&vecIdx[0], 2, (int)vecIdx.size(), D3D11_USAGE_DEFAULT,
			DXGI_FORMAT_R16_UINT);
	}

	SetMesh(GetName());

	return true;
}

void CSceneComponent::Start()
{
	m_bStart = true;

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();

	if (m_pRenderer)
		m_pRenderer->Start();

	m_pTransform->Start();
}

void CSceneComponent::Update(float fTime)
{
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			m_pTransform->m_ChildList.erase(iterTr);
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

	if (m_pRenderer)
	{
		m_pRenderer->Update(fTime);
	}
}

void CSceneComponent::PostUpdate(float fTime)
{
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			m_pTransform->m_ChildList.erase(iterTr);
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
	if (m_pRenderer)
	{
		m_pRenderer->PostUpdate(fTime);
	}
}

void CSceneComponent::Collision(float fTime)
{
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			m_pTransform->m_ChildList.erase(iterTr);
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
	if (m_bInFrustum)
	{
		GET_SINGLE(CRenderManager)->AddComponent(this);
	}
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			m_pTransform->m_ChildList.erase(iterTr);
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

	if (m_pRenderer)
	{
		m_pRenderer->Render(fTime);
	}
}

void CSceneComponent::PostRender(float fTime)
{
	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();
	std::list<CTransform*>::iterator iterTr = m_pTransform->m_ChildList.begin();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			Detach((*iter));
			iter = m_ChildList.erase(iter);
			iterEnd = m_ChildList.end();
			m_pTransform->m_ChildList.erase(iterTr);
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

void CSceneComponent::RenderShadow(float fTime)
{
	m_pTransform->SetShadow();

	if (m_pRenderer)
	{
		m_pRenderer->RenderShadow(fTime);
	}
}

void CSceneComponent::Save(FILE* pFile)
{
	CComponent::Save(pFile);

	m_pTransform->Save(pFile);

	fwrite(&m_eSceneComponentType, 4, 1, pFile);
	fwrite(&m_eSceneComponentClassType, 4, 1, pFile);

	m_pRenderer->Save(pFile);

	fwrite(&m_bInst, 1, 1, pFile);
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

	m_pRenderer->Load(pFile);

	fread(&m_bInst, 1, 1, pFile);
}

void CSceneComponent::CheckFrustum()
{
	if (m_bCulling)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		const Vector4& _vPos = GetMatWorld().v[3];
		Vector3 vPos = Vector3(_vPos.x, _vPos.y, _vPos.z);

		const Vector3& vMeshSize = GetMeshSize();

		const Vector3& vScale = GetWorldScale();

		Vector3 __vPos = vMeshSize * vScale / 2.f;

		float fRadius = __vPos.Length();

		m_bInFrustum = pCam->IsInFrustum(__vPos + vPos, fRadius);

		SAFE_RELEASE(pCam);
	}
	else
	{
		m_bInFrustum = true;
	}

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (;iter!=iterEnd;++iter)
	{
		(*iter)->CheckFrustum();
	}
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

void CSceneComponent::SetInheritPos(bool bIn)
{
	m_pTransform->SetInheritPos(bIn);
}

void CSceneComponent::SetUpdateScale(bool bScale)
{
	m_pTransform->SetUpdateScale(bScale);
}

void CSceneComponent::SetUpdateRot(bool bRot)
{
	m_pTransform->SetUpdateRot(bRot);
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

const Vector3& CSceneComponent::GetVelocityScale() const
{
	return m_pTransform->GetVelocityScale();
}

const Vector3& CSceneComponent::GetVelocityRot() const
{
	return m_pTransform->GetVelocityRot();
}

const Vector3& CSceneComponent::GetVelocity() const
{
	return m_pTransform->GetVelocity();
}

float CSceneComponent::GetVelocityAmt() const
{
	return m_pTransform->GetVelocityAmt();
}

const Vector3& CSceneComponent::GetRelativeScale() const
{
	return m_pTransform->GetRelativeScale();
}

const Vector3& CSceneComponent::GetRelativeRot() const
{
	return m_pTransform->GetRelativeRot();
}

const Vector3& CSceneComponent::GetRelativePos() const
{
	return m_pTransform->GetRelativePos();
}

const Vector3& CSceneComponent::GetRelativeAxis(WORLD_AXIS axis) const
{
	return m_pTransform->GetRelativeAxis(axis);
}

const Vector3& CSceneComponent::GetWorldScale() const
{
	return m_pTransform->GetWorldScale();
}

const Vector3& CSceneComponent::GetWorldRot() const
{
	return m_pTransform->GetWorldRot();
}

const Vector3& CSceneComponent::GetWorldPos() const
{
	return m_pTransform->GetWorldPos();
}

const Vector3& CSceneComponent::GetWorldAxis(WORLD_AXIS axis) const
{
	return m_pTransform->GetWorldAxis(axis);
}

const Vector3& CSceneComponent::GetPivot() const
{
	return m_pTransform->GetPivot();
}

const Vector3& CSceneComponent::GetMeshSize() const
{
	return m_pTransform->GetMeshSize();
}

const Matrix& CSceneComponent::GetMatScale() const
{
	return m_pTransform->GetMatScale();
}

const Matrix& CSceneComponent::GetMatRot() const
{
	return m_pTransform->GetMatRot();
}

const Matrix& CSceneComponent::GetMatPos() const
{
	return m_pTransform->GetMatPos();
}

const Matrix& CSceneComponent::GetMatWorld() const
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

void CSceneComponent::Slerp(const Vector4& p, const Vector4& q, float s)
{
	m_pTransform->Slerp(p,q,s);
}

void CSceneComponent::Slerp(const Vector4& q, float s)
{
	m_pTransform->Slerp( q, s);
}

void CSceneComponent::SetQuaternionRot(const Vector4& vAxis, float fAngle)
{
	m_pTransform->SetQuaternionRot(vAxis, fAngle);
}

void CSceneComponent::AddQuaternionRot(const Vector4& vAxis, float fAngle)
{
	m_pTransform->AddQuaternionRot(vAxis, fAngle);
}

void CSceneComponent::SetQuaternionRotNorm(const Vector4& vAxis, float fAngle)
{
	m_pTransform->SetQuaternionRotNorm(vAxis, fAngle);
}

void CSceneComponent::AddQuaternionRotNorm(const Vector4& vAxis, float fAngle)
{
	m_pTransform->AddQuaternionRotNorm(vAxis, fAngle);
}

void CSceneComponent::GetAllComponentName(std::vector<Hierarchy>& vecHierarchy)
{
	Hierarchy tHier;

	tHier.strName = GetName();

	if (m_pParent)
		tHier.strParent = m_pParent->GetName();

	vecHierarchy.push_back(tHier);

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->GetAllComponentName(vecHierarchy);
	}
}

void CSceneComponent::GetAllComponent(std::vector<CSceneComponent*>& vecCom)
{
	vecCom.push_back(this);

	std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
	std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->GetAllComponent(vecCom);
	}
}

void CSceneComponent::SpawnWindow()
{
	CRef::SpawnWindow();

	const char* pName = GetName().c_str();

	if (!strcmp(pName, ""))
	{
		pName = "None";
	}

	if (ImGui::Begin(pName))
	{
		if (m_pRenderer)
			m_pRenderer->SpawnWindow();

		if (m_pTransform)
			m_pTransform->SpawnWindow();
	}
	ImGui::End();
}

void CSceneComponent::ShowNode()
{
	const char* pName = GetName().c_str();

	if (!strcmp(pName, ""))
	{
		pName = "None";
	}

	if (ImGui::TreeNode(pName))
	{
		SpawnWindow();

		std::list<CSceneComponent*>::iterator iter = m_ChildList.begin();
		std::list<CSceneComponent*>::iterator iterEnd = m_ChildList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->ShowNode();
		}

		ImGui::TreePop();
	}
}
