// CConfigParameterSearchEngineNumber.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "CConfigParameterSearchEngineNumber.h"


// CConfigParameterSearchEngineNumber 對話方塊

IMPLEMENT_DYNAMIC(CConfigParameterSearchEngineNumber, CDialog)

CConfigParameterSearchEngineNumber::CConfigParameterSearchEngineNumber(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_PARAM_SEARCH_ENG_NUM, pParent)
{
	m_nMode = 0;
	m_nAttribute = 0;
}

CConfigParameterSearchEngineNumber::~CConfigParameterSearchEngineNumber()
{
}

void CConfigParameterSearchEngineNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigParameterSearchEngineNumber, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigParameterSearchEngineNumber::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_MODE, &CConfigParameterSearchEngineNumber::OnCbnSelchangeMode)
END_MESSAGE_MAP()


// CConfigParameterSearchEngineNumber 訊息處理常式
void CConfigParameterSearchEngineNumber::OnBnClickedOk()
{
	m_nMode = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();

	OnOK();
}

BOOL CConfigParameterSearchEngineNumber::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	((CComboBox*)GetDlgItem(IDC_MODE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);
	OnCbnSelchangeMode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CConfigParameterSearchEngineNumber::OnCbnSelchangeMode()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	int nSel = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();

	const char* szTable[] = {"0 : ROM version decide by HW power-on latch.\r\n     FLASH version : by SW define.",
		"1 : Low",
		"2 : Middle",
		"3 : High",
		"4 : Full"};

	GetDlgItem(IDC_DESCRIPTION)->SetWindowText(szTable[nSel]);

}
