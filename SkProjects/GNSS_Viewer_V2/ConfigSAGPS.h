#pragma once


// CConfigSAGPS 對話方塊

class CConfigSAGPS : public CDialog
{
	DECLARE_DYNAMIC(CConfigSAGPS)

public:
	CConfigSAGPS(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigSAGPS();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_SAGPS };
	int m_nEnable;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
