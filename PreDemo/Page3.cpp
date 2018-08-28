// Page3.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "Page3.h"


// CPage3 对话框

IMPLEMENT_DYNAMIC(CPage3, CMyDialog)

CPage3::CPage3(CWnd* pParent /*=NULL*/)
: CMyDialog(CPage3::IDD, pParent)
{
}

CPage3::~CPage3()
{
}

void CPage3::DoDataExchange(CDataExchange* pDX)
{
    CMyDialog::DoDataExchange(pDX);

    for (int i = 0; i < 11; i++)
        DDX_CBString(pDX, IDC_COMBO1 + i, m_strPos[i]);
}

void CPage3::RestoreState()
{
    CString str;
    for (int i = 0; i < 11; i++)
    {
        str.Format(_T("strPos%d"), i);
		m_strPos[i] = theApp.GetProfileString(_T("PosSetting"), str);
    }

    UpdateData(FALSE);
}

void CPage3::SaveState()
{
    UpdateData();

    CString str;
    for (int i = 0; i < 11; i++)
    {
        if (((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->GetCurSel() != -1)
        {
            str.Format(_T("strPos%d"), i);
			theApp.WriteProfileString(_T("PosSetting"), str, m_strPos[i]);
        }
    }
}

/////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPage3, CMyDialog)
    ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO1, IDC_COMBO11, &CPage3::OnChange)
END_MESSAGE_MAP()


// CPage3 消息处理程序

BOOL CPage3::OnInitDialog()
{
    CMyDialog::OnInitDialog();
    //初始化控件
    const CString strPos[21] = {
        _T("A1"), _T("A2"), _T("A3"), _T("A4"), _T("A5"),_T("B1"),_T("B2"),
        _T("B3"), _T("B4"), _T("B5"), _T("C1"), _T("C2"), _T("C3"),_T("C4"),
        _T("C5"), _T("D1"), _T("D2"), _T("D3"), _T("D4"), _T("D5")
    };

    for (int i = 0; i < 11; i++)
        for (int j = 0; j < 21; j++)
            ((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->AddString(strPos[j]);

    RestoreState();
    for (int i = 0; i < 11; i++)
    {
        CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1 + i);
        if (pCombox->GetCurSel() == -1)
            pCombox->SetCurSel(0);
    }

    return TRUE;
}

void CPage3::OnChange(UINT /*id*/)
{
    AfxGetMainWnd()->GetDlgItem(IDOK)->EnableWindow();
}

