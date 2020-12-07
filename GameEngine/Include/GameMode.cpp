#include "GameMode.h"
#include "Object/Obj.h"
#include "Scene/Scene.h"
#include "Collision/CollisionManager.h"

CGameMode::CGameMode()	:
	m_pScene(nullptr),
	m_pPlayer(nullptr)
{	
}

CGameMode::~CGameMode()
{
	SAFE_RELEASE(m_pPlayer);
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

bool CGameMode::Init()
{
	return true;
}

void CGameMode::Start()
{
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
