// AniEdit.cpp: 구현 파일
//

#include "pch.h"
#include "GameEditor.h"
#include "AniEdit.h"
#include "afxdialogex.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Resource/Texture.h"
#include "Resource/Animation2DSequence.h"

// CAniEdit 대화 상자

IMPLEMENT_DYNAMIC(CAniEdit, CDialogEx)

CAniEdit::CAniEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ANI_EDIT, pParent)
	, m_strTag(_T(""))
	, m_fRate(0)
	, m_fMaxTime(0)
	, m_strTex(_T(""))
{

}

CAniEdit::~CAniEdit()
{
}

void CAniEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TAG, m_strTag);
	DDX_Text(pDX, IDC_EDIT_MAX_TIME, m_fMaxTime);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_NAME, m_strTex);
	DDX_Control(pDX, IDC_LIST_SEQUENCE, m_SeqList);
	DDX_Text(pDX, IDC_EDIT_PLAY_RATE, m_fRate);
}


BEGIN_MESSAGE_MAP(CAniEdit, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_TAG, &CAniEdit::OnEnChangeEditTag)
	ON_BN_CLICKED(IDC_BUTTON_TEXURE_LOAD, &CAniEdit::OnBnClickedButtonTexureLoad)
	ON_EN_CHANGE(IDC_EDIT_PLAY_RATE, &CAniEdit::OnEnChangeEditPlayRate)
	ON_EN_CHANGE(IDC_EDIT_MAX_TIME, &CAniEdit::OnEnChangeEditMaxTime)
	ON_EN_CHANGE(IDC_EDIT_TEXTURE_NAME, &CAniEdit::OnEnChangeEditTextureName)
	ON_BN_CLICKED(IDC_BUTTON_ADD_SEQUENCE, &CAniEdit::OnBnClickedButtonAddSequence)
	ON_LBN_SELCHANGE(IDC_LIST_SEQUENCE, &CAniEdit::OnLbnSelchangeListSequence)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SEQ, &CAniEdit::OnBnClickedButtonSaveSeq)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SEQ, &CAniEdit::OnBnClickedButtonLoadSeq)
END_MESSAGE_MAP()


// CAniEdit 메시지 처리기


void CAniEdit::OnEnChangeEditTag()
{
}


void CAniEdit::OnBnClickedButtonTexureLoad()
{
	CFileDialog dlg(TRUE, TEXT(""), TEXT(""), OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, 
		TEXT("이미지 파일(*.JPG, *.BMP, *.PNG, *.DDS)|*.jpg;*.bmp;*.png;*.dds|모든 파일(*.*)|*.*||"));

	TCHAR strFile[2048] = {};

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(TEXTURE_PATH);
	dlg.m_ofn.lpstrFile = strFile;
	dlg.m_ofn.nMaxFile = 2048;

	if (dlg.DoModal() == IDOK)
	{
		POSITION pos = dlg.GetStartPosition();

		std::vector<const TCHAR*>	vecFullPath;

		while (pos)
		{
			CString strPath = dlg.GetNextPathName(pos);

			std::wstring strName = CT2CW(strPath);

			TCHAR* pPath = new TCHAR[MAX_PATH];

			memset(pPath, 0, sizeof(TCHAR) * MAX_PATH);

			lstrcpy(pPath, strName.c_str());

			vecFullPath.push_back(pPath);
		}

		UpdateData(TRUE);

		std::string strTex = CT2CA(m_strTex);

		GET_SINGLE(CResourceManager)->LoadTextureFromFullPath(strTex, vecFullPath);

		SAFE_DELETE_ARRAY_VECLIST(vecFullPath);
	}
}


void CAniEdit::OnEnChangeEditPlayRate()
{
}


void CAniEdit::OnEnChangeEditMaxTime()
{
}


void CAniEdit::OnEnChangeEditTextureName()
{
}


void CAniEdit::OnBnClickedButtonAddSequence()
{
	UpdateData(TRUE);

	std::string strTag = CT2CA(m_strTag);
	std::string strTex = CT2CA(m_strTex);

	CTexture* pTex = GET_SINGLE(CResourceManager)->FindTexture(strTex);

	bool bResult = GET_SINGLE(CResourceManager)->CreateAni2DSeq(strTag.c_str(), pTex, m_fMaxTime, m_fRate);

	SAFE_RELEASE(pTex);

	if(bResult)
		m_SeqList.AddString(m_strTag);
}


void CAniEdit::OnLbnSelchangeListSequence()
{
}


void CAniEdit::OnBnClickedButtonSaveSeq()
{
	TCHAR* strFilter = TEXT("시퀀스 파일(*.tsq)|*.tsq|모든 파일(*.*)|*.*||");

	CFileDialog dlg(FALSE, TEXT(""),TEXT(""), OFN_OVERWRITEPROMPT, strFilter);

	dlg.m_ofn.lpstrInitialDir = GET_SINGLE(CPathManager)->FindPath(RESOURCE_PATH);

	if (dlg.DoModal() == IDOK)
	{
		CString pPath = dlg.GetPathName();

		CString pFile = dlg.GetFileName();

		int iPos = m_SeqList.GetCurSel();

		CString strSeq;

		std::string strSequence = CT2CA(strSeq);

		m_SeqList.GetText(iPos, strSeq);

		CAnimation2DSequence* pSeq = GET_SINGLE(CResourceManager)->FindAni2DSeq(strSequence);

		

		SAFE_RELEASE(pSeq);
	}
}


void CAniEdit::OnBnClickedButtonLoadSeq()
{

}
