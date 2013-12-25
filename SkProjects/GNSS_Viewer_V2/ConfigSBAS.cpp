// ConfigSBAS.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigSBAS.h"


// CConfigSBAS 對話方塊

IMPLEMENT_DYNAMIC(CConfigSBAS, CDialog)

CConfigSBAS::CConfigSBAS(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_SBAS, pParent)
{
	m_bEnable = FALSE;
	m_bRanging = FALSE;
	m_bCorrection = FALSE;
	m_nUraMask = 0;
	m_nTrackingChannel = 0;
	m_bWAAS = FALSE;
	m_bEGNOS = FALSE;
	m_bMSAS = FALSE;
	m_nAttribute = 0;
}

CConfigSBAS::~CConfigSBAS()
{
}

void CConfigSBAS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigSBAS, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigSBAS::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigSBAS 訊息處理常式
void CConfigSBAS::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString txt;

	GetDlgItem(IDC_URAMASK)->GetWindowText(txt);
	m_nUraMask = atoi(txt);
	if(m_nUraMask<0 || m_nUraMask>15)
	{
		AfxMessageBox("URA Mask must be between 0 and 15.");
		return;
	}

	GetDlgItem(IDC_NUMBER_CHANNEL)->GetWindowText(txt);
	m_nTrackingChannel = atoi(txt);
	if(m_nTrackingChannel<0 || m_nTrackingChannel>3)
	{
		AfxMessageBox("Number of tracking channels must be between 0 and 3.");
		return;
	}

	m_bEnable = ((CButton*)GetDlgItem(IDC_ENABLE_SBAS))->GetCheck();
	m_bRanging = ((CComboBox*)GetDlgItem(IDC_ENABLE_NAV))->GetCurSel();


	m_bCorrection = ((CButton*)GetDlgItem(IDC_ENABLE_CORRECTION))->GetCheck();
	m_bWAAS = ((CButton*)GetDlgItem(IDC_ENABLE_WAAS))->GetCheck();
	m_bEGNOS = ((CButton*)GetDlgItem(IDC_ENABLE_EGNOS))->GetCheck();
	m_bMSAS = ((CButton*)GetDlgItem(IDC_ENABLE_MSAS))->GetCheck();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();;

	OnOK();
}

BOOL CConfigSBAS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;
	GetDlgItem(IDC_NUMBER_CHANNEL)->SetWindowText("0");
	GetDlgItem(IDC_URAMASK)->SetWindowText("8");
	((CComboBox*)GetDlgItem(IDC_ENABLE_NAV))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
