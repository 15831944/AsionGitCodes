#pragma once


// CConfigPositionFixNavigationMask ��ܤ��

class CConfigPositionFixNavigationMask : public CDialog
{
	DECLARE_DYNAMIC(CConfigPositionFixNavigationMask)

public:
	CConfigPositionFixNavigationMask(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigPositionFixNavigationMask();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_NOISE_PWR_CTRL };
	int m_nMask1;
	int m_nMask2;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
