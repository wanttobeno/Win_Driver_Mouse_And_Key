// Page1.cpp : 实现文件
//

#include "stdafx.h"
#include "PreDemo.h"
#include "Page1.h"
#include <Mmsystem.h>
#include "GameState.h"

// CPage1 对话框

IMPLEMENT_DYNAMIC(CPage1, CMyDialog)

CPage1::CPage1(CWnd* pParent /*=NULL*/)
: CMyDialog(CPage1::IDD, pParent)
, m_nSchool(0)
{
	m_nGameDelay	= 0;
	m_nLoadQuantity	= 3;
	m_nCounter		= 1;
	m_nHeathLower   = 0;
	m_nMagicLower   = 0;
	m_bNotify		= FALSE;
}

CPage1::~CPage1()
{
}

void CPage1::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nGameDelay);
	DDX_Text(pDX, IDC_EDIT2, m_nLoadQuantity);
	DDX_Text(pDX, IDC_EDIT3, m_nCounter);
	DDX_Text(pDX, IDC_EDIT4, m_nHeathLower);
	DDX_Text(pDX, IDC_EDIT5, m_nMagicLower);
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKeyStart);
	DDX_Control(pDX, IDC_HOTKEY2, m_HotKeyPause);
	DDX_Control(pDX, IDC_HOTKEY3, m_HotKeyClose);
	DDX_Control(pDX, IDC_HOTKEY4, m_HotKeyScreenshot);
	DDX_CBIndex(pDX, IDC_COMBO1, m_nSchool);
	DDX_CBIndex(pDX, IDC_COMBO2, m_nAlarm);
	DDX_Check(pDX, IDC_CHECK, m_bNotify);
	DDV_MinMaxInt(pDX, m_nHeathLower, 0, 100);
	DDV_MinMaxInt(pDX, m_nMagicLower, 0, 100);
}

void CPage1::RestoreState()
{
    m_nGameDelay	= theApp.GetProfileInt(_T("BaseSetting"), _T("GameDelay"), 0);
	m_nLoadQuantity	= theApp.GetProfileInt(_T("BaseSetting"), _T("LoadQuantity"), 3);
	m_nCounter		= theApp.GetProfileInt(_T("BaseSetting"), _T("Counter"), 1);
	m_nHeathLower   = theApp.GetProfileInt(_T("BaseSetting"), _T("HeathLower"), 0);
	m_nMagicLower   = theApp.GetProfileInt(_T("BaseSetting"), _T("MagicLower"), 0);
	m_nSchool       = theApp.GetProfileInt(_T("BaseSetting"), _T("School"), 0);
	m_nAlarm        = theApp.GetProfileInt(_T("BaseSetting"), _T("Alarm"), 0);
	m_bNotify		= theApp.GetProfileInt(_T("BaseSetting"), _T("bNotify"), 0);

    DWORD HotKeyStart      = theApp.GetProfileInt(_T("BaseSetting"), _T("HotKeyStart"), 0);
    DWORD HotKeyPause      = theApp.GetProfileInt(_T("BaseSetting"), _T("HotKeyPause"), 0);
    DWORD HotKeyClose      = theApp.GetProfileInt(_T("BaseSetting"), _T("HotKeyClose"), 0);
    DWORD HotKeyScreenshot = theApp.GetProfileInt(_T("BaseSetting"), _T("HotKeyScreenshot"), 0);

	m_HotKeyStart.SetHotKey(LOBYTE(HotKeyStart), HIBYTE(HotKeyStart));
	m_HotKeyPause.SetHotKey(LOBYTE(HotKeyPause), HIBYTE(HotKeyPause));
	m_HotKeyClose.SetHotKey(LOBYTE(HotKeyClose), HIBYTE(HotKeyClose));
    m_HotKeyScreenshot.SetHotKey(LOBYTE(HotKeyScreenshot), HIBYTE(HotKeyScreenshot));

	UpdateData(FALSE);
}

void CPage1::SaveState()
{
	int m_nLoadQuantityTemp	 = m_nLoadQuantity;
	int m_nCounterTemp		 = m_nCounter;
	int m_nSchoolTemp        = m_nSchool;
	int m_nHeathLowerTemp    = m_nHeathLower;
	int m_nMagicLowerTemp    = m_nMagicLower;
	int m_nAlarmTemp         = m_nAlarm;
	UpdateData();

	theApp.WriteProfileInt(_T("BaseSetting"), _T("GameDelay"), m_nGameDelay);
	
	if (m_nLoadQuantity < 3 || m_nLoadQuantity > 8)
	{
		AfxMessageBox(_T("请输入范围为3至8之间的数字！"));
		m_nLoadQuantity	=	m_nLoadQuantityTemp;
	}
	else
		theApp.WriteProfileInt(_T("BaseSetting"), _T("LoadQuantity"), m_nLoadQuantity);
	
	if (m_nCounter < 1 || m_nCounter > 10)
	{
		AfxMessageBox(_T("请输入范围为1至10之间的数字！"));
		m_nCounter	=	m_nCounterTemp;
	}
	else
		theApp.WriteProfileInt(_T("BaseSetting"), _T("Counter"), m_nCounter);

    if (m_nHeathLower < 0 || m_nHeathLower > 100)
		m_nHeathLower = m_nHeathLowerTemp;
	else
	    theApp.WriteProfileInt(_T("BaseSetting"), _T("HeathLower"), m_nHeathLower);

    if (m_nMagicLower < 0 || m_nMagicLower > 100)
		m_nMagicLower = m_nMagicLowerTemp;
	else
	    theApp.WriteProfileInt(_T("BaseSetting"), _T("MagicLower"), m_nMagicLower);

	if (m_nSchool == -1)
		m_nSchool = m_nSchoolTemp;
	else
		theApp.WriteProfileInt(_T("BaseSetting"), _T("School"), m_nSchool);
	
    if (m_nAlarm == -1)
		m_nAlarm = m_nAlarmTemp;
	else
	    theApp.WriteProfileInt(_T("BaseSetting"), _T("Alarm"), m_nAlarm);

	theApp.WriteProfileInt(_T("BaseSetting"), _T("bNotify"),   m_bNotify);
	theApp.WriteProfileInt(_T("BaseSetting"), _T("HotKeyStart"), m_HotKeyStart.GetHotKey());
	theApp.WriteProfileInt(_T("BaseSetting"), _T("HotKeyPause"), m_HotKeyPause.GetHotKey());
	theApp.WriteProfileInt(_T("BaseSetting"), _T("HotKeyClose"), m_HotKeyClose.GetHotKey());
    theApp.WriteProfileInt(_T("BaseSetting"), _T("HotKeyScreenshot"), m_HotKeyScreenshot.GetHotKey());
	UpdateData(FALSE);
}

//////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPage1, CMyDialog)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT5, &CPage1::OnChange)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO1, IDC_COMBO2, &CPage1::OnChange)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK, IDC_CHECK, &CPage1::OnChange)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CPage1::OnBnClickedButtonTest)
END_MESSAGE_MAP()


// CPage1 消息处理程序

BOOL CPage1::OnInitDialog()
{
	CMyDialog::OnInitDialog();
	//控件初始化
	const CString strSchool[13]	= {
		_T("大唐官府"), _T("化生寺"), _T("女儿村"), _T("方寸山"), _T("龙宫"), _T("阴曹地府"),
		_T("五庄观"), _T("天宫"), _T("盘丝洞"), _T("魔王寨"), _T("狮驼岭"), _T("普陀山")
	};	
	for (int i = 0; i < 12; i++)
        ((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(strSchool[i]);

	const CString strAlarm[2]	=	{_T("机箱报警"), _T("音箱报警")};
	for (int i = 0; i < 2; i++)
        ((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(strAlarm[i]);


	RestoreState();
    for (int i = 0; i < 2; i++)
    {
        CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1 + i);
        if (pCombox->GetCurSel() == -1)
            pCombox->SetCurSel(0);
    }
    
	return TRUE;
}

void CPage1::OnChange(UINT /*id*/)
{
	AfxGetMainWnd()->GetDlgItem(IDOK)->EnableWindow();
}


void CPage1::OnBnClickedButtonTest()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel())
	{
	case 0:
		::Beep(900, 300);
		::Beep(900, 300);
		::Beep(900, 300);
		::Beep(900, 300);
		break;
	case 1:
		::sndPlaySound(_T("wav\\BJ.wav"), SND_SYNC);
		break;
	default:
		break;
	}
}
