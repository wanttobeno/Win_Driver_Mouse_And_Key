// MyHotKeyCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "MyHotKeyCtrl.h"


// CMyHotKeyCtrl

IMPLEMENT_DYNAMIC(CMyHotKeyCtrl, CHotKeyCtrl)

CMyHotKeyCtrl::CMyHotKeyCtrl()
{

}

CMyHotKeyCtrl::~CMyHotKeyCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyHotKeyCtrl, CHotKeyCtrl)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CMyHotKeyCtrl 消息处理程序



void CMyHotKeyCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	AfxGetMainWnd()->GetDlgItem(IDOK)->EnableWindow();

	CHotKeyCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

