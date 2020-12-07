#pragma once


// CWorldDlg 보기

class CWorldDlg : public CFormView
{
	DECLARE_DYNCREATE(CWorldDlg)

protected:
	CWorldDlg();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CWorldDlg();
	
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORLDOBJ};
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
	std::vector<TreeItem>		m_vecItem;

public:
	void AddObject(const std::string& strTag);
	void DeleteObject(const std::string& strTag);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTreeObj(NMHDR* pNMHDR, LRESULT* pResult);
	HTREEITEM	m_hRoot;
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX);
	CTreeCtrl m_WorldObj;
};


