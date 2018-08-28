// PreDemo.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CPreDemoApp:
// 有关此类的实现，请参阅 PreDemo.cpp
//

class CPreDemoApp : public CWinApp
{
public:
	CPreDemoApp();
    ~CPreDemoApp();
	HBITMAP  m_hBitmap;
	BOOL     m_bLoadDriver;
    BOOL     m_bShutDown;
	HANDLE   m_Mutex;
	PCTSTR   m_AppDirectory;

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
public:
    void SetShutDown(BOOL bflag) {m_bShutDown = bflag;}
};

extern CPreDemoApp theApp;