
// CMyDialog
#pragma once


class CMyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDialog)

public:
	CMyDialog(UINT nIDTemplate, CWnd* pParent = NULL);
	virtual ~CMyDialog();

protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnOK(){}
	virtual void OnCancel(){}
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


