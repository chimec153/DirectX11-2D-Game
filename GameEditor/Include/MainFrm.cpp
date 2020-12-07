
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "GameEditor.h"

#include "MainFrm.h"
#include "Engine.h"
#include "WorldDlg.h"
#include "DetailDlg.h"
#include "GameEditorView.h"
#include "Scene/SceneManager.h"
#include "Client/GameMode/MainGameMode.h"
#include "ViewManager.h"
#include "PathManager.h"
#include "AniEdit.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_LOAD, &CMainFrame::OnLoad)
	ON_COMMAND(ID_SAVE, &CMainFrame::OnSave)
	ON_COMMAND(ID_32775, &CMainFrame::OnAniEdit)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept	:
	m_pEdit(nullptr)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	SAFE_DELETE(m_pEdit);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.cx = 1280;
	cs.cy = 720;
	cs.x = 0;
	cs.y = 0;
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	m_hSplit.CreateStatic(this, 1, 2);
	m_hSplitChild.CreateStatic(&m_hSplit, 2, 1, 
		WS_VISIBLE | WS_BORDER | WS_CHILD, m_hSplit.IdFromRowCol(0, 1));

	m_hSplit.CreateView(0, 0, RUNTIME_CLASS(CGameEditorView),
		CSize(1500, 1080), pContext);

	m_hSplitChild.CreateView(0, 0, RUNTIME_CLASS(CWorldDlg),
		CSize(420, 500), pContext);

	m_hSplitChild.CreateView(1, 0, RUNTIME_CLASS(CDetailDlg),
		CSize(420, 580), pContext);

	m_hSplit.SetActiveWindow();

	GET_SINGLE(CViewManager)->Init((CWorldDlg*)m_hSplitChild.GetPane(0, 0), (CDetailDlg*)m_hSplitChild.GetPane(1, 0));

	return TRUE;
}


void CMainFrame::OnLoad()
{
	const TCHAR* strFilter = TEXT("장면파일(*.fws)|*.fws|모든파일(*.*)|*.*||");

	CFileDialog dlg(TRUE, TEXT("*.fws"), TEXT("Scene"), OFN_HIDEREADONLY, strFilter,  this);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		std::string strFileName = CT2CA(dlg.GetFileName());

		GET_SINGLE(CSceneManager)->Load(strFileName.c_str());
	}
}


void CMainFrame::OnSave()
{
	const TCHAR* strFilter = TEXT("장면파일(*.fws)|*.fws|모든파일(*.*)|*.*||");

	CFileDialog dlg(FALSE, TEXT("*.fws"), TEXT("Scene"), OFN_OVERWRITEPROMPT, strFilter, this);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(DATA_PATH);

	if (dlg.DoModal() == IDOK)
	{
		std::string strFileName = CT2CA(dlg.GetFileName());

		GET_SINGLE(CSceneManager)->Save(strFileName.c_str());
	}
}


void CMainFrame::OnAniEdit()
{
	if (!m_pEdit)
	{
		m_pEdit = new CAniEdit;

		m_pEdit->Create(IDD_DIALOG_ANI_EDIT);
	}

	m_pEdit->ShowWindow(SW_SHOW);
}
