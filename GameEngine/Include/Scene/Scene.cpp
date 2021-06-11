#include "Scene.h"
#include "../Object/Obj.h"
#include "SceneCollision.h"
#include "../Layer.h"
#include "../Collision/CollisionManager.h"
#include "../Engine.h"

std::unordered_map<std::string, CObj*> CScene::m_mapProtoType[(int)SCENE_TYPE::SC_END];

CScene::CScene()	:
	m_bStart(false),
	m_eType(SCENE_TYPE::SC_CURRENT)
{
	m_pGameMode = new CGameMode;

	CreateLayer(TERRAIN_LAYER	, static_cast<int>(LAYER_TYPE::TERRAIN));
	CreateLayer(CUBEMAP_LAYER	, static_cast<int>(LAYER_TYPE::CUBEMAP));
	CreateLayer(ALBEDO_LAYER	, static_cast<int>(LAYER_TYPE::ALBEDO));
	CreateLayer(DECAL_LAYER		, static_cast<int>(LAYER_TYPE::DECAL));
	CreateLayer(PARTICLE_LAYER	, static_cast<int>(LAYER_TYPE::PARTICLE));
	CreateLayer(COLLIDER_LAYER	, static_cast<int>(LAYER_TYPE::COLLIDER));
	CreateLayer(ALPHA_LAYER		, static_cast<int>(LAYER_TYPE::ALPHA));
	CreateLayer(UI_LAYER		, static_cast<int>(LAYER_TYPE::UI));
}

CScene::~CScene()
{
	SAFE_RELEASE_VECLIST(m_LayerList);
	SAFE_DELETE(m_pGameMode);
}

SCENE_TYPE CScene::GetSceneType() const
{
	return m_eType;
}

void CScene::CreateLayer(const std::string& strKey, int iZ)
{
	CLayer* pLayer = new CLayer;

	pLayer->SetName(strKey);

	pLayer->m_pScene = this;

	if (!pLayer->Init())
	{
		SAFE_RELEASE(pLayer);
		return;
	}

	pLayer->SetZOrder(iZ);

	pLayer->m_EditorObjCreate = m_EditorObjCreate;

	m_LayerList.push_back(pLayer);

	if (m_LayerList.size() > 1)
		m_LayerList.sort(CScene::SortZ);
}

CLayer* CScene::FindLayer(const std::string& strKey)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == strKey)
			return (*iter);
	}

	return nullptr;
}

const std::list<class CLayer*>& CScene::GetLayerList() const
{
	return m_LayerList;
}

CGameMode* CScene::GetGameMode() const
{
	return m_pGameMode;
}

CObj* CScene::EditorCreate(int iType)
{
	return m_EditorObjCreate(iType);
}

CObj* CScene::FindProtoType(const std::string& strKey, SCENE_TYPE eType)
{
	std::unordered_map<std::string, class CObj*>::iterator iter = m_mapProtoType[(int)eType].find(strKey);

	if (iter == m_mapProtoType[(int)eType].end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}

void CScene::DelEditDeFunc()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->m_EditorDelete = nullptr;
	}
}

bool CScene::Init()
{

	return true;
}

void CScene::Start()
{
	std::list<CObj*>::iterator iter = m_ObjList.begin();
	std::list<CObj*>::iterator iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
		(*iter)->Start();
}

void CScene::Input(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
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

void CScene::Update(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
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

	if (!m_pGameMode->IsStart())
	{
		m_pGameMode->Start();
	}

	m_pGameMode->Update(fTime);
}

void CScene::PostUpdate(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
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

void CScene::Collision(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
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

	GET_SINGLE(CCollisionManager)->Collsion(fTime);
}

void CScene::PreRender(float fTime)
{
	m_pGameMode->PreRender(fTime);

	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
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

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		SpawnWindow();
	}
}

void CScene::Render(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();


	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Render(fTime);
		++iter;
	}

	m_pGameMode->Render(fTime);
}

void CScene::PostRender(float fTime)
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_LayerList.erase(iter);
			iterEnd = m_LayerList.end();
			continue;
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

void CScene::SaveFromFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (pFile)
	{
		int iType = m_pGameMode->GetGameModeClassType();

		fwrite(&iType, 4, 1, pFile);

		m_pGameMode->Save(pFile);

		int iSize = (int)m_LayerList.size();

		fwrite(&iSize, sizeof(int), 1, pFile);

		std::list<CLayer*>::iterator iter = m_LayerList.begin();
		std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Save(pFile);
		}

		fclose(pFile);
	}
}

void CScene::Save(const char* pFileName, const std::string& strRootPath)
{
}

void CScene::LoadFromFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (pFile)
	{
		int iType = 0;

		fread(&iType, 4, 1, pFile);

		SAFE_DELETE(m_pGameMode);

		m_pGameMode = m_EditorGameModeCreate(iType);

		m_pGameMode->m_pScene = this;

		m_pGameMode->Load(pFile);

		int iSize = 0;

		fread(&iSize, sizeof(int), 1, pFile);

		for (int i = 0; i < iSize; ++i)
		{
			CLayer* pLayer = new CLayer;

			pLayer->Load(pFile);

			m_LayerList.push_back(pLayer);
		}

		fclose(pFile);
	}
}

void CScene::Load(const char* pFileName, const std::string& strRootPath)
{
}

void CScene::CheckFrustum()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->CheckFrustum(); // ÆùÆ®Ä³½Ì
	}
}

bool CScene::SortZ(CLayer* pSrc, CLayer* pDst)
{
	return pSrc->GetZOrder() > pDst->GetZOrder();
}

void CScene::SortInstText()
{

	int i = 0;

	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetFontPos(Vector3(0.f, 20.f * i++, 0.f));
	}

}

void CScene::SpawnWindow()
{
	std::list<CLayer*>::iterator iter = m_LayerList.begin();
	std::list<CLayer*>::iterator iterEnd = m_LayerList.end();

	char* strLayers[32] = {};
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

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		if (ImGui::Begin("Layers"))
		{
			ImGui::ListBox("Layers", &item, strLayers, (int)m_LayerList.size());
		}
		ImGui::End();
	}

	size_t iSz = m_LayerList.size();

	for (size_t i = 0; i < iSz; ++i)
	{
		delete[] strLayers[i];
	}
}
