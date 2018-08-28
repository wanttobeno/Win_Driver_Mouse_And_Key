#include "StdAfx.h"
#include "GameState.h"
#include "PreDemo.h"
#include "mapLine.h"
#include "km.h"
#include <cmath>
#include <list>
#include <Mmsystem.h>
#pragma comment(lib, "WINMM.LIB")


int  FindNumberPic(const Cbm & bmWnd, LPCRECT rectTarget, PRECT retRect, int resemble = 100, COLORREF rgb = 0);

BOOL FindSymbolPic(const Cbm & bmWnd, int NumberSymbol, LPCRECT rectTarget, PRECT retRect, int resemble = 100, COLORREF rgb = 0);

BOOL BuyItem(HWND hWnd, int ItemType, PSIZE pSize = NULL);

BOOL UseItem(HWND hWnd, int & ItemType, PSIZE pSize, int MapIndex = MAP_CITY_ZZ);

BOOL IsItemExist(HWND hWnd, int ItemType, PSIZE pSize = NULL);

BOOL GetTeleportItemPos(int TeleportPos, SIZE & size);

void GetGamePackageRect(HWND hWnd, RECT & rect);




BOOL bF9 = FALSE;
BOOL bctrlE = FALSE;


/************************多图比较找图函数***************************************
*       形参说明hwndScreen: 截图窗口句柄
*                Folderpath: 小图所在的文件夹路径，类似“pic\\”
*            (OUT) FileName: 返回找到的文件名，如果没找到则返回NULL
*             (IN OUT) rect:返回找到匹配图的中心x\y坐标。没找到则返回零
*********************************************************************************/
BOOL FindSomePic(const Cbm & bmWnd, PCTSTR FileName, LPCRECT rectTarget, PRECT retRect, PTSTR retFileName, int resemble, COLORREF rgb)
{
    WIN32_FIND_DATA fData;
    BOOL  bFind = FALSE;
    TCHAR FilePath[MAX_PATH];
    TCHAR FileDir[MAX_PATH];
    _tcscpy_s(FilePath, MAX_PATH, FileName);
    _tcscpy_s(FileDir, MAX_PATH, FileName);


    if (FileName[lstrlen(FileName) - 1] == '\\')
        _tcscat_s(FilePath, MAX_PATH, _T("*.bmp"));
    else if (_tcschr(FileName, '*'))
        _tcsrchr(FileDir, '\\')[1] = '\0';
    else
    {
        bFind = FindPic(bmWnd, FileName, rectTarget, retRect, resemble, rgb);
        if (retFileName)
        {
            if (bFind)
                _tcscpy_s(retFileName, MAX_PATH, FileName);
            else
                retFileName[0] = '\0';
        }

        return bFind;
    }

    HANDLE hFile = FindFirstFile(FilePath, &fData);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("FindSomePic --- read file failed.\n"));
        return FALSE;
    }

    do{
        wsprintf(FilePath, _T("%s%s"), FileDir, fData.cFileName);
        bFind = FindPic(bmWnd, FilePath, rectTarget, retRect, resemble, rgb);
    }while (!bFind && FindNextFile(hFile, &fData));

    FindClose(hFile);

    if (retFileName)
    {
        if (bFind)
            _tcscpy_s(retFileName, MAX_PATH, fData.cFileName);
        else
            retFileName[0] = '\0';
    }

    return bFind;
}


BOOL FindSomePic(HWND hWnd, PCTSTR FileName, LPCRECT rectTarget, PRECT retRect, PTSTR retFileName, int resemble, COLORREF rgb)
{
    return FindSomePic(Cbm(hWnd), FileName, rectTarget, retRect, retFileName, resemble, rgb);
}


BOOL FindSomePicEx(const Cbm & bmWnd, PCTSTR FileName, PRECT pRect,
                   PTSTR retFileName, int deviation, int resemble, COLORREF rgb)
{
    WIN32_FIND_DATA fData;
    BOOL  bFind = FALSE;
    TCHAR FilePath[MAX_PATH];
    TCHAR FileDir[MAX_PATH];

    _tcscpy_s(FilePath, MAX_PATH, FileName);
    _tcscpy_s(FileDir, MAX_PATH, FileName);
    if (FileName[lstrlen(FileName) - 1] == '\\')
        _tcscat_s(FilePath, MAX_PATH, _T("*.bmp"));
    else if (_tcschr(FileName, '*'))
        _tcsrchr(FileDir, '\\')[1] = '\0';
    else
    {
        bFind = FindPicEx(bmWnd, FileName, pRect, deviation, resemble, rgb);
        if (retFileName)
        {
            if (bFind)
                _tcscpy_s(retFileName, MAX_PATH, FileName);
            else
                retFileName[0] = '\0';
        }

        return bFind;
    }

    HANDLE hFile = FindFirstFile(FilePath, &fData);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        TRACE(_T("FindSomePic --- read file failed.\n"));
        return FALSE;
    }

    do{
        wsprintf(FilePath, _T("%s%s"), FileDir, fData.cFileName);
        bFind = FindPicEx(bmWnd, Cbm(FilePath), pRect, deviation, resemble, rgb);
    }while (!bFind && FindNextFile(hFile, &fData));

    FindClose (hFile);

    if (retFileName)
    {
        if (bFind)
            _tcscpy_s(retFileName, MAX_PATH, fData.cFileName);
        else
            retFileName[0] = '\0';
    }

    return bFind;
}


BOOL FindSomePicEx(HWND hWnd, PCTSTR FileName, PRECT pRect,
                   PTSTR retFileName, int deviation, int resemble, COLORREF rgb)
{
    return FindSomePicEx(Cbm(hWnd), FileName, pRect, retFileName, deviation, resemble, rgb);
}


/***********通过游戏鼠标图标，查找光标在游戏窗口中的坐标******************/
BOOL GetGameCursorPos(HWND hWnd, POINT & pt, int resemble, COLORREF rgb)
{
    return GetGameCursorPos(Cbm(hWnd), pt, resemble, rgb);
}

BOOL GetGameCursorPos(const Cbm & bmWnd, POINT & pt, int resemble, COLORREF rgb)
{

    CRect rect;

    TCHAR retFileName[MAX_PATH];
    BOOL bFind = FindSomePic(bmWnd, _T("bmp\\gameCursor\\"), NULL, &rect, retFileName, resemble, rgb);
    if (bFind)
    {
        if (_tcscmp(retFileName, _T("puzhuo.bmp")) == 0)
        {
            pt.x = rect.left - 35;
            pt.y = rect.top + 5;
        }else if (_tcscmp(retFileName, _T("inputcursor.bmp")) == 0)
        {
            pt = rect.CenterPoint();
        }else
        {
            pt.x = rect.left - 5;
            pt.y = rect.top - 4;
        }
    }

    return bFind;
}


// 获取当前所在场景

int GetMapIndex(HWND hWnd, int resemble, COLORREF rgb)
{
    return GetMapIndex(Cbm(hWnd), resemble, rgb);
}


int GetMapIndex(const Cbm & bmWnd, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    int    mapIndex;
    const RECT rect = RECT_MAPINFO;
    RECT rectTemp = rect;
    if (FindSomePic(bmWnd, _T("bmp\\address\\"), &rect, &rectTemp, FileName, resemble, rgb))
    {
        mapIndex = _tstoi(FileName);
        rectTemp.left  = rect.left;
        rectTemp.right = rect.right;
        if (FindSomePic(bmWnd, _T("bmp\\chNum\\"), &rectTemp, NULL, FileName, resemble, rgb))
            mapIndex += _tstoi(FileName);
    }
    else
        mapIndex = UNKNOWN_MAP;

    return mapIndex;
}

UINT GetRoleStatusValue(HWND hWnd, int StatusType)
{
    CRect rect(RECT_STATUSBAR);
    POINT pt;
    GetGameCursorPos(hWnd, pt);

    if (rect.PtInRect(pt))
        MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);

    return GetRoleStatusValue(Cbm(hWnd), StatusType);
}

//获取当前血量，返回值为0~100的百分数
UINT GetRoleStatusValue(const Cbm & bmWnd, int StatusType)
{
    //血条在坐标(582,11)~(631,11)的RGB值
    static COLORREF RedValueRGB[50] = {
        0x000058, 0x000060, 0x000068, 0x000058, 0x000060, 0x0800A0, 0x000098, 0x0800A0, 0x0800A8, 0x000098, 
        0x080088, 0x000070, 0x000070, 0x0800A0, 0x080088, 0x080088, 0x080088, 0x080088, 0x080088, 0x000098, 
        0x000098, 0x0800B0, 0x0800A0, 0x080088, 0x080088, 0x080088, 0x080088, 0x000068, 0x000070, 0x000068,
        0x000070, 0x000070, 0x0800B0, 0x0800B0, 0x000098, 0x000070, 0x080490, 0x080078, 0x080090, 0x0800A8,
        0x0800B8, 0x0800B0, 0x000098, 0x080088, 0x080098, 0x0800B8, 0x0800B8, 0x0800B8, 0x0800B0, 0x000068
    };

    //蓝条在坐标(583,23)~(632,23)的RGB值
    static COLORREF BlueValueRGB[50] = {
        0x502000, 0x582800, 0x703000, 0x582400, 0x682C00, 0xA04C00, 0x984000, 0xA84800, 0xA05000, 0x984400, 
        0x903C00, 0x783800, 0x703400, 0xA04C00, 0x883C00, 0x903C00, 0x903C00, 0x883C00, 0x883C00, 0x984000, 
        0x984000, 0xB05400, 0xA04C00, 0x903C00, 0x883C00, 0x903C00, 0x903C00, 0x683000, 0x703400, 0x683000, 
        0x783800, 0x783800, 0xB05400, 0xB05000, 0x984000, 0x783800, 0x904C00, 0x783C00, 0x904800, 0xA05000, 
        0xB85C00, 0xB05400, 0x904800, 0x883C00, 0x985400, 0xB85C00, 0xB85C00, 0xB85C00, 0xB05400, 0x783C00
    };

    int i = 0;
    while (i < 50)
    {
        BOOL bFind = FALSE;
        switch (StatusType)
        {
        case HEALTH_POINT:
            bFind = bmWnd.GetPixel(582 + i, 11) == RedValueRGB[i];
            break;
        case MANA_POINT:
            bFind = bmWnd.GetPixel(583 + i, 23) == BlueValueRGB[i];
            break;
        default:
            break;
        }
        if (bFind)
            i++;
        else
            break;
    }

    return 2 * i;
}



//获取当前游戏窗口左上角显示的人物坐标
BOOL GetGameRolePos(HWND hWnd, POINT &ptPos, int resemble, COLORREF rgb)
{
    return GetGameRolePos(Cbm(hWnd), ptPos, resemble, rgb);
}

BOOL GetGameRolePos(const Cbm & bmWnd, POINT &ptPos, int resemble, COLORREF rgb)
{
    RECT rectLeftBracket, rectRightBracket, rectComma, rectNumber;
    rectComma = RECT_MAPINFO;
    BOOL bFindComma = FALSE;
    int i, num;

    memset(&ptPos, 0, sizeof(POINT));

    if (!FindSymbolPic(bmWnd, LEFTBRACKET, RECT_MAPINFO, &rectLeftBracket, resemble, rgb))
        return FALSE;
    if (!FindSymbolPic(bmWnd, RIGHTBRACKET, RECT_MAPINFO, &rectRightBracket, resemble, rgb))
        return FALSE;
    for (i = 0; i < 3; i++)
    {
        bFindComma = FindSymbolPic(bmWnd, COMMA1 + i, RECT_MAPINFO, &rectComma, resemble, rgb);
        if (bFindComma)
            break;
    }
    if (!bFindComma)
        return FALSE;

    for (i = 0; i < (rectComma.left - rectLeftBracket.right) / 6; i++)
    {
        rectNumber = rectLeftBracket;
        rectNumber.left  = rectLeftBracket.right + 1 + i * 6;
        rectNumber.right = rectLeftBracket.right + 6 + i * 6;

        num = FindNumberPic(bmWnd, &rectNumber, NULL, resemble, rgb);
        if (num == INVALID_VALUE)
            return FALSE;
        else
        {
            ptPos.x = ptPos.x * 10 + num;
        }
    }
    for (i = 0; i < (rectRightBracket.left - rectComma.right) / 6; i++)
    {
        rectNumber = rectLeftBracket;
        rectNumber.left  = rectComma.right + 1 + i * 6;
        rectNumber.right = rectComma.right + 6 + i * 6;

        num = FindNumberPic(bmWnd, &rectNumber, NULL, resemble, rgb);
        if (num == INVALID_VALUE)
            return FALSE;
        else
        {
            ptPos.y = ptPos.y * 10 + num;
        }
    }

    return TRUE;
}


BOOL FindSymbolPic(const Cbm & bmWnd, int NumberSymbol, LPCRECT rectTarget, PRECT retRect, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    wsprintf(FileName, _T("bmp\\symbol\\%02dSymbol.bmp"), NumberSymbol);

    return FindPic(bmWnd, FileName, rectTarget, retRect, resemble, rgb);
}


int FindNumberPic(const Cbm & bmWnd, LPCRECT rectTarget, PRECT retRect, int resemble, COLORREF rgb)
{
    TCHAR FileName[MAX_PATH];
    BOOL bFind = FindSomePic(bmWnd, _T("bmp\\number\\"), rectTarget, retRect, FileName, resemble, rgb);

    if (!bFind)
        return INVALID_VALUE;
    else
        return _tstoi(FileName);
}


// 获取道具包裹第一格的位置
void GetGamePackageRect(HWND hWnd, RECT & rect)
{
    // 移动游戏光标
    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);

    // 打开道具窗口
    while (!FindPic(hWnd, _T("bmp\\general\\DaoJuWindow.bmp"), NULL, &rect))
    {
        KeyboardButtonEx('E', VK_MENU);
        Sleep(1000);
        TRACE(_T("\n正在找图ing! DaoJuWindow\n"));
    }
    rect = CRect(rect.left + 20, rect.top + 92, rect.right + 22, rect.bottom + 109);
}

// 判断物品是否存在
BOOL IsItemExist(HWND hWnd, int ItemType, PSIZE pSize)
{
    if (ItemType > ITEM_MAX)
        return FALSE;

    // 移动游戏光标
    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
    Sleep(300);

    TCHAR FileName[MAX_PATH];
    CRect rect;
    GetGamePackageRect(hWnd, rect);
    rect.OffsetRect(*pSize);
    while (!FindSomePic(hWnd, _T("bmp\\xianLing\\"), &rect, NULL, FileName))
    {
        Sleep(200);
        TRACE(_T("Searching the item : %d\n"), ItemType);
    }

    return _tstoi(FileName) == ItemType;
}


BOOL GetTeleportItemPos(int TeleportPos, SIZE & size)
{
    CString strPos, strPosValue;

    strPos.Format(_T("strPos%d"), TeleportPos);
    strPosValue = theApp.GetProfileString(_T("PosSetting"), strPos);

    if (strPosValue.IsEmpty())
        return FALSE;
    else
    {
        size.cx = (strPosValue[1] - '1')* 51; 
        size.cy = (strPosValue[0] - 'A') * 51; 
        return TRUE;
    }
}


// 购买物品
BOOL BuyItem(HWND hWnd, int ItemType, PSIZE pSize)
{
    // 移动游戏光标
    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_XIANLING);
    BOOL bComplete = FALSE;
    CRect rect;

    while (!bComplete)
    {
        // 检测并点击奖励框
        ClickCheckWindow(hWnd);
        EscapeFighting(hWnd);

        // 获取仙灵店铺的标题位置
        if (!FindPic(hWnd, _T("bmp\\general\\buy.bmp"), NULL, &rect))
        {
            KeyboardButtonEx(VK_F8);
            TRACE(_T("等待打开仙灵店铺！\n"));
            Sleep(1000);
        }else
        {
            // 计算仙灵店铺第一格的位置
            rect = CRect(rect.left - 95, rect.top + 34, rect.left - 65, rect.top + 64);
            CRect RealRect(rect);
            switch (ItemType)
            {
            case BaoZi:                 // 包子
                break;
            case SheYaoXiang:           // 摄妖香
                RealRect.OffsetRect(153, 0); 
                break;
            case DongMingCao:           // 洞冥草
                RealRect.OffsetRect(102, 102);
                break;
            case GuiQieCao:             // 鬼切草
                RealRect.OffsetRect(102, 0);
                break;
            case ZiDanLuo:              // 紫丹罗
                RealRect.OffsetRect(0, 102);
                break;
            case DaoBiaoQi:             // 导标旗
                RealRect.OffsetRect(51, 51);
                break;
            case FeiXingFu:             // 飞行符
                RealRect.OffsetRect(153, 204);
                break;
            default:
                return FALSE;
            }

            // 左键选中物品
            MouseInputEx(hWnd, &RealRect, MOUSE_LEFT_BUTTON,
                _T("bmp\\general\\null*.bmp"), CLOSE_WINDOW, RECT_NOT_XIANLING, 5000);

            // 按退格键清空物品数量
            if (ItemType != DaoBiaoQi)
                KeyboardButtonEx(VK_BACK);

            // 判断小键盘是否开启，没有就打开
            if (!GetKeyState(VK_NUMLOCK))
                KeyboardButtonEx(VK_NUMLOCK);

            // 输入数量: 摄妖香、洞冥草、鬼切草、飞行符20个, 紫丹罗30个，包子10个，导标旗1个
            switch (ItemType)
            {
            case BaoZi:                 // 包子
                InputNumber(10);
                break;
            case SheYaoXiang:           // 摄妖香
                InputNumber(theApp.GetProfileInt(_T("AdvancedSetting"), _T("Incense"),0));
                break;
            case DongMingCao:           // 洞冥草
                InputNumber(theApp.GetProfileInt(_T("AdvancedSetting"), _T("Grass"),0));
                break;
            case FeiXingFu:             // 飞行符
                InputNumber(20);
                break;
            case GuiQieCao:             // 鬼切草
            case ZiDanLuo:              // 紫丹罗
                InputNumber(theApp.GetProfileInt(_T("AdvancedSetting"), _T("nMagicDrug"), 0));
                break;
            case DaoBiaoQi:             // 导标旗
                break;
            default:
                return FALSE;
            }

            RealRect = CRect(rect.left + 191, rect.top + 313, rect.right + 191, rect.bottom +298);
            MouseInputEx(hWnd, &RealRect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\null*.bmp"), OPEN_WINDOW, NULL, 5000);
            RealRect.OffsetRect(-10, -50);
            MouseInputEx(hWnd, &RealRect, MOUSE_RIGHT_BUTTON, _T("bmp\\general\\buy*.bmp"), CLOSE_WINDOW, NULL, 5000);

            if (FindPic(hWnd, _T("bmp\\general\\grayIcon.bmp"), NULL, NULL) ||
                FindPic(hWnd, _T("bmp\\general\\miandui.bmp"), NULL, NULL))
                bComplete = FALSE;
            else
                bComplete = TRUE;
        }
    }

    // 定导标旗
    if (ItemType == DaoBiaoQi)
    {
        if (!pSize)
            return FALSE;

        // 获取道具窗口位置
        GetGamePackageRect(hWnd, rect);
        rect.OffsetRect(*pSize);
        while (!FindSomePic(hWnd, _T("bmp\\xianLing\\07*.bmp"), &rect, NULL))
        {
            Sleep(200);
            TRACE(_T("查找已经购买到的导标旗ing\n"));
            BeWarning(_T("wav\\卡机了.wav"));
        }


        DWORD dwTimediff, dwTime = GetTickCount();
        UINT i = 0;
        CRect rectTemp;
        do {
            dwTimediff = GetTickCount() - dwTime;
            if (dwTimediff >= i * 20000 && dwTimediff < (i + 1) * 20000)
            {
                MouseInput(hWnd, &rect, MOUSE_RIGHT_BUTTON);
                i++;
            }

            if (FindSomePic(hWnd, _T("bmp\\general\\songwoqu*.bmp"), NULL, &rectTemp))
            {
                rectTemp.OffsetRect(0, 64);
                MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, _T("bmp\\general\\songwoqu*.bmp"), CLOSE_WINDOW, NULL, 5000);
                break;
            }
        }while (!FindPic(hWnd, _T("bmp\\general\\lubiao.bmp")));
    }

    return TRUE;
}


BOOL UseItem(HWND hWnd, int & ItemType, PSIZE pSize, int MapIndex)
{
    CPoint ptBeg, ptEnd;
    TCHAR FileName[MAX_PATH];
    CRect rect, rectTemp;
    GetGamePackageRect(hWnd, rect);

    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
    Sleep(300);
    // 确定物品的位置
    if (ItemType == DaoBiaoQi)
    {
        rect.OffsetRect(*pSize);
        while (!FindSomePic(hWnd, _T("bmp\\xianLing\\"), &rect, &rect, FileName))
        {
            TRACE(_T("没有找到导标旗\n"));
            Sleep(500);
        }
        if (_tstoi(FileName) != DaoBiaoQi)
        {
            if (MapIndex <= UNKNOWN_MAP || MapIndex > MAP_CITY_MAX)
                return FALSE;
            else
            {
                ItemType = FeiXingFu;
                return UseItem(hWnd, ItemType, pSize, MapIndex);
            }
        }
    }else
    {
        wsprintf(FileName, _T("bmp\\xianLing\\%02ditem*.bmp"), ItemType);
        while (!FindSomePic(hWnd, FileName, NULL, &rectTemp))
        {
            Sleep(500);
            TRACE(_T("Searching the item: %d\n"), ItemType);
        }

        if (pSize)
        {
            pSize->cx = (rectTemp.left - rect.left) / 51 * 51;
            pSize->cy = (rectTemp.top - rect.top) / 51 * 51;
            rect.OffsetRect(*pSize);
        }
    }

    // 使用前获取位置信息
    while (!GetGameRolePos(hWnd, ptBeg))
    {
        Sleep(200);
        BeWarning(_T("wav\\BJ.wav"));
        TRACE(_T("UseItem >> ptBeg"));
    }


    switch (ItemType)
    {
    case SheYaoXiang:
    case DongMingCao:
        _tcscpy_s(FileName, MAX_PATH, _T("bmp\\general\\use*.bmp"));
        break;
    case FeiXingFu:
        _tcscpy_s(FileName, MAX_PATH, _T("bmp\\xianLing\\ffmap\\ff_map*.bmp"));
        break;
    case DaoBiaoQi:
        _tcscpy_s(FileName, MAX_PATH, _T("bmp\\general\\songwoqu*.bmp"));
        break;
    default:
        TRACE(_T("error parameter.\n"));
        return FALSE;
    }


    // 右键点击使用物品
    MouseInputEx(hWnd, &rect, MOUSE_RIGHT_BUTTON, FileName, OPEN_WINDOW, NULL, 10000);


    // 如果物品为77或者FF，则需要继续处理后续对话框窗口
    if (ItemType == DaoBiaoQi)
    {
        while(!FindSomePic(hWnd, FileName, NULL, &rectTemp))
        {
            Sleep(500);
            TRACE(_T("Searching! bmp\\general\\songwoqu*.bmp \n"));
        }
        MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, FileName, CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
        // 等待移动到新的位置
        do{
            while (!GetGameRolePos(hWnd, ptEnd))
            {
                Sleep(200);
                TRACE(_T("UseItem >> ptEnd"));
            }
            Sleep(200);
        } while (ptBeg == ptEnd);
    }
    else if (ItemType == FeiXingFu)
    {
        wsprintf(FileName, _T("bmp\\xianLing\\ffmap\\ff_map%02d.bmp"), MapIndex);
        while (!FindPic(hWnd, FileName, NULL, &rectTemp))
        {
            Sleep(500);
            TRACE(_T("Searching! bmp/XianLing/ffmap/ \n"));
        }

        MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON,
            _T("bmp\\xianLing\\ffmap\\ff_map*.bmp"), CLOSE_WINDOW, NULL, 5000);

        // 等待移动到新的位置
        do{
            while (!GetGameRolePos(hWnd, ptEnd))
            {
                Sleep(200);
                TRACE(_T("UseItem >> ptEnd"));
            }
            Sleep(200);
        } while (ptBeg == ptEnd);
    }

    return TRUE;
}


BOOL UseItemEx(HWND hWnd, int ItemType, int MapIndex, int TeleportPos)
{
    SIZE size;
    if (ItemType == DaoBiaoQi)
    {
        ItemType = GetTeleportItemPos(TeleportPos, size) ? ItemType : FeiXingFu;
    }

    if (ItemType == FeiXingFu)
    {
        if (MapIndex <= UNKNOWN_MAP || MapIndex > MAP_CITY_MAX)
            return FALSE;
    }

    if (!UseItem(hWnd, ItemType, &size, MapIndex))
        return FALSE;
    if (!IsItemExist(hWnd, ItemType, &size))
    {
        if (!BuyItem(hWnd, ItemType, &size))
            return FALSE;
    }

    return TRUE;
}

void BeWarning(PCTSTR alarmFileName)
{
    switch (theApp.GetProfileInt(_T("BaseSetting"), _T("Alarm"), 0))
    {
    case 0:
        ::Beep(900, 300);
        ::Beep(900, 300);
        ::Beep(900, 300);
        ::Beep(900, 300);
        break;
    case 1:
        sndPlaySound(alarmFileName, SND_SYNC);
        break;
    default:
        break;
    }
}

BOOL IsRoleStatic(HWND hWnd, PPOINT ptPos)
{
    COLORREF colorBeg1, colorBeg2, colorEnd1, colorEnd2;
    colorBeg1 = Cbm(hWnd).GetPixel(200, 60);
    colorBeg2 = Cbm(hWnd).GetPixel(580, 240);

    CPoint ptBeg, ptEnd;
    GetGameRolePos(hWnd, ptBeg);
    Sleep(1000);


    colorEnd1 = Cbm(hWnd).GetPixel(200, 60);
    colorEnd2 = Cbm(hWnd).GetPixel(580, 240);
    GetGameRolePos(hWnd, ptEnd);
    if (ptPos)
        *ptPos = ptEnd;

    return (colorBeg1 == colorEnd1 || colorBeg2 == colorEnd2) && ptBeg == ptEnd;
}


BOOL IsRoleStaticEx(HWND hWnd, PPOINT ptPos)
{
    CPoint ptBeg, ptEnd;
    GetGameRolePos(hWnd, ptBeg);
    Sleep(500);


    GetGameRolePos(hWnd, ptEnd);
    if (ptPos)
        *ptPos = ptEnd;

    return ptBeg == ptEnd;
}


void EscapeFighting(HWND hWnd)
{
    CRect rect;
    while (FindPic(hWnd, _T("bmp\\general\\grayIcon.bmp"), NULL, NULL))
    {
        if (FindPic(hWnd, _T("bmp\\General\\fashu.bmp"), NULL, &rect))
        {
            rect.OffsetRect(-12, 264);
            MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
        }
    }
}


void ClickCheckWindow(HWND hWnd)
{
    RECT  rect;
    TCHAR retFileName[MAX_PATH];
    if (!FindPic(hWnd, _T("bmp\\general\\miandui.bmp"), NULL, &rect))
        return;

    TRACE(_T("出现奖励框！\n"));
    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
    rect = CRect(rect.left - 314, rect.top + 33, rect.right + 17, rect.bottom + 140);
    if (FindSomePicEx(hWnd, _T("bmp\\check\\"), &rect, retFileName, 10))
    {
        TRACE(_T("对比图片为：%s\n"), retFileName);
        MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
        MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
        Sleep(700);
    }else
    {
        Cbm(hWnd).SaveBitmapToFile(_T("checkWindow\\"), &rect);
    }

    while (FindPic(hWnd, _T("bmp\\general\\miandui.bmp"), NULL, NULL))
    {
        BeWarning(_T("wav\\BJ.wav"));
        Sleep(1000);
    }
}

//
//BOOL ConvertPointToGamePos(HWND hWnd, PPOINT pPoint)
//{
//    RECT rect;
//    GetClientRect(hWnd, &rect);
//    int cx = (rect.right -rect.left) / 20;
//    int cy = (rect.bottom - rect.top) / 20;
//    BOOL bFind = FALSE;
//
//    for (int y = 0; y <= cy; y++)
//        for (int x = 0; x <= cx; x++)
//        {
//            CRect rectTemp(x * 20 - 10, y * 20 - 10, x * 20 + 10, y * 20 + 10);
//            bFind = rectTemp.PtInRect(*pPoint);
//            if (bFind)
//            {
//                pPoint->x = x;
//                pPoint->y = cy - y;
//                return TRUE;
//            }
//        }
//
//        return bFind;
//}


BOOL CreateFolder(PCTSTR FilePath)
{
    int len = lstrlen(FilePath);
    for (int pos = 0; pos < len - 1; pos++)
    {
        if (FilePath[pos] == '\\' || FilePath[pos] == '/')
        {
            CString FileDirectory(FilePath, pos + 1);
            // 判断文件夹路径是否存在，不存在则创建文件夹
            if (!PathIsDirectory(FileDirectory))
            {
                if (!CreateDirectory(FileDirectory, NULL))
                    return FALSE;
            }
        }
    }
    return TRUE;
}


BOOL MoveGameCursorToSpecifyPos(HWND hWnd, const RECT & rect)
{
    POINT pt;
    BOOL bRet = TRUE;

    if (RECT_NOT_MONSTER.EqualRect(&rect))
    {
        if (!GetGameCursorPos(hWnd, pt))
        {
            TRACE(_T("找不到游戏光标!\n"));
            return FALSE;
        }

        if (RECT_MONSTER.PtInRect(pt))
        {
            bRet = MouseMoveEx(hWnd, &rect);
            Sleep(500);
        }
    }else
    {
        bRet = MouseMoveEx(hWnd, &rect);
        Sleep(500);
    }
    return bRet;
}


void CheckRoleStatus(HWND hWnd, int StatusType, int LimitValue)
{
    RECT rect;
    int Value, ValueTemp, item;


    switch (StatusType)
    {
    case HEALTH_POINT:
        rect = RED_BAR_RECT;
        item = BaoZi;
        break;
    case MANA_POINT:
        rect = BLUE_BAR_RECT;
        item = theApp.GetProfileInt(_T("AdvancedSetting"), _T("strMagicDrug"), 0) ? GuiQieCao : ZiDanLuo;
        break;
    default:
        return;
    }


    Value  = GetRoleStatusValue(hWnd, StatusType);
    while (Value <= LimitValue)
    {
        MouseInput(hWnd, &rect, MOUSE_RIGHT_BUTTON);
        MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
        Sleep(1000);
        ValueTemp = Value;
        Value = GetRoleStatusValue(hWnd, StatusType);
        if (Value == ValueTemp)
        {
            BuyItem(hWnd, item);
        }
    }
}


void MoveToSchool(HWND hWnd, int SchoolMapIndex)
{
    while (GetMapIndex(hWnd) != SchoolMapIndex)
    {
        CheckRoleStatus(hWnd, HEALTH_POINT, 50);
        CheckRoleStatus(hWnd, MANA_POINT, 10);

        KeyboardButtonEx(VK_F1);
        Sleep(1000);
    }
}


int MonsterToMap(int monster)
{
    int monsterAddr[44] = {
        UNKNOWN_MAP, MAP_SUBURB_DongHHD, MAP_SUBURB_DongHHD, MAP_SUBURB_JiangNYW, MAP_SUBURB_JiangNYW, MAP_SUBURB_DaTGJ,
        MAP_SUBURB_DaTGJ, MAP_SUBURB_DaTGJ, MAP_SUBURB_DaTJW, MAP_SUBURB_DaTJW, MAP_SUBURB_DaTJW, MAP_MAZE_T1, MAP_MAZE_T1,
        MAP_SM_MW, MAP_SM_PS, MAP_SUBURB_ChangSJW, MAP_SUBURB_ChangSJW, MAP_MAZE_H1, MAP_MAZE_H2, MAP_MAZE_H3, MAP_SM_MW,
        MAP_SM_PT, MAP_SM_PS, MAP_SUBURB_YueG, MAP_MAZE_D1, MAP_MAZE_D1, MAP_MAZE_D3, MAP_MAZE_D3, MAP_SM_ST, MAP_SM_ST,
        MAP_SUBURB_BeiJLZ, MAP_SUBURB_BeiJLZ, MAP_SUBURB_BeiJLZ, MAP_MAZE_L1, MAP_MAZE_L3, MAP_MAZE_L3, MAP_MAZE_F3, 
        MAP_MAZE_L4, MAP_MAZE_F4, MAP_MAZE_L6, MAP_MAZE_L6, MAP_MAZE_F6, MAP_MAZE_L7, MAP_MAZE_F7
    };

    return monsterAddr[monster];
}


BOOL GetFarthestPoint(HWND hWnd, int mapIndex, PRECT pRect)
{
    TCHAR  FileName[MAX_PATH];
    POINT  ptOwner,   ptTarget;
    CRect  rectOwner, rectTarget;
    std::multimap<int, RECT> mtmpPos;
    int distance;

    while (!FindSomePic(hWnd, _T("bmp\\tabMap\\tabmap\\"), NULL, NULL))
    {
        if (FindPic(hWnd, _T("bmp\\general\\grayIcon.bmp"))
            || FindPic(hWnd, _T("bmp\\general\\miandui.bmp")))
            return FALSE;
        KeyboardButtonEx(VK_TAB);
        Sleep(500);
    }
    // 获取红点位置
    if (!FindPic(hWnd, _T("bmp\\general\\redPoint.bmp"), NULL, &rectOwner))
    {
        FindSomePic(hWnd, _T("bmp\\gameCursor\\"), NULL, &rectOwner);
    }
    ptOwner = rectOwner.CenterPoint();

    while (mapIndex != GetMapIndex(hWnd))
    {
        TRACE(_T("当前地图与目标场景不一致。\n"));
        BeWarning(_T("wav\\BJ.wav"));
        Sleep(500);
    }

    for (int i = 0; i < 4; i++)
    {
        wsprintf(FileName, _T("bmp\\tabMap\\%02d\\%d.bmp"), mapIndex, i);
        if (FindPic(hWnd, FileName, NULL, &rectTarget))
        {
            ptTarget = rectTarget.CenterPoint();
            distance = (int)std::sqrt(double((ptTarget.x - ptOwner.x) * (ptTarget.x - ptOwner.x) + (ptTarget.y - ptOwner.y) * (ptTarget.y - ptOwner.y)));
        }else 
        {
            distance = 0;
            rectTarget = rectOwner;
        }

        mtmpPos.insert(std::pair<int, RECT>(distance, rectTarget));
    }


    std::multimap<int, RECT>::reverse_iterator it = mtmpPos.rbegin();
    if (rand()%2)
        ++it;
    *pRect = it->second;

    return TRUE;
}


void GetLineOfTargetMapIndex(int mapIndex, std::list<Cmap> & line)
{
    switch (mapIndex)
    {
    case MAP_SUBURB_DongHHD:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_AL, CRect(), INDIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_AL, MAP_SUBURB_DongHW, CRect(162, 7, 181, 24), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_SUBURB_DongHW, MAP_SUBURB_DongHHD, CRect(95, 1, 120, 17)));
        break;
    case MAP_SUBURB_JiangNYW:
        line.push_back(Cmap(MAP_CITY_CA, MAP_SUBURB_JiangNYW, CRect(534, 0, 549, 16), DIRECT_EXIT, Cmap::CITY_MOVE));
        break;
    case MAP_MAZE_T1:
        break;
    case MAP_SUBURB_DaTJW:
    case MAP_SM_MW:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_ZZ, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_ZZ, MAP_SUBURB_DaTJW, CRect(0, 0, 16, 12)));
        line.push_back(Cmap(MAP_SUBURB_DaTJW, MAP_SM_MW, CRect(48, 107, 65, 120)));
        break;
    case MAP_SM_PS:
        line.push_back(Cmap(MAP_CITY_CA, MAP_SUBURB_DaTGJ, CRect(0, 0, 16, 12), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_SUBURB_DaTGJ, MAP_SUBURB_DaTJW, CRect(0, 76, 16, 86)));
        line.push_back(Cmap(MAP_SUBURB_DaTJW, MAP_SM_PS, CRect(520, 108, 536, 120)));
        break;
    case MAP_MAZE_H1:
    case MAP_MAZE_H2:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_AL, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_AL, MAP_SUBURB_HuaGS, CRect(208, 138, 222, 148)));
        line.push_back(Cmap(MAP_SUBURB_HuaGS, MAP_MAZE_H1, CRect(90, 0, 111, 14), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_MAZE_H1, MAP_MAZE_H2, CRect(48, 33, 64, 47)));
        break;
    case MAP_MAZE_H3:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_AL, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_AL, MAP_SUBURB_HuaGS, CRect(208, 138, 222, 148)));
        line.push_back(Cmap(MAP_SUBURB_HuaGS, MAP_MAZE_H1, CRect(90, 0, 111, 14), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_MAZE_H1, MAP_MAZE_H3, CRect(0, 36, 16, 48)));
        break;
    case MAP_SUBURB_DaTGJ:
    case MAP_SM_PT:
        line.push_back(Cmap(MAP_CITY_CA, MAP_SUBURB_DaTGJ, CRect(0, 0, 16, 12), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_SUBURB_DaTGJ, MAP_SM_PT, CRect(213, 56, 232, 72), INDIRECT_EXIT));
        break;
    case MAP_SUBURB_ChangSJW:
    case MAP_SUBURB_YueG:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_CS, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_CS, MAP_SUBURB_ChangSJW, CRect(133, 0, 158, 12)));
        line.push_back(Cmap(MAP_SUBURB_ChangSJW, MAP_SM_TG, CRect(13, 54, 32, 66), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_SM_TG, MAP_SUBURB_YueG, CRect(17, 157, 51, 168)));
        break;
    case MAP_MAZE_D1:
    case MAP_MAZE_D3:
        line.push_back(Cmap(MAP_CITY_CA, MAP_SUBURB_DaTGJ, CRect(272, 37, 287, 47), INDIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_SUBURB_DaTGJ, MAP_SM_DF, CRect(45, 322, 63, 336)));
        line.push_back(Cmap(MAP_SM_DF, MAP_MAZE_D1, CRect(31, 106, 48, 118)));
        line.push_back(Cmap(MAP_MAZE_D1, MAP_MAZE_D2, CRect(0, 0, 16, 12)));
        line.push_back(Cmap(MAP_MAZE_D2, MAP_MAZE_D3, CRect(104, 30, 120, 42)));
        break;
    case MAP_SM_ST:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_ZZ, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_ZZ, MAP_SUBURB_DaTJW, CRect(0, 0, 16, 12)));
        line.push_back(Cmap(MAP_SUBURB_DaTJW, MAP_SM_ST, CRect(0, 47, 16, 59)));
        break;
    case MAP_SUBURB_BeiJLZ:
    case MAP_MAZE_L1:
    case MAP_MAZE_L2:
    case MAP_MAZE_L3:
    case MAP_MAZE_L4:
    case MAP_MAZE_L5:
    case MAP_MAZE_L6:
    case MAP_MAZE_L7:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_AL, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_AL, MAP_SUBURB_HuaGS, CRect(208, 138, 222, 148)));
        line.push_back(Cmap(MAP_SUBURB_HuaGS, MAP_SUBURB_BeiJLZ, CRect(18, 93, 39, 109), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_SUBURB_BeiJLZ, MAP_MAZE_L1, CRect(0, 78, 22, 86)));
        line.push_back(Cmap(MAP_MAZE_L1, MAP_MAZE_L2, CRect(0, 65, 25, 73)));
        line.push_back(Cmap(MAP_MAZE_L2, MAP_MAZE_L3, CRect(0, 0, 29, 14)));
        line.push_back(Cmap(MAP_MAZE_L3, MAP_MAZE_L4, CRect(121, 15, 145, 32)));
        line.push_back(Cmap(MAP_MAZE_L4, MAP_MAZE_L5, CRect(123, 22, 134, 30)));
        line.push_back(Cmap(MAP_MAZE_L5, MAP_MAZE_L6, CRect(41, 0, 60, 14)));
        line.push_back(Cmap(MAP_MAZE_L6, MAP_MAZE_L7, CRect(110, 60, 128, 68)));
        break;
    case MAP_MAZE_F1:
    case MAP_MAZE_F2:
    case MAP_MAZE_F3:
    case MAP_MAZE_F4:
    case MAP_MAZE_F5:
    case MAP_MAZE_F6:
    case MAP_MAZE_F7:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CITY_AL, CRect(), DIRECT_EXIT, Cmap::CITY_MOVE));
        line.push_back(Cmap(MAP_CITY_AL, MAP_SUBURB_HuaGS, CRect(208, 138, 222, 148)));
        line.push_back(Cmap(MAP_SUBURB_HuaGS, MAP_SUBURB_BeiJLZ, CRect(18, 93, 39, 109), INDIRECT_EXIT));
        line.push_back(Cmap(MAP_SUBURB_BeiJLZ, MAP_MAZE_F1, CRect(68, 146, 92, 168)));
        line.push_back(Cmap(MAP_MAZE_F1, MAP_MAZE_F2, CRect(46, 0, 56, 12)));
        line.push_back(Cmap(MAP_MAZE_F2, MAP_MAZE_F3, CRect(113, 42, 128, 49)));
        line.push_back(Cmap(MAP_MAZE_F3, MAP_MAZE_F4, CRect(112, 0, 128, 10)));
        line.push_back(Cmap(MAP_MAZE_F4, MAP_MAZE_F5, CRect(70, 59, 80, 72)));
        line.push_back(Cmap(MAP_MAZE_F5, MAP_MAZE_F6, CRect(109, 62, 127, 72)));
        line.push_back(Cmap(MAP_MAZE_F6, MAP_MAZE_F7, CRect(112, 40, 127, 49)));
        break;
    case MAP_CA_HOTEL:
        line.push_back(Cmap(MAP_CITY_CA, MAP_CA_HOTEL, CRect(449, 159, 465, 169), DIRECT_EXIT, Cmap::CITY_MOVE));
        break;
    default:
        break;
    }
}



BOOL CatchMonster(HWND hWnd, const std::multimap<Mst_Num, Mst_Ty> & mtmp)
{
    // 获取当前行动路线
    std::multimap<Mst_Num, Mst_Ty>::const_iterator it = mtmp.begin();
    int MonsterIndex = it->second;

    std::list<Cmap> line;
    int mapIndex = MonsterToMap(MonsterIndex);
    GetLineOfTargetMapIndex(mapIndex, line);
    if (!LineMove(hWnd, line, mapIndex)) return FALSE;


    CRect rect;
    if (FindSomePic(hWnd, _T("bmp\\general\\shide*.bmp"), NULL, &rect))
    {
        MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\shide*.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
    }


    int round    = 0;
    int roundMax = (int)theApp.GetProfileInt(_T("AdvancedSetting"), _T("Round"), 0);
    int nLoad    = 0;

    std::map<Mst_Ty, Mst_Num> mpLoad;
    int MonsterTotal = 0;
    for (it = mtmp.begin();it != mtmp.end(); ++it)
    {
        MonsterTotal += 16 - it->first;
        mpLoad.insert(std::pair<Mst_Ty, Mst_Num>(it->second, 0));
    }
    int nLoadMax = min((int)theApp.GetProfileInt(_T("BaseSetting"), _T("LoadQuantity"), 3), MonsterTotal);


    BOOL bStatic = TRUE, bTest = FALSE, bFlag = FALSE, bFind;
    TCHAR FileName[MAX_PATH];
    while (TRUE)
    {
        if (bStatic)
        {
            ClickCheckWindow(hWnd);

            while (FindPic(hWnd, _T("bmp\\General\\grayIcon.bmp"), NULL, NULL))
            {
                if (!bFlag)
                {
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_MONSTER);
                    bFlag = TRUE;
                }
                if (FindPic(hWnd, _T("bmp\\General\\fashu.bmp"), NULL, NULL))
                {
                    if (bTest)
                    {
                        if (FindPic(hWnd, _T("bmp\\general\\zhaohuanshou.bmp"), CENTER_RECT, NULL))
                        {
                            nLoad = nLoadMax;
                            TRACE(_T("携带宠物已满。\n"));
                        }
                        else if (!FindSomePic(hWnd, FileName, &rect, NULL, 0, 90)) {
                            mpLoad[it->second]++;
                            nLoad++;
                            TRACE(_T("已抓到种类 %d 的宠物数/期望数：%d/%d\n"), it->second, mpLoad[it->second], 16 - it->first);
                            TRACE(_T("已抓到宠物总数/携带数：%d/%d\n"), nLoad, nLoadMax);
                        }
                        bTest = FALSE;
                    }

                    if (round < roundMax &&
                        nLoad < nLoadMax && 
                        GetRoleStatusValue(hWnd, HEALTH_POINT) >= 30 &&
                        GetRoleStatusValue(hWnd, MANA_POINT) >= 10)
                    {
                        for (it = mtmp.begin();it != mtmp.end(); ++it)
                        {
                            wsprintf(FileName, _T("bmp\\monster\\%02d\\"), it->second);
                            bFind = mpLoad[it->second] < (16 - it->first) && FindSomePic(hWnd, FileName, RECT_MONSTER, &rect, 0, 90);
                            if (bFind)
                                break;
                        }
                    }else
                        bFind = FALSE;

                    if (bFind)
                    {
                        KeyboardButtonEx('G', VK_MENU);
                        MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\gameCursor\\puzhuo*.bmp"), CLOSE_WINDOW, RECT_NOT_MONSTER, 5000);
                        round++;
                        bTest = TRUE;
                        TRACE(_T("已使用回合数：%d/%d\n"), round, roundMax);
                    }else
                    {
                        if (FindPic(hWnd, _T("bmp\\General\\fashu.bmp"), NULL, &rect))
                        {
                            rect.OffsetRect(-12, 264);
                            MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                        }
                    }
                }
            }

            if (bFlag) {
                KeyboardButtonEx(VK_F7);
                Sleep(200);
                round = 0;
                bFlag = FALSE;
            }
            if (bTest) {
                mpLoad[it->second]++;
                nLoad++;
                TRACE(_T("已抓到种类%d的宠物数/缺少数：%d/%d\n"), it->second, mpLoad[it->second], 16 - it->first);
                TRACE(_T("已抓到宠物数/携带数：%d/%d\n"), nLoad, nLoadMax);
                bTest = FALSE;
            }


            if (nLoad >= nLoadMax || GetRoleStatusValue(hWnd, MANA_POINT) < 10)
                break;

            CheckRoleStatus(hWnd, HEALTH_POINT, 50);
            if (GetFarthestPoint(hWnd, mapIndex, &rect)) {

                MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
            }
        }else
        {
            TRACE(_T("找怪ing...\n"));
        }

        bStatic = IsRoleStatic(hWnd);
    }

    return TRUE;
}



BOOL GotoHotel(HWND hWnd)
{
    int movetype;
    POINT ptPos;

    if (GetMapIndex(hWnd) == MAP_CITY_CA && GetGameRolePos(hWnd, ptPos))
    {
        int distance = (int)std::sqrt(double((ptPos.x - 460) * (ptPos.x - 460) + (ptPos.y - 164) * (ptPos.y - 164)));
        movetype = (distance < 100) ? (Cmap::TAB_MOVE) : (Cmap::CITY_MOVE);    
    }else
        movetype = Cmap::CITY_MOVE;

    Cmap mp(MAP_CITY_CA, MAP_CA_HOTEL, CRect(449, 159, 465, 169), DIRECT_EXIT, movetype);
    std::list<Cmap> line;
    line.push_back(mp);
    if (!LineMove(hWnd, line, MAP_CA_HOTEL)) {

        TRACE(_T("move to shop failed.\n"));
        return FALSE;
    }

    CRect  rect;
    BOOL  bStatic = TRUE;
    while (!FindSomePic(hWnd, _T("bmp\\general\\xiuxi*.bmp"), NULL, &rect))
    {
        if (!FindSomePic(hWnd, _T("bmp\\general\\zd*.bmp"), NULL, &rect))
        {
            if (GetMapIndex(hWnd) != MAP_CA_HOTEL)
            {
                TRACE(_T("当前场景不在酒店里面!\n"));
                return GotoHotel(hWnd);
            }
            if (!GetGameRolePos(hWnd, ptPos))
                continue;
            if (!CRect(33, 13, 54, 22).PtInRect(ptPos))
            {
                if (!CRect(25, 13, 34, 20).PtInRect(ptPos))
                {
                    if (bStatic)
                    {
                        if (!FindSomePic(hWnd, _T("bmp\\mazeMap\\60\\0move*.bmp"), NULL, &rect))
                        {
                            // 屏蔽玩家
                            KeyboardButtonEx(VK_F9);
                            Sleep(500);
                            TRACE(_T("等待查找移动的位置1。。\n"));
                        }else
                        {
                            MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                        }
                    }
                    bStatic = IsRoleStaticEx(hWnd, &ptPos);
                }else // 到达CRect(25, 13, 34, 20)
                {
                    if (bStatic)
                    {
                        if (!FindSomePic(hWnd, _T("bmp\\mazeMap\\60\\1move*.bmp"), NULL, &rect))
                        {
                            // 屏蔽玩家
                            KeyboardButtonEx(VK_F9);
                            TRACE(_T("等待查找移动的位置2。。\n"));
                        }else
                        {
                            MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                        }
                    }
                    bStatic = IsRoleStaticEx(hWnd, &ptPos);
                }
            }
            else // 到达CRect(33, 13, 54, 22)
            {
                // 等待人物停止行动
                while (!IsRoleStatic(hWnd))
                {
                    TRACE(_T("wait for static\n"));
                    Sleep(200);
                }
                if (!FindSomePic(hWnd, _T("bmp\\npc\\jdlb\\"), NULL, &rect))
                {
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
                    // 屏蔽玩家
                    if (!bF9)
                    {
                        KeyboardButtonEx(VK_F9);
                        Sleep(500);
                        bF9 = TRUE;
                    }
                    TRACE(_T("查找酒店老板。。\n"));
                }else{
                    // 对话酒店老板
                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
                    Sleep(1000);
                    TRACE(_T("等待与酒店老板对话。。\n"));
                }
            }  
        }else{
            // 点击住店
            MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\zd*.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
            while (!FindSomePic(hWnd, _T("bmp\\general\\xiuxi*.bmp")))
            {
                TRACE(_T("等待点击住店。。\n"));
                Sleep(500);
            }
        }
    }


    do {
        rect.InflateRect(0, 0, 0, 60);
        MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
        Sleep(1000);
        TRACE(_T("住店完成！\n"));
    }while (FindSomePic(hWnd, _T("bmp\\general\\xiuxi*.bmp"), NULL, &rect));

    return TRUE;
}



void GetConfigMonsterInfo(std::vector<MONSTERSHOPINFO> & vecShopInfo)
{
    MONSTERSHOPINFO monsterShopInfo;
    TCHAR monster[MAX_PATH], price[MAX_PATH];
    for (int i = 0; i < 10; i++)
    {
        wsprintf(monster, _T("monster%d"), i);
        wsprintf(price, _T("Price%d"), i);
        monsterShopInfo.monsterType  = theApp.GetProfileInt(_T("ShopSetting"), monster, 0);
        monsterShopInfo.monsterPrice = theApp.GetProfileInt(_T("ShopSetting"), price, 0);
        vecShopInfo.push_back(monsterShopInfo);
    }
}


int GetShopMonsterNum(HWND hWnd)
{
    CRect rectBegin, rectEnd;

    if (!FindPic(hWnd, _T("bmp\\number\\num\\begin.bmp"), NULL, &rectBegin))
        return INVALID_VALUE;

    rectEnd = rectBegin;
    rectEnd.right += 110;


    if (!FindPic(hWnd, _T("bmp\\number\\num\\end.bmp"), &rectEnd, &rectEnd))
        return INVALID_VALUE;

    RECT rect = {rectEnd.left - 8, rectEnd.top, rectEnd.left, rectEnd.bottom};

    TCHAR FileName[MAX_PATH];
    if (!FindSomePic(hWnd, _T("bmp\\number\\num\\"), &rect, &rect, FileName))
        return INVALID_VALUE;

    int num = _tstoi(FileName);
    rectEnd.left  = rect.left - 8;
    rectEnd.right = rect.left;
    if (!FindSomePic(hWnd, _T("bmp\\number\\num\\"), &rectEnd, NULL, FileName))
        return num;
    else
        return 10 * _tstoi(FileName) + num;
}


//BOOL CheckShop(HWND hWnd, const std::vector<MONSTERSHOPINFO> & vecShopInfo, std::multimap<Mst_Num, Mst_Ty> & mtmp)
//{
//    int movetype;
//    if (GetMapIndex(hWnd) == MAP_CITY_CA)
//    {
//        POINT ptPos;
//        while (!GetGameRolePos(hWnd, ptPos))
//        {
//            BeWarning(_T("wav\\BJ.wav"));
//            TRACE(_T("CheckShop >> GetGameRolePos failed.\n"));
//            Sleep(200);
//        }
//        int distance = (int)std::sqrt(double((ptPos.x - 326) * (ptPos.x - 326) + (ptPos.y - 14) * (ptPos.y - 14)));
//        movetype = (distance < 100) ? (Cmap::TAB_MOVE) : (Cmap::CITY_MOVE);
//    }else
//    {
//        movetype = Cmap::CITY_MOVE;
//    }
//
//    Cmap mp(MAP_CITY_CA, MAP_CA_SHOP, CRect(316, 10, 337, 24), DIRECT_EXIT, movetype);
//    if (!mp.MapMove(hWnd)) {
//
//        TRACE(_T("move to shop failed.\n"));
//        return FALSE;
//    }
//
//
//    CRect rect, rectTemp;
//    while (!FindPic(hWnd, _T("bmp\\general\\chuangdian.bmp"), NULL, &rect))
//    {
//        if (!FindPic(hWnd, _T("bmp\\general\\suanle.bmp"), NULL, &rect))
//        {
//            if (!FindSomePic(hWnd, _T("bmp\\general\\dakaisd*.bmp"), NULL, &rect))
//            {
//                if (!FindSomePic(hWnd, _T("bmp\\npc\\shlb\\"), NULL, &rect)) 
//                {
//
//                    KeyboardButtonEx(VK_F9);
//                    KeyboardButtonEx('H', VK_MENU);
//                    Sleep(500);
//                    TRACE(_T("查找NPC--商会老板.\n"));
//                }else
//                {
//                    // 对话商会老板
//                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
//                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
//                    Sleep(1000);
//                    TRACE(_T("等待与商会老板对话。。\n"));
//                }
//            }else
//            {
//                // 点击我要进入我的商店列表
//                MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\dakaisd*.bmp"), CLOSE_WINDOW, NULL);
//                TRACE(_T("等待打开我的商店列表。。\n"));
//            }
//        }else
//        {
//            rect = CRect(rect.left, rect.top - 15, rect.right + 20, rect.bottom - 15);
//            // 点击商店名
//            MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\suanle.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN);
//        }
//    }
//
//
//    // 先检查身上的宠物情况
//    std::list<Mst_Ty> loadlist;
//    std::list<Mst_Ty>::iterator it;
//    TCHAR retFileName[MAX_PATH], FileName[MAX_PATH];
//    UINT j = 0;
//    rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
//    while (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, &rectTemp, retFileName, 100, RGB(180, 180, 180)))
//    {
//        loadlist.push_back(_tstoi(retFileName));
//        rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
//        j++;
//        rectTemp.OffsetRect(0, 20 * j);
//    }
//
//
//    // 开始检查商店并上架
//    BOOL   bEmpty = TRUE;
//    int    monsterNum;
//    static int InputPrice;
//    int shopNum = theApp.GetProfileInt(_T("BaseSetting"), _T("Counter"), 1);
//    for (int i = 0; i < shopNum; i++)
//    {
//        // 判断身上是否有宠物
//        if (!loadlist.empty())
//        {
//            int total = loadlist.size();
//            int num = 0;
//            for (it = loadlist.begin(); it != loadlist.end(); ++it)
//            {
//                if (*it == vecShopInfo[i].monsterType)
//                    num++;
//            }
//
//            if (num == total)
//            {
//                // 选择全选
//                rectTemp = CRect(rect.left + 501, rect.top + 302, rect.right + 489, rect.bottom + 304);
//                if (!FindPic(hWnd, _T("bmp\\general\\whitegou.bmp"), &rectTemp, &rectTemp)) {
//
//                    MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//                }
//
//                loadlist.clear();
//            }else if (num < total && num > 0)
//            {
//                // 单个选中与柜台设置同类型的宠物并删除已经选中的同类宠物
//                j = 0;
//                for (it = loadlist.begin(); it != loadlist.end();)
//                {
//                    if (*it == vecShopInfo[i].monsterType)
//                    {
//                        rectTemp = CRect(rect.left + 502, rect.top + 100, rect.right + 486, rect.bottom + 96);
//                        rectTemp.OffsetRect(0, 20 * j);
//                        MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//                        it = loadlist.erase(it);
//                    }
//                    else
//                        ++it;
//
//                    j++;
//                }
//            }
//
//            if (num != 0)
//            {
//                // 点击放入商店
//                rectTemp = CRect(rect.left + 333, rect.top + 333, rect.right + 358, rect.bottom + 332);
//                MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//
//                wsprintf(FileName, _T("bmp\\monster\\%d.bmp"), vecShopInfo[i].monsterType);
//                rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
//                while (FindPic(hWnd, FileName, &rectTemp, &rectTemp, 100, RGB(180, 180, 180))) 
//                {
//                    TRACE(_T("等待宠物放入商店！\n"));
//                    Sleep(200);
//                }
//            }
//        }
//
//        // 判断店内是否有未上架的宠物
//        rectTemp = CRect(rect.left + 19, rect.top + 53, rect.right + 217, rect.bottom + 239);
//        if (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, &rectTemp, retFileName, 100, RGB(180, 180, 180)))
//        {
//            // 选中要上架的宠物
//            MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, _T("bmp\\general\\shanggui*.bmp"), OPEN_WINDOW, NULL);
//
//            // 选择批量
//            rectTemp = CRect(rect.left + 218, rect.top + 267, rect.right + 206, rect.bottom + 269);
//            if (!FindPic(hWnd, _T("bmp\\general\\whitegou.bmp"), &rectTemp, &rectTemp)) {
//
//                MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//            }
//
//            if (InputPrice != vecShopInfo[i].monsterPrice)
//            {
//                // 整理输入框
//                rectTemp = CRect(rect.left + 54, rect.top + 263, rect.right + 73, rect.bottom + 269);
//                while (!FindSomePic(hWnd, _T("bmp\\general\\null*.bmp"), &rectTemp, NULL))
//                {
//                    KeyboardButtonEx(VK_BACK);
//                }
//
//                // 输入价格
//                InputNumber(vecShopInfo[i].monsterPrice);
//                InputPrice = vecShopInfo[i].monsterPrice;
//            }
//
//            // 点击上架
//            rectTemp = CRect(rect.left + 172, rect.top + 267, rect.right + 169, rect.bottom + 267);
//            MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//            Sleep(500);
//        }
//
//        // 获取当前柜台宠物数量
//        do {
//            monsterNum = GetShopMonsterNum(hWnd);
//            Sleep(200);
//        }while(monsterNum == INVALID_VALUE);
//
//        mtmp.insert(std::pair<Mst_Num, Mst_Ty>(monsterNum, vecShopInfo[i].monsterType));
//
//
//        // 如果是最后一间, 先判断身上是否还有宠物，然后退出循环
//        if (i + 1 == shopNum)
//        {
//            rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
//            if (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, NULL, retFileName, 100, RGB(180, 180, 180)))
//                bEmpty = FALSE;
//            break;
//        }
//
//
//        // 判断当前的柜台位置
//        DWORD dwTimediff, dwTime = GetTickCount();
//        j = 0;
//        do {
//            dwTimediff = GetTickCount() - dwTime;
//            if (dwTimediff >= j * WAIT_TIME_OUT && dwTimediff < (j + 1) * WAIT_TIME_OUT)
//            {
//                // 下间按钮位置
//                rectTemp = CRect(rect.left + 144, rect.top + 337, rect.right + 136, rect.bottom + 331);
//                MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
//                j++;
//            }
//
//            rectTemp = CRect(rect.left + 263, rect.top + 41, rect.right + 272, rect.bottom + 221);
//            while (!FindSomePic(hWnd, _T("bmp\\number\\greenNum\\"), &rectTemp, NULL, retFileName))
//            {
//                TRACE(_T("找不到柜台序号！\n"));
//                Sleep(200);
//                BeWarning(_T("wav\\BJ.wav"));
//            }
//        }while (_tstoi(retFileName) != i + 2);
//    }
//
//    // 点击离开关闭店铺
//    rectTemp = CRect(rect.left + 525, rect.top + 332, rect.right + 521, rect.bottom + 331);
//    MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, _T("bmp\\general\\chuangdian*.bmp"), CLOSE_WINDOW, NULL);
//
//    if (!bEmpty)
//    {
//        mtmp.clear();
//        return CheckShop(hWnd, vecShopInfo, mtmp);
//    }
//    else
//        return TRUE;
//}


BOOL CheckShop(HWND hWnd, const std::vector<MONSTERSHOPINFO> & vecShopInfo, std::multimap<Mst_Num, Mst_Ty> & mtmp)
{
    int movetype;
    if (GetMapIndex(hWnd) == MAP_CITY_CA)
    {
        POINT ptPos;
        while (!GetGameRolePos(hWnd, ptPos))
        {
            BeWarning(_T("wav\\BJ.wav"));
            TRACE(_T("CheckShop >> GetGameRolePos failed.\n"));
            Sleep(200);
        }
        int distance = (int)std::sqrt(double((ptPos.x - 326) * (ptPos.x - 326) + (ptPos.y - 14) * (ptPos.y - 14)));
        movetype = (distance < 100) ? (Cmap::TAB_MOVE) : (Cmap::CITY_MOVE);
    }else
    {
        movetype = Cmap::CITY_MOVE;
    }

    Cmap mp(MAP_CITY_CA, MAP_CA_SHOP, CRect(316, 10, 337, 24), DIRECT_EXIT, movetype);
    if (!mp.MapMove(hWnd)) {

        TRACE(_T("move to shop failed.\n"));
        return FALSE;
    }


    CRect rect, rectTemp, rectTemp2;
    while (!FindPic(hWnd, _T("bmp\\general\\chuangdian.bmp"), NULL, &rect))
    {
        if (!FindPic(hWnd, _T("bmp\\general\\suanle.bmp"), NULL, &rect))
        {
            if (!FindSomePic(hWnd, _T("bmp\\general\\dakaisd*.bmp"), NULL, &rect))
            {
                if (!FindSomePic(hWnd, _T("bmp\\npc\\shlb\\"), NULL, &rect)) 
                {

                    if (!bF9)
                    {
                        KeyboardButtonEx(VK_F9);
                        bF9 = TRUE;
                    }
                    if (!bctrlE)
                    {
                        KeyboardButtonEx('H', VK_MENU);
                        bctrlE = TRUE;
                    }
                    Sleep(2000);
                    TRACE(_T("查找NPC--商会老板.\n"));
                }else
                {
                    // 对话商会老板
                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
                    Sleep(1000);
                    TRACE(_T("等待与商会老板对话。。\n"));
                }
            }else
            {
                // 点击我要进入我的商店列表
                MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\dakaisd*.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
                while (!FindSomePic(hWnd, _T("bmp\\general\\suanle.bmp")))
                {
                    TRACE(_T("等待打开我的商店列表。。\n"));
                    Sleep(500);
                }
            }
        }else
        {
            rect = CRect(rect.left, rect.top - 15, rect.right + 20, rect.bottom - 15);
            // 点击商店名
            MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\suanle.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
            while (!FindPic(hWnd, _T("bmp\\general\\chuangdian.bmp")))
            {
                TRACE(_T("等待打开我的商店界面。。\n"));
                Sleep(500);
            }
        }
    }


    // 开始检查商店并上架
    UINT j;
    std::list<Mst_Ty> loadlist;
    std::list<Mst_Ty>::iterator it;
    TCHAR retFileName[MAX_PATH];
    BOOL   bEmpty = TRUE;
    int    monsterNum;
    static int InputPrice;
    int shopNum = theApp.GetProfileInt(_T("BaseSetting"), _T("Counter"), 1);
    for (int i = 0; i < shopNum; i++)
    {
        // 先检查身上的宠物情况
        rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
        while (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, NULL, retFileName, 100, RGB(180, 180, 180)))
        {
            loadlist.push_back(_tstoi(retFileName));
            rectTemp.OffsetRect(0, 20);
        }


        // 判断身上是否有宠物
        if (!loadlist.empty())
        {
            int total = loadlist.size();
            int num = 0;
            for (it = loadlist.begin(); it != loadlist.end(); ++it)
            {
                if (*it == vecShopInfo[i].monsterType)
                    num++;
            }

            if (num == total)
            {
                // 选择全选
                rectTemp = CRect(rect.left + 501, rect.top + 302, rect.right + 489, rect.bottom + 304);
                rectTemp2 = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 244);
                 do{

                    MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
                    Sleep(1000);
                }while (FindPic(hWnd, _T("bmp\\general\\unchecked.bmp"), &rectTemp2));

            }else if (num < total && num > 0)
            {
                // 单个选中与柜台设置同类型的宠物并删除已经选中的同类宠物
                rectTemp = CRect(rect.left + 502, rect.top + 100, rect.right + 486, rect.bottom + 96);
                for (it = loadlist.begin(); it != loadlist.end(); ++it)
                {
                    if (*it == vecShopInfo[i].monsterType)
                        MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);

                    rectTemp.OffsetRect(0, 20);
                }
            }

            if (num != 0)
            {
                // 点击放入商店
                rectTemp  = CRect(rect.left + 333, rect.top + 333, rect.right + 358, rect.bottom + 332);
                rectTemp2 = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 244);
                do {
                    MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
                    Sleep(1000);
                }while (FindPic(hWnd, _T("bmp\\general\\checked.bmp"), rectTemp2, NULL, 100, RGB(255,255,255)));
            }
        }

        // 判断店内是否有未上架的宠物
        rectTemp = CRect(rect.left + 19, rect.top + 53, rect.right + 217, rect.bottom + 239);
        if (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, &rectTemp, retFileName, 100, RGB(180, 180, 180)))
        {
            // 选中要上架的宠物
            MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, _T("bmp\\general\\shanggui*.bmp"), OPEN_WINDOW, NULL, 5000);

            // 选择批量
            rectTemp = CRect(rect.left + 218, rect.top + 267, rect.right + 206, rect.bottom + 269);
            if (!FindPic(hWnd, _T("bmp\\general\\whitegou.bmp"), &rectTemp, &rectTemp)) {

                MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
            }

            if (InputPrice != vecShopInfo[i].monsterPrice)
            {
                // 整理输入框
                rectTemp = CRect(rect.left + 54, rect.top + 263, rect.right + 73, rect.bottom + 269);
                while (!FindSomePic(hWnd, _T("bmp\\general\\null*.bmp"), &rectTemp, NULL))
                {
                    KeyboardButtonEx(VK_BACK);
                }

                // 输入价格
                InputNumber(vecShopInfo[i].monsterPrice);
                InputPrice = vecShopInfo[i].monsterPrice;
            }

            // 点击上柜
            rectTemp = CRect(rect.left + 172, rect.top + 267, rect.right + 169, rect.bottom + 267);
            MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
            Sleep(1000);
        }

        // 获取当前柜台宠物数量
        do {
            monsterNum = GetShopMonsterNum(hWnd);
            Sleep(200);
        }while(monsterNum == INVALID_VALUE);

        mtmp.insert(std::pair<Mst_Num, Mst_Ty>(monsterNum, vecShopInfo[i].monsterType));


        // 如果是最后一间, 先判断身上是否还有宠物，然后退出循环
        if (i + 1 == shopNum)
        {
            rectTemp = CRect(rect.left + 301, rect.top + 97, rect.right + 504, rect.bottom + 104);
            if (FindSomePic(hWnd, _T("bmp\\monster\\"), &rectTemp, NULL, retFileName, 100, RGB(180, 180, 180)))
                bEmpty = FALSE;
            break;
        }


        // 判断当前的柜台位置
        DWORD dwTimediff, dwTime = GetTickCount();
        j = 0;
        do {
            dwTimediff = GetTickCount() - dwTime;
            if (dwTimediff >= j * 20000 && dwTimediff < (j + 1) * 20000)
            {
                // 下间按钮位置
                rectTemp = CRect(rect.left + 144, rect.top + 337, rect.right + 136, rect.bottom + 331);
                MouseInput(hWnd, &rectTemp, MOUSE_LEFT_BUTTON);
                j++;
            }

            rectTemp = CRect(rect.left + 263, rect.top + 41, rect.right + 272, rect.bottom + 221);
            while (!FindSomePic(hWnd, _T("bmp\\number\\greenNum\\"), &rectTemp, NULL, retFileName))
            {
                TRACE(_T("找不到柜台序号！\n"));
                Sleep(200);
            }
        }while (_tstoi(retFileName) != i + 2);

        loadlist.clear();
    }

    // 点击离开关闭店铺
    rectTemp = CRect(rect.left + 525, rect.top + 332, rect.right + 521, rect.bottom + 331);
    MouseInputEx(hWnd, &rectTemp, MOUSE_LEFT_BUTTON, _T("bmp\\general\\chuangdian*.bmp"), CLOSE_WINDOW, NULL, 5000);

    if (!bEmpty)
    {
        mtmp.clear();
        return CheckShop(hWnd, vecShopInfo, mtmp);
    }
    else
    {
        bF9    = FALSE;
        bctrlE = FALSE;
        return TRUE;
    }
}


BOOL ShutDown(DWORD dwFlag)
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    // 打开当前进程访问代号
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return FALSE;

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);//获取本地唯一标识用于在特定系统中设置权限
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    //提升访问令牌权限

    return ExitWindowsEx(dwFlag, 0);
}



__declspec(naked)
int GetSystem()
{
    _asm
    {
        mov eax,fs:[0x30]
        mov eax,[eax+0A4h]
        ret
    }
}
