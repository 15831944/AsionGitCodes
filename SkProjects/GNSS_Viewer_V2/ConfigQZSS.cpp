// ConfigSBAS.cpp : 實作檔
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigQZSS.h"


// CConfigQZSS 對話方塊

IMPLEMENT_DYNAMIC(CConfigQZSS, CDialog)

CConfigQZSS::CConfigQZSS(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_QZSS, pParent)
{
	m_bEnable = FALSE;
	m_nTrackingChannel = 0;
	m_nAttribute = 0;
}

CConfigQZSS::~CConfigQZSS()
{
}

void CConfigQZSS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigQZSS, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigQZSS::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigQZSS 訊息處理常式
void CConfigQZSS::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	CString txt;

	GetDlgItem(IDC_NUMBER_CHANNEL)->GetWindowText(txt);
	m_nTrackingChannel = atoi(txt);
	if(m_nTrackingChannel<0 || m_nTrackingChannel>3)
	{
		AfxMessageBox("Number of tracking channels must be between 0 and 3.");
		return;
	}

	m_bEnable = ((CButton*)GetDlgItem(IDC_ENABLE_QZSS))->GetCheck();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();;

	OnOK();
}

BOOL CConfigQZSS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;
	GetDlgItem(IDC_NUMBER_CHANNEL)->SetWindowText("1");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
