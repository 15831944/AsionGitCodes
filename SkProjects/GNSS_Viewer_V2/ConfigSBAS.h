#pragma once


// CConfigSBAS ��ܤ��

class CConfigSBAS : public CDialog
{
	DECLARE_DYNAMIC(CConfigSBAS)

public:
	CConfigSBAS(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigSBAS();

// ��ܤ�����
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
