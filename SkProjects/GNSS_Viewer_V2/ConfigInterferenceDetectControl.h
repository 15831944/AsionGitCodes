#pragma once


// CConfigInterferenceDetectControl 對話方塊

class CConfigInterferenceDetectControl : public CDialog
{
	DECLARE_DYNAMIC(CConfigInterferenceDetectControl)

public:
	CConfigInterferenceDetectControl(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigInterferenceDetectControl();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_INF_DTE_CTRL };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
