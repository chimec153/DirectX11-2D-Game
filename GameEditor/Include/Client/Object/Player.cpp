#include "Player.h"
#include "Component/Mesh2DComponent.h"
#include "Resource/Material.h"
#include "Bullet.h"
#include "Scene/Scene.h"

CPlayer::CPlayer()	:
	m_pMesh(nullptr),
	m_pChildMesh(nullptr),
	m_fSpeed(400.f)
{
}

CPlayer::CPlayer(const CPlayer& player)	:
	CObj(player)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pChildMesh);
}

void CPlayer::SetMeshComponent(CMesh2DComponent* pMesh)
{
	SAFE_RELEASE(m_pMesh);

	m_pMesh = pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();
}

bool CPlayer::Init()
{
	m_pMesh = CreateComponent<CMesh2DComponent>("Mesh");
	m_pChildMesh = CreateComponent<CMesh2DComponent>("Mesh");

	SetRootComponent(m_pMesh);

	m_pMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pMesh->SetRelativeScale(200.f, 100.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	CMaterial* pMaterial = m_pMesh->GetMaterial();

	pMaterial->SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.f);

	SAFE_RELEASE(pMaterial);

	m_pChildMesh->SetRelativePos(200.f, 0.f, 0.f);
	m_pChildMesh->SetInheritScale(false);
	m_pChildMesh->SetRelativeScale(100.f, 50.f, 1.f);
	m_pChildMesh->SetPivot(0.5f, 0.5f, 0.f);

	m_pMesh->AddChild(m_pChildMesh);

	return true;
}

void CPlayer::Input(float fTime)
{
	if (GetAsyncKeyState('W') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_Y) * m_fSpeed * fTime);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_Y) * -m_fSpeed * fTime);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_X) * -m_fSpeed * fTime);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pMesh->AddRelativePos(m_pMesh->GetRelativeAxis(AXIS_X) * m_fSpeed * fTime);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pMesh->AddRelativeRotZ(180.f * fTime);

	if(GetAsyncKeyState('E') & 0x8000)
		m_pMesh->AddRelativeRotZ(-180.f * fTime);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

		pBullet->SetRelativePos(m_pMesh->GetRelativePos());
		pBullet->SetRelativeRot(m_pMesh->GetRelativeRot());

		pBullet->AddRelativePos(pBullet->GetRelativeAxis(AXIS_Y) * 150.f);

		SAFE_RELEASE(pBullet);
	}

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		for (int i = 0; i < 3; ++i)
		{
			CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

			Vector3 vAngle(0.f, 0.f, 15.f * (i - 1));

			pBullet->SetRelativePos(m_pMesh->GetRelativePos());
			pBullet->SetRelativeRot(m_pMesh->GetRelativeRot() + vAngle);

			pBullet->AddRelativePos(pBullet->GetRelativeAxis(AXIS_Y) * 210.f);

			SAFE_RELEASE(pBullet);
		}
	}

	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		for (int i = 0; i < 128; ++i)
		{
			CBullet* pBullet = m_pScene->CreateObject<CBullet>("Bullet");

			Vector3 vAngle(0.f, 0.f, 360.f / 128 * (i - 1));

			pBullet->SetRelativePos(m_pMesh->GetRelativePos());
			pBullet->SetRelativeRot(m_pMesh->GetRelativeRot() + vAngle);

			pBullet->AddRelativePos(pBullet->GetRelativeAxis(AXIS_Y) * 80.f);
			pBullet->SetRelativeScale(10.f, 10.f, 1.f);

			SAFE_RELEASE(pBullet);
		}
	}
}

void CPlayer::Update(float fTime)
{
	CObj::Update(fTime);
}

void CPlayer::PostUpdate(float fTime)
{
	CObj::PostUpdate(fTime);
}

void CPlayer::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CPlayer::PreRender(float fTime)
{
	CObj::PreRender(fTime);
}

void CPlayer::Render(float fTime)
{
	CObj::Render(fTime);
}

void CPlayer::PostRender(float fTime)
{
	CObj::PostRender(fTime);
}
