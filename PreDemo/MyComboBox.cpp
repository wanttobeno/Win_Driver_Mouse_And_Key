// MyComboBox.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "MyComboBox.h"


// CMyComboBox

IMPLEMENT_DYNAMIC(CMyComboBox, CComboBox)

CMyComboBox::CMyComboBox()
{
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
}

CMyComboBox::~CMyComboBox()
{
}


BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CMyComboBox 消息处理程序


void CMyComboBox::OnDestroy()
{
	CComboBox::OnDestroy();

	if (m_edit.GetSafeHwnd() != NULL)
		m_edit.UnsubclassWindow();
}


HBRUSH CMyComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		if (m_edit.GetSafeHwnd() == NULL)
			m_edit.SubclassWindow(pWnd->GetSafeHwnd());
		m_edit.SetReadOnly();
		break;
	case CTLCOLOR_STATIC:
		pDC->SetBkColor(RGB(255,255,255));
		return (HBRUSH)m_brush.GetSafeHandle();

	default:
		break;
	}
	return hbr;
}

