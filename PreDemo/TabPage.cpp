// TabPage.cpp : implementation file
//

#include "stdafx.h"
#include "PreDemo.h"
#include "TabPage.h"


// CTabPage

IMPLEMENT_DYNAMIC(CTabPage, CTabCtrl)

CTabPage::CTabPage()
{
	m_nPageCount = 0;
}

CTabPage::~CTabPage()
{
}

void CTabPage::AddPage(CWnd *pParentWnd, PTSTR pTitle)
{
	TCITEM tcItem;
	memset(&tcItem, 0, sizeof(tcItem));
	tcItem.mask		= TCIF_TEXT | TCIF_PARAM;
	tcItem.pszText	= pTitle;
	tcItem.lParam	= (LPARAM)pParentWnd;

	InsertItem(m_nPageCount, &tcItem);
	m_nPageCount == 0 ? pParentWnd->ShowWindow(SW_SHOW) : pParentWnd->ShowWindow(SW_HIDE);
	m_nPageCount++;
}

void CTabPage::ShowPageWindow(int nShowState)
{
	TCITEM tcItem;
	memset(&tcItem, 0, sizeof(tcItem));
	tcItem.mask = TCIF_PARAM;

	int nItem = GetCurSel();
	GetItem(nItem, &tcItem);

	((CWnd*)(tcItem.lParam))->ShowWindow(nShowState);
}

BEGIN_MESSAGE_MAP(CTabPage, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabPage::OnTcnSelChange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, &CTabPage::OnTcnSelChanging)
END_MESSAGE_MAP()



// CTabPage message handlers

void CTabPage::OnTcnSelChanging(NMHDR *pNmhdr, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNmhdr);

	ShowPageWindow(SW_HIDE);
	*pResult = 0;
}

void CTabPage::OnTcnSelChange(NMHDR *pNmhdr, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNmhdr);

	ShowPageWindow(SW_SHOW);

	*pResult = 0;
}

