#pragma once


// CAniEdit 대화 상자

class CAniEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CAniEdit)

public:
	CAniEdit(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAniEdit();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANI_EDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTag;
	afx_msg void OnEnChangeEditTag();
	afx_msg void OnBnClickedButtonTexureLoad();
	float m_fRate;
	afx_msg void OnEnChangeEditPlayRate();
	float m_fMaxTime;
	afx_msg void OnEnChangeEditMaxTime();
	CString m_strTex;
	afx_msg void OnEnChangeEditTextureName();
	afx_msg void OnBnClickedButtonAddSequence();
	CListBox m_SeqList;
	afx_msg void OnLbnSelchangeListSequence();
	afx_msg void OnBnClickedButtonSaveSeq();
	afx_msg void OnBnClickedButtonLoadSeq();
};
