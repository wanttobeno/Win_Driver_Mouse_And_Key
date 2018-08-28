#pragma once
#include "MyDialog.h"


// CPage4 对话框

class CPage4 : public CMyDialog
{
	DECLARE_DYNAMIC(CPage4)

public:
	CPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPage4();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void SaveState();
	void RestoreState();
private:
	int m_nIncense;					//摄妖香数量
	int m_nGrass;					//洞冥草数量
	int m_nMagicDrug;				//蓝药数量
	int m_nRound;					//回合数限制
	int m_MagicDrugType;		    //蓝药种类
	int m_nIsDoWork;		        //是否自动卖体
	int m_nAfterComplete;	        //抓完怪之后处理
	int m_nIsCatchSpecial;          //遇宝宝处理
public:
	afx_msg void OnChange(UINT id);
};
