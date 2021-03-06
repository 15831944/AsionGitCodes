// FirmwareDownloadDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "FirmwareDownloadDlg.h"
//#include "resource.h"
#include "Registry.h"
#include "GPS.h"
#include "Serial.h"

extern CGPSApp theApp;
// CFirmwareDownloadDlg 對話方塊

IMPLEMENT_DYNAMIC(CFirmwareDownloadDlg, CDialog)

CFirmwareDownloadDlg::CFirmwareDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FIRMWARE_DL, pParent)
{
}

CFirmwareDownloadDlg::~CFirmwareDownloadDlg()
{
}

void CFirmwareDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFirmwareDownloadDlg, CDialog)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)

END_MESSAGE_MAP()

// CFirmwareDownloadDlg 訊息處理常式
BOOL CFirmwareDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CComboBox* icTypeCombo = (CComboBox*)GetDlgItem(IDC_TYPE);
	CComboBox* baudrateCombo = (CComboBox*)GetDlgItem(IDC_BAUDRATE_IDX);
	CComboBox* bufferCombo = (CComboBox*)GetDlgItem(IDC_BUFFER_IDX);

	if(_V8_SUPPORT)
	{
		baudrateCombo->ResetContent();
		for(int i=0; i<9; ++i)
		{
			CString strIdx;
			strIdx.Format("%d", CSerial::BaudrateTable[i]);
			baudrateCombo->AddString(strIdx);
		}
	}
	baudrateCombo->AddString("SPI");
	baudrateCombo->AddString("I2C");

	CRegistry reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	if(reg.SetKey("Software\\GNSSViewer\\GPS", true))
	{
		m_nLoaderType = (LoaderType)reg.ReadInt("fw_ic_type", (int)UsingExternalLoader);
		m_nBaudrateIdx = reg.ReadInt("fw_baudrate", g_setting.boostBaudIndex);
		m_isInternal = reg.ReadInt("fw_internal", 0);
		m_strPath = reg.ReadString("fw_firmware", "");
		m_nBufferIdx = reg.ReadInt("fw_buffer", 0);
	}
	else
	{
		m_nLoaderType = UsingExternalLoader;
		m_nBaudrateIdx = g_setting.boostBaudIndex;
	}

	if(m_strPath.IsEmpty())
	{
		m_strPath = theApp.GetCurrrentDir();
		m_strPath += "\\prom.bin";
	}

	icTypeCombo->SetCurSel(m_nLoaderType);
	baudrateCombo->SetCurSel(m_nBaudrateIdx);
	GetDlgItem(IDC_IMG_PATH)->SetWindowText(m_strPath);
	((CButton*)GetDlgItem(IDC_IN_LOADER))->SetCheck(m_isInternal);
	bufferCombo->SetCurSel(m_nBufferIdx);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void CFirmwareDownloadDlg::OnOK()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	m_nLoaderType = (LoaderType)((CComboBox*)GetDlgItem(IDC_TYPE))->GetCurSel();
	m_nBaudrateIdx = ((CComboBox*)GetDlgItem(IDC_BAUDRATE_IDX))->GetCurSel();
	GetDlgItem(IDC_IMG_PATH)->GetWindowText(m_strPath);
	m_isInternal = ((CButton*)GetDlgItem(IDC_IN_LOADER))->GetCheck();
	m_nBufferIdx = ((CComboBox*)GetDlgItem(IDC_BUFFER_IDX))->GetCurSel();

	CRegistry reg;
	reg.SetRootKey(HKEY_CURRENT_USER);
	if(reg.SetKey("Software\\GNSSViewer\\GPS", false))
	{
		reg.WriteInt("fw_ic_type", m_nLoaderType);
		reg.WriteInt("fw_baudrate", m_nBaudrateIdx);
		reg.WriteString("fw_firmware", m_strPath);
		reg.WriteInt("fw_internal", m_isInternal);
		reg.WriteInt("fw_buffer", m_nBufferIdx);
	}

	CDialog::OnOK();
}

void CFirmwareDownloadDlg::OnBnClickedBrowse()
{
	CFileDialog fd(TRUE, "*.bin", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "*.bin|*.bin||");
	if(fd.DoModal() == IDOK)
	{
		m_strPath = fd.GetPathName();
		GetDlgItem(IDC_IMG_PATH)->SetWindowText(m_strPath);
	}

}