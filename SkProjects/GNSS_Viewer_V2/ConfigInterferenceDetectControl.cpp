// ConfigSBAS.cpp : ��@��
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigInterferenceDetectControl.h"


// CConfigInterferenceDetectControl ��ܤ��

IMPLEMENT_DYNAMIC(CConfigInterferenceDetectControl, CDialog)

CConfigInterferenceDetectControl::CConfigInterferenceDetectControl(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_INF_DTE_CTRL, pParent)
{
	m_nMode = 0;
	m_nAttribute = 0;
}

CConfigInterferenceDetectControl::~CConfigInterferenceDetectControl()
{
}

void CConfigInterferenceDetectControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigInterferenceDetectControl, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigInterferenceDetectControl::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigInterferenceDetectControl �T���B�z�`��
void CConfigInterferenceDetectControl::OnBnClickedOk()
{
	m_nMode = ((CComboBox*)GetDlgItem(IDC_MODE))->GetCurSel();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();

	OnOK();
}

BOOL CConfigInterferenceDetectControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	((CComboBox*)GetDlgItem(IDC_MODE))->SetCurSel(0);;
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
