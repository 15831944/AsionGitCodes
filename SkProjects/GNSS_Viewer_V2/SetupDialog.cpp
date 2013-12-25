// SetupDialog.cpp : ��@��
//

#include "stdafx.h"
#include "SetupDialog.h"
#include "resource.h"


// CSetupDialog ��ܤ��

IMPLEMENT_DYNAMIC(CSetupDialog, CDialog)

CSetupDialog::CSetupDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SETUP, pParent)
{
	setting = NULL;
}

CSetupDialog::~CSetupDialog()
{
}

void CSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CSetupDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetupDialog �T���B�z�`��
#define BOOST_BAUDIDX_BASE		5
void CSetupDialog::OnBnClickedOk()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	CString s;
	GetDlgItem(IDC_BINSIZE_DELAY)->GetWindowText(s);
	if(s.IsEmpty())
	{
		setting->delayBeforeBinsize = 0;
	}
	else
	{
		setting->delayBeforeBinsize = atoi(s);
	}

	int boostBaudIdx = ((CComboBox*)GetDlgItem(IDC_BOOST_BAUDRATE))->GetCurSel() + BOOST_BAUDIDX_BASE;
	setting->boostBaudIndex = boostBaudIdx;

	setting->autoQueryVersion = ((CButton*)GetDlgItem(IDC_AUTO_QUERY_VERSION))->GetCheck();;
	setting->boostEphemeris = ((CButton*)GetDlgItem(IDC_BOOST_EPHEMERIS))->GetCheck();;

	OnOK();
}

BOOL CSetupDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	if(setting)
	{
		CString s;
		s.Format("%d", setting->delayBeforeBinsize);
		GetDlgItem(IDC_BINSIZE_DELAY)->SetWindowText(s);
		
		if(setting->boostBaudIndex >= BOOST_BAUDIDX_BASE)
		{
			((CComboBox*)GetDlgItem(IDC_BOOST_BAUDRATE))->SetCurSel(setting->boostBaudIndex - BOOST_BAUDIDX_BASE);
		}
		((CButton*)GetDlgItem(IDC_AUTO_QUERY_VERSION))->SetCheck(setting->autoQueryVersion);
		((CButton*)GetDlgItem(IDC_BOOST_EPHEMERIS))->SetCheck(setting->boostEphemeris);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
