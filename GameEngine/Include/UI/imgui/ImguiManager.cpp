#include "ImguiManager.h"
#include "imgui.h"
#include "../../Timer.h"
#include "../../TimerManager.h"
#include "../../Component/Camera.h"
#include "../../Camera/CameraManager.h"
#include "../../Device.h"

DEFINITION_SINGLE(CImguiManager)

CImguiManager::CImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui_ImplWin32_EnableDpiAwareness();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

CImguiManager::~CImguiManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();
}

bool CImguiManager::Init(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;		
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

	return true;
}

void CImguiManager::PreRender(float fTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	if (ImGui::Begin("Simulation Speed"))
	{
		CTimer* pTimer = GET_SINGLE(CTimerManager)->GetMainTimer();

		float fScale = pTimer->GetTimeScale();

		ImGui::SliderFloat("Speed Factor", &fScale, 0.f, 4.f);

		pTimer->SetTimeScale(fScale);

		ImGui::Text("Application averge %.3f ms/frame (%.1f FPS)",
			pTimer->GetDeltaTime() * 1000.f, pTimer->GetFPS());

		SAFE_RELEASE(pTimer);
	}

	bool bOpen = true;
	ImGui::ShowDemoWindow(&bOpen);

	ImGui::End();

	CCamera* pCam = GET_SINGLE(CCameraManager)->GetMainCam();

	pCam->SpawnControlWindow();

	SAFE_RELEASE(pCam);
}

void CImguiManager::Render(float fTime)
{
	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	GET_SINGLE(CDevice)->SetTarget();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
