// ConMultiMode.cpp : ��@��
//

#include "stdafx.h"
#include "GPS.h"
#include "ConMultiMode.h"


// CConMultiMode ��ܤ��

IMPLEMENT_DYNAMIC(CConMultiMode, CDialog)
CConMultiMode::CConMultiMode(CWnd* pParent /*=NULL*/)
	: CDialog(CConMultiMode::IDD, pParent)
{
	mode = 0;
	attribute = 0;
}

CConMultiMode::~CConMultiMode()
{
}

void CConMultiMode::DoDataExchange(CDataExchange* pDX)
{
	DDX_CBIndex(pDX,IDC_MULTI_MODE,mode);
	DDX_CBIndex(pDX,IDC_MULTIMODE_ATTR,attribute);

	DDX_Control(pDX,IDC_MULTI_MODE,m_mode);
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CConMultiMode, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CConMultiMode �T���B�z�`��

void CConMultiMode::OnBnClickedOk()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	OnOK();
}

BOOL CConMultiMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	m_mode.Clear();
	if(_V8_SUPPORT)
	{
		m_mode.AddString("Auto");
		m_mode.AddString("Pedestrian");
		m_mode.AddString("Car");
		m_mode.AddString("Marine");
		m_mode.AddString("Ballon");
		m_mode.AddString("Airborne");
	}
	else if(IS_DEBUG)
	{
		m_mode.AddString("Car");
		m_mode.AddString("Pedestrian");
		m_mode.AddString("Bike");
		m_mode.AddString("Boat");
	}
	else
	{
		m_mode.AddString("Car");
		m_mode.AddString("Pedestrian");
	}
	m_mode.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}
