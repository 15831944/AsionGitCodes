// ConfigPositionFixNavigationMask.cpp : ��@��
//

#include "stdafx.h"
#include "Resource.h"
#include "ConfigPositionFixNavigationMask.h"


// CConfigPositionFixNavigationMask ��ܤ��

IMPLEMENT_DYNAMIC(CConfigPositionFixNavigationMask, CDialog)

CConfigPositionFixNavigationMask::CConfigPositionFixNavigationMask(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONFIG_POS_FIX_NAV_MASK, pParent)
{
	m_nMask1 = 0;
	m_nMask2 = 0;
	m_nAttribute = 0;
}

CConfigPositionFixNavigationMask::~CConfigPositionFixNavigationMask()
{
}

void CConfigPositionFixNavigationMask::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigPositionFixNavigationMask, CDialog)
	ON_BN_CLICKED(IDOK, &CConfigPositionFixNavigationMask::OnBnClickedOk)
END_MESSAGE_MAP()


// CConfigPositionFixNavigationMask �T���B�z�`��
void CConfigPositionFixNavigationMask::OnBnClickedOk()
{
	m_nMask1 = ((CComboBox*)GetDlgItem(IDC_MASK1))->GetCurSel();
	m_nMask2 = ((CComboBox*)GetDlgItem(IDC_MASK2))->GetCurSel();
	m_nAttribute = ((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->GetCurSel();

	OnOK();
}

BOOL CConfigPositionFixNavigationMask::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	((CComboBox*)GetDlgItem(IDC_MASK1))->SetCurSel(0);;
	((CComboBox*)GetDlgItem(IDC_MASK2))->SetCurSel(0);;
	((CComboBox*)GetDlgItem(IDC_BINARY_ATTRI))->SetCurSel(0);;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
