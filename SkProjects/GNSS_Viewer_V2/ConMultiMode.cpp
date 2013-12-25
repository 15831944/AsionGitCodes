// ConMultiMode.cpp : 實作檔
//

#include "stdafx.h"
#include "GPS.h"
#include "ConMultiMode.h"


// CConMultiMode 對話方塊

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


// CConMultiMode 訊息處理常式

void CConMultiMode::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	OnOK();
}

BOOL CConMultiMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
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
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}
