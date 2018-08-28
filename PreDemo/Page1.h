#pragma once
#include "MyDialog.h"
#include "MyHotKeyCtrl.h"


// CPage1 对话框

class CPage1 : public CMyDialog
{
	DECLARE_DYNAMIC(CPage1)

public:
	CPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage1();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SaveState();
	void RestoreState();
private:
	int m_nGameDelay;
	int m_nLoadQuantity;
	int m_nCounter;
	int m_nSchool;
	int m_nHeathLower;
    int m_nMagicLower;
    int m_nAlarm;
	BOOL m_bNotify;
	CMyHotKeyCtrl m_HotKeyStart;
	CMyHotKeyCtrl m_HotKeyPause;
	CMyHotKeyCtrl m_HotKeyClose;
    CMyHotKeyCtrl m_HotKeyScreenshot;
public:
	afx_msg void OnChange(UINT id);
	afx_msg void OnBnClickedButtonTest();   
};
