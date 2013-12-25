// ConfigSAGPS.cpp : 實作檔
//
#include "stdafx.h"
#include "Resource.h"
#include "ConfigSAGPS.h"

// CConfigSAGPS 對話方塊
IMPLEMENT_DYNAMIC(CConfigSAGPS, CDialog)

CConfigSAGPS::CConfigSAGPS(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_SAGPS, pParent)
{
	m_nEnable = 0;
	m_nAttribute = 0;
}

CConfigSAGPS::~CConfigSAGPS()
{
}

void CConfigSAGPS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigSAGPS, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigSAGPS::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigSAGPS 訊息處理常式
void CConfigSAGPS::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString txt;

	m_nEnable = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();;

	OnOK();
}

BOOL CConfigSAGPS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	((CComboBox*)GetDlgItem(IDC_MODE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;
	GetDlgItem(IDC_WARNING)->SetWindowText("Warning :\r\nPlease disable multi-hz position update rate before enable SAEE.");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
