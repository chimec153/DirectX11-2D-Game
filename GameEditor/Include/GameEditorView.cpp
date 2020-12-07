
// GameEditorView.cpp: CGameEditorView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GameEditor.h"
#endif

#include "GameEditorDoc.h"
#include "GameEditorView.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Client/GameMode/StartGameMode.h"
#include "ClientCreateSystem.h"
#include "Client/Client.h"
#include "ClientInstance.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#endif


// CGameEditorView

IMPLEMENT_DYNCREATE(CGameEditorView, CView)

BEGIN_MESSAGE_MAP(CGameEditorView, CView)
END_MESSAGE_MAP()

// CGameEditorView 생성/소멸

CGameEditorView::CGameEditorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGameEditorView::~CGameEditorView()
{
}

BOOL CGameEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CGameEditorView 그리기

void CGameEditorView::OnDraw(CDC* /*pDC*/)
{
	CGameEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CGameEditorView 진단

#ifdef _DEBUG
void CGameEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CGameEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameEditorDoc* CGameEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameEditorDoc)));
	return (CGameEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameEditorView 메시지 처리기


void CGameEditorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GET_SINGLE(CEngine)->Init(AfxGetInstanceHandle(), 
		m_hWnd, TEXT("ASSORT30"), 1280, 720, true);

	GET_SINGLE(CClientCreateSystem)->Init();

	GET_SINGLE(CEngine)->SetInstance<CClientInstance>();

	GET_SINGLE(CClient)->GlobalSettings();

	GET_SINGLE(CSceneManager)->SetGameMode<CStartGameMode>();
}
