#include "stdafx.h"
#include "mapLine.h"
#include "PreDemo.h"
#include "GameState.h"
#include "km.h"



// 这是已导出类的构造函数。


Cmap::Cmap(int MapIndex, int NextMapIndex, const RECT & ExitRect, BOOL ExitType, int MoveType)
: m_ExitRect(ExitRect)
{
    m_nCurrentMap  =  MapIndex;
    m_nNextMap     =  NextMapIndex;
    m_bExitType    =  ExitType;
    m_nMoveType    =  MoveType;
}


int Cmap::GetTeleportPos() const
{
    if (m_nCurrentMap == MAP_CITY_CA)
    {
        switch (m_nNextMap)
        {
        case MAP_SUBURB_DaTGJ:
            if (m_bExitType)
                return GuoJing_POS;
            else
                return YiZhan_POS;
        case MAP_SUBURB_JiangNYW:
            return YeWai_POS;
        case MAP_MAZE_T1:
            return DaYanTa_POS;
        case MAP_CITY_AL:
            if (m_bExitType)
                return HuaGuoShan_POS;
            else
                return DongHaiWan_POS;
        case MAP_CITY_CS:
            return JiaoWai_POS;
        case MAP_CITY_ZZ:
            return ZhuZiGuo_POS;
        case MAP_CA_HOTEL:
            return JiuDian_POS;
        case MAP_CA_BOOKSHOP:
            return ShuDian_POS;
        case MAP_CA_SHOP:
            return ShangHui_POS;
        default:
            return UNKNOWN_POS;
        }
    }
    else
        return UNKNOWN_POS;
}

// 过传送点
void Cmap::CrossExitPoint(HWND hWnd) const
{
    // 关闭tabmap
    if (FindSomePic(hWnd, _T("bmp\\tabMap\\tabmap\\"), NULL, NULL))
    {
        KeyboardButtonEx(VK_TAB);
    }

    CRect rect;
    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);

    if (m_bExitType == DIRECT_EXIT)
    {
        TCHAR FileDir[MAX_PATH];
        wsprintf(FileDir, _T("bmp\\exitPic\\%02d%02d\\"), m_nCurrentMap, m_nNextMap);
        while (GetMapIndex(hWnd) != m_nNextMap)
        {
            if (!FindSomePic(hWnd, FileDir, NULL, &rect))
            {
                TRACE(_T("CrossExitPoint >> DIRECT_EXIT\n"));
                KeyboardButtonEx(VK_F9);
                if (m_nCurrentMap > UNKNOWN_MAP && m_nCurrentMap <= MAP_CITY_MAX)
                    KeyboardButtonEx('H', VK_MENU);
            }else
            {
                MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                Sleep(2000);
            }
        }
    }else
    {
        TCHAR FolderName[MAX_PATH];
        if (m_nCurrentMap <= MAP_CITY_MAX && m_nCurrentMap > UNKNOWN_MAP)
            _tcscpy_s(FolderName, MAX_PATH, _T("bmp\\npc\\incity\\"));
        else if (m_nCurrentMap > MAP_CITY_MAX)
            _tcscpy_s(FolderName, MAX_PATH, _T("bmp\\npc\\outcity\\"));

        while (GetMapIndex(hWnd) != m_nNextMap)
        {
            if (!FindSomePic(hWnd, _T("bmp\\general\\shide*.bmp"), NULL, &rect)) 
            {
                // 查找传送人的位置
                if (!FindSomePic(hWnd, FolderName, NULL, &rect))
                {
                    TRACE(_T("\n正在查找传送人位置ing bmp\\npc\\\n"));
                    KeyboardButtonEx(VK_F9);
                    if (m_nCurrentMap > UNKNOWN_MAP && m_nCurrentMap <= MAP_CITY_MAX)
                    KeyboardButtonEx('H', VK_MENU);
                }else
                {
                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_DAOJULAN);
                }       
            }else
            {
                MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\shide*.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
                while (GetMapIndex(hWnd) != m_nNextMap)
                {
                    TRACE(_T("等待传送到下个场景！\n"));
                    Sleep(500);
                }
            }
        }
    }
}


BOOL Cmap::TabMove(HWND hWnd) const
{
    CRect rect;
    CString FileName;
    POINT ptPos;

    if (GetMapIndex(hWnd) >= MAP_MAZE_MIN)
    {
        TRACE(_T("当前场景无小地图！\n"));
        return FALSE;
    }

    if (FindPic(hWnd, _T("bmp\\general\\DaoJuWindow.bmp"), NULL, NULL))
    {
        KeyboardButtonEx('E', VK_MENU);
        Sleep(200);
    }


    if (FindSomePic(hWnd, _T("bmp\\general\\shide*.bmp"), NULL, &rect))
    {
        MouseInputEx(hWnd, &rect, MOUSE_LEFT_BUTTON, _T("bmp\\general\\shide*.bmp"), CLOSE_WINDOW, RECT_NOT_DAOJULAN, 5000);
    }

    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_TABMAP);

    while (!GetGameRolePos(hWnd, ptPos))
    {
        BeWarning(_T("wav\\BJ.wav"));
        TRACE(_T("TabMove >> GetGameRolePos"));
        Sleep(200);
    }

    BOOL bStatic = TRUE, bFlag = TRUE;
    while ((!(bStatic && m_ExitRect.PtInRect(ptPos)) || GetMapIndex(hWnd) == m_nNextMap))
    {
        if (bStatic)
        {
            if (FindSomePic(hWnd, _T("bmp\\tabMap\\tabmap\\"), NULL ,NULL))
            {
                if (m_nNextMap == MAP_SM_MW && FindSomePic(hWnd, _T("bmp\\general\\gou*.bmp"), NULL, &rect))
                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);

                if (m_nCurrentMap == MAP_CITY_CA && m_bExitType == INDIRECT_EXIT)
                    FileName.Format(_T("bmp\\tabMap\\%02d%02d2.bmp"), m_nCurrentMap, m_nNextMap);
                else
                {
                    bFlag = !bFlag;
                    FileName.Format(_T("bmp\\tabMap\\%02d%02d%d.bmp"), m_nCurrentMap, m_nNextMap, bFlag);
                }

                if (FindPic(hWnd, FileName, NULL, &rect))
                {
                    MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
                    MoveGameCursorToSpecifyPos(hWnd, RECT_NOT_TABMAP);
                }
            }else
            {
                KeyboardButtonEx(VK_TAB);
                Sleep(500);
                TRACE(_T("正在查找小地图.\n"));
                continue;
            }

        }else
        {
            // 关闭tabmap
            if (FindSomePic(hWnd, _T("bmp\\tabMap\\tabmap\\"), NULL, NULL))
            {
                KeyboardButtonEx(VK_TAB);
                Sleep(400);
            }

            if (FindPic(hWnd, _T("bmp\\general\\closeButton.bmp"), NULL, &rect))
            {
                MouseInput(hWnd, &rect, MOUSE_LEFT_BUTTON);
            }
        }

        bStatic = IsRoleStatic(hWnd, &ptPos);
    }

    return TRUE;
}


BOOL Cmap::CityMove(HWND hWnd) const
{
    if (!UseItemEx(hWnd, DaoBiaoQi, m_nNextMap, GetTeleportPos()))
    {
        if (GetMapIndex(hWnd) != m_nCurrentMap)
        {
            if (!UseItemEx(hWnd, FeiXingFu, m_nCurrentMap))
                return FALSE;

            if (FindPic(hWnd, _T("bmp\\general\\DaoJuWindow.bmp"), NULL, NULL))
            {
                KeyboardButtonEx('E', VK_MENU);
                Sleep(500);
            }
        }
        if (!TabMove(hWnd))
        {
            TRACE(_T("MapMove >> TAB_MOVE failed\n"));
            return FALSE;
        }
    }

    if (FindPic(hWnd, _T("bmp\\general\\DaoJuWindow.bmp"), NULL, NULL))
    {
        KeyboardButtonEx('E', VK_MENU);
        Sleep(500);
    }
    return TRUE;
}


BOOL Cmap::MazeMove(HWND) const
{
    return TRUE;
}

BOOL Cmap::MapMove(HWND hWnd) const
{
    BOOL bMove;
    switch (m_nMoveType)
    {
    case CITY_MOVE:
        bMove = CityMove(hWnd);
        break;
    case TAB_MOVE:
        bMove = TabMove(hWnd);
        break;
    case MAZE_MOVE:
        bMove = MazeMove(hWnd);
        break;
    default:
        TRACE(_T("参数错误: m_nMoveType.\n"));
        return FALSE;
    }

    if (!bMove)
    {
        TRACE(_T("MapMove failed: %d, %d\n"), m_nCurrentMap, m_nNextMap);
        return FALSE;
    }

    while (!IsRoleStatic(hWnd))
    {
        TRACE(_T("wait for MapMove static\n"));
        Sleep(200);
    }

    if (m_nNextMap != MAP_CA_SHOP)
        CrossExitPoint(hWnd);
    return TRUE;
}


BOOL LineMove(HWND hWnd, const std::list<Cmap> & line, int TargetMapIndex)
{
    std::list<Cmap>::const_iterator iteBeg = line.begin(), iteEnd = line.end(), iteTemp;
    BOOL bUseIncense = FALSE, bTargetMap = FALSE;
    int mapIndex, School;

    // 检查终点场景是否在线路中
    if (TargetMapIndex != UNKNOWN_MAP)
    {
        for(iteTemp = iteBeg; iteTemp != iteEnd; ++iteTemp)
        {
            bTargetMap = iteTemp->NextMap() == TargetMapIndex;
            if (bTargetMap)
            {
                ++iteTemp;
                iteEnd = iteTemp;
                break;
            }
        }

        if (!bTargetMap)
            return FALSE;
    }
    // 检查当前所在场景是否在线路中
    mapIndex = GetMapIndex(hWnd);
    for (iteTemp = iteBeg; iteTemp != iteEnd; ++iteTemp)
    {
        if (mapIndex == iteTemp->NextMap())
        {
            ++iteTemp;
            iteBeg = iteTemp;
            break;
        }
    }
    // 检查人物师门场景是否在线路中
    School = theApp.GetProfileInt(_T("BaseSetting"), _T("School"), 0);
    for (iteTemp = iteBeg; iteTemp != iteEnd; ++iteTemp)
    {
        if (iteTemp->NextMap() == School + 8)
        {
            ++iteTemp;
            iteBeg = iteTemp;
            MoveToSchool(hWnd, School + 8);
            break;
        }
    }


    // 检查是否需要点香
    for (iteTemp = iteBeg; iteTemp != iteEnd; ++iteTemp)
    {
        bUseIncense =  iteTemp->CurrentMap() > MAP_CITY_MAX;
        if (bUseIncense)
            break;
    }

    if (bUseIncense)
    {
        if (!UseItemEx(hWnd, SheYaoXiang))
            return FALSE;
    }


    // 开始跑线路
    while (iteBeg != iteEnd)
    {
        TRACE(_T("nextmap: %d\n"), iteBeg->NextMap());
        if (!(iteBeg->MapMove(hWnd)))
            return FALSE;
        ++iteBeg;
    }

    if (bUseIncense)
    {
        if (!UseItemEx(hWnd, DongMingCao))
            return FALSE;

        if (FindPic(hWnd, _T("bmp\\General\\DaoJuWindow.bmp"), NULL, NULL))
        {
            KeyboardButtonEx('E', VK_MENU);
            Sleep(200);
        }
    }
    return TRUE;
}
