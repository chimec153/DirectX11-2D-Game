#include "StartGameMode.h"
#include "Object/Obj.h"
#include "Scene/Scene.h"
#include "UI/UIButton.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "MainGameMode.h"
#include "Engine.h"

CStartGameMode::CStartGameMode()
{
}

CStartGameMode::~CStartGameMode()
{
}

bool CStartGameMode::Init()
{
	if (!LoadResources())
		return false;

	if (!SetUI())
		return false;

	return true;
}

bool CStartGameMode::SetUI()
{
	CLayer* pUI = m_pScene->FindLayer("UI");

	CObj* pObj = m_pScene->CreateObject<CObj>("StartButton", pUI);

	CUIButton* pBtn = pObj->CreateComponent<CUIButton>("Button");

	pBtn->SetTexture(BUTTON_STATE::OFF, "Start");
	pBtn->SetTexture(BUTTON_STATE::ON, "Start");
	pBtn->SetTexture(BUTTON_STATE::PUSH, "Start");
	pBtn->SetTexture(BUTTON_STATE::DISABLE, "Start");

	pBtn->SetColor(BUTTON_STATE::OFF, 0.5f, 0.5f, 0.5f, 0.5f);
	pBtn->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	pBtn->SetColor(BUTTON_STATE::PUSH, 0.7f, 0.7f, 0.7f, 0.7f);
	pBtn->SetColor(BUTTON_STATE::DISABLE, 0.3f, 0.3f, 0.3f, 0.3f);

	pBtn->SetCallBack<CStartGameMode>(BUTTON_STATE::PUSH, this, &CStartGameMode::MainGameMode);

	pBtn->SetWorldPos(200.f, 500.f, 0.f);
	pBtn->SetPivot(0.5f, 0.5f, 0.f);
	pBtn->SetWorldScale(240.f, 120.f, 0.f);

	pObj->SetRootComponent(pBtn);

	SAFE_RELEASE(pBtn);

	SAFE_RELEASE(pObj);

	CObj* pEnd = m_pScene->CreateObject<CObj>("EndButton", pUI);

	CUIButton* pEndButton = pEnd->CreateComponent<CUIButton>("EndButton");

	pEndButton->SetTexture(BUTTON_STATE::OFF, "End");
	pEndButton->SetTexture(BUTTON_STATE::ON, "End");
	pEndButton->SetTexture(BUTTON_STATE::PUSH, "End");
	pEndButton->SetTexture(BUTTON_STATE::DISABLE, "End");

	pEndButton->SetColor(BUTTON_STATE::OFF, 0.5f, 0.5f, 0.5f, 0.5f);
	pEndButton->SetColor(BUTTON_STATE::ON, 1.f, 1.f, 1.f, 1.f);
	pEndButton->SetColor(BUTTON_STATE::PUSH, 0.7f, 0.7f, 0.7f, 0.7f);
	pEndButton->SetColor(BUTTON_STATE::DISABLE, 0.3f, 0.3f, 0.3f, 0.3f);

	pEndButton->SetCallBack<CStartGameMode>(BUTTON_STATE::PUSH, this, &CStartGameMode::MainGameMode);

	pEndButton->SetWorldPos(600.f, 500.f, 0.f);
	pEndButton->SetPivot(0.5f, 0.5f, 0.f);
	pEndButton->SetWorldScale(240.f, 120.f, 0.f);

	pEnd->SetRootComponent(pEndButton);

	SAFE_RELEASE(pEndButton);

	SAFE_RELEASE(pEnd);

	return true;
}

bool CStartGameMode::LoadResources()
{
	if (!GET_SINGLE(CResourceManager)->LoadTexture("Start", TEXT("Start.png")))
		return true;

	if (!GET_SINGLE(CResourceManager)->LoadTexture("End", TEXT("End.png")))
		return false;

	return true;
}

void CStartGameMode::MainGameMode(float)
{
	GET_SINGLE(CSceneManager)->CreateNextScene();

	GET_SINGLE(CSceneManager)->SetGameMode<CMainGameMode>(false);
}

void CStartGameMode::ExitGame(float)
{
	GET_SINGLE(CEngine);
}
