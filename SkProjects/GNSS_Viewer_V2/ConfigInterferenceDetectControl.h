#pragma once


// CConfigInterferenceDetectControl ��ܤ��

class CConfigInterferenceDetectControl : public CDialog
{
	DECLARE_DYNAMIC(CConfigInterferenceDetectControl)

public:
	CConfigInterferenceDetectControl(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigInterferenceDetectControl();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_INF_DTE_CTRL };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
