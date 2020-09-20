#include "Engine.h"
#include "Device.h"
#include "Scene/SceneManager.h"
#include "PathManager.h"
#include "Timer.h"
#include "Resource/ResourceManager.h"
#include "Resource/Mesh2D.h"
#include "Resource/ShaderManager.h"
#include "Resource/Shader.h"
#include "RenderManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_bLoop = true;

CEngine::CEngine()	:
	m_hDC(0),
	m_hWnd(0),
	m_hInst(0),
	m_tRS()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc();
}

CEngine::~CEngine()
{
	DESTROY_SINGLE(CDevice);
	DESTROY_SINGLE(CSceneManager);
	DESTROY_SINGLE(CResourceManager);
	DESTROY_SINGLE(CRenderManager);
	DESTROY_SINGLE(CPathManager);
	DESTROY_SINGLE(CTimer);
}

bool CEngine::Init(const TCHAR* pClass, const TCHAR* pTitle,
	HINSTANCE hInst, int iWidth, int iHeight, bool bWindowMode)
{
	m_hInst = hInst;

	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	Register(pClass);
	Create(pClass, pTitle, m_tRS.iWidth, m_tRS.iHeight);

	return Init(hInst, m_hWnd, pClass, m_tRS.iWidth, m_tRS.iHeight, bWindowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, const TCHAR* pClass, int iWidth, int iHeight, bool bWindowMode)
{
	m_hWnd = hWnd;
	m_hInst = hInst;

	m_tRS.iWidth = iWidth;
	m_tRS.iHeight = iHeight;

	// 장치 초기화
	if (!GET_SINGLE(CDevice)->Init(hWnd, iWidth, iHeight, bWindowMode))
		return false;

	// 시간 관리자 초기화
	if (!GET_SINGLE(CTimer)->Init())
		return false;

	// 경로 관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// 자원 관리자 초기화
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

	// 장면 관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	// 랜더 관리자 초기화
	if (!GET_SINGLE(CRenderManager)->Init())
		return false;

	return true;
}

int CEngine::Run()
{
	MSG msg = {};

	// 기본 메시지 루프입니다:
	while (m_bLoop)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	GET_SINGLE(CTimer)->Update();

	float fTime = GET_SINGLE(CTimer)->GetDeltaTime();

	Input(fTime);
	Update(fTime);
	Collision(fTime);
	Render(fTime);
}

int CEngine::Input(float fTime)
{
	GET_SINGLE(CSceneManager)->Input(fTime);

	return 0;
}

int CEngine::Update(float fTime)
{
	GET_SINGLE(CSceneManager)->Update(fTime);

	GET_SINGLE(CSceneManager)->PostUpdate(fTime);

	return 0;
}

int CEngine::Collision(float fTime)
{
	GET_SINGLE(CSceneManager)->Collision(fTime);

	return 0;
}

int CEngine::Render(float fTime)
{
	GET_SINGLE(CSceneManager)->PreRender(fTime);

	GET_SINGLE(CDevice)->ClearState();

	//GET_SINGLE(CSceneManager)->Render(fTime);
	GET_SINGLE(CRenderManager)->Render(fTime);

	GET_SINGLE(CRenderManager)->ClearComponent();

	GET_SINGLE(CDevice)->Render();

	GET_SINGLE(CSceneManager)->PostRender(fTime);
	return 0;
}

int CEngine::Create(const TCHAR* pClass, const TCHAR* pTitle, int iWidth, int iHeight)
{
	m_hWnd = CreateWindowW(pClass, pTitle, WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT tRect = { 0, 0, iWidth, iHeight };

	AdjustWindowRect(&tRect, WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, false);
	SetWindowPos(m_hWnd, 0, 100, 100, 
		tRect.right - tRect.left, tRect.bottom - tRect.top, SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return 0;
}

int CEngine::Register(const TCHAR* pClass)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

LRESULT __stdcall CEngine::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		m_bLoop = false;
		break;
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

	return 0;
}
