
#ifndef _BM_H
#define _BM_H

#pragma once

class Cbm {
private:
	BITMAPFILEHEADER   bmfh;                 // 位图文件头
	BITMAPINFOHEADER   bmih;                 // 位图信息头
	PBYTE              pBits;                // 位图像素位指针
	int                cBits;                // 位图每行所用字节总数
	int                cxDib;                // 位图水平像素宽度
	int                cyDib;                // 位图垂直像素高度

	void SetcBits() {cBits = ((cxDib * bmih.biBitCount + 31) & ~31) >> 3;}
	void SetcxDib() {cxDib = bmih.biWidth;}
	void SetcyDib() {cyDib = bmih.biHeight;}
	inline BOOL IsInRect(const RECT & rect) const;    // 判断系统光标是否在窗口范围内
public:
	// TODO: 在此添加您的方法。
	explicit Cbm(HWND hwndScreen = NULL);        // 通过窗口句柄截图获取位图信息
	explicit Cbm(PCTSTR FilePath);               // 通过加载文件获取位图信息
	Cbm(const Cbm & bm, LPCRECT pRect = NULL);   // 截取指定区域的位图信息
	~Cbm();   // 析构函数

	//操作符重载
	Cbm & operator=(const Cbm & bm);
	friend BOOL operator==(const Cbm & bm1, const Cbm & bm2);
	//类方法
	COLORREF  GetPixel(int x, int y) const;		// 通过位图坐标查找RGB颜色值
	void SetPixel(int x, int y, COLORREF targetColor);	 // 改写指定坐标的位图RGB颜色
	void GrayBitmap();	// 将图像灰度化
	void DisplayBitmap(HWND hwnd) const;	// 显示位图
    inline void  GetBitmapRect(RECT & rect) const;  // 获取位图的rect范围
    BOOL SaveBitmapToFile(PCTSTR FileName = NULL, LPCRECT pRect = NULL) const; // 保存位图到文件
    BOOL ResolveBitmapFile(const SIZE size, int i = 0) const;   // 分解图片文件
    BOOL IsRectOnTheEdge(const RECT & rect);   // 判断rect是否在图片范围边缘

	/*****************************比较找图函数***************************************
	*        形参说明bmWnd: 大图、屏幕截图、源图
	*              bmFile: 特征图、小图、从文件加载
	*       (IN OUT) rect: 找图的范围,返回特征图在源图中的坐标范围
	*                 rgb: 透明色RGB值，透明色视为找图中可以被忽略的颜色
	*            resemble: 相似度，相同像素占总像素的百分比,默认值为100
	**********************************************************************************/
    friend BOOL FindPic(HWND hWnd, PCTSTR FileName, LPCRECT rectTarget = NULL, OUT PRECT retRect = NULL, int resemble = 100, COLORREF rgb = 0);

	friend BOOL FindPic(const Cbm & bmWnd, const Cbm & bmFile, LPCRECT rectTarget = NULL, OUT PRECT retRect = NULL, int resemble = 100, COLORREF rgb = 0);

    friend BOOL FindPic(const Cbm & bmWnd, PCTSTR FileName, LPCRECT rectTarget = NULL, OUT PRECT retRect = NULL, int resemble = 100, COLORREF rgb = 0);

    friend BOOL FindPic(PCTSTR destFileName, PCTSTR srcFileName, LPCRECT rectTarget = NULL, OUT PRECT retRect = NULL, int resemble = 100, COLORREF rgb = 0);
	
    friend BOOL FindPicEx(const Cbm & bmWnd, const Cbm & bmFile,
        PRECT pRect = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);

    friend BOOL FindPicEx(const Cbm & bmWnd, PCTSTR FileName,
        PRECT pRect = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);

    friend BOOL FindPicEx(PCTSTR destFileName, PCTSTR srcFileName,
        PRECT pRect = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);

    friend BOOL FindPicEx(HWND hWnd, PCTSTR FileName,
        PRECT pRect = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);
	//批量修改位图文件,形参:strFolderpath,忽略颜色ignoreColor,要将其他颜色修改成的目标颜色targetColor
	friend BOOL ModifyBitmapFile(PCTSTR FilePath, COLORREF ignoreColor = 0xf8fcf8, COLORREF targetColor = 0);
	//friend BOOL BM_API ModifyBitmapFile(tstring strFolderpath);
};

#endif // _BM_H