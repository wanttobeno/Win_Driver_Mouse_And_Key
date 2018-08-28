#pragma once
#include "MyDialog.h"


// CPage3 对话框

class CPage3 : public CMyDialog
{
	DECLARE_DYNAMIC(CPage3)

public:
	CPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SaveState();
	void RestoreState();
private:
	CString m_strPos[11];
public:
	afx_msg void OnChange(UINT id);
};
