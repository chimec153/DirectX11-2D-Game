#include "Player.h"
#include "Component/Mesh2DComponent.h"

CPlayer::CPlayer()	:
	m_pMesh(nullptr)
{
}

CPlayer::CPlayer(const CPlayer& player)	:
	CObj(player)
{
}

CPlayer::~CPlayer()
{
	SAFE_RELEASE(m_pMesh);
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

	SetRootComponent(m_pMesh);

	return true;
}

void CPlayer::Update(float fTime)
{
}

void CPlayer::PostUpdate(float fTime)
{
}

void CPlayer::Collision(float fTime)
{
}

void CPlayer::PreRender(float fTime)
{
}

void CPlayer::Render(float fTime)
{
}

void CPlayer::PostRender(float fTime)
{
}
