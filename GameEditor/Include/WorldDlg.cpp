// WorldDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "WorldDlg.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "ViewManager.h"
#include "DetailDlg.h"
#include "Object/Obj.h"

// CWorldDlg

#undef new
IMPLEMENT_DYNCREATE(CWorldDlg, CFormView)

CWorldDlg::CWorldDlg()	:
	CFormView(IDD_WORLDOBJ)
{

}

CWorldDlg::~CWorldDlg()
{
}

BEGIN_MESSAGE_MAP(CWorldDlg, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OBJ, &CWorldDlg::OnTvnSelchangedTreeObj)
END_MESSAGE_MAP()


// CWorldDlg 그리기

void CWorldDlg::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CWorldDlg 진단

#ifdef _DEBUG
void CWorldDlg::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CWorldDlg::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

void CWorldDlg::AddObject(const std::string& strTag)
{
	CString strName = CA2CT(strTag.c_str());

	TreeItem tItem;

	tItem.hItem = m_WorldObj.InsertItem(strName, 0, 0, m_hRoot, TVI_LAST);

	tItem.tTag = strName;

	m_vecItem.push_back(tItem);
}

void CWorldDlg::DeleteObject(const std::string& strTag)
{
	CString tTag = CA2CT(strTag.c_str());

	size_t iSize = m_vecItem.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		if (m_vecItem[i].tTag == tTag)
		{
			m_WorldObj.DeleteItem(m_vecItem[i].hItem);

			std::vector<TreeItem>::iterator iter = m_vecItem.begin() + i;

			m_vecItem.erase(iter);

			GET_SINGLE(CViewManager)->GetDetailDlg()->DeleteObj(strTag);

			//SAFE_RELEASE(pObj);

			break;
		}			
	}
}


// CWorldDlg 메시지 처리기


void CWorldDlg::OnTvnSelchangedTreeObj(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hItem = m_WorldObj.GetSelectedItem();

	CString strTag = m_WorldObj.GetItemText(hItem);

	std::string strName = CT2CA(strTag);

	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CObj* pObj = nullptr;

	if (pScene)
	{
		CLayer* pLayer = pScene->FindLayer("Default");

		pObj = pLayer->FindObj(strName);

		if (!pObj)
		{
			CLayer* pUI = pScene->FindLayer("UI");

			pObj = pUI->FindObj(strName);

			if (!pObj)
			{
				CLayer* pBack = pScene->FindLayer("Back");

				pObj = pBack->FindObj(strName);
			}
		}
	}

	GET_SINGLE(CViewManager)->GetDetailDlg()->SetObj(pObj);

	if (pObj)
	{
		if (pObj->IsActive())
			SAFE_RELEASE(pObj);
	}
}


void CWorldDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_hRoot = m_WorldObj.InsertItem(L"Root", 0, 0, TVI_ROOT, TVI_LAST);
}


void CWorldDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_OBJ, m_WorldObj);
}
