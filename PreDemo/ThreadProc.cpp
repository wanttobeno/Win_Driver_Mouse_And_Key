#include "stdafx.h"
#include "ThreadProc.h"
#include "GameState.h"
#include "PreDemo.h"


UINT ThreadHostProc(PVOID pParam)
{
    volatile PPARAMS pparams = (PPARAMS)pParam;
    HWND hWnd = pparams->hwndScreen;
    SetForegroundWindow(hWnd);

    Sleep(500);
    //GotoHotel(hWnd);
    /*RECT rect;
    TCHAR retFileName[MAX_PATH];
    Cbm bmWnd(_T("G:\\num\\20130425215539993.bmp"));
    bmWnd.GetBitmapRect(rect);
    BOOL bFind = FindPicEx(_T("G:\\1.bmp"), _T("G:\\2.bmp"), 0, 8);*/
    //WIN32_FIND_DATA fData;
    //PCTSTR FileDir = _T("screen\\checkWindow\\");
    //TCHAR FileName[MAX_PATH], retFileName[MAX_PATH];
    //BOOL bFind;
    //wsprintf(FileName, _T("%s*.bmp"), FileDir);
    //HANDLE hFile = FindFirstFile(FileName, &fData);
    //if (hFile == INVALID_HANDLE_VALUE)
    //{
    //    TRACE(_T("FindSomePic --- read file failed.\n"));
    //    return FALSE;
    //}
    //do{
    //    wsprintf(FileName, _T("%s%s"), FileDir, fData.cFileName);
    //    bFind = FindSomePicEx(Cbm(FileName), _T("bmp\\check\\"), 0, retFileName, 10);
    //    if (!bFind)
    //        TRACE(_T("0 :%s\n"), fData.cFileName);
    //        //DeleteFile(FileName);
    //    else
    //        TRACE(_T("1: %s\n"), retFileName);
    //}while (FindNextFile(hFile, &fData));

    //FindClose (hFile);
    std::vector<MONSTERSHOPINFO> vecShopInfo;
    vecShopInfo.reserve(10);
    GetConfigMonsterInfo(vecShopInfo);
    UINT HeathLower = theApp.GetProfileInt(_T("BaseSetting"), _T("HeathLower"), 0);
    UINT MagicLower = theApp.GetProfileInt(_T("BaseSetting"), _T("MagicLower"), 0);
    BeWarning(_T("wav\\开始挂机.wav"));
    while (TRUE)
    {
        std::multimap<Mst_Num, Mst_Ty> mtmp;
        if (GetRoleStatusValue(hWnd, HEALTH_POINT) < HeathLower ||
            GetRoleStatusValue(hWnd,   MANA_POINT) < MagicLower)
        {
            if (!GotoHotel(hWnd)) break;
        }
        if (!CheckShop(hWnd, vecShopInfo, mtmp)) break;

        for (std::multimap<Mst_Num, Mst_Ty>::iterator it = mtmp.begin(); it != mtmp.end();)
        {
            if (MonsterToMap(it->second) == MonsterToMap(mtmp.begin()->second))
            {
                TRACE(_T("捕捉怪物类型：%d, 数量：%d\n"), it->second, 16 - it->first);
                ++it;
            }else
                it = mtmp.erase(it);
        }
        if (mtmp.begin()->first >= 11)
        {
            BeWarning(_T("wav\\宠物捉满了.wav"));
            theApp.SetShutDown(TRUE);
            PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
            break;
        }else
        {
            if (!CatchMonster(hWnd, mtmp))
                break;
        }
    }
    /*std::vector<MONSTERSHOPINFO> vecShopInfo;
    vecShopInfo.reserve(10);
    GetConfigMonsterInfo(vecShopInfo);
    std::multimap<Mst_Num, Mst_Ty> mtmp;
    CheckShop(hWnd, vecShopInfo, mtmp);*/
    //ModifyBitmapFile(_T("G:\\num\\"));
    /*UINT monster, num;
    CheckShop(hWnd, monster, num);*/
    //CString FileName;
    //int n = 0;
    //for (int i = 0; i < 200; i++)
    //{
    //    TRACE(_T("# %d\n"), i);
    //    Cbm bmWnd(hWnd);
    //    if (!FindSomePic(bmWnd, _T("bmp\\npc\\shlb\\")))
    //    {
    //        //DeleteFile(FileName);
    //        FileName.Format(_T("G:\\yizhan\\%d.bmp"), n++);
    //        bmWnd.SaveBitmapToFile(FileName);
    //    }
    //    Sleep(400);
    //}
    /*RECT rect ={0, 0, 640, 480};
    clock_t t1 = clock();
    TCHAR FileName[MAX_PATH];
    Cbm bmWnd(_T("screen\\checkWindow\\20130402223804510.bmp"));
    BOOL bFind = FindSomePicEx(bmWnd, _T("bmp\\check\\"), &rect, FileName, 30);
    clock_t time = clock() - t1;
    wsprintf(FileName, _T("time: %d\nresult: %d"), time, bFind);*/
    //AfxMessageBox(FileName);
    //CatchMonster(hWnd, Monster_GuiChengXiang, 7);
    //BOOL bFind;
    //POINT pt;
    //CString FileName;
    //int n =0;
    //POINT pt;
    //RECT rect = {225,110,232,117};
    //RECT rect = {165, 140, 172, 147};
    //RECT rect = {111, 168, 118, 175};
    /*if (!FindSomePic(Cbm(_T("G:\\131\\20130417142946312.bmp")), _T("bmp\\monster\\13\\"), &rect))
    return 2;*/
    /*Cbm bm(_T("G:\\13\\0.bmp"));
    bm.SaveBitmapToFile(_T("G:\\newbmp\\31.bmp"), &rect);*/
    /*for (int i = 0; i < 20; i++)
    {
        FileName.Format(_T("G:\\13\\%d.bmp"), i);
        Cbm bmWnd(FileName);
        BOOL bFind = FindSomePic(bmWnd, _T("bmp\\monster\\13\\"), &rect, 0, 90);
        if (bFind)
        {
            TRACE(_T("#%d:\n"), i);
            DeleteFile(FileName);
        }
    }*/

    /*for (int i = 0; i < 20;)
    {

        Cbm bmWnd(hWnd);
        BOOL bFind = FindSomePic(bmWnd, _T("bmp\\monster\\13\\"), 0, 0, 90);
        if (!bFind)
        {
            TRACE(_T("#%d:\n"), i);
            FileName.Format(_T("G:\\13\\%d.bmp"), i);
            bmWnd.SaveBitmapToFile(FileName);
            i++;
        }
        Sleep(270);
    }*/
    /*Cbm bmFile(_T("G:\\yizhan\\3.bmp"));
    bmFile.ResolveBitmapFile(CSize(7, 9), 18);*/
    /*CString decFileName;
    for (int i = 0; i < 6; i++)
    {
    decFileName.Format(_T("G:\\新建文件夹\\%d.bmp"), i);
    RECT rect = {0, 0, 640, 480};
    BOOL bFind = FindSomePic(Cbm(decFileName), _T("G:\\check\\"), &rect);
    TRACE(_T("#%d: %d\n"), i, bFind);
    }*/
    TRACE(_T("......HostThread Close!\n"));
    return 0;
}
