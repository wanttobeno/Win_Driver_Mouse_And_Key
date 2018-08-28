// MyDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "MyDialog.h"


// CMyDialog

IMPLEMENT_DYNAMIC(CMyDialog, CDialogEx)

CMyDialog::CMyDialog(UINT nIDTemplate, CWnd* pParent)
: CDialogEx(nIDTemplate, pParent)
{	
}

CMyDialog::~CMyDialog()
{
}


BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CMyDialog 消息处理程序

BOOL CMyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundImage(theApp.m_hBitmap);

	return TRUE;
}


HBRUSH CMyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetTextColor(RGB(0, 0, 255));

	return hbr;
}
