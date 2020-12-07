// DetailDlg.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "DetailDlg.h"
#include "Object/Obj.h"
#include "Component/SceneComponent.h"


// CDetailDlg
#undef new
IMPLEMENT_DYNCREATE(CDetailDlg, CFormView)

CDetailDlg::CDetailDlg()	:
	CFormView(IDD_DETAIL),
	m_pObj(nullptr)
{

}

CDetailDlg::~CDetailDlg()
{
}

BEGIN_MESSAGE_MAP(CDetailDlg, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DETAIL, &CDetailDlg::OnTvnSelchangedDetail)
	ON_EN_CHANGE(IDC_EDIT_POSX, &CDetailDlg::OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSY, &CDetailDlg::OnEnChangeEditPosy)
	ON_EN_CHANGE(IDC_EDIT_POSZ, &CDetailDlg::OnEnChangeEditPosz)
	ON_EN_CHANGE(IDC_EDIT_ROTX, &CDetailDlg::OnEnChangeEditRotx)
	ON_EN_CHANGE(IDC_EDIT_ROTY, &CDetailDlg::OnEnChangeEditRoty)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, &CDetailDlg::OnEnChangeEditRotz)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, &CDetailDlg::OnEnChangeEditScalex)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, &CDetailDlg::OnEnChangeEditScaley)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, &CDetailDlg::OnEnChangeEditScalez)
END_MESSAGE_MAP()


// CDetailDlg 그리기

void CDetailDlg::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDetailDlg 진단

#ifdef _DEBUG
void CDetailDlg::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetailDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetailDlg 메시지 처리기


void CDetailDlg::SetObj(CObj* pObj)
{
	m_Component.DeleteAllItems();
	m_vecItem.clear();

	m_Root = m_Component.InsertItem(TEXT("Root"), 0, 0, TVI_ROOT, TVI_LAST);

	m_pObj = pObj;

	if (!m_pObj)
		return;

	if (!m_pObj->IsActive())
		return;

	std::vector<Hierarchy> vecHier;

	m_pObj->GetAllComponentName(vecHier);

	size_t iSize = vecHier.size();

	for (size_t i = 0; i < iSize; ++i)
	{
		CString strName = CA2CT(vecHier[i].strName.c_str());

		if (vecHier[i].strParent.empty())
		{
			TreeItem tItem;

			tItem.hItem = m_Component.InsertItem(strName, 0, 0, m_Root, TVI_LAST);
			tItem.tTag = strName;

			m_vecItem.push_back(tItem);

			continue;
		}

		CString strParent = CA2CT(vecHier[i].strParent.c_str());

		size_t iTreeSize = m_vecItem.size();

		for (size_t j = 0; j < iTreeSize; ++j)
		{
			if (m_vecItem[j].tTag == strParent)
			{
				TreeItem tItem;

				tItem.hItem = m_Component.InsertItem(strName, 0, 0, m_vecItem[j].hItem, TVI_LAST);
				tItem.tTag = strName;

				m_vecItem.push_back(tItem);

				break;
			}
		}
	}
}

void CDetailDlg::DeleteObj(const std::string& strTag)
{
	if (!m_pObj)
		return;

	if (m_pObj->GetName() == strTag)
		m_pObj = nullptr;
}

void CDetailDlg::Update(float fTime)
{
	if (m_pObj)
	{
		HTREEITEM hItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(hItem);

		std::string strName = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strName);

		if (pCom)
		{
			m_vPos = pCom->GetWorldPos();
			m_vRot = pCom->GetWorldRot();
			m_vScale = pCom->GetWorldScale();

			UpdateData(FALSE);

			SAFE_RELEASE(pCom);
		}
	}
}

void CDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DETAIL, m_Component);
	DDX_Text(pDX, IDC_EDIT_POSX, m_vPos.x);
	DDX_Text(pDX, IDC_EDIT_POSY, m_vPos.y);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_vPos.z);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_vRot.x);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_vRot.y);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_vRot.z);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_vScale.x);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_vScale.y);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_vScale.z);
}


void CDetailDlg::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}


void CDetailDlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CDetailDlg::OnTvnSelchangedDetail(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CDetailDlg::OnEnChangeEditPosx()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if(pCom)
			pCom->SetWorldPos(m_vPos);

		SAFE_RELEASE(pCom);
	}
		
}


void CDetailDlg::OnEnChangeEditPosy()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldPos(m_vPos);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditPosz()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldPos(m_vPos);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditRotx()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldRot(m_vRot);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditRoty()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldRot(m_vRot);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditRotz()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldRot(m_vRot);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditScalex()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldScale(m_vScale);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditScaley()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldScale(m_vScale);

		SAFE_RELEASE(pCom);
	}
}


void CDetailDlg::OnEnChangeEditScalez()
{
	UpdateData(TRUE);

	if (m_pObj)
	{
		HTREEITEM tItem = m_Component.GetSelectedItem();

		CString strTag = m_Component.GetItemText(tItem);

		std::string strCom = CT2CA(strTag);

		CSceneComponent* pCom = m_pObj->FindSceneComponent(strCom);

		if (pCom)
			pCom->SetWorldScale(m_vScale);

		SAFE_RELEASE(pCom);
	}
}
