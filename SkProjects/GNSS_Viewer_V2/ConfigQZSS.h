#pragma once


// CConfigQZSS ��ܤ��

class CConfigQZSS : public CDialog
{
	DECLARE_DYNAMIC(CConfigQZSS)

public:
	CConfigQZSS(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigQZSS();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_SBAS };
	BOOL m_bEnable;
	int m_nTrackingChannel;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
