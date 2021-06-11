#include "Player3D.h"
#include "Component/Mesh2DComponent.h"
#include "Camera/CameraManager.h"
#include "Component/Camera.h"
#include "Input.h"
#include "InputObj.h"
#include "Component/Navigation2D.h"
#include "Scene/Scene.h"
#include "Tile/TileMap.h"
#include "Component/ColliderLine.h"
#include "Component/PaperBurn.h"
#include "Component/Renderer.h"
#include "Render/Distortion.h"
#include "Resource/Material.h"
#include "Resource/Animation.h"
#include "Component/Terrain.h"
#include "Component/ColliderCircle.h"

CPlayer3D::CPlayer3D()	:
	m_pCom(nullptr)
	, m_pCam(nullptr)
	, m_pNav(nullptr)
	, m_pPaper(nullptr)
	, m_pAni(nullptr)
	, m_pTerrain(nullptr)
{
}

CPlayer3D::CPlayer3D(const CPlayer3D& obj)	:
	CObj(obj)
	, m_pCam(nullptr)
{
}

CPlayer3D::~CPlayer3D()
{
	SAFE_RELEASE(m_pCam);
	SAFE_RELEASE(m_pNav);
	SAFE_RELEASE(m_pPaper);

	SAFE_RELEASE(m_pCom);
	SAFE_RELEASE(m_pAni);
	SAFE_RELEASE(m_pTerrain);
}

bool CPlayer3D::Init()
{
	if (!CObj::Init())
		return false;

	CInput::GetInst()->AddAxisKey("WS", DIK_W, 1.f);
	CInput::GetInst()->AddAxisKey("WS", DIK_S, -1.f);
	CInput::GetInst()->AddAxisKey("DA", DIK_D, 1.f);
	CInput::GetInst()->AddAxisKey("DA", DIK_A, -1.f);
	CInput::GetInst()->AddActionKey("Space", DIK_SPACE);
	CInput::GetInst()->AddAxisKey("Yaw", DIK_R, 1.f);
	CInput::GetInst()->AddAxisKey("Yaw", DIK_F, -1.f);
	CInput::GetInst()->AddAxisKey("Pitch", DIK_T, 1.f);
	CInput::GetInst()->AddAxisKey("Pitch", DIK_G, -1.f);
	CInput::GetInst()->AddAxisKey("Roll", DIK_Y, 1.f);
	CInput::GetInst()->AddAxisKey("Roll", DIK_H, -1.f);

	m_pInput->SetAxisFunc("WS", this, &CPlayer3D::MoveV);
	m_pInput->SetAxisFunc("DA", this, &CPlayer3D::MoveH);

	m_pInput->SetAxisFunc("Yaw", this, &CPlayer3D::Yaw);
	m_pInput->SetAxisFunc("Pitch", this, &CPlayer3D::Pitch);
	m_pInput->SetAxisFunc("Roll", this, &CPlayer3D::Roll);
	m_pInput->SetActionFunc("Space", KEY_TYPE::KT_DOWN ,this, &CPlayer3D::Space);

	m_pCom = CreateComponent<CMesh2DComponent>("Blade", m_pLayer);

	m_pCom->SetMesh("Player_Default");
	m_pCom->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_3D);
	m_pAni = CAnimation::CreateAnimation<CAnimation>("Ani", m_pScene, "Player_Default","Player_Default.bne");

	m_pAni->SetSkeleton("Player_Default");

	m_pAni->AddSequence("Idle", "Player_Default");

	m_pPaper = CreateComponent<CPaperBurn>("Paper", m_pLayer);

	m_pPaper->SetInFilter(0.2f);
	m_pPaper->SetMidFilter(0.5f);
	m_pPaper->SetOutFilter(0.8f);
	m_pPaper->SetInColor(0.8f, 0.8f, 0.f, 1.f);
	m_pPaper->SetOutColor(0.9f, 0.1f, 0.f, 1.f);

	CRenderer* pRenderer = m_pCom->GetRenderer();
	pRenderer->SetAnimation(m_pAni);
	pRenderer->SetPaper(m_pPaper);

	SAFE_RELEASE(pRenderer);

	SetRootComponent(m_pCom);

	CColliderCircle* pCollider = CreateComponent<CColliderCircle>("Line", m_pLayer);

	pCollider->SetRadius(25.f);
	pCollider->AddRelativePos(0.f, 0.f, -50.f);

	m_pCom->AddChild(pCollider);

	SAFE_RELEASE(pCollider);

	m_pCam = GET_SINGLE(CCameraManager)->GetMainCam();
	m_pCam->SetName("Camera");
	m_pCam->AddRelativePos(0.f, 50.f, -200.f);
	m_pCam->SetInheritRotX(true);
	m_pCam->SetInheritRotY(true);
	m_pCam->SetInheritRotZ(true);
	m_pCom->AddChild(m_pCam);

	CMesh2DComponent* pDistortCom = CreateComponent<CMesh2DComponent>("DistCom", m_pLayer);

	CDistortion* pDistort = pDistortCom->CreatePostProcess<CDistortion>("Distort");

	pDistort->SetSpeed(1.f);
	pDistort->SetAmp(0.1f);

	SAFE_RELEASE(pDistort);

	pDistortCom->SetInheritScale(false);
	pDistortCom->SetShader("DistortShader");
	pDistortCom->AddRelativePos(100.f, 100.f, 0.f);

	pDistortCom->SetWorldScale(100.f, 100.f, 0.f);

	m_pCom->AddChild(pDistortCom);

	SAFE_RELEASE(pDistortCom);

	CObj* pBlade = m_pScene->CreateObject<CObj>("Blade", 
		m_pScene->FindLayer(ALBEDO_LAYER), m_pScene->GetSceneType());

	CMesh2DComponent* pMesh = pBlade->CreateComponent<CMesh2DComponent>("Blade");

	pBlade->SetRootComponent(pMesh);

	pMesh->SetSceneComType(SCENE_COMPONENT_TYPE::SCT_3D);
	pMesh->SetMesh("Blade");
	pMesh->SetWorldScale(1.f, 1.f, 1.f);

	Vector4 qRot = Vector4(
		XMQuaternionMultiply(
			XMQuaternionRotationNormal(Vector3(1.f, 0.f, 0.f).Convert(),
		DirectX::XM_PIDIV2),
		XMQuaternionRotationNormal(Vector3(0.f, 1.f, 0.f).Convert(),
			DirectX::XM_PI)));

	m_pAni->CreateSocket("Blade", "bone19");
	m_pAni->SetSocketComponent("Blade", pMesh);
	m_pAni->SetSocketRotation("Blade", qRot);
	m_pAni->SetSocketOffset("Blade", Vector3(1.f, 3.f, 0.f));

	SAFE_RELEASE(pMesh);

	SAFE_RELEASE(pBlade);

	return true;
}

void CPlayer3D::Start()
{
	CObj::Start();

	m_vMovePos = GetWorldPos();

	m_pTerrain = CTerrain::GetMainTerrain();
}

void CPlayer3D::Update(float fTime)
{
	CObj::Update(fTime);

	if (GET_SINGLE(CInput)->IsMouseUp(MOUSE_BUTTON_TYPE::RIGHT))
	{
		Vector3 vPos = GET_SINGLE(CInput)->GetWorldMousePos();

		//m_pNav->FindPath(GetWorldPos(), vPos);
		int i = 0;
	}

	/*Vector3 vDir = m_vMovePos - GetWorldPos();
	float fLength = vDir.Length();
	if (fLength > 300.f * fTime)
	{
		vDir /= fLength;

		AddWorldPos(vDir * 300.f * fTime);
	}*/

	if (m_pTerrain)
	{
		Vector3 vPos = GetWorldPos();

		float fHeight = m_pTerrain->GetHeight(vPos);

		//if (vPos.y < fHeight)
		{
			SetWorldPos(vPos.x, fHeight, vPos.z);
		}
	}
}

void CPlayer3D::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CPlayer3D::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CPlayer3D::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CPlayer3D::Render(float fTime)
{
	CObj::Render(fTime);
}

void CPlayer3D::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}

CPlayer3D* CPlayer3D::Clone(float fTime)
{
	return new CPlayer3D(*this);
}

void CPlayer3D::Save(FILE* pFile)
{
	CObj::Save(pFile);
}

void CPlayer3D::Load(FILE* pFile)
{
	CObj::Load(pFile);
}

void CPlayer3D::MoveH(float fScale, float fTime)
{
	if (fScale != 0.f)
	AddWorldPos(GetWorldAxis(WORLD_AXIS::AXIS_X) * fScale * fTime * 300.f);
if(fScale !=0.f)
	m_pAni->ChangeSequence("Walk");
else
m_pAni->ChangeSequence("Idle");
}

void CPlayer3D::MoveV(float fScale, float fTime)
{
	if (fScale != 0.f)
	AddWorldPos(GetWorldAxis(WORLD_AXIS::AXIS_Z) * fScale * fTime * 300.f);
	/*if (fScale != 0.f)
		m_pAni->ChangeSequence("Walk");
	else
		m_pAni->ChangeSequence("Idle");*/
}

void CPlayer3D::Space(float, float)
{
	if (m_pPaper)
	{
		m_pPaper->Reset();
	}

 	m_pAni->SetFrame(0);
}

void CPlayer3D::Yaw(float fScale, float fTime)
{
	if (fScale != 0.f)
	AddQuaternionRotNorm(Vector4(1.f, 0.f, 0.f, 0.f), fScale * fTime * 60.f);
	//AddWorldRotX(fScale * fTime * 60.f);
}

void CPlayer3D::Pitch(float fScale, float fTime)
{
	if (fScale != 0.f)
	AddQuaternionRotNorm(Vector4(0.f, 1.f, 0.f, 0.f), fScale * fTime * 60.f);
	//AddWorldRotY(fScale * fTime * 60.f);
}

void CPlayer3D::Roll(float fScale, float fTime)
{
	if(fScale!=0.f)
	AddQuaternionRotNorm(Vector4(0.f, 0.f, 1.f, 0.f), fScale * fTime * 60.f);
	//AddWorldRotZ(fScale * fTime * 60.f);
}
