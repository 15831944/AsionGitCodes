// NmeaChecksumCalDlg.cpp : ��@��
//

#include "stdafx.h"
#include "NmeaChecksumCalDlg.h"
#include "Resource.h"


// NmeaChecksumCalDlg ��ܤ��

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


// NmeaChecksumCalDlg �T���B�z�`��

BOOL NmeaChecksumCalDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �b���[�J�B�~����l��
	//IDC_INPUT, IDC_RESULT

	GetDlgItem(IDC_INPUT)->SetWindowText("");
	GetDlgItem(IDC_RESULT)->SetWindowText("$*00");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX �ݩʭ����Ǧ^ FALSE
}

void NmeaChecksumCalDlg::OnEnChangeInput()
{
	// TODO:  �p�G�o�O RICHEDIT ����A����N���|
	// �ǰe���i���A���D�z�мg CDialog::OnInitDialog()
	// �禡�M�I�s CRichEditCtrl().SetEventMask()
	// ���㦳 ENM_CHANGE �X�� ORed �[�J�B�n�C
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
	// TODO:  �b���[�J����i���B�z�`���{���X
}

BOOL NmeaChecksumCalDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

	return CDialog::PreTranslateMessage(pMsg);
}

void NmeaChecksumCalDlg::OnOK()
{
	// TODO: �b���[�J�S�w���{���X�M (��) �I�s�����O

	//CDialog::OnOK();
}
