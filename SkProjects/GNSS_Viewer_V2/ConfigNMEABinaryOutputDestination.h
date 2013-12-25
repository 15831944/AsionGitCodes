#pragma once


// CConfigNMEABinaryOutputDestination 對話方塊

class CConfigNMEABinaryOutputDestination : public CDialog
{
	DECLARE_DYNAMIC(CConfigNMEABinaryOutputDestination)

public:
	CConfigNMEABinaryOutputDestination(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigNMEABinaryOutputDestination();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_NMBI_OUT_DES };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
