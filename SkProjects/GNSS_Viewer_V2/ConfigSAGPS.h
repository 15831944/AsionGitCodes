#pragma once


// CConfigSAGPS ��ܤ��

class CConfigSAGPS : public CDialog
{
	DECLARE_DYNAMIC(CConfigSAGPS)

public:
	CConfigSAGPS(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigSAGPS();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_SAGPS };
	int m_nEnable;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
