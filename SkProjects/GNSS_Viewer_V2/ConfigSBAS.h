#pragma once


// CConfigSBAS 對話方塊

class CConfigSBAS : public CDialog
{
	DECLARE_DYNAMIC(CConfigSBAS)

public:
	CConfigSBAS(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigSBAS();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_SBAS };
	BOOL m_bEnable;
	BOOL m_bRanging;
	BOOL m_bCorrection;
	int m_nUraMask;
	int m_nTrackingChannel;
	BOOL m_bWAAS;
	BOOL m_bEGNOS;
	BOOL m_bMSAS;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
