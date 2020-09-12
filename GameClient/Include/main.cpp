#include "Engine.h"
#include "Client.h"

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

	int iRetVal = GET_SINGLE(CEngine)->Run();
	DESTROY_SINGLE(CEngine);

	return iRetVal;
}