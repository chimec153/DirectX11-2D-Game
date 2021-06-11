#include "MainMode.h"
#include "Object/Player3D.h"
#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "Tile/TileMap.h"
#include "../GeometryGenerator.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Component/Light.h"
#include "Object/Monster3D.h"
#include "Resource/Animation.h"
#include "Component/Terrain.h"
#include "Component/Renderer.h"
#include "Component/ColliderTerrain.h"
#include "Component/Decal.h"
#include "Component/Particle.h"
#include "ParticleSystem.h"
//몬스터, 안개, 스킬
CMainMode::CMainMode()
{
}

CMainMode::~CMainMode()
{
}

bool CMainMode::Init()
{
	if (!CGameMode::Init())
		return false;

	if (!LoadTexture())
		return false;

	if (!GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_ANIMATION,
		"Player_Default", L"Player_Default.msh"))
		return false;

	if (!GET_SINGLE(CResourceManager)->LoadSkeleton("Player_Default",
		"Player_Default.bne"))
		return false;

	GET_SINGLE(CResourceManager)->LoadSequence("Player_Default",
		"Player_Default.sqc");

	if (!GET_SINGLE(CResourceManager)->LoadMesh(MESH_TYPE::MT_STATIC, 
		"Blade", TEXT("Blade.FBX")))
		return false;

	GeometryGenerator::loadFile();

	CPlayer3D* pPlayer = m_pScene->CreateObject<CPlayer3D>("Player", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

	CSceneComponent* pCom = pPlayer->GetRootComponent();

	SAFE_RELEASE(pCom);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			CMonster3D* pMon = m_pScene->CreateObject<CMonster3D>("test", m_pScene->FindLayer(ALBEDO_LAYER));

			pMon->SetWorldPos(i * 50.f, 5.f, j * 50.f);

			SAFE_RELEASE(pMon);
		}
	}

	SAFE_RELEASE(pPlayer);

	CObj* pGround = m_pScene->CreateObject<CObj>("Ground", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

	CMesh2DComponent* pGroundCom = pGround->CreateComponent<CMesh2DComponent>("Ground", m_pScene->FindLayer(ALBEDO_LAYER));

	pGroundCom->SetWorldScale(1000.f, 1000.f, 0.f);
	pGroundCom->SetPivot(0.5f, 0.5f, 0.f);
	CMaterial* pMtrl = pGroundCom->GetMaterial();
	pMtrl->SetDiffuseColor(0.1f, 0.6f, 0.1f, 1.f);
	pMtrl->SetAmbientColor(0.1f, 0.6f, 0.1f, 1.f);
	pMtrl->SetSpecularColor(0.1f, 0.6f, 0.1f, 1.f);
	SAFE_RELEASE(pMtrl);
	pGroundCom->SetWorldRotX(-90.f);
	pGroundCom->SetShader("Test");

	pGround->SetRootComponent(pGroundCom);

	SAFE_RELEASE(pGroundCom);

	SAFE_RELEASE(pGround);

	CObj* pLight = m_pScene->CreateObject<CObj>("Light", m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

	Light* pLightCom = Light::CreateLight<Light>("Light");

	pLight->AddComponent(pLightCom);

	pLight->SetQuaternionRot(Vector4(1.f, 0.f, 0.f, 0.f), 45.f);
	pLight->AddQuaternionRot(Vector4(0.f, 1.f, 0.f, 0.f), 90.f);
	pLight->SetWorldPos(0.f, -800.f, 500.f);
	
	pLightCom->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightCom->SetRange(512.f);
	pLightCom->SetAngleIn(22.5f);
	pLightCom->SetAngleOut(45.f);	

	SAFE_RELEASE(pLightCom);

	SAFE_RELEASE(pLight);

	

	CObj* pTerrain = m_pScene->CreateObject<CObj>("Terrain", 
		m_pScene->FindLayer(TERRAIN_LAYER), m_pScene->GetSceneType());

	CTerrain* pTerrainCom = pTerrain->CreateComponent<CTerrain>("Terrain", 
		m_pScene->FindLayer(TERRAIN_LAYER));

	CTerrain::AddTerrain(pTerrainCom);

	pTerrain->SetRootComponent(pTerrainCom);

	pTerrainCom->CreateHeightMap(129, 129, "HeightMap", L"HeightMap.png",TEXTURE_PATH);

	pTerrainCom->SetTexture(REGISTER_TYPE::RT_CUSTOM, "TerrainDiffuses", 30);
	pTerrainCom->SetTexture(REGISTER_TYPE::RT_CUSTOM,"TerrainNormals",31);
	pTerrainCom->SetTexture(REGISTER_TYPE::RT_CUSTOM, "TerrainSpeculars", 32);
	pTerrainCom->SetTexture(REGISTER_TYPE::RT_CUSTOM, "TerrainAlphas", 40);
	CMaterial* pTerrainMtrl = pTerrainCom->GetMaterial();

	pTerrainMtrl->SetBump(true);
	pTerrainMtrl->SetDiffuseTexture(true);
	pTerrainMtrl->SetNormalTexture(true);
	pTerrainMtrl->SetSpecularTexture(true);
	pTerrainMtrl->SetDecal(true);
	

	SAFE_RELEASE(pTerrainMtrl);

	pTerrainCom->SetTiling(60.f, 60.f);
	pTerrainCom->SetSplatCount(5);
	pTerrainCom->SetWorldScale(10.f, 10.f, 10.f);

	SAFE_RELEASE(pTerrainCom);

	SAFE_RELEASE(pTerrain);

	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			CObj* pDecalObj = m_pScene->CreateObject<CObj>("Decal",
				m_pScene->FindLayer(DECAL_LAYER),
				m_pScene->GetSceneType());

			CDecal* pDecal = pDecalObj->CreateComponent<CDecal>("Decal");

			pDecalObj->SetRootComponent(pDecal);

			pDecal->SetWorldScale(100.f, 100.f, 100.f);
			pDecal->SetWorldPos(300.f + i * 100.f, 50.f, 300.f + j * 100.f);

			switch ((i + j) % 3)
			{
			case 0:
				pDecal->SetTexture(REGISTER_TYPE::RT_DIF, "Decal", L"Decal\\Decal.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_NRM, "Decal_NRM", L"Decal\\Decal_NRM.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_SPE, "Decal_SPEC", L"Decal\\Decal_SPEC.png");
				break;
			case 1:
				pDecal->SetTexture(REGISTER_TYPE::RT_DIF, "Decal2", L"Decal\\free-blood-texture_COLOR.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_NRM, "Decal_NRM2", L"Decal\\free-blood-texture_NRM.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_SPE, "Decal_SPEC2", L"Decal\\free-blood-texture_SPEC.png");
				break;
			case 2:
				pDecal->SetTexture(REGISTER_TYPE::RT_DIF, "Decal3", L"Decal\\Decal1.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_NRM, "Decal_NRM3", L"Decal\\Decal1_NRM.png");
				pDecal->SetTexture(REGISTER_TYPE::RT_SPE, "Decal_SPEC3", L"Decal\\Decal1_SPEC.png");
				break;
			}
			pDecal->SetAlpha(0.5f);
			pDecal->SetInTime(rand()%5 / 5.f + 1.f);
			pDecal->SetDuration(rand() % 4 / 2.f);
			pDecal->SetOutTime(rand() % 5/5.f + 1.f);
			pDecal->SetLoop(true);

			CMaterial* pDecalMtrl = pDecal->GetMaterial();

			pDecalMtrl->SetBump(true);
			SAFE_RELEASE(pDecalMtrl);

			SAFE_RELEASE(pDecal);
			SAFE_RELEASE(pDecalObj);
		}
	}

	CParticleSystem* pSmoke = GET_SINGLE(CResourceManager)->CreateParticle("Smoke", false,
		100, static_cast<int>(sizeof(PARTICLE)), 0, 0);
	CParticleSystem* pSmokeShare = GET_SINGLE(CResourceManager)->CreateParticle("SmokeShare", false,
		1, static_cast<int>(sizeof(PARTICLESHARE)), 0, 0);
	SAFE_RELEASE(pSmoke);
	pSmokeShare->SetMaxSpeed(50.f);
	pSmokeShare->SetMinSpeed(20.f);
	pSmokeShare->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
	pSmokeShare->SetEndColor(Vector4(1.f, 1.f, 1.f, 1.f));
	pSmokeShare->SetMaxLifeTime(2.f);
	pSmokeShare->SetMinLifeTime(1.f);
	pSmokeShare->SetRange(Vector3(200.f, 200.f, 200.f));
	pSmokeShare->SetMove(true);
	pSmokeShare->SetMaxCount(100);
	pSmokeShare->SetStartScale(Vector3(100.f, 100.f, 1.f));
	pSmokeShare->SetEndScale(Vector3(300.f, 300.f, 1.f));
	SAFE_RELEASE(pSmokeShare);


	CObj* pParticleObj = m_pScene->CreateObject<CObj>("Particle",
		m_pScene->FindLayer(PARTICLE_LAYER),
		m_pScene->GetSceneType());

	CParticle* pParticle = pParticleObj->CreateComponent<CParticle>("Particle");

	pParticleObj->SetRootComponent(pParticle);

	pParticle->SetSpawnLimit(0.1f);
	pParticle->SetTexture(REGISTER_TYPE::RT_DIF, "smokeparticle", L"Particle\\smokeparticle.png");
	pParticle->AddParticle("Smoke");
	pParticle->AddParticle("SmokeShare");

	SAFE_RELEASE(pParticle);
	SAFE_RELEASE(pParticleObj);
	return true;
}

bool CMainMode::LoadTexture()
{
	std::vector<const TCHAR*>	vecPath;

	vecPath.push_back(L"LandScape\\BD_Terrain_Cave_01.dds");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_11.dds");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_15_Large.dds");
	vecPath.push_back(L"LandScape\\BD_Terrain_Cliff05.dds");
	vecPath.push_back(L"LandScape\\ROCK_01+MOSS_COLOR.png");

	GET_SINGLE(CResourceManager)->LoadTextureArray("TerrainDiffuses",vecPath);

	vecPath.clear();

	vecPath.push_back(L"LandScape\\BD_Terrain_Cave_01_NRM.bmp");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_11_NRM.bmp");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_15_Large_NRM.bmp");
	vecPath.push_back(L"LandScape\\ROCK_01+MOSS_NRM.png");
	vecPath.push_back(L"LandScape\\BD_Terrain_Cliff05_NRM.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("TerrainNormals", vecPath);

	vecPath.clear();

	vecPath.push_back(L"LandScape\\BD_Terrain_Cave_01_SPEC.bmp");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_11_SPEC.bmp");
	vecPath.push_back(L"LandScape\\Terrain_Cliff_15_Large_SPEC.bmp");
	vecPath.push_back(L"LandScape\\ROCK_01+MOSS_SPEC.png");
	vecPath.push_back(L"LandScape\\BD_Terrain_Cliff05_SPEC.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("TerrainSpeculars", vecPath);

	vecPath.clear();

	vecPath.push_back(L"LandScape\\FlowerAlpha.bmp");
	vecPath.push_back(L"LandScape\\SandBaseAlpha.bmp");
	vecPath.push_back(L"LandScape\\WaterBaseAlpha.bmp");
	vecPath.push_back(L"LandScape\\StonAlpha.bmp");

	GET_SINGLE(CResourceManager)->LoadTextureArray("TerrainAlphas", vecPath);

	return true;
}
