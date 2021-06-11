#include "Engine.h"
#include "Client.h"
#include "Client/Client.h"
#include "Client/GameMode/StartGameMode.h"
#include "Scene/SceneManager.h"
#include "Client/BossManager.h"
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "Client/SocketManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!GET_SINGLE(CEngine)->Init(TEXT("TITAN SOULS"), TEXT("TITAN SOULS"), hInstance, 1920, 1080))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	GET_SINGLE(CSocketManager)->Init();

	if (!GET_SINGLE(CClient)->GlobalSettings())
	{
		DESTROY_SINGLE(CEngine);
		DESTROY_SINGLE(CSocketManager);
		DESTROY_SINGLE(CClient);
		return 0;
	}

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>();

	int iRetVal = GET_SINGLE(CEngine)->Run();

	DESTROY_SINGLE(CBossManager);

	DESTROY_SINGLE(CEngine);

	DESTROY_SINGLE(CClient);

	DESTROY_SINGLE(CSocketManager);

	return iRetVal;
}