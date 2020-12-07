#include "Engine.h"
#include "Client.h"
#include "Client/Client.h"
#include "Client/GameMode/MainGameMode.h"
#include "Scene/SceneManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!GET_SINGLE(CEngine)->Init(TEXT("ASSORT30"), TEXT("ASSORT30"), hInstance, 1280, 720))
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	if (!GET_SINGLE(CClient)->GlobalSettings())
	{
		DESTROY_SINGLE(CEngine);
		return 0;
	}

	GET_SINGLE(CSceneManager)->SetGameMode<CMainGameMode>();

	int iRetVal = GET_SINGLE(CEngine)->Run();
	DESTROY_SINGLE(CEngine);

	DESTROY_SINGLE(CClient);

	return iRetVal;
}