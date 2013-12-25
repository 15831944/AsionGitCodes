// ConfigSAGPS.cpp : ��@��
//
#include "stdafx.h"
#include "Resource.h"
#include "ConfigSAGPS.h"

// CConfigSAGPS ��ܤ��
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


// CConfigSAGPS �T���B�z�`��
void CConfigSAGPS::OnBnClickedOk()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	CString txt;

	m_nEnable = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();;

	OnOK();
}

BOOL CConfigSAGPS::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	((CComboBox*)GetDlgItem(IDC_MODE))->SetCurSel(0);
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;
	GetDlgItem(IDC_WARNING)->SetWindowText("Warning :\r\nPlease disable multi-hz position update rate before enable SAEE.");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
