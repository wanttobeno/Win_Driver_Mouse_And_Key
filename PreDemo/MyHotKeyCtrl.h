#pragma once


// CMyHotKeyCtrl

class CMyHotKeyCtrl : public CHotKeyCtrl
{
	DECLARE_DYNAMIC(CMyHotKeyCtrl)

public:
	CMyHotKeyCtrl();
	virtual ~CMyHotKeyCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


