// Page2.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "Page2.h"


// CPage2 对话框

IMPLEMENT_DYNAMIC(CPage2, CMyDialog)

CPage2::CPage2(CWnd* pParent /*=NULL*/)
: CMyDialog(CPage2::IDD, pParent)
{
}

CPage2::~CPage2()
{
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
    CMyDialog::DoDataExchange(pDX);
    for (int i = 0; i < 10; i++)
        DDX_CBIndex(pDX, IDC_COMBO1 + i, m_nMonster[i]);

    for (int i = 0; i < 10; i++)
        DDX_Text(pDX, IDC_EDIT1 + i, m_Price[i]);

}

void CPage2::RestoreState()
{
    CString str;
    for (int i = 0; i < 10; i++)
    {
        str.Format(_T("monster%d"), i);
		m_nMonster[i] = theApp.GetProfileInt(_T("ShopSetting"), str, 0);
    }

    str.Empty();
    for (int i = 0; i < 10; i++)
    {
        str.Format(_T("Price%d"), i);
		m_Price[i]	= theApp.GetProfileInt(_T("ShopSetting"), str, 0);
    }

    UpdateData(FALSE);
}

void CPage2::SaveState()
{
    UpdateData();

    CString str;
    for (int i = 0; i < 10; i++)
    {
        if (m_nMonster[i] != -1)
        {
            str.Format(_T("monster%d"), i);
			theApp.WriteProfileInt(_T("ShopSetting"), str, m_nMonster[i]);
        }
    }

    str.Empty();
    for (int i = 0; i < 10; i++)
    {
        str.Format(_T("Price%d"), i);
        theApp.WriteProfileInt(_T("ShopSetting"), str, m_Price[i]);
    }
}

BEGIN_MESSAGE_MAP(CPage2, CMyDialog)
    ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT10, &CPage2::OnChange)
    ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO1, IDC_COMBO10, &CPage2::OnChange)
END_MESSAGE_MAP()


// CPage2 消息处理程序

BOOL CPage2::OnInitDialog()
{
    CMyDialog::OnInitDialog();

    const CString MonsterList[44] = {
        _T(""), _T("大海龟"), _T("巨蛙"), _T("树怪"), _T("野猪"), _T("赌徒"), _T("山贼"), _T("强盗"),
        _T("老虎"), _T("黑熊"), _T("花妖"), _T("羊头怪"), _T("骷髅怪"), _T("蛤蟆精"), _T("狐狸精"),
        _T("小龙女"), _T("狼"), _T("虾兵"), _T("蟹将"), _T("龟丞相"), _T("牛妖"), _T("黑熊精"),_T("蜘蛛精"),
        _T("兔子怪"), _T("野鬼"), _T("僵尸"), _T("牛头"), _T("马面"), _T("雷鸟"),_T("蝴蝶仙子"),_T("古代瑞兽"),
        _T("白熊"), _T("天将"), _T("黑山老妖"), _T("天兵"), _T("风伯"),_T("地狱战神"), _T("蛟龙"), _T("凤凰"),
        _T("雨师"), _T("巡游天神"), _T("芙蓉仙子"), _T("星灵仙子"),_T("如意仙子")
    }; 


    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 44; j++)
            ((CComboBox*)GetDlgItem(IDC_COMBO1 + i))->AddString(MonsterList[j]);

    RestoreState();

    for (int i = 0; i < 10; i++)
    {
        CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1 + i);
        if (pCombox->GetCurSel() == -1)
            pCombox->SetCurSel(0);
    }

    return TRUE;
}

void CPage2::OnChange(UINT /*id*/)
{
    AfxGetMainWnd()->GetDlgItem(IDOK)->EnableWindow();
}

