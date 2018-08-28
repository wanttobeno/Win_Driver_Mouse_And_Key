// bm.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "bm.h"
#include "GameState.h"
#include <cmath>



#define GRAY(r,g,b)      (BYTE)((r * 299 + g * 587 + b * 114) / 1000)   //灰度值计算
// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 bm.h


// 通过窗口截图获取位图信息
Cbm::Cbm(HWND hwndScreen)
{
    HDC     hdc, hdcMem, hdcScreen;
    HBITMAP hBitmap;
    RECT    rect;

    if (!hwndScreen)
    {
        memset(&rect, 0, sizeof(RECT));
        rect.right   = GetSystemMetrics(SM_CXSCREEN);
        rect.bottom  = GetSystemMetrics(SM_CYSCREEN);
    }else
        GetClientRect(hwndScreen, &rect);    //获得截图窗口的范围大小

    hdc     = GetDC(NULL);
    hdcMem  = CreateCompatibleDC(hdc);
    hBitmap = CreateCompatibleBitmap(hdc, rect.right - rect.left, rect.bottom - rect.top);
    SelectObject(hdcMem, hBitmap);

    hdcScreen  = GetDC(hwndScreen);
    BitBlt(hdcMem, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcScreen, 0, 0, SRCCOPY);

    DeleteDC(hdcMem);
    ReleaseDC(hwndScreen, hdcScreen);

    //初始化信息头bmi结构
    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
    bmih.biSize        = sizeof(BITMAPINFOHEADER);
    bmih.biWidth       = rect.right - rect.left;
    bmih.biHeight      = rect.bottom - rect.top;
    bmih.biBitCount    = 24;
    bmih.biCompression = BI_RGB;
    bmih.biPlanes      = 1;

    SetcxDib();
    SetcyDib();
    SetcBits();

    //获取pBits的值
    pBits = new BYTE [cBits * cyDib];

    GetDIBits(hdc, hBitmap, 0, cyDib, pBits, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);

    //初始化文件头bmfh
    bmfh.bfType      = 0x4D42;
    bmfh.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + cBits * cyDib;
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    ReleaseDC(NULL, hdc);
    DeleteObject(hBitmap);
}


// 通过加载文件获取位图信息
Cbm::Cbm(PCTSTR FilePath)
{
    HANDLE hFile = CreateFile(FilePath, GENERIC_READ, 
        FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        pBits = NULL;
        TRACE(_T("read file failed. FileName: %s\n"), FilePath);
        return;
    }

    DWORD dwBytesRead;
    if ( !(ReadFile(hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL) &&
        ReadFile(hFile, &bmih, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL) &&
        bmfh.bfType == 0x4D42) )
    {
        pBits = NULL;
        TRACE(_T("read file failed. FileName: %s\n"), FilePath);
        CloseHandle(hFile);
        return;
    }

    SetcxDib();
    SetcyDib();
    SetcBits();

    pBits = new BYTE [cBits * cyDib];
    if (!ReadFile(hFile, pBits, cBits * cyDib, &dwBytesRead, NULL))
    {
        delete [] pBits;
        pBits = NULL;
        TRACE(_T("read file failed. FileName: %s\n"), FilePath);
    }
    CloseHandle(hFile);
}

//截取指定范围的位图信息
Cbm::Cbm(const Cbm & bm, LPCRECT pRect)
{
    //判断pRect范围变量
    if (!(pRect && bm.pBits && bm.IsInRect(*pRect)))
    {
        *this = bm;
        return;
    }

    //初始化信息头
    bmih = bm.bmih;
    bmih.biWidth  = pRect->right - pRect->left;
    bmih.biHeight = pRect->bottom - pRect->top;

    SetcxDib();
    SetcyDib();
    SetcBits();
    bmih.biSizeImage = cBits * cyDib;

    //初始化文件头
    bmfh = bm.bmfh;
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + cBits * cyDib;

    //初始化像素指针
    pBits = new BYTE [cBits * cyDib];

    //复制位图像素数据
    for (int y = 0; y < cyDib; y++)
        for (int x = 0; x < cxDib; x++)
        {
            int index   = (cyDib - 1 - y) * cBits + 3 * x;
            int bmIndex = (bm.cyDib - 1 - pRect->top - y) * bm.cBits + 3 * (x + pRect->left);
            pBits[index]     = bm.pBits[bmIndex];
            pBits[index + 1] = bm.pBits[bmIndex + 1];
            pBits[index + 2] = bm.pBits[bmIndex + 2];
        }
}

Cbm & Cbm::operator =(const Cbm & bm)
{
    if (this == &bm)
        return *this;

    bmfh  = bm.bmfh;
    bmih  = bm.bmih;
    cxDib = bm.cxDib;
    cyDib = bm.cyDib;
    cBits = bm.cBits;
    if (bm.pBits)
    {
        pBits = new BYTE [cBits * cyDib];
        for (int i = 0; i < cBits * cyDib; i++)
            pBits[i] = bm.pBits[i];
    }
    else
        pBits = NULL;

    return *this;
}


BOOL operator==(const Cbm & bm1, const Cbm & bm2)
{
    BOOL bRet = FALSE;
    if ((bm1.cxDib == bm2.cxDib) && (bm1.cyDib == bm2.cyDib))
    {
        for (int i = 0; i < (bm1.cBits * bm1.cyDib); i++)
        {
            bRet = (bm1.pBits[i] == bm2.pBits[i]);
            if (!bRet)
                return FALSE;
        }
        return bRet;
    }else
        return FALSE;
}
// 析构函数
Cbm::~Cbm()
{
    if (pBits)
        delete [] pBits;
}


// 将图像灰度化
void Cbm::GrayBitmap()
{
    for (int y = 0; y < cyDib; y++)
        for (int x = 0; x < cxDib; x++)
        {
            BYTE grayColor = GRAY(pBits[y * cBits + 3 * x + 2], pBits[y * cBits + 3 * x + 1], pBits[y * cBits + 3 * x]);
            pBits[y * cBits + 3 * x] = pBits[y * cBits + 3 * x + 1] = pBits[y * cBits + 3 * x + 2] = grayColor;
        }
}


// 通过位图坐标查找RGB颜色值
COLORREF Cbm::GetPixel(int x, int y) const
{
    // 注意DIB像素是从下往上存储, x坐标不变,y坐标值从下往上递增.
    y = cyDib - 1 - y;
    return RGB(pBits[y * cBits + 3 * x + 2],
        pBits[y * cBits + 3 * x + 1],
        pBits[y * cBits + 3 * x]);
}


// 改写指定坐标的位图RGB颜色
void Cbm::SetPixel(int x, int y, COLORREF targetColor)
{
    y = cyDib - 1 - y;
    pBits[y * cBits + 3 * x + 2] = GetRValue(targetColor);
    pBits[y * cBits + 3 * x + 1] = GetGValue(targetColor);
    pBits[y * cBits + 3 * x]     = GetBValue(targetColor);
}

//判断矩形区域是否在位图范围内
BOOL Cbm::IsInRect(const RECT & rect) const
{
    return rect.left >= 0  &&
           rect.top  >= 0  &&
           rect.right <= cxDib &&
           rect.bottom <= cyDib ;
}


BOOL Cbm::IsRectOnTheEdge(const RECT &rect)
{
    return rect.left > 10 &&
           rect.top > 10 &&
           rect.right < cxDib - 10 &&
           rect.bottom < cyDib - 10;
}


// 显示位图
void Cbm::DisplayBitmap(HWND hwnd) const
{
    HDC hdc = GetDC(hwnd);
    SetDIBitsToDevice (hdc, 0, 0, cxDib, cyDib, 0, 0, 0, cyDib, pBits, (LPBITMAPINFO)&bmih, DIB_RGB_COLORS);
    ReleaseDC(hwnd, hdc);
}


// 获取位图的rect区域
void Cbm::GetBitmapRect(RECT & rect) const
{
    rect.left   = 0;
    rect.top    = 0;
    rect.right  = cxDib;
    rect.bottom = cyDib;
}

// 保存位图到文件
BOOL Cbm::SaveBitmapToFile(PCTSTR FileName, LPCRECT pRect) const
{
    ASSERT(pBits);

    TCHAR FilePath[MAX_PATH], DefaultFileName[MAX_PATH];
    //创建以系统时间命名的bmp文件
    SYSTEMTIME time;
    GetLocalTime(&time);
    wsprintf(DefaultFileName, _T("%04u%02u%02u%02u%02u%02u%03u.bmp"),
        time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

    //修正保存路径，默认保存至当前程序目录Screen文件夹
    if (!FileName)
        wsprintf(FilePath, _T("%s\\%s"), _T("screen"), DefaultFileName);
    else
    {
        if (FileName[1] == ':')
            _tcscpy_s(FilePath, FileName);
        else
            wsprintf(FilePath, _T("%s\\%s"), _T("screen"), FileName);


        if (FileName[lstrlen(FileName) - 1] == '\\' || FileName[lstrlen(FileName) - 1] == '/')
            _tcscat_s(FilePath, MAX_PATH, DefaultFileName);
    }

    // 判断文件路径是否有效，无效则创建路径中没有的文件夹
    if (!PathIsDirectory(FilePath))
        CreateFolder(FilePath);

    //保存数据
    HANDLE hFile = CreateFile(FilePath, GENERIC_WRITE, 0 ,NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;


    DWORD dwBytesWritten;
    Cbm bmFile(*this, pRect);
    BOOL bSuccess = WriteFile(hFile, &bmFile.bmfh, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL) &&
                    WriteFile(hFile, &bmFile.bmih, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL) &&
                    WriteFile(hFile, bmFile.pBits, bmFile.cBits * bmFile.cyDib, &dwBytesWritten, NULL);

    CloseHandle(hFile);
    if (!bSuccess)
        DeleteFile(FilePath);

    return bSuccess;
}

inline int hmean(int dest, int src)
{
    return 100 * (dest + 255) / (src + 255);
}


/*****************************比较找图函数***************************************
*     形参说明 bmWnd: 大图、屏幕截图、源图
*              bmFile: 特征图、小图、从文件加载
*       (IN OUT) rect: 找图的范围,返回特征图在源图中的坐标范围
*                 rgb: 透明色RGB值，透明色视为找图中可以被忽略的颜色
*            resemble: 相似度，相同像素占总像素的比例,默认值为0.9
**********************************************************************************/
BOOL FindPic(HWND hWnd, PCTSTR FileName, IN LPCRECT rectTarget, OUT PRECT retRect, int resemble, COLORREF rgb)
{
    return FindPic(Cbm(hWnd), Cbm(FileName), rectTarget, retRect, resemble, rgb);
}

BOOL FindPic(const Cbm & bmWnd, PCTSTR FileName, IN LPCRECT rectTarget, OUT PRECT retRect, int resemble, COLORREF rgb)
{
    return FindPic(bmWnd, Cbm(FileName), rectTarget, retRect, resemble, rgb);
}

BOOL FindPic(PCTSTR destFileName, PCTSTR srcFileName, LPCRECT rectTarget, PRECT retRect, int resemble, COLORREF rgb)
{
    return FindPic(Cbm(destFileName), Cbm(srcFileName), rectTarget, retRect, resemble, rgb);
}

BOOL FindPic(const Cbm & bmWnd, const Cbm & bmFile, LPCRECT rectTarget, OUT PRECT retRect, int resemble, COLORREF rgb)
{
    if (!(bmFile.pBits && bmWnd.pBits) || bmFile.cxDib > bmWnd.cxDib || bmFile.cyDib > bmWnd.cyDib)
        return FALSE;

    resemble = max(resemble, 0);
    resemble = min(resemble, 100);

    BYTE r = GetRValue(rgb);
    BYTE g = GetGValue(rgb);
    BYTE b = GetBValue(rgb);


    // 实际范围
    RECT rectDefault;
    if (rectTarget && bmWnd.IsInRect(*rectTarget))
        rectDefault = *rectTarget;
    else
        bmWnd.GetBitmapRect(rectDefault);

    // bmFile图像坐标(x, y),  bmWnd图像坐标(x + xOffset, y + yOffset)
    int yTotal        =    rectDefault.bottom - bmFile.cyDib;
    int xTotal        =    rectDefault.right - bmFile.cxDib;
    int invalidTotal  =    (100 - resemble) * (bmFile.cxDib * bmFile.cyDib);
    int validTotal    =    resemble * (bmFile.cxDib * bmFile.cyDib);


    //  ignoreNum忽略值, validNum有效值，invalidNum无效值
    int invalidNum = 0, validNum = 0,  ignoreNum = 0;
    for (int yOffset = rectDefault.top; yOffset <= yTotal; yOffset++)
        for (int xOffset = rectDefault.left; xOffset <= xTotal; xOffset++)
        {
            for (int y = 0, bflag = TRUE; bflag && (y < bmFile.cyDib); y++)
                for (int x = 0; x < bmFile.cxDib; x++)
                {
                    int FileIndex = (bmFile.cyDib - 1 - y) * bmFile.cBits + 3 * x;
                    int WndIndex  = (bmWnd.cyDib - 1 - yOffset - y) * bmWnd.cBits + 3 * (xOffset + x);


                    if (r    == bmFile.pBits[FileIndex + 2] &&
                        g    == bmFile.pBits[FileIndex + 1] &&
                        b    == bmFile.pBits[FileIndex]     &&
                        0xF8 != bmWnd.pBits[WndIndex + 2]   &&
                        0xFC != bmWnd.pBits[WndIndex + 1]   &&
                        0xF8 != bmWnd.pBits[WndIndex]) {

                            ignoreNum++;
                    }		
                    else if (bmFile.pBits[FileIndex + 2] == bmWnd.pBits[WndIndex + 2] &&
                        bmFile.pBits[FileIndex + 1] == bmWnd.pBits[WndIndex + 1] &&
                        bmFile.pBits[FileIndex] == bmWnd.pBits[WndIndex]) {

                            validNum++;
                    }
                    else
                        invalidNum++;


                    if (100 * invalidNum > invalidTotal)
                    {
                        invalidNum = validNum = ignoreNum = 0;
                        bflag = FALSE;
                        break;
                    }

                    if (100 * (validNum + ignoreNum) >= validTotal)
                    {
                        if (retRect)
                        {
                            retRect->left   = xOffset;
                            retRect->top    = yOffset;
                            retRect->right  = xOffset + bmFile.cxDib;
                            retRect->bottom = yOffset + bmFile.cyDib;
                        }
                        return TRUE;
                    }
                }
        }
        return FALSE;
}


BOOL FindPicEx(const Cbm & bmWnd, PCTSTR FileName,
        PRECT pRect, int deviation, int resemble, COLORREF rgb)
{return FindPicEx(bmWnd, Cbm(FileName), pRect, deviation, resemble, rgb);}

BOOL FindPicEx(PCTSTR destFileName, PCTSTR srcFileName,
        PRECT pRect, int deviation, int resemble, COLORREF rgb)
{return FindPicEx(Cbm(destFileName), Cbm(srcFileName), pRect, deviation, resemble, rgb);}

BOOL FindPicEx(HWND hWnd, PCTSTR FileName,
        PRECT pRect, int deviation, int resemble, COLORREF rgb)
{return FindPicEx(Cbm(hWnd), Cbm(FileName), pRect, deviation, resemble, rgb);}

BOOL FindPicEx(const Cbm & bmWnd, const Cbm & bmFile, PRECT pRect, int deviation, int resemble, COLORREF rgb)
{
    if (!(bmFile.pBits && bmWnd.pBits) || bmFile.cxDib > bmWnd.cxDib || bmFile.cyDib > bmWnd.cyDib)
        return FALSE;

    resemble = max(resemble, 0);
    resemble = min(resemble, 100);

    deviation = max(deviation, 0);
    deviation = min(deviation, 100);

    BYTE r = GetRValue(rgb);
    BYTE g = GetGValue(rgb);
    BYTE b = GetBValue(rgb);


    // 实际范围
    RECT rectDefault;
    if (pRect && bmWnd.IsInRect(*pRect))
        rectDefault = *pRect;
    else
        bmWnd.GetBitmapRect(rectDefault);


    // bmFile图像坐标(x, y),  bmWnd图像坐标(x + xOffset, y + yOffset)
    int yTotal        =     rectDefault.bottom - bmFile.cyDib;
    int xTotal        =     rectDefault.right - bmFile.cxDib;
    int invalidTotal  =     (100 - resemble) * (bmFile.cxDib * bmFile.cyDib);
    int validTotal    =     resemble * (bmFile.cxDib * bmFile.cyDib);


    //  ignoreNum忽略值, validNum有效值，invalidNum无效值
    int  invalidNum = 0, validNum = 0, ignoreNum = 0;
    // 误差值
    int  rdev = 100, gdev = 100, bdev = 100, rdevTemp, gdevTemp, bdevTemp;
    BYTE rWnd, gWnd, bWnd, rFile, gFile, bFile;
    for (int yOffset = rectDefault.top; yOffset <= yTotal; yOffset++)
        for (int xOffset = rectDefault.left; xOffset <= xTotal; xOffset++)
        {
            for (int y = 0, bflag = TRUE; bflag && (y < bmFile.cyDib); y++)
                for (int x = 0; x < bmFile.cxDib; x++)
                {
                    int FileIndex = (bmFile.cyDib - 1 - y) * bmFile.cBits + 3 * x;
                    int WndIndex  = (bmWnd.cyDib - 1 - yOffset - y) * bmWnd.cBits + 3 * (xOffset + x);


                    if (r    == bmFile.pBits[FileIndex + 2] &&
                        g    == bmFile.pBits[FileIndex + 1] &&
                        b    == bmFile.pBits[FileIndex]     &&
                        0xF8 != bmWnd.pBits[WndIndex + 2]   &&
                        0xFC != bmWnd.pBits[WndIndex + 1]   &&
                        0xF8 != bmWnd.pBits[WndIndex]) {

                        ignoreNum++;
                    }else {
                        rdevTemp = rdev;
                        gdevTemp = gdev;
                        bdevTemp = bdev;
                        rWnd     = bmWnd.pBits[WndIndex + 2];
                        gWnd     = bmWnd.pBits[WndIndex + 1];
                        bWnd     = bmWnd.pBits[WndIndex];
                        rFile    = bmFile.pBits[FileIndex + 2];
                        gFile    = bmFile.pBits[FileIndex + 1];
                        bFile    = bmFile.pBits[FileIndex];

                        rdev = hmean(rWnd, rFile);
                        gdev = hmean(gWnd, gFile);
                        bdev = hmean(bWnd, bFile);

                        if (x == 0 && y == 0)
                        {
                            ignoreNum++;
                            continue;
                        }
                        if (abs(rdev - rdevTemp) <= deviation &&
                            abs(gdev - gdevTemp) <= deviation &&
                            abs(bdev - bdevTemp) <= deviation)
                        {
                            validNum++;
                        }else
                            invalidNum++;
                    }


                    if (100 * invalidNum > invalidTotal)
                    {
                        invalidNum = validNum = ignoreNum = 0;
                        bflag = FALSE;
                        break;
                    }

                    if (100 * (validNum + ignoreNum) >= validTotal)
                    {
                        if (pRect)
                        {
                            pRect->left   = xOffset;
                            pRect->top    = yOffset;
                            pRect->right  = xOffset + bmFile.cxDib;
                            pRect->bottom = yOffset + bmFile.cyDib;
                        }
                        return TRUE;
                    }
                }
        }
        return FALSE;
}


//批量修改位图文件,形参:strFolderpath,忽略颜色ignoreColor,要将其他颜色修改成的目标颜色targetColor
BOOL ModifyBitmapFile(PCTSTR FilePath, COLORREF ignoreColor, COLORREF targetColor)
{
    TCHAR FileName[MAX_PATH];                             //存放路径的变量
    WIN32_FIND_DATA fData;

    _tcscpy_s(FileName, MAX_PATH, FilePath);
    if (_tcschr(FileName, '*'))
    {
        _tcsrchr(FileName, '\\')[1] = '\0';
        FilePath = StrDup(FileName);
    }

    _tcscat_s(FileName, MAX_PATH, _T("*.bmp"));
    HANDLE hFindFile = FindFirstFile(FileName, &fData);

    if (hFindFile == INVALID_HANDLE_VALUE) return FALSE;

    do{
        wsprintf(FileName, _T("%s%s"), FilePath, fData.cFileName);
        Cbm bmFile(FileName);
        BOOL bModify = FALSE;

        for (int y = 0; y < bmFile.cyDib; y++)
        {
            for (int x = 0; x < bmFile.cxDib; x++)
            {
                if (bmFile.GetPixel(x, y) != ignoreColor)
                {
                    bmFile.SetPixel(x, y, targetColor);
                    bModify = TRUE;
                }
            }
        }

        if (bModify)
            bmFile.SaveBitmapToFile(FileName);

    }while (FindNextFile(hFindFile, &fData));

    FindClose(hFindFile);
    return TRUE;
}


BOOL Cbm::ResolveBitmapFile(const SIZE size, int i) const
{
    TCHAR FileName[MAX_PATH];
    int cx = cxDib / size.cx;
    int cy = cyDib / size.cy;

    for (int y = 0; y < cy; y++) {
        for (int x = 0; x < cx; x++)
        {
            wsprintf(FileName, _T("resolvFile\\%d.bmp"), i++);
            RECT rect = {x * size.cx, y * size.cy, (x + 1) * size.cx, (y + 1) * size.cy};
            if (!SaveBitmapToFile(FileName, &rect))
                return FALSE;
        }
    }
    return TRUE;
}

