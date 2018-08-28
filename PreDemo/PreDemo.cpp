// PreDemo.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "PreDemo.h"
#include "PreDemoDlg.h"
#include "GameState.h"
#include "km.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPreDemoApp

BEGIN_MESSAGE_MAP(CPreDemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPreDemoApp 构造

CPreDemoApp::CPreDemoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
    srand(GetTickCount());
    m_bShutDown   = FALSE;
    m_bLoadDriver = FALSE;
    m_hBitmap     = NULL;
}

//析构函数
CPreDemoApp::~CPreDemoApp()
{
    if (m_bShutDown)
        ShutDown(EWX_SHUTDOWN);
}


// 唯一的一个 CPreDemoApp 对象

CPreDemoApp theApp;

// CPreDemoApp 初始化

BOOL CPreDemoApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("ASSoft"));
	_tsetlocale(0,_T(""));

	// 限制多开
	m_Mutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, m_pszAppName);
	if (m_Mutex == NULL)
	{
		m_Mutex = CreateMutex(NULL, TRUE, m_pszAppName);
	}
	else
	{
		AfxMessageBox(_T("程序已经运行！"));
		return FALSE;
	}


	CString strAppDirectory(m_pszHelpFilePath);
	strAppDirectory = strAppDirectory.Left(strAppDirectory.ReverseFind('\\') + 1);

	CImage image;
	if (image.Load(_T("Background.jpg")) != E_FAIL)
		m_hBitmap = image.Detach();

	m_pszProfileName = _tcsdup(strAppDirectory + m_pszAppName + _T(".ini"));
	m_AppDirectory   = _tcsdup(strAppDirectory);

	//加载驱动
	m_bLoadDriver = ManageDriver(KEYMOUSE_DRIVER_NAME, DRIVER_FUNC_INSTALL);
	if (!m_bLoadDriver)
	{
		AfxMessageBox(_T("驱动加载失败!"));
#ifndef _DEBUG
		return FALSE;
#endif //  _DEBUG
	}

	CPreDemoDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

int CPreDemoApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_Mutex)
		ReleaseMutex(m_Mutex);
	if (m_bLoadDriver)
		ManageDriver(KEYMOUSE_DRIVER_NAME, DRIVER_FUNC_REMOVE);

	free((PVOID)m_AppDirectory);

	return CWinApp::ExitInstance();
}
