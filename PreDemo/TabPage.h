#pragma once


// CTabPage

class CTabPage : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabPage)

public:
	CTabPage();
	virtual ~CTabPage();

private:
	int m_nPageCount;

protected:
	DECLARE_MESSAGE_MAP()
public:
	void AddPage(CWnd* pParentWnd, PTSTR pTitle);
	void ShowPageWindow(int nShowState);
	afx_msg void OnTcnSelChange(NMHDR* pNmhdr, LRESULT* pResult);
	afx_msg void OnTcnSelChanging(NMHDR* pNmhdr, LRESULT* pResult);
};

