// ConfigSBAS.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigNMEABinaryOutputDestination.h"


// CConfigNMEABinaryOutputDestination 對話方塊

IMPLEMENT_DYNAMIC(CConfigNMEABinaryOutputDestination, CDialog)

CConfigNMEABinaryOutputDestination::CConfigNMEABinaryOutputDestination(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_NMBI_OUT_DES, pParent)
{
	m_nMode = 0;
	m_nAttribute = 0;
}

CConfigNMEABinaryOutputDestination::~CConfigNMEABinaryOutputDestination()
{
}

void CConfigNMEABinaryOutputDestination::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigNMEABinaryOutputDestination, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigNMEABinaryOutputDestination::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigNMEABinaryOutputDestination 訊息處理常式
void CConfigNMEABinaryOutputDestination::OnBnClickedOk()
{
	m_nMode = 0;
	if(((CButton*)GetDlgItem(IDC_UART))->GetCheck())
	{
		m_nMode |= 0x01;
	}
	int nSlaveSel = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();
	if(nSlaveSel==1)
	{
		m_nMode |= 0x02;
	}
	else if(nSlaveSel==2)
	{
		m_nMode |= 0x04;
	}

	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();

	OnOK();
}

BOOL CConfigNMEABinaryOutputDestination::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	((CButton*)GetDlgItem(IDC_UART))->SetCheck(1);
	((CComboBox*)GetDlgItem(IDC_MODE))->SetCurSel(0);;
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
