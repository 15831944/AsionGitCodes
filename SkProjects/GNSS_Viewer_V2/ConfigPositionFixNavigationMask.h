#pragma once


// CConfigPositionFixNavigationMask 對話方塊

class CConfigPositionFixNavigationMask : public CDialog
{
	DECLARE_DYNAMIC(CConfigPositionFixNavigationMask)

public:
	CConfigPositionFixNavigationMask(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~CConfigPositionFixNavigationMask();

// 對話方塊資料
//	enum { IDD = IDD_CONFIG_NOISE_PWR_CTRL };
	int m_nMask1;
	int m_nMask2;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
