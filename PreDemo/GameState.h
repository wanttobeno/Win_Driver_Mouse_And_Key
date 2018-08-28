
#ifndef _GAMESTATE_H
#define _GAMESTATE_H

#pragma once
#include "bm.h"
#include <vector>
#include <map>


// 游戏地图场景
#define UNKNOWN_MAP 0   //未知场景

// 城市场景
#define MAP_CITY_CA 1   //长安城
#define MAP_CITY_JY 2   //建邺城
#define MAP_CITY_AL 3   //傲来国
#define MAP_CITY_CS 4   //长寿村
#define MAP_CITY_ZZ 5   //朱紫国
#define MAP_CITY_XL 6   //西凉女国
#define MAP_CITY_BX 7   //宝象国 

//师门场景
#define MAP_SM_DT    8   //大唐官府
#define MAP_SM_HS    9   //化生寺
#define MAP_SM_NE    10  //女儿村
#define MAP_SM_FC    11  //方寸山
#define MAP_SM_LG    12  //龙宫
#define MAP_SM_DF    13  //阴曹地府
#define MAP_SM_WZ    14  //五庄观
#define MAP_SM_TG    15  //天宫
#define MAP_CITY_MAX 15 
#define MAP_SM_PS    16  //盘丝岭
#define MAP_SM_MW    17  //魔王寨
#define MAP_SM_ST    18  //狮驼岭
#define MAP_SM_PT    19  //普陀山

//野外场景
#define MAP_SUBURB_DongHW       20  //东海湾
#define MAP_SUBURB_DongHHD      21  //东海海底
#define MAP_SUBURB_JiangNYW     22  //江南野外
#define MAP_SUBURB_DaTGJ        23  //大唐国境
#define MAP_SUBURB_DaTJW        24  //大唐境外
#define MAP_SUBURB_YueG         25  //月宫 
#define MAP_SUBURB_HuaGS	    26  //花果山
#define MAP_SUBURB_ChangSJW     27  //长寿郊外
#define MAP_SUBURB_BeiJLZ       28  //北俱芦洲

//迷宫场景
//大雁塔
#define MAP_MAZE_T1 29  //T1
#define MAP_MAZE_T2 30  //T2
#define MAP_MAZE_T3 31  //T3
#define MAP_MAZE_T4 32  //T4
#define MAP_MAZE_T5 33  //T5
#define MAP_MAZE_T6 34  //T6
#define MAP_MAZE_T7 35  //T7

//海底迷宫
#define MAP_MAZE_H1 36  //H1
#define MAP_MAZE_H2 37  //H2
#define MAP_MAZE_H3 38  //H3
#define MAP_MAZE_H4 39  //H4
#define MAP_MAZE_H5 40  //H5

//地狱迷宫
#define MAP_MAZE_D1 41  //D1
#define MAP_MAZE_D2 42  //D2
#define MAP_MAZE_D3 43  //D3
#define MAP_MAZE_D4 44  //D4
#define MAP_MAZE_D5 45  //D5

//龙窟
#define MAP_MAZE_L1 46  //L1
#define MAP_MAZE_L2 47  //L2
#define MAP_MAZE_L3 48  //L3
#define MAP_MAZE_L4 49  //L4
#define MAP_MAZE_L5 50  //L5
#define MAP_MAZE_L6 51  //L6
#define MAP_MAZE_L7 52  //L7

//凤巢
#define MAP_MAZE_F1 53  //F1
#define MAP_MAZE_F2 54  //F2
#define MAP_MAZE_F3 55  //F3
#define MAP_MAZE_F4 56  //F4
#define MAP_MAZE_F5 57  //F5
#define MAP_MAZE_F6 58  //F6
#define MAP_MAZE_F7 59  //F7

// NPC商店场景
#define MAP_MAZE_MIN    60  // 无小地图场景最小值
#define MAP_CA_HOTEL    60  // 长安酒店
#define MAP_CA_HOTEL_2L 61  // 长安酒店二楼
#define MAP_CA_BOOKSHOP 62  // 长安书店
#define MAP_CA_SHOP     63  // 长安商会

//怪物名称
#define Monster_NO             0         //空
#define Monster_DaHaiGui       1         //大海龟
#define Monster_JuWa           2         //巨蛙
#define Monster_ShuGuai        3         //树怪
#define Monster_YeZhu          4         //野猪
#define Monster_DuTu           5         //赌徒
#define Monster_ShanZei        6         //山贼
#define Monster_QiangDao       7         //强盗
#define Monster_LaoHu          8         //老虎
#define Monster_HeiXiong       9         //黑熊
#define Monster_HuaYao         10        //花妖
#define Monster_YangTouGuai    11        //羊头怪
#define Monster_KuLouGuai      12        //骷髅怪
#define Monster_HaMaJing       13        //蛤蟆精
#define Monster_HuLiJing       14        //狐狸精
#define Monster_XiaoLongNv     15        //小龙女
#define Monster_Lang           16        //狼
#define Monster_XiaBing        17        //虾兵
#define Monster_XieJiang       18        //蟹将
#define Monster_GuiChengXiang  19        //龟丞相
#define Monster_NiuYao         20        //牛妖
#define Monster_HeiXiongJing   21        //黑熊精
#define Monster_ZhiZhuJing     22        //蜘蛛精
#define Monster_TuZiGuai       23        //兔子怪
#define Monster_YeGui          24        //野鬼
#define Monster_JiangShi       25        //僵尸
#define Monster_NiuTou         26        //牛头
#define Monster_MaMian         27        //马面
#define Monster_LeiNiao        28        //雷鸟
#define Monster_HuDieXianZi    29        //蝴蝶仙子
#define Monster_GuDaiRuiShou   30        //古代瑞兽
#define Monster_BaiXiong       31        //白熊
#define Monster_TianJiang      32        //天将
#define Monster_HeiShanLaoYao  33        //黑山老妖
#define Monster_TianBing       34        //天兵
#define Monster_FengBo         35        //风伯
#define Monster_DiYuZhanShen   36        //地狱战神
#define Monster_JiaoLong       37        //蛟龙
#define Monster_FengHuang      38        //凤凰
#define Monster_YuShi          39        //雨师
#define Monster_XunYouTianShen 40        //巡游天神
#define Monster_FuRongXianZi   41        //芙蓉仙子
#define Monster_XingLingXianZi 42        //星灵仙子
#define Monster_RuYiXianZi     43        //如意仙子


//数字符号标识
#define INVALID_VALUE   -1  //无效数值
#define BITMAP_ZERO     0   //阿拉伯数字图像0
#define BITMAP_ONE      1   //阿拉伯数字图像1
#define BITMAP_TWO      2   //阿拉伯数字图像2
#define BITMAP_THREE    3   //阿拉伯数字图像3
#define BITMAP_FOUR     4   //阿拉伯数字图像4
#define BITMAP_FIVE     5   //阿拉伯数字图像5
#define BITMAP_SIX      6   //阿拉伯数字图像6
#define BITMAP_SEVEN    7   //阿拉伯数字图像7
#define BITMAP_EIGHT    8   //阿拉伯数字图像8
#define BITMAP_NINE     9   //阿拉伯数字图像9
#define LEFTBRACKET     10  //左方括号
#define RIGHTBRACKET    11  //右方括号
#define COMMA1          12  //逗号1
#define COMMA2          13  //逗号2
#define COMMA3          14  //逗号3


//游戏人物健康状况
#define HEALTH_POINT    1    //生命值
#define MANA_POINT      2    //魔法值
#define SULK_POINT      3    //愤怒值
#define EXPERIENCE      4    //经验值


/////////////////////////////////////////////////////////////
enum {
    BaoZi = 1,    // 包子
    SheYaoXiang,  // 摄妖香
    DongMingCao,  // 洞冥草
    GuiQieCao,    // 鬼切草
    ZiDanLuo,     // 紫丹罗
    FeiXingFu,    // 飞行符
	DaoBiaoQi,    // 导标旗
	ITEM_MAX = 7
};

enum {UNKNOWN_POS = -1, ShangHui_POS, JiuDian_POS, DaYanTa_POS, GuoJing_POS, YeWai_POS,
    ShuDian_POS, YiZhan_POS, HuaGuoShan_POS, DongHaiWan_POS, JiaoWai_POS, ZhuZiGuo_POS};


#ifdef UNICODE
#define SOUND_FILE_NAME L"BJ.wav"
#else
#define SOUND_FILE_NAME "BJ.wav"
#endif


// 一些自定义的RECT屏幕区域
#define RECT_NOT_DAOJULAN    CRect(220, 30, 520, 60)
#define RECT_NOT_TABMAP      CRect(220, 30, 520, 65)
#define RECT_NOT_XIANLING    CRect(480, 60, 580, 160)
#define RECT_NOT_MONSTER     CRect(110, 275, 380, 380)
#define RECT_MONSTER         CRect(53, 48, 292, 270)
#define RECT_MAPINFO         CRect(18, 23, 130, 55)
#define RECT_STATUSBAR       CRect(571, 0, 640, 51)
#define RED_BAR_RECT         CRect(596, 7, 619, 12)
#define BLUE_BAR_RECT        CRect(598, 18, 621, 24)
#define CENTER_RECT          CRect(200, 160, 440, 320)

#define OPEN_WINDOW          TRUE
#define CLOSE_WINDOW         FALSE


#define WAIT_TIME_OUT        10000

typedef int Mst_Ty;
typedef int Mst_Prc;
typedef int Mst_Num;
struct MONSTERSHOPINFO {

    Mst_Ty  monsterType;
    Mst_Prc monsterPrice;

};


struct MONSTERINFO {

    Mst_Ty  monsterType;
    Mst_Num monsterNumber;

};

/************************多图比较找图函数***************************************
	*       形参说明hwndScreen: 截图窗口句柄
	*                Folderpath: 小图所在的文件夹路径，类似“pic\\”
	*            (OUT) FileName: 返回找到的文件名
	*             (IN OUT) rect:返回找到匹配图的范围坐标。没找到则返回零
	*********************************************************************************/
BOOL FindSomePic(const Cbm & bmWnd, PCTSTR FileName, LPCRECT rectTarget = NULL, PRECT retRect = NULL, PTSTR retFileName = NULL, int resemble = 100, COLORREF rgb = 0);
	
BOOL FindSomePic(HWND hWnd, PCTSTR FileName, LPCRECT rectTarget = NULL, PRECT retRect = NULL, PTSTR retFileName = NULL, int resemble = 100, COLORREF rgb = 0);

BOOL FindSomePicEx(const Cbm & bmWnd, PCTSTR FileName, PRECT pRect = NULL,
                   PTSTR retFileName = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);

BOOL FindSomePicEx(HWND hWnd, PCTSTR FileName, PRECT pRect = NULL,
                   PTSTR retFileName = NULL, int deviation = 0, int resemble = 100, COLORREF rgb = 0);

int GetMapIndex(const Cbm & bmWnd, int resemble = 100, COLORREF rgb = 0);

int GetMapIndex(HWND hWnd, int resemble = 100, COLORREF rgb = 0);

UINT GetRoleStatusValue(const Cbm & bmWnd, int StatusType);

UINT GetRoleStatusValue(HWND hWnd, int StatusType);

BOOL GetGameRolePos(const Cbm & bmWnd, POINT &ptPos, int resemble = 100, COLORREF rgb = 0);

BOOL GetGameRolePos(HWND hWnd, POINT &ptPos, int resemble = 100, COLORREF rgb = 0);

BOOL UseItemEx(HWND hWnd, int ItemType, int MapIndex = UNKNOWN_MAP, int TeleportPos = UNKNOWN_POS);

BOOL IsRoleStatic(HWND hWnd, PPOINT ptPos = NULL);

/***********通过游戏鼠标图标，查找光标在游戏窗口中的坐标******************/
BOOL GetGameCursorPos(const Cbm & bmWnd, POINT & pt, int resemble = 100, COLORREF rgb = 0);

BOOL GetGameCursorPos(HWND hWnd, POINT & pt, int resemble = 100, COLORREF rgb = 0);

BOOL MoveGameCursorToSpecifyPos(HWND hWnd, const RECT & rect);

void MoveToSchool(HWND hWnd, int SchoolMapIndex);

int MonsterToMap(int monster);

BOOL CreateFolder(PCTSTR FilePath);

void ClickCheckWindow(HWND hWnd);

void EscapeFighting(HWND hWnd);

int GetShopMonsterNum(HWND hWnd);

void BeWarning(PCTSTR alarmFileName);

BOOL GotoHotel(HWND hWnd);

BOOL ShutDown(DWORD dwFlag);

void CheckRoleStatus(HWND hWnd, int StatusType, int LimitValue);

void GetConfigMonsterInfo(std::vector<MONSTERSHOPINFO> & vecShopInfo);

BOOL CatchMonster(HWND hWnd, const std::multimap<Mst_Num, Mst_Ty> & mtmp);

BOOL CheckShop(HWND hWnd, const std::vector<MONSTERSHOPINFO> & vecShopInfo, std::multimap<Mst_Num, Mst_Ty> & mtmp);


#endif // _GAMESTATE_H