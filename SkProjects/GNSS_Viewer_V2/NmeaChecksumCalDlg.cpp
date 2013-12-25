// NmeaChecksumCalDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "NmeaChecksumCalDlg.h"
#include "Resource.h"


// NmeaChecksumCalDlg 對話方塊

IMPLEMENT_DYNAMIC(NmeaChecksumCalDlg, CDialog)

NmeaChecksumCalDlg::NmeaChecksumCalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NMEA_CHECKSUM_CAL, pParent)
{

}

NmeaChecksumCalDlg::~NmeaChecksumCalDlg()
{
}

void NmeaChecksumCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NmeaChecksumCalDlg, CDialog)
	ON_EN_CHANGE(IDC_INPUT, &NmeaChecksumCalDlg::OnEnChangeInput)
END_MESSAGE_MAP()


// NmeaChecksumCalDlg 訊息處理常式

BOOL NmeaChecksumCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此加入額外的初始化
	//IDC_INPUT, IDC_RESULT

	GetDlgItem(IDC_INPUT)->SetWindowText("");
	GetDlgItem(IDC_RESULT)->SetWindowText("$*00");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX 屬性頁應傳回 FALSE
}

void NmeaChecksumCalDlg::OnEnChangeInput()
{
	// TODO:  如果這是 RICHEDIT 控制項，控制項將不會
	// 傳送此告知，除非您覆寫 CDialog::OnInitDialog()
	// 函式和呼叫 CRichEditCtrl().SetEventMask()
	// 讓具有 ENM_CHANGE 旗標 ORed 加入遮罩。
	CString strInput;
	GetDlgItem(IDC_INPUT)->GetWindowText(strInput);

	char checkSum = 0;
	for(int i=0; i<strInput.GetLength(); ++i)
	{
		checkSum ^= strInput.GetAt(i);
	}
	CString strResult;
	strResult.Format("$%s*%02X", strInput, checkSum);
	GetDlgItem(IDC_RESULT)->SetWindowText(strResult);
	// TODO:  在此加入控制項告知處理常式程式碼
}

BOOL NmeaChecksumCalDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	return CDialog::PreTranslateMessage(pMsg);
}

void NmeaChecksumCalDlg::OnOK()
{
	// TODO: 在此加入特定的程式碼和 (或) 呼叫基底類別

	//CDialog::OnOK();
}
