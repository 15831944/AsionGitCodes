#pragma once


// CConfigParameterSearchEngineNumber ��ܤ��

class CConfigParameterSearchEngineNumber : public CDialog
{
	DECLARE_DYNAMIC(CConfigParameterSearchEngineNumber)

public:
	CConfigParameterSearchEngineNumber(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CConfigParameterSearchEngineNumber();

// ��ܤ�����
//	enum { IDD = IDD_CONFIG_PARAM_SEARCH_ENG_NUM };
	BOOL m_nMode;
	int m_nAttribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeMode();
};
