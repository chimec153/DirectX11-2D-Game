#include "pch.h"
#include "Eyecube.h"
#include "Component/SpriteComponent.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Component/Renderer.h"
#include "Component/ColliderRect.h"
#include "../../IdleState.h"
#include "Scene/Scene.h"
#include "UI/UIFont.h"
#include "InputObj.h"
#include "Component/Transform.h"
#include "../GameMode/TileMode.h"
#include "Component/ColliderOBB2D.h"
#include "Resource/ShaderManager.h"
#include "Bullet.h"
#include "Component/Mesh2DComponent.h"
#include "Player.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "../../State.h"
#include "../BossManager.h"
#include "Component/Sound.h"
#include "../GameMode/EyeCubeMode.h"
#include "Engine.h"
#include "Scene/SceneManager.h"

float CEyecube::m_fTurnMax = 0.25f;
float CEyecube::m_fDelayMax = 0.4f;

CEyecube::CEyecube()	:
	m_eDir(DIR_8::U)
	, m_bTurned(true)
	, m_fTurnTime(0.f)
#ifdef _DEBUG
	, m_pXText(nullptr)
#endif
	, m_fDelay(0.15f)
	, m_fAngleX(0.f)
	, m_vNextQuat()
	, m_vPrevQuat()
	, m_pRC(nullptr)
	, m_pOC(nullptr)
	, m_pCube(nullptr)
	, m_pCylinder(nullptr)
	, m_pRedCylinder(nullptr)
	, m_pFace(nullptr)
	, m_pLight(nullptr)
	, m_pLightMtrl(nullptr)
	, m_iRotate(0)
	, m_pShadow(nullptr)
	, m_eX(CUBE_AXIS::U)
#ifdef _DEBUG
	, m_pStage(nullptr)
	, m_pTurnText(nullptr)
	, m_pX(nullptr)
	, m_pTurn(nullptr)
	, m_pDelay(nullptr)
#endif
	, m_bAirRotDown(false)
	, m_bAir(false)
	, m_pCylinderBody(nullptr)
	, m_pEft(nullptr)
	, m_pBGM(nullptr)
	, m_iRot(0)
{
	SetBossType(BOSS_TYPE::EYECUBE);
}

CEyecube::CEyecube(const CEyecube& obj)	:
	CSoulMonster(obj)
	, m_eDir(obj.m_eDir)
	, m_bTurned(obj.m_bTurned)
	, m_fTurnTime(0.f)
#ifdef _DEBUG
	, m_pXText((CUIFont*)FindSceneComponent("X"))
#endif
	, m_fDelay(obj.m_fDelay)
	, m_fAngleX(obj.m_fAngleX)
	, m_vNextQuat()
	, m_vPrevQuat()
	, m_pRC((CColliderRect*)FindSceneComponent("CubeBody"))
	, m_pOC((CColliderOBB2D*)FindSceneComponent("EyeBody"))
	, m_pCube((CSpriteComponent*)FindSceneComponent("eyecube"))
	, m_pCylinder((CMesh2DComponent*)FindSceneComponent("cylinder"))
	, m_pRedCylinder((CMesh2DComponent*)FindSceneComponent("redcylinder"))
	, m_pFace((CSpriteComponent*)FindSceneComponent("eyeface"))
	, m_pLight((CSpriteComponent*)FindSceneComponent("Light"))
	, m_pLightMtrl(m_pLight->GetMaterial())
	, m_iRotate(0)
	, m_pShadow((CMesh2DComponent*)FindSceneComponent("Shadow"))
	, m_eX(obj.m_eX)
#ifdef _DEBUG
	, m_pStage((CUIFont*)FindSceneComponent("Stage"))
	, m_pTurnText((CUIFont*)FindSceneComponent("Turned"))
	, m_pX((CUIFont*)FindSceneComponent("Xaxis"))
	, m_pTurn((CUIFont*)FindSceneComponent("Turn"))
	, m_pDelay((CUIFont*)FindSceneComponent("Delay"))
#endif
	, m_bAirRotDown(false)
	, m_bAir(false)
	, m_pCylinderBody((CColliderOBB2D*)FindSceneComponent("CylinderBody"))
	, m_pEft((CSound*)FindSceneComponent("Effect"))
	, m_pBGM((CSound*)FindSceneComponent("BGM"))
	, m_iRot(0)
{
	m_pInput->SetActionFunc<CEyecube>("P", KEY_TYPE::KT_DOWN, this, &CEyecube::Stop);
	m_pInput->SetAxisFunc<CEyecube>("1", this, &CEyecube::RotX);
	m_pInput->SetAxisFunc<CEyecube>("2", this, &CEyecube::RotY);
	m_pInput->SetAxisFunc<CEyecube>("3", this, &CEyecube::RotZ);
	m_pInput->SetAxisFunc<CEyecube>("4", this, &CEyecube::RotX);
	m_pInput->SetAxisFunc<CEyecube>("5", this, &CEyecube::RotY);
	m_pInput->SetAxisFunc<CEyecube>("6", this, &CEyecube::RotZ);

	m_pRC->SetCallBack(COLLISION_STATE::INIT, this, &CEyecube::ColInit);
	m_pRC->SetCallBack(COLLISION_STATE::STAY, this, &CEyecube::ColStay);
	m_pRC->SetCallBack(COLLISION_STATE::END, this, &CEyecube::ColEnd);

	m_pOC->SetCallBack(COLLISION_STATE::INIT, this, &CEyecube::ColEyeInit);
	m_pOC->SetCallBack(COLLISION_STATE::STAY, this, &CEyecube::ColEyeStay);
	m_pOC->SetCallBack(COLLISION_STATE::END, this, &CEyecube::ColEyeEnd);

#ifdef _DEBUG
	m_pX->SetText(L"X: R");
#endif
	m_pFace->AddCallBack("Close", "Close", this, &CEyecube::EffectClose);

	m_pFace->AddCallBack("Opening1", "Open", this, &CEyecube::EffectOpen);

	m_pFace->AddCallBack("Light", "Light", this, &CEyecube::EffectLaserCharge);

	m_pFace->AddCallBack("Attack", "Attack", this, &CEyecube::EffectLaser);
}

CEyecube::~CEyecube()
{
#ifdef _DEBUG
	SAFE_RELEASE(m_pXText);
#endif
	SAFE_RELEASE(m_pRC);
	SAFE_RELEASE(m_pOC);
	SAFE_RELEASE(m_pCube);
	SAFE_RELEASE(m_pCylinder);
	SAFE_RELEASE(m_pRedCylinder);
	SAFE_RELEASE(m_pFace);
	SAFE_RELEASE(m_pLight);
	SAFE_RELEASE(m_pLightMtrl);
	SAFE_RELEASE(m_pShadow);
#ifdef _DEBUG
	SAFE_RELEASE(m_pStage);
	SAFE_RELEASE(m_pTurnText);
	SAFE_RELEASE(m_pX);
	SAFE_RELEASE(m_pTurn);
	SAFE_RELEASE(m_pDelay);
#endif
	SAFE_RELEASE(m_pCylinderBody);
	SAFE_RELEASE(m_pEft);
	SAFE_RELEASE(m_pBGM);
}

bool CEyecube::Init()
{
#ifdef _DEBUG
	m_pInput->SetActionFunc<CEyecube>("P", KEY_TYPE::KT_DOWN, this, &CEyecube::Stop);
	m_pInput->SetAxisFunc<CEyecube>("1", this, &CEyecube::RotX);
	m_pInput->SetAxisFunc<CEyecube>("2", this, &CEyecube::RotY);
	m_pInput->SetAxisFunc<CEyecube>("3", this, &CEyecube::RotZ);
	m_pInput->SetAxisFunc<CEyecube>("4", this, &CEyecube::RotX);
	m_pInput->SetAxisFunc<CEyecube>("5", this, &CEyecube::RotY);
	m_pInput->SetAxisFunc<CEyecube>("6", this, &CEyecube::RotZ);
#endif

	m_pCube = CreateComponent<CSpriteComponent>("eyecube",
		"IMG\\BOSS\\EYECUBE\\eyecube.obj", DATA_PATH, m_pScene->FindLayer("BackDefault"));

	m_pCube->SetInheritScale(false);
	m_pCube->SetRelativeScale(32.f, 32.f, 32.f);
	m_pCube->CreateSprite("Idle", "EyeClosed", LOOP_OPTION::LOOP);
	m_pCube->AddRenderState("DepthWriteForeGround");

	SetRootComponent(m_pCube);

	VertexColor tVertex[24] = {};

	tVertex[0].vPos = Vector3(1.0f, -1.0f, -1.0f);
	tVertex[1].vPos = Vector3(1.000000f, -1.000000f, 1.000000f);
	tVertex[2].vPos = Vector3(-1.000000f, -1.000000f, 1.000000f);
	tVertex[3].vPos = Vector3(-1.000000f, -1.000000f, -1.000000f);
	tVertex[4].vPos = Vector3(1.000000f, 1.000000f, -0.999999f);
	tVertex[5].vPos = Vector3(0.999999f, 1.000000f, 1.000001f);
	tVertex[6].vPos = Vector3(-1.000000f, 1.000000f, 1.000000f);
	tVertex[7].vPos = Vector3(-1.000000f, 1.000000f, -1.000000f);

	tVertex[8].vPos = Vector3(1.0f, -1.0f, -1.0f);
	tVertex[9].vPos = Vector3(1.000000f, -1.000000f, 1.000000f);
	tVertex[10].vPos = Vector3(-1.000000f, -1.000000f, 1.000000f);
	tVertex[11].vPos = Vector3(-1.000000f, -1.000000f, -1.000000f);
	tVertex[12].vPos = Vector3(1.000000f, 1.000000f, -0.999999f);
	tVertex[13].vPos = Vector3(0.999999f, 1.000000f, 1.000001f);
	tVertex[14].vPos = Vector3(-1.000000f, 1.000000f, 1.000000f);
	tVertex[15].vPos = Vector3(-1.000000f, 1.000000f, -1.000000f);

	tVertex[16].vPos = Vector3(1.0f, -1.0f, -1.0f);
	tVertex[17].vPos = Vector3(1.000000f, -1.000000f, 1.000000f);
	tVertex[18].vPos = Vector3(-1.000000f, -1.000000f, 1.000000f);
	tVertex[19].vPos = Vector3(-1.000000f, -1.000000f, -1.000000f);
	tVertex[20].vPos = Vector3(1.000000f, 1.000000f, -0.999999f);
	tVertex[21].vPos = Vector3(0.999999f, 1.000000f, 1.000001f);
	tVertex[22].vPos = Vector3(-1.000000f, 1.000000f, 1.000000f);
	tVertex[23].vPos = Vector3(-1.000000f, 1.000000f, -1.000000f);

	tVertex[4].vUV = Vector2(0.f, 0.f);
	tVertex[5].vUV = Vector2(1.f, 0.f);
	tVertex[0].vUV = Vector2(0.f, 1.f);
	tVertex[1].vUV = Vector2(1.f, 1.f);

	tVertex[6].vUV = Vector2(0.f, 5.f);
	tVertex[7].vUV = Vector2(1.f, 5.f);
	tVertex[2].vUV = Vector2(0.f, 6.f);
	tVertex[3].vUV = Vector2(1.f, 6.f);

	tVertex[14].vUV = Vector2(0.f, 3.f);
	tVertex[15].vUV = Vector2(1.f, 3.f);
	tVertex[13].vUV = Vector2(0.f, 4.f);
	tVertex[12].vUV = Vector2(1.f, 4.f);

	tVertex[8].vUV = Vector2(0.f, 4.f);
	tVertex[9].vUV = Vector2(1.f, 4.f);
	tVertex[11].vUV = Vector2(0.f, 5.f);
	tVertex[10].vUV = Vector2(1.f, 5.f);

	tVertex[21].vUV = Vector2(0.f, 1.f);
	tVertex[22].vUV = Vector2(1.f, 1.f);
	tVertex[17].vUV = Vector2(0.f, 2.f);
	tVertex[18].vUV = Vector2(1.f, 2.f);

	tVertex[23].vUV = Vector2(0.f, 2.f);
	tVertex[20].vUV = Vector2(1.f, 2.f);
	tVertex[19].vUV = Vector2(0.f, 3.f);
	tVertex[16].vUV = Vector2(1.f, 3.f);

	unsigned short pIdx[36] =
	{
		1, 0, 5,
		0, 4, 5,
		3, 2, 6,
		3, 6, 7,
		12, 15, 13,
		14, 13, 15,
		8, 9, 11,
		9, 10, 11,
		16, 19, 20,
		19, 23, 20,
		17, 21, 18,
		18, 21, 22
	};

	GET_SINGLE(CResourceManager)->CreateMesh(MESH_TYPE::MT_2D, "EyeCube", D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		(void*)tVertex, (int)sizeof(VertexColor), 24, D3D11_USAGE_DEFAULT,
		(void*)pIdx, 2, 36, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT);

	CMesh* pMesh = GET_SINGLE(CResourceManager)->FindMesh("EyeCube");

	CMaterial* pMtrl = m_pCube->GetMaterial();

	m_pCube->SetMesh(pMesh);
	m_pCube->SetShader("Sprite");

	m_pCylinder = CreateComponent<CMesh2DComponent>("cylinder",
		"IMG\\BOSS\\EYECUBE\\cylinderlr.obj", DATA_PATH, m_pLayer);

	m_pCylinder->SetInheritScale(false);
	m_pCylinder->AddRelativePos(200.f, 0.f, 0.f);
	m_pCylinder->AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), 90.f);
	m_pCylinder->SetRelativeScale(16.f, 160.f, 16.f);
	m_pCylinder->SetTexture(REGISTER_TYPE::RT_DIF, "util", TEXT("IMG\\UTIL\\util.png"),DATA_PATH);
	m_pCylinder->AddRenderState("NoCullBack");
	m_pCylinder->AddRenderState("AlphaBlend");
	m_pCylinder->AddRenderState("DepthReadNoWrite");
	m_pCylinder->Enable(false);	

	CMaterial* pCylinderMtrl = m_pCylinder->GetMaterial();

	pCylinderMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.5f);

	SAFE_RELEASE(pCylinderMtrl);

	m_pCube->AddChild(m_pCylinder);

	m_pRedCylinder = CreateComponent<CMesh2DComponent>("redcylinder",
		"IMG\\BOSS\\EYECUBE\\cylinderlr.obj", DATA_PATH, m_pLayer);

	m_pRedCylinder->SetInheritScale(false);
	m_pRedCylinder->AddRelativePos(200.f, 0.f, 0.f);
	m_pRedCylinder->AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), 90.f);
	m_pRedCylinder->SetRelativeScale(8.f, 160.f, 8.f);
	m_pRedCylinder->SetTexture(REGISTER_TYPE::RT_DIF, "util");
	m_pRedCylinder->AddRenderState("NoCullBack");
	m_pRedCylinder->AddRenderState("AlphaBlend");
	m_pRedCylinder->AddRenderState("DepthReadNoWrite");
	m_pRedCylinder->Enable(false);

	CMaterial* pRedCylinderMtrl = m_pRedCylinder->GetMaterial();

	pRedCylinderMtrl->SetDiffuseColor(166.f/255.f, 20.f / 255.f, 64.f / 255.f, 0.5f);

	SAFE_RELEASE(pRedCylinderMtrl);

	m_pCube->AddChild(m_pRedCylinder);

	SAFE_RELEASE(pMtrl);

	m_pLight = CreateComponent<CSpriteComponent>("Light", m_pLayer);

	m_pLight->SetMesh(pMesh);
	m_pLight->SetWorldScale(32.f, 32.f, 32.f);
	m_pLight->SetInheritScale(false);
	m_pLight->CreateSprite("Light", "EyeLightFix", LOOP_OPTION::LOOP);
	m_pLight->AddRenderState("DepthNoWrite");

	m_pLightMtrl = m_pLight->GetMaterial();
	m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

	m_pCube->AddChild(m_pLight);

	SAFE_RELEASE(pMesh);

	m_pFace = CreateComponent<CSpriteComponent>("eyeface",
		"IMG\\BOSS\\EYECUBE\\eyeface.obj",DATA_PATH, m_pScene->FindLayer("BackDefault"));

	m_pFace->SetInheritScale(false);
	m_pFace->SetRelativeScale(32.f, 32.f, 32.f);

	m_pFace->CreateSprite("Closed", "EyeClosed", LOOP_OPTION::LOOP);
	m_pFace->CreateSprite("Close", "EyeClose", LOOP_OPTION::ONCE_RETURN);
	m_pFace->CreateSprite("Idle", "EyeIdle", LOOP_OPTION::LOOP);
	m_pFace->CreateSprite("Opening1", "EyeOpening1", LOOP_OPTION::ONCE_RETURN);
	m_pFace->CreateSprite("Opening2", "EyeOpening2", LOOP_OPTION::ONCE_RETURN);
	m_pFace->CreateSprite("Light", "EyeLight", LOOP_OPTION::ONCE_RETURN);
	m_pFace->CreateSprite("Attack", "EyeAttack", LOOP_OPTION::ONCE_RETURN);

	m_pFace->AddNotify("Close", "Close", 0);
	m_pFace->AddNotify("ColliderDisable", "Close", 1);

	m_pFace->AddCallBack("Close", "Close", this, &CEyecube::EffectClose);
	m_pFace->AddCallBack("Close", "Close", this, &CEyecube::DisableCollider);

	m_pFace->AddNotify("Opening1", "Open", 0);
	m_pFace->AddNotify("ColliderEnable", "Open", 1);

	m_pFace->AddCallBack("Opening1", "Open", this, &CEyecube::EffectOpen);
	m_pFace->AddCallBack("Opening1", "Open", this, &CEyecube::EnableCollider);

	m_pFace->AddNotify("Light", "Light", 0);

	m_pFace->AddCallBack("Light", "Light", this, &CEyecube::EffectLaserCharge);

	m_pFace->AddNotify("Attack", "Attack", 0);

	m_pFace->AddCallBack("Attack", "Attack", this, &CEyecube::EffectLaser);

	m_pFace->AddRenderState("DepthWriteForeGround");

	m_pOC = CreateComponent<CColliderOBB2D>("EyeBody", m_pLayer);

	m_pOC->SetExtend(4.f, 16.f, 1.f);
	m_pOC->AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), 90.f);
	m_pOC->AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), 90.f);
	m_pOC->SetRelativePos(40.f, 0.f, 0.f);

	m_pOC->SetCallBack(COLLISION_STATE::INIT, this, &CEyecube::ColEyeInit);
	m_pOC->SetCallBack(COLLISION_STATE::STAY, this, &CEyecube::ColEyeStay);
	m_pOC->SetCallBack(COLLISION_STATE::END, this, &CEyecube::ColEyeEnd);

	m_pFace->AddChild(m_pOC);

	m_pCube->AddChild(m_pFace);
#ifdef _DEBUG
	m_pXText = CreateComponent<CUIFont>("X", m_pLayer);

	m_pXText->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pXText->SetRect(0, 50.f, 400.f, 20.f);
	m_pXText->SetFormat("Normal1");
	m_pXText->SetColor(0.f, 1.f, 0.f, 1.f);

	m_pXText->SetInheritRotX(false);
	m_pXText->SetInheritRotY(false);
	m_pXText->SetInheritRotZ(false);
	m_pXText->SetInheritPos(false);

	m_pXText->AddRelativePos(300.f, 480.f, 0.f);

	m_pCube->AddChild(m_pXText);

	m_pStage = CreateComponent<CUIFont>("Stage", m_pLayer);

	m_pStage->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pStage->SetRect(0, 50.f, 400.f, 20.f);
	m_pStage->SetFormat("Normal1");
	m_pStage->SetColor(0.f, 1.f, 0.f, 1.f);

	m_pStage->SetInheritRotX(false);
	m_pStage->SetInheritRotY(false);
	m_pStage->SetInheritRotZ(false);
	m_pStage->SetInheritPos(false);

	m_pStage->AddRelativePos(0.f, 480.f, 0.f);

	m_pCube->AddChild(m_pStage);
	m_pTurnText = CreateComponent<CUIFont>("Turned", m_pLayer);

	m_pTurnText->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pTurnText->SetRect(0, 50.f, 400.f, 20.f);
	m_pTurnText->SetFormat("Normal1");
	m_pTurnText->SetColor(0.f, 1.f, 0.f, 1.f);

	m_pTurnText->SetInheritRotX(false);
	m_pTurnText->SetInheritRotY(false);
	m_pTurnText->SetInheritRotZ(false);
	m_pTurnText->SetInheritPos(false);

	m_pTurnText->AddRelativePos(0.f, 440.f, 0.f);

	m_pCube->AddChild(m_pTurnText);

	m_pX = CreateComponent<CUIFont>("Xaxis", m_pLayer);

	m_pX->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pX->SetRect(0, 50.f, 400.f, 20.f);
	m_pX->SetFormat("Normal1");
	m_pX->SetColor(0.f, 1.f, 0.f, 1.f);

	m_pX->SetInheritRotX(false);
	m_pX->SetInheritRotY(false);
	m_pX->SetInheritRotZ(false);
	m_pX->SetInheritPos(false);

	m_pX->AddRelativePos(300.f, 520.f, 0.f);

	m_pCube->AddChild(m_pX);

	m_pTurn = CreateComponent<CUIFont>("Turn", m_pLayer);

	m_pTurn->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pTurn->SetRect(0, 50.f, 400.f, 20.f);
	m_pTurn->SetFormat("Normal1");
	m_pTurn->SetColor(0.f, 1.f, 0.f, 1.f);

	m_pTurn->SetInheritRotX(false);
	m_pTurn->SetInheritRotY(false);
	m_pTurn->SetInheritRotZ(false);
	m_pTurn->SetInheritPos(false);

	m_pTurn->AddRelativePos(300.f, 440.f, 0.f);

	m_pCube->AddChild(m_pTurn);

	m_pDelay = CreateComponent<CUIFont>("Delay", m_pLayer);

	m_pDelay->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_2D);
	m_pDelay->SetRect(0, 50.f, 400.f, 20.f);
	m_pDelay->SetFormat("Normal1");
	m_pDelay->SetColor(0.f, 1.f, 0.f, 1.f);
	m_pDelay->SetInheritRotX(false);
	m_pDelay->SetInheritRotY(false);
	m_pDelay->SetInheritRotZ(false);
	m_pDelay->SetInheritPos(false);
	m_pDelay->AddRelativePos(500.f, 440.f, 0.f);

	m_pCube->AddChild(m_pDelay);


#endif
	m_pRC = CreateComponent<CColliderRect>("CubeBody", m_pLayer);

	m_pRC->SetExtent(64.f, 64.f);
	m_pRC->SetInheritScale(false);
	m_pRC->SetInheritRotX(false);
	m_pRC->SetInheritRotY(false);
	m_pRC->SetInheritRotZ(false);
	m_pRC->SetRelativePos(0.f, -16.f, 0.f);

	m_pRC->SetCallBack(COLLISION_STATE::INIT, this, &CEyecube::ColInit);
	m_pRC->SetCallBack(COLLISION_STATE::STAY, this, &CEyecube::ColStay);
	m_pRC->SetCallBack(COLLISION_STATE::END, this, &CEyecube::ColEnd);

	m_pCube->AddChild(m_pRC);

	m_pShadow = CreateComponent<CMesh2DComponent>("Shadow", m_pScene->FindLayer("Ground"));

	m_pShadow->SetInheritScale(false);
	m_pShadow->SetInheritRotX(false);
	m_pShadow->SetInheritRotY(false);
	m_pShadow->SetInheritRotZ(false);
	m_pShadow->SetRelativeScale(66.f, 66.f, 0.f);
	m_pShadow->AddRelativePos(0.f, -16.f, 0.f);
	m_pShadow->SetPivot(0.5f, 0.5f, 0.f);
	m_pShadow->AddRenderState("AlphaBlend");
	m_pShadow->AddRenderState("DepthNoWrite");

	CMaterial* pShadowMtrl = m_pShadow->GetMaterial();

	pShadowMtrl->SetDiffuseColor(0.f, 0.f, 0.f, 0.5f);

	SAFE_RELEASE(pShadowMtrl);

	m_pCube->AddChild(m_pShadow);

	m_pCylinderBody = CreateComponent<CColliderOBB2D>("CylinderBody", m_pLayer);

	m_pCylinderBody->SetExtend(24.f, 160.f, 24.f);

	m_pCylinder->AddChild(m_pCylinderBody);

	m_pEft = CreateComponent<CSound>("Effect", m_pLayer);

	m_pEft->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pCube->AddChild(m_pEft);

	m_pBGM = CreateComponent<CSound>("BGM", m_pLayer);

	m_pBGM->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_UI);

	m_pCube->AddChild(m_pBGM);

	RotZ(90.f, 1.f);
	RotY(90.f, 1.f);
	RotX(-45.f, 1.f);

	return true;
}

bool CEyecube::Init(const char* pFileName, const char* pPathKey)
{
	return false;
}

void CEyecube::Start()
{
	CSoulMonster::Start();
}

void CEyecube::Update(float fTime)
{
	CSoulMonster::Update(fTime);

	TCHAR strDelay[32] = {};
	TCHAR strTurn[32] = {};

	swprintf_s(strDelay, L"Delay: %.2f", m_fDelay);
	swprintf_s(strTurn, L"Turn: %.2f", m_fTurnTime);

#ifdef _DEBUG
	m_pDelay->SetText(strDelay);
	m_pTurn->SetText(strTurn);
#endif

	if (m_iRotate == (int)EYE_STAGE::ATTACK )
	{
		if (m_fDelay < -m_fDelayMax)
		{
			m_bAirRotDown = false;

			float fTurnTime = fTime / m_fTurnMax /2.f;

			static bool bLight = true;

			m_pCylinder->Enable(bLight);

			m_pRedCylinder->Enable(!bLight);

			((CTileMode*)m_pScene->GetGameMode())->FadeOut(fTurnTime * 0.15f);

			bLight ^= true;

			CPlayer* pObj = (CPlayer*)m_pScene->GetGameMode()->GetPlayer();

			Vector3 vTargetPos = pObj->GetWorldPos();

			Vector3 vPos = GetWorldPos();

			if (m_eX == CUBE_AXIS::D)
			{
				AddWorldPos(0.f, 256.f * fTurnTime, 0.f);

				m_pShadow->AddRelativePos(0.f, -256.f * fTurnTime, 0.f);

				m_pShadow->AddRelativeScale(-32.f * fTurnTime, -32.f * fTurnTime, 0.f);

				Slerp(m_vPrevQuat, m_vNextQuat, (m_fDelay + m_fTurnMax * 2.f + m_fDelayMax) / m_fTurnMax/2.f);

				m_pRC->Enable(false);
			}

			else if (m_eX == CUBE_AXIS::L)
			{
				if (vPos.x - 32.f > vTargetPos.x &&
					vPos.y + 16.f > vTargetPos.y &&
					vPos.y - 16.f < vTargetPos.y)
				{
					pObj->SetBurn();
					pObj->SetState(STATE::DIE);

					m_pBGM->Stop();
				}
			}

			else if (m_eX == CUBE_AXIS::R)
			{
				if (vPos.x + 32.f < vTargetPos.x &&
					vPos.y + 16.f > vTargetPos.y &&
					vPos.y - 16.f < vTargetPos.y)
				{
					pObj->SetBurn();
					pObj->SetState(STATE::DIE);

					m_pBGM->Stop();
				}
			}

			else if(m_eX == CUBE_AXIS::N)
			{
				if (vPos.y - 32.f > vTargetPos.y &&
					vPos.x + 16.f > vTargetPos.x &&
					vPos.x - 16.f < vTargetPos.x)
				{
					pObj->SetBurn();
					pObj->SetState(STATE::DIE);

					m_pBGM->Stop();
				}
			}

			else if (m_eX == CUBE_AXIS::F)
			{
				if (vPos.y + 32.f < vTargetPos.y &&
					vPos.x + 16.f > vTargetPos.x &&
					vPos.x - 16.f < vTargetPos.x)
				{
					pObj->SetBurn();
					pObj->SetState(STATE::DIE);

					m_pBGM->Stop();
				}
			}

			SAFE_RELEASE(pObj);
		}

		else
		{
			if (!m_bAirRotDown)
			{
				m_bAirRotDown = true;

				Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

				m_vPrevQuat = m_vNextQuat;

				m_vNextQuat = XMQuaternionMultiply(m_vPrevQuat.Convert(),
					XMQuaternionRotationNormal(Vector4(1.f, 0.f, 0.f, 0.f).Convert(), DegToRad(90.f)));
			}

			float fTurnTime = fTime / m_fDelayMax / 2.f;

			Vector4 vDif = m_pLightMtrl->GetDif();

			if (vDif.w > 0.f)
			{
				m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, vDif.w - fTurnTime);
			}

			m_pCylinder->Enable(false);
			m_pRedCylinder->Enable(false);

			((CTileMode*)m_pScene->GetGameMode())->FadeIn(fTurnTime * 2.f);

			if (m_eX == CUBE_AXIS::D && m_bTurned)
			{
				AddWorldPos(0.f, -256.f * fTurnTime, 0.f);

				m_pShadow->AddRelativePos(0.f, 256.f * fTurnTime, 0.f);

				m_pShadow->AddRelativeScale(32.f * fTurnTime, 32.f * fTurnTime, 0.f);

				Slerp(m_vPrevQuat, m_vNextQuat, (m_fDelay + m_fDelayMax) / m_fDelayMax /2.f);
#ifdef _DEBUG
				m_pX->SetText(L"X: Near");
#endif
			}
		}
	}

	else if (m_iRotate == (int)EYE_STAGE::LIGHT)
	{
		Vector4 vDif = m_pLightMtrl->GetDif();

		if (vDif.w < 1.f)
		{
			m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, vDif.w + fTime);
		}
	}

	State eStat = GetState();

	if (eStat == State::ATTACK)
	{
		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

		Vector3 vTargetPos = pObj->GetWorldPos();

		Vector3 vPos = GetWorldPos();

		vTargetPos = (vTargetPos + vPos) / 2.f;

		pCam->SetZoom(true, 0.f);
		pCam->SetMovePos(vTargetPos);

		SAFE_RELEASE(pObj);

		SAFE_RELEASE(pCam);
	}

	CTransform* pCom = m_pRootComponent->GetTransform();

	Vector4 vQ = pCom->GetQuarternion();
#ifdef _DEBUG
	TCHAR strX[32] = {};

	swprintf_s(strX, L"Q:%.2f,\t%.2f,\t%.2f,\t%.2f", vQ.x, vQ.y, vQ.z, vQ.w);

	m_pXText->SetText(strX);
#endif
	if (m_bTurned)
	{
		State eStat = GetState();

		if (eStat == State::ATTACK)
		{
			m_fDelay += fTime;

			if (m_fDelay >= m_fDelayMax)
			{
				m_fDelay = 0.f;

				Vector3 vPos = GetWorldPos();

				CObj* pObj = m_pScene->GetGameMode()->GetPlayer();

				Vector3 vTargetPos = pObj->GetWorldPos();

				SAFE_RELEASE(pObj);

				Vector3 vDir = vTargetPos - vPos;

				if (m_iRotate == (int)EYE_STAGE::ATTACK)
				{
					if (m_eX == CUBE_AXIS::D)
					{
						m_bAir = false;
						m_pRC->Enable(true);

						((CTileMode*)m_pScene->GetGameMode())->ShakeCam(200.f,7.5f);
#ifdef _DEBUG
						m_pX->SetText(L"Near");
#endif
						m_eX = CUBE_AXIS::N;

						Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

						m_pEft->SetSoundAndPlay("Land");
					}
				}

				m_vPrevQuat = m_pRootComponent->GetTransform()->GetQuarternion();

				if (vDir.x > vDir.y)
				{
					if (-vDir.x > vDir.y)
					{
						if (GetWorldPos().y < 410.f)
						{
							m_eDir = DIR_8::U;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationNormal(Vector4(1.f, 0.f, 0.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::N:
								m_eX = CUBE_AXIS::U; 
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::F:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::F;
#ifdef _DEBUG
								m_pX->SetText(L"X: F");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::N;
#ifdef _DEBUG
								m_pX->SetText(L"X: N");
#endif
								break;
							}
						}

						else
						{
							m_eDir = DIR_8::D;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationNormal(Vector4(-1.f, 0.f, 0.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::N:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::F:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::N;
#ifdef _DEBUG
								m_pX->SetText(L"X: N");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::F;
#ifdef _DEBUG
								m_pX->SetText(L"X: F");
#endif
								break;
							}
						}
					}

					else
					{
						if (GetWorldPos().x > 660.f)
						{
							m_eDir = DIR_8::L;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationAxis(Vector4(0.f, 1.f, 1.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::L:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::R:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::L;
#ifdef _DEBUG
								m_pX->SetText(L"X: L");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::R;
#ifdef _DEBUG
								m_pX->SetText(L"X: R");
#endif
								break;
							}
						}
						
						else
						{
							m_eDir = DIR_8::R;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationAxis(Vector4(0.f, -1.f, -1.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::L:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::R:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::R;
#ifdef _DEBUG
								m_pX->SetText(L"X: R");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::L;
#ifdef _DEBUG
								m_pX->SetText(L"X: L");
#endif
								break;
							}
						}
					}
				}

				else
				{
					if (-vDir.x > vDir.y)
					{
						if (GetWorldPos().x < 300.f)
						{
							m_eDir = DIR_8::R;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationAxis(Vector4(0.f, -1.f, -1.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::L:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::R:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::R;
#ifdef _DEBUG
								m_pX->SetText(L"X: R");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::L;
#ifdef _DEBUG
								m_pX->SetText(L"X: L");
#endif
								break;
							}
						}

						else
						{
							m_eDir = DIR_8::L;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationAxis(Vector4(0.f, 1.f, 1.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::L:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::R:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::L;
#ifdef _DEBUG
								m_pX->SetText(L"X: L");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::R;
#ifdef _DEBUG
								m_pX->SetText(L"X: R");
#endif
								break;
							}
						}
					}

					else
					{
						if (GetWorldPos().y > 700.f)
						{
							m_eDir = DIR_8::D;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationNormal(Vector4(-1.f, 0.f, 0.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::N:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::F:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::N;
#ifdef _DEBUG
								m_pX->SetText(L"X: N");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::F;
#ifdef _DEBUG
								m_pX->SetText(L"X: F");
#endif
								break;
							}
						}

						else
						{
							m_eDir = DIR_8::U;

							m_vNextQuat = XMQuaternionMultiply(
								m_vPrevQuat.Convert(),
								XMQuaternionRotationNormal(Vector4(1.f, 0.f, 0.f, 0.f).Convert(), DegToRad(90.f)));

							switch (m_eX)
							{
							case CUBE_AXIS::N:
								m_eX = CUBE_AXIS::U;
#ifdef _DEBUG
								m_pX->SetText(L"X: U");
#endif
								break;
							case CUBE_AXIS::F:
								m_eX = CUBE_AXIS::D;
#ifdef _DEBUG
								m_pX->SetText(L"X: D");
#endif
								break;
							case CUBE_AXIS::U:
								m_eX = CUBE_AXIS::F;
#ifdef _DEBUG
								m_pX->SetText(L"X: F");
#endif
								break;
							case CUBE_AXIS::D:
								m_eX = CUBE_AXIS::N;
#ifdef _DEBUG
								m_pX->SetText(L"X: N");
#endif
								break;
							}
						}
					}
				}

				m_bTurned = false;
#ifdef _DEBUG
				m_pTurnText->SetText(L"Turned: FALSE");
#endif

				static int iCount = 0;

				++iCount %= 8;

				char strTag[32] = {};

				sprintf_s(strTag, "Roll%d", iCount + 1);

				m_pEft->SetSoundAndPlay(strTag);
			}
		}

		else
		{
			static bool bDir = true;

			m_fTurnTime += fTime * (bDir * 2 - 1) / 2.f;

			if (m_fTurnTime >= m_fTurnMax ||
				m_fTurnTime <= 0.f)
			{
				m_fTurnTime = m_fTurnMax * bDir;

				bDir ^= true;
			}

			m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, m_fTurnTime / m_fTurnMax);
		}
	}

	else
	{
		m_fTurnTime += fTime;
		m_pRC->Enable(m_bTurned);

		if (m_fTurnTime >= m_fTurnMax)
		{
			m_fTurnTime = 0.f;

			++m_iRot;

			if (m_iRot > 3)
			{
				((CTileMode*)m_pScene->GetGameMode())->SetText(false);
				m_iRot = 0;
			}

			m_bTurned = true;

#ifdef _DEBUG
			m_pTurnText->SetText(L"Turned: TRUE");
#endif

			static int iCount = 0;

			++iCount %= 8;

			char strTag[32] = {};

			sprintf_s(strTag, "Slam%d", iCount + 1);

			m_pEft->SetSoundAndPlay(strTag);

			m_pRC->Enable(m_bTurned && !m_bAir);

			((CTileMode*)m_pScene->GetGameMode())->ShakeCam();

			switch ((EYE_STAGE)m_iRotate)
			{
			case EYE_STAGE::CLOSE:
				m_pFace->ChangeSequence("Opening1");
#ifdef _DEBUG
				m_pStage->SetText(L"OPEN1");
#endif
				break;
			case EYE_STAGE::OPEN1:
				m_pFace->ChangeSequence("Opening2");
#ifdef _DEBUG
				m_pStage->SetText(L"OPEN2");
#endif
				break;
			case EYE_STAGE::OPEN2:
				m_pFace->ChangeSequence("Light");
#ifdef _DEBUG
				m_pStage->SetText(L"LIGHT");
#endif
				break;
			case EYE_STAGE::LIGHT:
				m_pFace->ChangeSequence("Attack");
				m_pFace->SetDefaultSeq("Idle");
#ifdef _DEBUG
				m_pStage->SetText(L"ATTACK");
#endif				

				if (m_eX == CUBE_AXIS::D)
				{
					m_fDelay = -m_fTurnMax * 2 - m_fDelayMax;
					((CTileMode*)m_pScene->GetGameMode())->ShakeCam(600.f, 20.f, -m_fDelay * 1.5f, Vector3(0.f, 1.f, 0.f));
					m_bAir = true;

					Slerp(m_vPrevQuat, m_vNextQuat, 1.f);

					m_vPrevQuat = m_vNextQuat;

					m_vNextQuat = XMQuaternionMultiply(m_vPrevQuat.Convert(),
						XMQuaternionRotationAxis(Vector4(0.f, 1.f, -1.f, 0.f).Convert(), DegToRad(90.f)));

					m_pEft->SetSoundAndPlay("LaserJump");
				}

				else if (m_eX == CUBE_AXIS::L ||
					m_eX == CUBE_AXIS::R)
				{
					m_fDelay = (-m_fTurnMax * 2 - m_fDelayMax) * 1.5f;
					((CTileMode*)m_pScene->GetGameMode())->ShakeCam(600.f, 20.f, -m_fDelay /1.5f, Vector3(1.f, 0.f, 0.f));
				}

				else
				{
					m_fDelay = (-m_fTurnMax * 2 - m_fDelayMax) * 1.5f;
					((CTileMode*)m_pScene->GetGameMode())->ShakeCam(600.f, 20.f, -m_fDelay / 1.5f, Vector3(0.f, 1.f, 0.f));
				}

				((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(166.f / 255.f, 20.f / 255.f, 64.f / 255.f);

				break;
			case EYE_STAGE::ATTACK:
				m_pFace->ChangeSequence("Opening1");
				m_pCylinder->Enable(false);

				m_pRedCylinder->Enable(false);
				m_iRotate = (int)EYE_STAGE::CLOSE;
#ifdef _DEBUG
				m_pStage->SetText(L"OPEN1");
#endif
				break;
			}

			++m_iRotate;

			Vector3 vPos = GetWorldPos();

			int iX = (int)vPos.x;

			int iDif = (iX - 24) % 64;

			if (iDif > 32)
			{
				iX = iX - iDif + 64;
			}

			else
			{
				iX = iX - iDif;
			}

			int iY = (int)vPos.y;

			iDif = (iY - 5) % 64;

			if (iDif > 32)
			{
				iY = iY - iDif + 64;
			}

			else
			{
				iY = iY - iDif;
			}

			SetWorldPos((float)iX, (float)iY, vPos.z);

			Slerp(m_vPrevQuat, m_vNextQuat, 1.f);
		}

		else
		{

			if (m_fTurnTime >= m_fTurnMax * 0.9f)
			{
				m_pRC->Enable(!m_bAir);
			}

			float fTurnTime = m_fTurnTime / m_fTurnMax;
			Slerp(m_vPrevQuat, m_vNextQuat, fTurnTime);
			switch (m_eDir)
			{
			case DIR_8::U:
			{
				AddWorldPos(0.f, 64.f * fTime / m_fTurnMax, 0.f);
			}
			break;
			case DIR_8::R:
			{
				AddWorldPos( 64.f * fTime / m_fTurnMax, cosf(DegToRad(fTurnTime * 180.f)) * fTime * 160.f, 0.f);
				m_pShadow->AddRelativePos(0.f, cosf(DegToRad(fTurnTime * 180.f))* fTime * -160.f, 0.f);
			}
			break;
			case DIR_8::D:
			{
				AddWorldPos(0.f, -64.f * fTime / m_fTurnMax, 0.f);
			}
			break;
			case DIR_8::L:
			{
				AddWorldPos(-64.f * fTime / m_fTurnMax, cosf(DegToRad(fTurnTime * 180.f))* fTime * 160.f, 0.f);
				m_pShadow->AddRelativePos(0.f, cosf(DegToRad(fTurnTime * 180.f))* fTime * -160.f, 0.f);
			}
			break;
			}
		}
	}
}

void CEyecube::PostUpdate(float fTime)
{
	CSoulMonster::PostUpdate(fTime);
}

void CEyecube::Collision(float fTime)
{
	CSoulMonster::Collision(fTime);
}

void CEyecube::PreRender(float fTime)
{
	CSoulMonster::PreRender(fTime);

	if (GET_SINGLE(CEngine)->IsImgui())
	{
		SpawnWindow();
	}
}

void CEyecube::Render(float fTime)
{
	CSoulMonster::Render(fTime);
}

void CEyecube::PostRender(float fTime)
{
	CSoulMonster::PostRender(fTime);
}

CEyecube* CEyecube::Clone()
{
	return new CEyecube(*this);
}

void CEyecube::Save(FILE* pFile)
{
	CSoulMonster::Save(pFile);
}

void CEyecube::Load(FILE* pFile)
{
	CSoulMonster::Load(pFile);

#ifdef _DEBUG
	m_pXText = (CUIFont*)FindSceneComponent("X");
#endif
	m_pRC = (CColliderRect*)FindSceneComponent("CubeBody");
	m_pOC = (CColliderOBB2D*)FindSceneComponent("EyeBody");
	m_pCube = (CSpriteComponent*)FindSceneComponent("eyecube");
	m_pCylinder = (CMesh2DComponent*)FindSceneComponent("cylinder");
	m_pRedCylinder = (CMesh2DComponent*)FindSceneComponent("redcylinder");
	m_pFace = (CSpriteComponent*)FindSceneComponent("eyeface");
	m_pLight = (CSpriteComponent*)FindSceneComponent("Light");
	m_pShadow = (CMesh2DComponent*)FindSceneComponent("Shadow");
#ifdef _DEBUG
	m_pStage = (CUIFont*)FindSceneComponent("Stage");
	m_pTurnText = (CUIFont*)FindSceneComponent("Turned");
	m_pX = (CUIFont*)FindSceneComponent("Xaxis");
	m_pTurn = (CUIFont*)FindSceneComponent("Turn");
	m_pDelay = (CUIFont*)FindSceneComponent("Delay");
#endif
	m_pCylinderBody = (CColliderOBB2D*)FindSceneComponent("CylinderBody");
	m_pEft = (CSound*)FindSceneComponent("Effect");
	m_pBGM = (CSound*)FindSceneComponent("BGM");
	m_pLightMtrl = m_pLight->GetMaterial();

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();
	m_pShadow->SetLayer(pScene->FindLayer("Ground"));
	m_pFace->SetLayer(pScene->FindLayer("BackDefault"));
	m_pCube->SetLayer(pScene->FindLayer("BackDefault"));
}

void CEyecube::ColInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strDst = pDst->GetName();

	if (strDst == "BulletBody")
	{
		if (((CBullet*)pDst->GetObj())->GetSpeed())
		{
			State eStat = GetState();

			if (eStat == State::IDLE)
			{
				SetState(State::ATTACK); 

				m_iRot = 0;

				((CTileMode*)m_pScene->GetGameMode())->SetText(true);
			}
		}
	}

	else if (strDst == "PlayerBody")
	{
		if (!m_bTurned)
		{
			m_pBGM->SetSound("Eyecube");
			m_pBGM->Stop();
		}
	}
}

void CEyecube::ColStay(CCollider* pSrc, CCollider* pDst, float fTime)
{

}

void CEyecube::ColEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{

}

void CEyecube::ColEyeInit(CCollider* pSrc, CCollider* pDst, float fTime)
{
	std::string strDst = pDst->GetName();

	if (strDst == "BulletBody")
	{
		State eStat = GetState();

		if (eStat == State::ATTACK)
		{
			CBullet* pBullet = (CBullet*)pDst->GetObj();

			float fSpeed = pBullet->GetSpeed();

			if (fSpeed != 0.f)
			{
				Vector3 vRot = pBullet->GetWorldRot();

				CSound* pSnd = pBullet->FindComByType<CSound>();

				if (vRot.z >= -135.f &&
					vRot.z <= -45.f &&
					m_eX == CUBE_AXIS::L)
				{
					SetState(State::STOP);

					pBullet->SetSpeed(0.f);
					pBullet->ChangeSprite("ArrowStop");
					pBullet->SetFix(true);
					pBullet->SetFixObj(this);

					pSnd->SetSound("ArrowImpact1");
				}

				else if (vRot.z <= 45.f &&
					vRot.z >= -45.f &&
					m_eX == CUBE_AXIS::N)
				{
					SetState(State::STOP);

					pBullet->SetSpeed(0.f);
					pBullet->ChangeSprite("ArrowStop");
					pBullet->SetFix(true);
					pBullet->SetFixObj(this);

					pSnd->SetSound("ArrowImpact2");
				}

				else if (vRot.z <= 135.f &&
					vRot.z >= 45.f &&
					m_eX == CUBE_AXIS::R)
				{
					SetState(State::STOP);

					pBullet->SetSpeed(0.f);
					pBullet->ChangeSprite("ArrowStop");
					pBullet->SetFix(true);
					pBullet->SetFixObj(this);

					pSnd->SetSound("ArrowImpact3");
				}

				else if ((vRot.z >= 135.f ||
					vRot.z <= -135.f) &&
					m_eX == CUBE_AXIS::F)
				{
					SetState(State::STOP);

					pBullet->SetSpeed(0.f);
					pBullet->ChangeSprite("ArrowStop");
					pBullet->SetFix(true);
					pBullet->SetFixObj(this);

					pSnd->SetSound("ArrowImpact4");
				}

				SAFE_RELEASE(pSnd);
			}
		}
	}
}

void CEyecube::ColEyeStay(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CEyecube::ColEyeEnd(CCollider* pSrc, CCollider* pDst, float fTime)
{
}

void CEyecube::Stop(float fPush, float fTime)
{
	if (m_fDelayMax > 100.f)
	{
		SetDelayMax(0.5f);
	}

	else
	{
		SetDelayMax(10000.f);
	}
}

void CEyecube::RotX(float fScale, float fTime)
{
	if(fScale != 0.f)
		AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), fScale * fTime);
}

void CEyecube::RotY(float fScale, float fTime)
{
	if (fScale != 0.f)
		AddQuaternionRotNorm(Vector4(0.f, 1.f, 0.f, 0.f), fScale * fTime);
}

void CEyecube::RotZ(float fScale, float fTime)
{
	if (fScale != 0.f)
		AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), fScale * fTime);
}

void CEyecube::EffectOpen(float)
{
	m_pEft->SetSoundAndPlay("EyeOpen");
}

void CEyecube::EffectClose(float)
{
	m_pEft->SetSoundAndPlay("EyeClose");
}

void CEyecube::EffectLaser(float)
{
	m_pEft->SetSoundAndPlay("Laser");
}

void CEyecube::EffectLaserCharge(float)
{
	m_pEft->SetSoundAndPlay("LaserCharge");
}

void CEyecube::SpawnWindow()
{
	if (ImGui::Begin("Eyecube"))
	{
		ImGui::SliderFloat("TurnTime", &m_fTurnMax, 0.f, 1.f);
		ImGui::SliderFloat("DelayMax", &m_fDelayMax, 0.f, 1.f);
	}
	ImGui::End();
}

void CEyecube::DisableCollider(float)
{
	m_pOC->Enable(false);
}

void CEyecube::EnableCollider(float)
{
	m_pOC->Enable(true);
}

void CEyecube::SetState(State eStat)
{
	switch (eStat)
	{
	case State::IDLE:
		break;
	case State::ATTACK:

		switch (GetState())
		{
		case State::IDLE:
			m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

			m_fTurnTime = 0.f;

			((CTileMode*)m_pScene->GetGameMode())->Lock(true);

			m_pBGM->SetSoundAndPlay("Eyecube");
			break;
		case State::ATTACK:
			break;
		case State::STOP:
			break;
		case State::DIE:
			break;
		}

		break;
	case State::STOP:
	{
		m_iRotate = (int)EYE_STAGE::END;

		m_pCylinder->Enable(false);
		m_pRedCylinder->Enable(false);

		m_pLightMtrl->SetDiffuseColor(1.f, 1.f, 1.f, 0.f);

		((CTileMode*)m_pScene->GetGameMode())->SetFadeColor(1.f, 1.f, 1.f);
		((CTileMode*)m_pScene->GetGameMode())->SetFade(1.f, 0.f, -3.f);

		CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

		pCam->SetZoom(false);
		pCam->SetFocus(nullptr);

		SAFE_RELEASE(pCam);

		FADE tFade = {};

		tFade.fGray = 1.f;

		GET_SINGLE(CShaderManager)->UpdateCBuffer("Fade", &tFade);

		m_pBGM->SetSound("Eyecube");
		m_pBGM->Stop();

		m_pFace->ChangeSequence("Idle");

		m_pCube->SetShader("GrayShader");
		m_pFace->SetShader("GrayShader");
		m_pLight->SetShader("GrayShader");

	}
		break;

	case State::DIE:

		m_pFace->ChangeSequence("Closed");
		GET_SINGLE(CBossManager)->AddMonster(this);

		((CEyeCubeMode*)m_pScene->GetGameMode())->Clear();

		m_pLight->Enable(false);
		break;
	}
	CSoulMonster::SetState(eStat);
}
