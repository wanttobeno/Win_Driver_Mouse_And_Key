#pragma once


// CMyComboBox

class CMyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComboBox)

public:
	CMyComboBox();
	virtual ~CMyComboBox();

private:
	CEdit      m_edit;
	CBrush	   m_brush;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


