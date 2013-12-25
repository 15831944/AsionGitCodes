#pragma once


// CConfigParameterSearchEngineNumber 對話方塊

class CConfigParameterSearchEngineNumber : public CDialog
{
	DECLARE_DYNAMIC(CConfigParameterSearchEngineNumber)

public:
	CConfigParameterSearchEngineNumber(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigParameterSearchEngineNumber();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_PARAM_SEARCH_ENG_NUM };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeMode();
};
