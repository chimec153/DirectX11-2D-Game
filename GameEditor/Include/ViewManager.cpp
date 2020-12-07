#include "pch.h"
#include "ViewManager.h"
#include "Scene/SceneManager.h"
#include "WorldDlg.h"
#include "DetailDlg.h"
#include "Engine.h"

DEFINITION_SINGLE(CViewManager)

CViewManager::CViewManager()	:
	m_pWorldDlg(nullptr),
	m_pDetailDlg(nullptr)
{

}

CViewManager::~CViewManager()
{
	GET_SINGLE(CSceneManager)->GetScene()->DelEditDeFunc();
}

bool CViewManager::Init(CWorldDlg* pWorld, CDetailDlg* pDetail)
{
	m_pWorldDlg = pWorld;
	m_pDetailDlg = pDetail;

	GET_SINGLE(CSceneManager)->SetEditorCreate(m_pWorldDlg, &CWorldDlg::AddObject);
	GET_SINGLE(CSceneManager)->SetEditorDelete(m_pWorldDlg, &CWorldDlg::DeleteObject);

	GET_SINGLE(CEngine)->SetEditorUpdate(m_pDetailDlg, &CDetailDlg::Update);

	return true;
}
