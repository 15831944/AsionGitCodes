#pragma once


// CConfigQZSS 對話方塊

class CConfigQZSS : public CDialog
{
	DECLARE_DYNAMIC(CConfigQZSS)

public:
	CConfigQZSS(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigQZSS();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_SBAS };
	BOOL m_bEnable;
	int m_nTrackingChannel;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
