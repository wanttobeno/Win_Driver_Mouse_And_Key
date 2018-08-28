// PreDemoDlg.h : 头文件
//

#pragma once
#include "MyDialog.h"
#include "Page1.h"
#include "Page2.h"
#include "Page3.h"
#include "Page4.h"
#include "TabPage.h"
#include "ThreadProc.h"


// CPreDemoDlg 对话框
class CPreDemoDlg : public CMyDialog
{
    // 构造
public:
    CPreDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

    // 对话框数据
    enum { IDD = IDD_PREDEMO_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


    // 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
private:
    CTabPage m_TabPage;
    CPage1   m_Page1;
    CPage2   m_Page2;
    CPage3   m_Page3;
    CPage4	 m_Page4;
    NOTIFYICONDATA	m_NotifyIcon;
    CWinThread     *m_pHostThread;
    BOOL		    m_bThread;
    PARAMS		    params;

public:
    afx_msg void OnBnClickedBtnStart();
    afx_msg void OnBnClickedBtnPause();
    afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
    afx_msg LRESULT OnMyNotify(WPARAM wParam, LPARAM lParam);
    afx_msg void OnNotifyAboutbox();
    afx_msg void OnNotifyShow();
    afx_msg void OnNotifyClose();
    afx_msg void OnBnClickedCancel();
    void OnScreenshot();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
