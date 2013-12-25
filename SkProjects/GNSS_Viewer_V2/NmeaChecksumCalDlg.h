#pragma once


// NmeaChecksumCalDlg 對話方塊

class NmeaChecksumCalDlg : public CDialog
{
	DECLARE_DYNAMIC(NmeaChecksumCalDlg)

public:
	NmeaChecksumCalDlg(CWnd* pParent = NULL);   // 標準建構函式
	virtual ~NmeaChecksumCalDlg();

// 對話方塊資料
//	enum { IDD = IDD_NMEA_CHECKSUM_CAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeInput();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void OnOK();
};
