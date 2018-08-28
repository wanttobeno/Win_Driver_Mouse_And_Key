// km.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <winsvc.h>
#include "km.h"
#include "GameState.h"
#include "PreDemo.h"


BOOL MouseMove(POINT ptTarget, USHORT Flags = MOUSE_MOVE_RELATIVE);

BOOL MouseMove(LONG dx, LONG dy, USHORT Flags = MOUSE_MOVE_RELATIVE);

BOOL KeyboardButton(USHORT VirtualKey, USHORT Flags);

BOOL MouseButton(USHORT ButtonFlags);

BOOL InstallDriver(SC_HANDLE  SchSCManager, LPCTSTR DriverName, LPCTSTR ServiceExe);

BOOL RemoveDriver(SC_HANDLE  SchSCManager, LPCTSTR DriverName);

BOOL StartDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName);

BOOL StopDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName);

BOOL SetMouseInsideTheGameWindow(HWND hWnd, LPCRECT pRectTarget, POINT & ptGameCursor);



BOOL InstallDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName, LPCTSTR ServiceExe)
{
    SC_HANDLE   schService;
    DWORD       err;

    schService = CreateService(SchSCManager,           // handle of service control manager database
        DriverName,             // address of name of service to start
        DriverName,             // address of display name
        SERVICE_ALL_ACCESS,     // type of access to service
        SERVICE_KERNEL_DRIVER,  // type of service
        SERVICE_DEMAND_START,   // when to start service
        SERVICE_ERROR_NORMAL,   // severity if service fails to start
        ServiceExe,             // address of name of binary file
        NULL,                   // service does not belong to a group
        NULL,                   // no tag requested
        NULL,                   // no dependency names
        NULL,                   // use LocalSystem account
        NULL                    // no password for service account
        );

    if (schService == NULL) {

        err = GetLastError();
        if (err == ERROR_SERVICE_EXISTS)
            return TRUE;
        else {
            TRACE(_T("CreateService failed!  Error = %d \n"), err );
            return FALSE;
        }
    }


    if (schService)
        CloseServiceHandle(schService);

    return TRUE;

}   // InstallDriver

BOOL ManageDriver(LPCTSTR DriverName, USHORT Function )
{

    TCHAR       ServiceName[MAX_PATH] = {'\0'};
    SC_HANDLE   schSCManager;

    BOOL rCode = TRUE;

    if (!DriverName) {
        TRACE(_T("Invalid Driver or Service provided to ManageDriver() \n"));
        return FALSE;
    }

    schSCManager = OpenSCManager(NULL, NULL,  SC_MANAGER_ALL_ACCESS);

    if (!schSCManager) {
        TRACE(_T("Open SC Manager failed! Error = %d \n"), GetLastError());
        return FALSE;
    }

    switch( Function ) {

        case DRIVER_FUNC_INSTALL:

            wsprintf(ServiceName, _T("%s%s.sys"), theApp.m_AppDirectory, KEYMOUSE_DRIVER_NAME);

            if (InstallDriver(schSCManager, DriverName, ServiceName ))
                rCode = StartDriver(schSCManager, DriverName);
            else
                rCode = FALSE;

            break;

        case DRIVER_FUNC_REMOVE:

            StopDriver(schSCManager, DriverName );
            RemoveDriver(schSCManager, DriverName );
            rCode = TRUE;
            break;

        default:
            TRACE(_T("Unknown ManageDriver() function. \n"));
            rCode = FALSE;
            break;
    }

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    return rCode;

}   // ManageDriver


BOOL RemoveDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName )
{
    SC_HANDLE   schService;
    BOOL        rCode;

    schService = OpenService(SchSCManager, DriverName, SERVICE_ALL_ACCESS);

    if (schService == NULL) {
        TRACE(_T("OpenService failed!  Error = %d \n"), GetLastError());
        return FALSE;
    }

    if (DeleteService(schService))
        rCode = TRUE;
    else {

        TRACE(_T("DeleteService failed!  Error = %d \n"), GetLastError());
        rCode = FALSE;
    }

    if (schService)
        CloseServiceHandle(schService);

    return rCode;

}   // RemoveDriver



BOOL StartDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName )
{
    SC_HANDLE   schService;
    DWORD       err;

    schService = OpenService(SchSCManager, DriverName, SERVICE_ALL_ACCESS);

    if (schService == NULL) {

        TRACE(_T("OpenService failed!  Error = %d \n"), GetLastError());
        return FALSE;
    }

    if (!StartService(schService, 0, NULL)) {

        err = GetLastError();
        if (err == ERROR_SERVICE_ALREADY_RUNNING)
            return TRUE;
        else {

            TRACE(_T("StartService failure! Error = %d \n"), err );
            return FALSE;
        }
    }

    if (schService)
        CloseServiceHandle(schService);

    return TRUE;

}   // StartDriver



BOOL StopDriver(SC_HANDLE SchSCManager, LPCTSTR DriverName)
{
    BOOL            rCode = TRUE;
    SC_HANDLE       schService;
    SERVICE_STATUS  serviceStatus;

    schService = OpenService(SchSCManager, DriverName, SERVICE_ALL_ACCESS);

    if (schService == NULL) {
        TRACE(_T("OpenService failed!  Error = %d \n"), GetLastError());
        return FALSE;
    }

    if (ControlService(schService, SERVICE_CONTROL_STOP, &serviceStatus))
        rCode = TRUE;
    else {

        TRACE(_T("ControlService failed!  Error = %d \n"), GetLastError() );
        rCode = FALSE;
    }

    if (schService)
        CloseServiceHandle(schService);

    return rCode;

}   //  StopDriver


/******************************************************************
*
*   以下函数模拟键盘和鼠标操作，依赖于驱动文件kmclass.sys
*
******************************************************************/

//参数1 VirtualKey,虚拟键码
//参数2 Flags,按键的状态
BOOL KeyboardButton(USHORT VirtualKey, USHORT Flags)
{
    KEYBOARD_INPUT_DATA  kid ;
    DWORD dwOutput;

    HANDLE hDevice = CreateFile(KEYMOUSE_WIN32_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
        return FALSE;

    memset(&kid, 0, sizeof(KEYBOARD_INPUT_DATA));

    kid.Flags    = Flags;
    kid.MakeCode = (USHORT)MapVirtualKey(VirtualKey, 0);

    BOOL bRet = DeviceIoControl(hDevice, IOCTL_KEYBOARD, &kid, sizeof(KEYBOARD_INPUT_DATA), NULL, 0, &dwOutput, NULL);

    if (!bRet)
        TRACE(_T("Error! please open the simulate kmclass driver!\n"));
    CloseHandle(hDevice);

    return bRet;
}


// 输入数字
BOOL InputNumber(int number)
{
    if (number == 0)
        return TRUE;

    TCHAR str[MAX_PATH];
    _itot_s(number, str, MAX_PATH, 10);

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!KeyboardButton(str[i] - 48 + VK_NUMPAD0, KEY_DOWN))
            return FALSE;
        Sleep(60 + rand()%30);
        if (!KeyboardButton(str[i] - 48 + VK_NUMPAD0, KEY_UP))
            return FALSE;
        Sleep(60 + rand()%30);
    }
    Sleep(500);

    return TRUE;
}

BOOL KeyboardButtonEx(USHORT VirtualKey, USHORT ControlVirtualKey)
{
    if (ControlVirtualKey != KEY_BLANK)
    {
        if (!KeyboardButton(ControlVirtualKey, KEY_DOWN))
            return FALSE;
        Sleep(70 + rand()%50);
    }
    if (!KeyboardButton(VirtualKey, KEY_DOWN))
        return FALSE;
    Sleep(70 + rand()%50);
    if (!KeyboardButton(VirtualKey, KEY_UP))
        return FALSE;
    Sleep(70 + rand()%50);
    if (ControlVirtualKey != KEY_BLANK)
    {
        if (!KeyboardButton(ControlVirtualKey, KEY_UP))
            return FALSE;
    }
    Sleep(500);

    return TRUE;
}


/*参数
*x,x坐标;
*y,y坐标;
*Flags,使用相对坐标(MOUSE_MOVE_RELATIVE)还是绝对坐标(MOUSE_MOVE_ABSOLUTE)
*/
BOOL MouseMove(LONG dx, LONG dy, USHORT Flags)
{
    MOUSE_INPUT_DATA  mid ;
    DWORD dwOutput;

    HANDLE hDevice = CreateFile(KEYMOUSE_WIN32_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
        return FALSE;

    memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));

    mid.Flags = Flags;
    switch (mid.Flags)
    {
    case MOUSE_MOVE_RELATIVE:
        mid.LastX = dx;
        mid.LastY = dy;
        break;
    case MOUSE_MOVE_ABSOLUTE:
        mid.LastX = dx * 0xffff / GetSystemMetrics(SM_CXSCREEN);
        mid.LastY = dy * 0xffff / GetSystemMetrics(SM_CYSCREEN);
        break;
    default:
        TRACE(_T("Flags: Parameter error!\n"));
        return FALSE;
    }

    BOOL bRet = DeviceIoControl(hDevice, IOCTL_MOUSE, &mid, sizeof(MOUSE_INPUT_DATA), NULL, 0, &dwOutput, NULL);

    if (!bRet)
        TRACE(_T("Error! please open the simulate kmclass driver!\n"));
    CloseHandle(hDevice);

    return bRet;
}


BOOL MouseMove(POINT ptTarget, USHORT Flags)
{
    return MouseMove(ptTarget.x, ptTarget.y, Flags);
}


//ButtonFlags,鼠标按钮状态标志
BOOL MouseButton(USHORT ButtonFlags)
{
    MOUSE_INPUT_DATA  mid;
    DWORD dwOutput;

    HANDLE hDevice = CreateFile(KEYMOUSE_WIN32_DEVICE_NAME, GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
        return FALSE;

    memset(&mid, 0, sizeof(MOUSE_INPUT_DATA));

    mid.ButtonFlags = ButtonFlags;

    BOOL bRet = DeviceIoControl(hDevice, IOCTL_MOUSE, &mid, sizeof(MOUSE_INPUT_DATA), NULL, 0, &dwOutput, NULL);

    if (!bRet)
        TRACE(_T("Error! please open the simulate kmclass driver!\n"));
    CloseHandle(hDevice);

    return bRet;
}


BOOL MouseButtonEx(USHORT ButtonFlags)
{
    if (ButtonFlags == MOUSE_LEFT_BUTTON)
    {
        if (!MouseButton(MOUSE_LEFT_BUTTON_DOWN))
            return FALSE;
        Sleep(80 + rand()%40);
        if (!MouseButton(MOUSE_LEFT_BUTTON_UP))
            return FALSE;
        Sleep(280 + rand()%40);
        return TRUE;
    }
    else if (ButtonFlags == MOUSE_RIGHT_BUTTON)
    {
        if (!MouseButton(MOUSE_RIGHT_BUTTON_DOWN))
            return FALSE;
        Sleep(80 + (rand()%40));
        if (!MouseButton(MOUSE_RIGHT_BUTTON_UP))
            return FALSE;
        Sleep(280 + rand()%40);
        return TRUE;
    }
    else
        return FALSE;
}


BOOL SetMouseInsideTheGameWindow(HWND hWnd, LPCRECT pRectTarget, POINT & ptGameCursor)
{
    POINT ptSystemCursor, ptTarget;
    CRect rect;
    GetClientRect(hWnd, &rect);


    // 修正游戏光标或者系统光标超界问题
    GetCursorPos(&ptSystemCursor);
    ScreenToClient(hWnd, &ptSystemCursor);
    ptTarget = CRect(pRectTarget).CenterPoint();
    while (!(GetGameCursorPos(hWnd, ptGameCursor) && rect.PtInRect(ptSystemCursor)))
    {
        if (ptTarget.x < rect.left+100)
            ptTarget.x += 20;
        else if (ptTarget.x > rect.right-100)
            ptTarget.x -= 20;

        if (ptTarget.y < rect.top+100)
            ptTarget.y += 20;
        else if (ptTarget.y > rect.bottom-100)
            ptTarget.y -= 20;

        TRACE(_T("修正目标点坐标: (%d, %d)\n"), ptTarget.x, ptTarget.y);
        if(!MouseMove(ptTarget.x - ptSystemCursor.x, ptTarget.y - ptSystemCursor.y, MOUSE_MOVE_RELATIVE))
            return FALSE;
        Sleep(200 + rand()%60);
        GetCursorPos(&ptSystemCursor);
        ScreenToClient(hWnd, &ptSystemCursor);
    }

    return TRUE;
}

BOOL MouseMoveEx(HWND hWnd, LPCRECT pRectTarget)
{    
    POINT ptGameCursor;
    CRect rect;
    GetClientRect(hWnd, &rect);

    const POINT ptTarget = CRect(pRectTarget).CenterPoint();

    //  光标超界修正
    if (!SetMouseInsideTheGameWindow(hWnd, pRectTarget, ptGameCursor))
        return FALSE;

    // 判断游戏光标是否在目标区域，FALSE则模拟移动鼠标
    long dx, dy, dxTemp, dyTemp;
    while (!CRect(pRectTarget).PtInRect(ptGameCursor))
    {
        dx = ptTarget.x - ptGameCursor.x;
        dy = ptTarget.y - ptGameCursor.y;

        for (int i = 0; i < 50; i++)
        {
            dxTemp = (long)(dx * (1 - i / 50.0));
            dyTemp = (long)(dy * (1 - i / 50.0));
            TRACE(_T("移动超界修正：%f\n"), 1 - (i + 1) / 50.0);

            if(rect.PtInRect(CPoint(ptGameCursor.x + (long)(1.25 * dxTemp), ptGameCursor.y + (long)(1.25 * dyTemp))))
            {
                dx = dxTemp;
                dy = dyTemp;
                break;
            }
        }

        if (!MouseMove(dx, dy, MOUSE_MOVE_RELATIVE))
            return FALSE;
        Sleep(200 + rand()%60);

        //  光标超界修正
        if (!SetMouseInsideTheGameWindow(hWnd, pRectTarget, ptGameCursor))
            return FALSE;
    }

    return TRUE;
}


BOOL MouseInput(HWND hWnd, LPCRECT pRectTarget, USHORT ButtonFlags)
{
    POINT ptGameCursor;
    if (!GetGameCursorPos(hWnd, ptGameCursor))
        return FALSE;
    if (!CRect(pRectTarget).PtInRect(ptGameCursor))
    {
        if (!MouseMoveEx(hWnd, pRectTarget))
            return FALSE;
        Sleep(400);
    }

    if (!MouseButtonEx(ButtonFlags))
        return FALSE;
    Sleep(1000);
    return TRUE;
}


BOOL MouseInputEx(HWND hWnd, LPCRECT pRectTarget, USHORT ButtonFlags, 
                  PCTSTR compareFilePath, BOOL bFlag, LPCRECT rectMouseOutOfTarget, DWORD dwMilliseconds)
{
    BOOL bFind;
    DWORD dwTimediff, dwTime = GetTickCount();
    UINT i = 0;

    do{
        dwTimediff = GetTickCount() - dwTime;
        if (dwTimediff >= i * dwMilliseconds && dwTimediff < (i + 1) * dwMilliseconds)
        {
            if (!MouseInput(hWnd, pRectTarget, ButtonFlags))
            {
                TRACE(_T("操作鼠标失败！\n"));
                return FALSE;
            }
            i++;
            if (i > 3)
            {
                TRACE(_T("鼠标操作失败，请手动调整！\n"));
                BeWarning(_T("wav\\BJ.wav"));
            }
        }

        if (rectMouseOutOfTarget)
            MoveGameCursorToSpecifyPos(hWnd, *rectMouseOutOfTarget);

        bFind = FindSomePic(hWnd, compareFilePath);
    }while (bFlag ? !bFind : bFind);

    return TRUE;
}


BOOL MouseInputEx(HWND hWnd, PCTSTR clickFilePath, LPCRECT rectClick, USHORT ButtonFlags, 
                  PCTSTR compareFilePath, LPCRECT rectCompare, BOOL bFlag, LPCRECT rectMouseOutOfClick)
{
    BOOL bFind;
    RECT rect;
    DWORD dwTimediff, dwTime = GetTickCount();
    UINT i = 0;

    do{
        dwTimediff = GetTickCount() - dwTime;
        if (dwTimediff >= i * WAIT_TIME_OUT && dwTimediff < (i + 1) * WAIT_TIME_OUT)
        {
            if (FindSomePic(hWnd, clickFilePath, rectClick, &rect))
            {
                MouseInput(hWnd, &rect, ButtonFlags);
                if (rectMouseOutOfClick)
                    MoveGameCursorToSpecifyPos(hWnd, *rectMouseOutOfClick);
                i++;
            }
            if (i > 3)
            {
                TRACE(_T("鼠标操作失败，请手动调整！\n"));
                BeWarning(_T("wav\\BJ.wav"));
            }
        }

        bFind = FindSomePic(hWnd, compareFilePath, rectCompare);
    }while (bFlag ? !bFind : bFind);

    return TRUE;
}