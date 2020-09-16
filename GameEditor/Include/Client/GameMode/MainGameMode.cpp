#include "MainGameMode.h"
#include "../Object/Player.h"
#include "Scene/Scene.h"

CMainGameMode::CMainGameMode()
{
}

CMainGameMode::~CMainGameMode()
{
}

bool CMainGameMode::Init()
{
	CPlayer* pPlayer = m_pScene->CreateObject<CPlayer>("player");

	SetPlayer(pPlayer);

	SAFE_RELEASE(pPlayer);

	return true;
}
