#include "GameMode.h"
#include "Object/Obj.h"
#include "Scene/Scene.h"
#include "Collision/CollisionManager.h"
#include "Component/Mesh2DComponent.h"

CGameMode::CGameMode()	:
	m_pScene(nullptr),
	m_pPlayer(nullptr),
	m_iGameModeClassType(0),
	m_bStart(false),
	m_pCubeMap(nullptr)
{	
}

CGameMode::~CGameMode()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pCubeMap);
}

void CGameMode::SetPlayer(CObj* pPlayer)
{
	SAFE_RELEASE(m_pPlayer);

	m_pPlayer = pPlayer;

	if (m_pPlayer)
		m_pPlayer->AddRef();
}

CObj* CGameMode::GetPlayer() const
{
	if (m_pPlayer)
		m_pPlayer->AddRef();

	return m_pPlayer;
}

int CGameMode::GetGameModeClassType() const
{
	return m_iGameModeClassType;
}

bool CGameMode::IsStart() const
{
	return m_bStart;
}

void CGameMode::SetCubeMapTexture(const std::string& strKey)
{
	CSceneComponent* pCube = m_pCubeMap->GetRootComponent();

	pCube->SetTexture(REGISTER_TYPE::RT_CUSTOM, strKey, 20);

	SAFE_RELEASE(pCube);
}

bool CGameMode::Init()
{
	m_pCubeMap = m_pScene->CreateObject<CObj>("CubeMap",
		m_pScene->FindLayer(CUBEMAP_LAYER),
		m_pScene->GetSceneType());

	CSceneComponent* pCubeMap = m_pCubeMap->CreateComponent<CMesh2DComponent>("CubeMap");

	m_pCubeMap->SetRootComponent(pCubeMap);

	pCubeMap->SetMesh("Sphere");
	pCubeMap->SetShader("CubeMapShader");
	pCubeMap->SetMaterial("Color");
	pCubeMap->SetWorldScale(100000.f, 100000.f, 100000.f);
	pCubeMap->SetTexture(REGISTER_TYPE::RT_CUSTOM, "Sky", 
		L"Sky\\Sky.dds", 
		TEXTURE_PATH, 20);
	pCubeMap->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_3D);
	pCubeMap->AddRenderState(CULL_FRONT);
	pCubeMap->AddRenderState(NO_DEPTH);

	SAFE_RELEASE(pCubeMap);

	return true;
}

void CGameMode::Start()
{
	m_bStart = true;
}

void CGameMode::Update(float fTime)
{
	if (m_pPlayer)
	{
		GET_SINGLE(CCollisionManager)->SetCenter(m_pPlayer->GetWorldPos());
	}
}

void CGameMode::PostUpdate(float fTime)
{
}

void CGameMode::Collision(float fTime)
{
}

void CGameMode::PreRender(float fTime)
{
}

void CGameMode::Render(float fTime)
{
}

void CGameMode::PostRender(float fTime)
{
}

void CGameMode::Save(FILE* pFile)
{
	fwrite(&m_iGameModeClassType, 4, 1, pFile);

	bool bPlayer = false;

	if (m_pPlayer)
		bPlayer = true;

	fwrite(&bPlayer, 1, 1, pFile);

	if (m_pPlayer)
	{
		int iLength = (int)m_pPlayer->GetName().length();

		fwrite(&iLength, 4, 1, pFile);
		fwrite(m_pPlayer->GetName().c_str(), 1, iLength, pFile);
	}
}

void CGameMode::Load(FILE* pFile)
{
	fread(&m_iGameModeClassType, 4, 1, pFile);

	bool bPlayer = false;

	fread(&bPlayer, 1, 1, pFile);

	if (bPlayer)
	{
		int iLength = 0;

		char strTag[256] = {};

		fread(&iLength, 4, 1, pFile);
		fread(strTag, 1, iLength, pFile);

		CLayer* pLayer = m_pScene->FindLayer("Default");

		m_pPlayer = pLayer->FindObj(strTag);
	}
}
