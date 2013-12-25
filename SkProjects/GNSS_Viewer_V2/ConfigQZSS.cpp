// ConfigSBAS.cpp : ��@��
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigQZSS.h"


// CConfigQZSS ��ܤ��

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


// CConfigQZSS �T���B�z�`��
void CConfigQZSS::OnBnClickedOk()
{
	// TODO: �b���[�J����i���B�z�`���{���X
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

	// TODO:  �b���[�J�B�~����l��
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;
	GetDlgItem(IDC_NUMBER_CHANNEL)->SetWindowText("1");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
