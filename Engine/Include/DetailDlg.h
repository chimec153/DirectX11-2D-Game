#pragma once


// CDetailDlg 보기

class CDetailDlg : public CFormView
{
	DECLARE_DYNCREATE(CDetailDlg)

protected:
	CDetailDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CDetailDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETAIL };
#endif

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	class CObj*					m_pObj;
	std::vector<TreeItem>		m_vecItem;

public:
	void SetObj(class CObj* pObj);
	void DeleteObj(const std::string& strTag);
	void Update(float fTime);

protected:
	DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	CTreeCtrl m_Component;
	afx_msg void OnTvnSelchangedDetail(NMHDR* pNMHDR, LRESULT* pResult);
	HTREEITEM	m_Root;
	afx_msg void OnEnChangeEditPosx();
	afx_msg void OnEnChangeEditPosy();
	afx_msg void OnEnChangeEditPosz();
	afx_msg void OnEnChangeEditRotx();
	afx_msg void OnEnChangeEditRoty();
	afx_msg void OnEnChangeEditRotz();
	afx_msg void OnEnChangeEditScalex();
	afx_msg void OnEnChangeEditScaley();
	afx_msg void OnEnChangeEditScalez();
	Vector3		m_vPos;
	Vector3		m_vRot;
	Vector3		m_vScale;
};


