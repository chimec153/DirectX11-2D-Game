#include "Layer.h"
#include "Object/Obj.h"
#include "Instancing.h"
#include "Render/RenderManager.h"
#include "UI/UIControl.h"
#include "Tile/TileMap.h"
#include "Component/Renderer.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"

CLayer::CLayer() :
	m_iZOrder(0),
	m_bStart(false),
	m_pScene(nullptr),
	m_b2D(false),
	m_bUI(false),
	m_pTileInst(nullptr),
	m_pTileMap(nullptr)
{
}

CLayer::CLayer(const CLayer& layer)	:
	CRef(layer)
{
	m_iZOrder = layer.m_iZOrder;
	m_bStart = layer.m_bStart;
	m_pScene = layer.m_pScene;

	m_ObjList.clear();

	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		CObj* pObj = (*iter)->Clone();

		m_ObjList.push_back(pObj);
	}

	m_b2D = layer.m_b2D;
	m_bUI = layer.m_bUI;

	if (layer.m_pTileMap)
		m_pTileMap = layer.m_pTileMap->Clone();

	else
		m_pTileMap = nullptr;

	m_pTileInst = layer.m_pTileInst;
}

CLayer::~CLayer()
{
	SAFE_RELEASE_VECLIST(m_ObjList);
	SAFE_DELETE(m_pTileInst);
	SAFE_RELEASE(m_pTileMap);
}

void CLayer::SetZOrder(int iZ)
{
	m_iZOrder = iZ;
}

int CLayer::GetZOrder() const
{
	return m_iZOrder;
}

CObj* CLayer::FindObj(const std::string& strTag) const
{
	std::list<CObj*>::const_iterator iter = m_ObjList.begin();
	std::list<CObj*>::const_iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter))
		{
			if ((*iter)->GetName() == strTag)
			{
				(*iter)->AddRef();

				return (*iter);
			}
		}
	}

	return nullptr;
}

bool CLayer::IsStart() const
{
	return m_bStart;
}

void CLayer::AddObj(CObj* pObj)
{
	pObj->AddRef();
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);

	m_ObjList.push_back(pObj);
}

void CLayer::AddInst(CInstancing* pInst)
{
	m_vecInst.push_back(pInst);
}

void CLayer::AddCom(CSceneComponent* pCom)
{

	m_vecCom.push_back(pCom);
}

void CLayer::Clear()
{
	m_vecCom.clear();
}

void CLayer::Set2D()
{
	m_b2D = true;
}

void CLayer::SetUI()
{
	m_bUI = true;
}

CInstancing* CLayer::GetTileInst() const
{
	return m_pTileInst;
}

void CLayer::SetTileInst(CInstancing* pInst)
{
	SAFE_DELETE(m_pTileInst);

	m_pTileInst = pInst;

	if (m_pTileInst)
		m_pTileInst->SetLayer(this);
}

void CLayer::SetTileMap(CTileMap* pMap)
{
	m_pTileMap = pMap;

	if (m_pTileMap)
		m_pTileMap->AddRef();
}

CTileMap* CLayer::GetTileMap() const
{
	if (m_pTileMap)
		m_pTileMap->AddRef();

	return m_pTileMap;
}

void CLayer::SetFontPos(const Vector3& vPos)
{
	if(m_pTileInst)
		m_pTileInst->SetFontPos(vPos);
}

void CLayer::AddTileInstState(const std::string& strKey)
{
	if (m_pTileInst)
		m_pTileInst->AddState(strKey);
}

CScene* CLayer::GetScene() const
{
	return m_pScene;
}

void CLayer::DeleteObject(const std::string& strTag)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strTag)
		{
			(*iter)->Release();
			m_ObjList.erase(iter);
			return;
		}
	}
}

bool CLayer::Init()
{
	return true;
}

void CLayer::Start()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	m_bStart = true;
}

void CLayer::Input(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Input(fTime);
		++iter;
	}
}

void CLayer::Update(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		else if (!(*iter)->IsStart())
		{
			(*iter)->Start();
		}

		(*iter)->Update(fTime);
		++iter;
	}
}

void CLayer::PostUpdate(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(fTime);
		++iter;
	}
}

void CLayer::Collision(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Collision(fTime);
		++iter;
	}
}

void CLayer::PreRender(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PreRender(fTime);
		++iter;
	}

	GET_SINGLE(CRenderManager)->AddLayer(this);
}

void CLayer::Render(float fTime)
{
	if (m_pTileInst)
	{
		m_pTileInst->Update(fTime);
		m_pTileInst->Render(fTime);
		m_pTileInst->Clear();
	}

	size_t iSz = m_vecInst.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		if (m_vecInst[i]->GetCount() <= 6)
		{
			const std::vector<CSceneComponent*> vecCom = *m_vecInst[i]->GetComs();

			size_t iSize = vecCom.size();

			for (size_t j = 0; j < iSize; ++j)
			{
				m_vecCom.push_back(vecCom[j]);
			}

			continue;
		}

		m_vecInst[i]->Update(fTime);
		m_vecInst[i]->Render(fTime);
		m_vecInst[i]->Clear();
	}

	if (m_bUI)
	{
		std::sort(m_vecCom.begin(), m_vecCom.end(), &CLayer::SortZ);
	}

	size_t iComSz = m_vecCom.size();

	for (size_t i = 0; i < iComSz; ++i)
	{
		CRenderer* pRenderer = m_vecCom[i]->GetRenderer();

		pRenderer->SetState();

		m_vecCom[i]->Render(fTime);

		pRenderer->ResetState();

		SAFE_RELEASE(pRenderer);
	}
}

void CLayer::PostRender(float fTime)
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostRender(fTime);
		++iter;
	}
}

void CLayer::RenderShadow(float fTime)
{
	size_t iSize = m_vecCom.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		m_vecCom[i]->RenderShadow(fTime);
	}
}

void CLayer::Save(FILE* pFile)
{
	CRef::Save(pFile);

	fwrite(&m_iZOrder, 4, 1, pFile);
	fwrite(&m_bStart, 1, 1, pFile);

	int iSz = (int)m_ObjList.size();

	fwrite(&iSz, 4, 1, pFile);

	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		int iType = (*iter)->GetClassType();

		fwrite(&iType, 4, 1, pFile);
		(*iter)->Save(pFile);
	}
}

void CLayer::Load(FILE* pFile)
{
	CRef::Load(pFile);

	fread(&m_iZOrder, 4, 1, pFile);
	fread(&m_bStart, 1, 1, pFile);

	int iSz = 0;

	fread(&iSz, 4, 1, pFile);

	for (int i=0;i< iSz;++i)
	{
		int iType = 0;

		fread(&iType, 4, 1, pFile);

		CObj* pObj = m_EditorObjCreate(iType);

		pObj->Load(pFile);

		m_ObjList.push_back(pObj);
	}
}

void CLayer::CheckFrustum()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->CheckFrustum();
	}
}

bool CLayer::SortY(CObj* pSrc, CObj* pDst)
{
	return pSrc->GetWorldPos().y < pDst->GetWorldPos().y;
}

bool CLayer::SortYCom(CSceneComponent* pSrc, CSceneComponent* pDst)
{
	return pSrc->GetWorldPos().y < pDst->GetWorldPos().y;
}

bool CLayer::SortZ(CSceneComponent* pSrc, CSceneComponent* pDst)
{
	return ((CUIControl*)pSrc)->GetZOrder() > ((CUIControl*)pDst)->GetZOrder();
}

bool CLayer::SortZOrder(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	float fSrcZ = 0.f;
	float fDestZ = 0.f;

	Vector3 vSrcPos = pSrc->GetWorldPos();
	Vector3 vDestPos = pDest->GetWorldPos();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matVP = pCam->GetVP();

	SAFE_RELEASE(pCam);

	float fSrcW = vSrcPos.x * matVP[0][3] + vSrcPos.x * matVP[1][3]
		+vSrcPos.z * matVP[2][3] + matVP[3][3];

	Vector3 vSrcProjPos = vSrcPos.TransformCoord(matVP);

	fSrcZ = vSrcProjPos.z;// / fSrcW;

	float fDestW = vDestPos.x * matVP[0][3] + vDestPos.x * matVP[1][3]
		+ vDestPos.z * matVP[2][3] + matVP[3][3];

	Vector3 vDestProjPos = vDestPos.TransformCoord(matVP);

	fDestZ = vDestProjPos.z / fDestW;

	return fSrcZ < fDestZ;
}

bool CLayer::SortInvZOrder(CSceneComponent* pSrc, CSceneComponent* pDest)
{
	float fSrcZ = 0.f;
	float fDestZ = 0.f;

	Vector3 vSrcPos = pSrc->GetWorldPos();
	Vector3 vDestPos = pDest->GetWorldPos();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	Matrix matVP = pCam->GetVP();

	SAFE_RELEASE(pCam);

	float fSrcW = vSrcPos.x * matVP[0][3] + vSrcPos.x * matVP[1][3]
		+ vSrcPos.z * matVP[2][3] + matVP[3][3];

	Vector3 vSrcProjPos = vSrcPos.TransformCoord(matVP);

	fSrcZ = vSrcProjPos.z;// / fSrcW;

	float fDestW = vDestPos.x * matVP[0][3] + vDestPos.x * matVP[1][3]
		+ vDestPos.z * matVP[2][3] + matVP[3][3];

	Vector3 vDestProjPos = vDestPos.TransformCoord(matVP);

	fDestZ = vDestProjPos.z / fDestW;

	return fSrcZ > fDestZ;
}

void CLayer::SpawnWindow()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	size_t iSz = m_ObjList.size();
	char** strLayers = new char* [iSz];
	int i = 0;

	static int item = 0;
	for (; iter != iterEnd;++iter)
	{
		strLayers[i] = new char[256];

		strcpy_s(strLayers[i++], (*iter)->GetName().length() + 1, (*iter)->GetName().c_str());

		if (item == i - 1)
		{
			(*iter)->SpawnWindow();
		}

	}

	if (ImGui::Begin("Objects"))
	{
		ImGui::ListBox("Objects", &item, strLayers, (int)iSz);
	}
	ImGui::End();


	for (size_t i = 0; i < iSz; ++i)
	{
		delete[] strLayers[i];
	}

	delete[] strLayers;
}

void CLayer::CameraZSort(bool bInverse)
{
	if (m_vecCom.empty())
		return;

	if (!bInverse)
	{
		SortZOrder(*m_vecCom.begin(), *(--m_vecCom.end()));
	}
	else
	{
		SortInvZOrder(*m_vecCom.begin(), *(--m_vecCom.end()));
	}
}
