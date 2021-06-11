#include "3D.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Client/MainMode.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	if (!CEngine::GetInst()->Init(TEXT("ASSORT 30TH Client"), 
		TEXT("ASSORT 30TH Client"), hInstance, 1280, 720, true))
		return false;

	if (!CSceneManager::GetInst()->SetGameMode<CMainMode>())
		return false;

	int iRetVal = CEngine::GetInst()->Run();

	CEngine::DestroyInst();

	return iRetVal;
}