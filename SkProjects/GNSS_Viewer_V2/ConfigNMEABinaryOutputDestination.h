#pragma once


// CConfigNMEABinaryOutputDestination ��ܤ��

class CConfigNMEABinaryOutputDestination : public CDialog
{
	DECLARE_DYNAMIC(CConfigNMEABinaryOutputDestination)

public:
	CConfigNMEABinaryOutputDestination(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigNMEABinaryOutputDestination();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_NMBI_OUT_DES };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
