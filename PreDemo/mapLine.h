
#ifndef _MAPLINE_H
#define _MAPLINE_H


#pragma once
#include <list>


#define DIRECT_EXIT      TRUE
#define INDIRECT_EXIT    FALSE


class Cmap
{
// 类成员
public:
    enum {CITY_MOVE, TAB_MOVE, MAZE_MOVE};
private:
    int       m_nCurrentMap;          // 地图索引
	int       m_nNextMap;             // 下一个地图索
    int       m_nMoveType;            // 移动类型
    BOOL      m_bExitType;            // 出口类型
	CRect     m_ExitRect;             // 出口RECT

    // 私有方法
    BOOL TabMove(HWND hWnd) const;
    BOOL MazeMove(HWND hWnd) const;
	BOOL CityMove(HWND hWnd) const;
	int  GetTeleportPos() const;
	void CrossExitPoint(HWND hWnd) const;
// 类方法
public:
    Cmap(int MapIndex, int NextMapIndex, const RECT & ExitRect = CRect(), BOOL ExitType = DIRECT_EXIT, int MoveType = TAB_MOVE);
    BOOL MapMove(HWND hWnd) const;
    int CurrentMap() const {return m_nCurrentMap;}
    int NextMap() const {return m_nNextMap;}
};

BOOL LineMove(HWND hWnd, const std::list<Cmap> & line, int TargetMapIndex);


#endif // _MAPLINE_H

