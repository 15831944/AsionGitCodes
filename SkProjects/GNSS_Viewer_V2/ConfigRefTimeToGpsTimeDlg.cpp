// FirmwareDownloadDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "ConfigRefTimeToGpsTimeDlg.h"
//#include "resource.h"
#include "Registry.h"
#include "GPS.h"
#include "Serial.h"

extern CGPSApp theApp;
// ConfigRefTimeToGpsTimeDlg 對話方塊

IMPLEMENT_DYNAMIC(ConfigRefTimeToGpsTimeDlg, CDialog)

ConfigRefTimeToGpsTimeDlg::ConfigRefTimeToGpsTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_REF_TIME_TO_GPS_TIME, pParent)
{

}

ConfigRefTimeToGpsTimeDlg::~ConfigRefTimeToGpsTimeDlg()
{

}

void ConfigRefTimeToGpsTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConfigRefTimeToGpsTimeDlg, CDialog)
//	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()

// ConfigRefTimeToGpsTimeDlg 訊息處理常式
BOOL ConfigRefTimeToGpsTimeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox* monthCombo = (CComboBox*)GetDlgItem(IDC_MONTH);
	CComboBox* dayCombo = (CComboBox*)GetDlgItem(IDC_DAY);

	((CButton*)GetDlgItem(IDC_ENABLE))->SetCheck(0);
	GetDlgItem(IDC_YEAR)->SetWindowText("2013");
	monthCombo->SetCurSel(0);
	dayCombo->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void ConfigRefTimeToGpsTimeDlg::OnOK()
{
	CString strValue;
	m_isEnable = ((CButton*)GetDlgItem(IDC_ENABLE))->GetCheck();
	GetDlgItem(IDC_YEAR)->GetWindowText(strValue);
	m_nYear = atoi(strValue);
	GetDlgItem(IDC_MONTH)->GetWindowText(strValue);
	m_nMonth = atoi(strValue);
	GetDlgItem(IDC_DAY)->GetWindowText(strValue);
	m_nDay = atoi(strValue);
	m_nAttribute = 0;
	
	CDialog::OnOK();
}
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(ConfigQueryGnssNavSolDlg, CDialog)

ConfigQueryGnssNavSolDlg::ConfigQueryGnssNavSolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_GNSS_NAV_SOL, pParent)
{

}

ConfigQueryGnssNavSolDlg::~ConfigQueryGnssNavSolDlg()
{

}

void ConfigQueryGnssNavSolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ConfigQueryGnssNavSolDlg, CDialog)
//	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
END_MESSAGE_MAP()

// ConfigRefTimeToGpsTimeDlg 訊息處理常式
BOOL ConfigQueryGnssNavSolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_GPS))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_GLONASS))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_GALILEO))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_BEIDOU))->SetCheck(0);

	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void ConfigQueryGnssNavSolDlg::OnOK()
{
	m_mode = 0;
	if(((CButton*)GetDlgItem(IDC_GPS))->GetCheck())
	{
		m_mode |= 0x1;
	}
	if(((CButton*)GetDlgItem(IDC_GLONASS))->GetCheck())
	{
		m_mode |= 0x2;
	}
	if(((CButton*)GetDlgItem(IDC_GALILEO))->GetCheck())
	{
		m_mode |= 0x4;
	}
	if(((CButton*)GetDlgItem(IDC_BEIDOU))->GetCheck())
	{
		m_mode |= 0x8;
	}
	m_attribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();

	CDialog::OnOK();
}
