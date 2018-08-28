#ifndef _KM_H_
#define _KM_H_

#pragma once
#include "..\kmclass\kmclass.h"


#define DRIVER_FUNC_INSTALL     0x01
#define DRIVER_FUNC_REMOVE      0x02

BOOL InputNumber(int number);

BOOL MouseButtonEx(USHORT ButtonFlags);

BOOL MouseMoveEx(HWND hWnd, LPCRECT pRectTarget);

BOOL ManageDriver(LPCTSTR DriverName, USHORT Function);

BOOL MouseInput(HWND hWnd, LPCRECT pRectTarget, USHORT ButtonFlags);

BOOL KeyboardButtonEx(USHORT VirtualKey, USHORT ControlVirtualKey = KEY_BLANK);

BOOL MouseInputEx(HWND hWnd, LPCRECT pRectTarget, USHORT ButtonFlags,
                  PCTSTR compareFilePath, BOOL bFlag, LPCRECT rectMouseOutOfTarget, DWORD dwMilliseconds);

BOOL MouseInputEx(HWND hWnd, PCTSTR clickFilePath, LPCRECT rectClick, USHORT ButtonFlags, 
                  PCTSTR compareFilePath, LPCRECT rectCompare, BOOL bFlag, LPCRECT rectMouseOutOfClick);

#endif // _KM_H_