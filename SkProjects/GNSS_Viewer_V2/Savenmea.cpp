// Savenmea.cpp : 實作檔
//

#include "stdafx.h"
#include "GPS.h"
#include "Savenmea.h"
#include "GPSDlg.h"


// CSaveNmea 對話方塊

IMPLEMENT_DYNAMIC(CSaveNmea, CDialog)
CSaveNmea::CSaveNmea(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveNmea::IDD, pParent)
{
	m_dialogEvent = 0;
	m_notifyWindow = NULL;
}

CSaveNmea::~CSaveNmea()
{
}

void CSaveNmea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaveNmea, CDialog)
	ON_BN_CLICKED(IDC_CONTINUE, OnBnClickedContinue)
	ON_BN_CLICKED(IDC_STOP, OnBnClickedStop)
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSaveNmea 訊息處理常式
void CSaveNmea::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CSaveNmea::PostNcDestroy()
{	
	CDialog::PostNcDestroy();
	//AfxGetMainWnd()->SendMessage(WM_USER_DIALOG_DESTROYED,0,0);
//	delete this;
//	CGPSDlg::gpsDlg->m_saveNmea = NULL;
}

UINT CSaveNmea::RegisterEventMessage()
{
	m_dialogEvent = ::RegisterWindowMessage("CSaveNmea-Event");
	return m_dialogEvent;
}

void CSaveNmea::OnBnClickedContinue()
{
	//CGPSDlg::gpsDlg->continue_write_nmea();
	::PostMessage(m_notifyWindow, m_dialogEvent, StopWriteNmea, 0);

	GetDlgItem(IDC_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}

void CSaveNmea::OnBnClickedStop()
{
	//CGPSDlg::gpsDlg->stop_write_nmea();
	::PostMessage(m_notifyWindow, m_dialogEvent, StopWriteNmea, 1);

	GetDlgItem(IDC_CONTINUE)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);
}

void CSaveNmea::OnBnClickedClose()
{
	::PostMessage(m_notifyWindow, m_dialogEvent, ClickClose, 0);
	ShowWindow(SW_HIDE);
}

void CSaveNmea::Initialize(LPCSTR nmeaFile)
{
	GetDlgItem(IDC_FILENAME)->SetWindowText(nmeaFile);
	GetDlgItem(IDC_FILESIZE)->SetWindowText("0 Bytes");
	GetDlgItem(IDC_CONTINUE)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP)->EnableWindow(TRUE);
}

void CSaveNmea::DisplaySize(int size)
{
	CString strMsg;
	strMsg.Format("%d Bytes", size);
	GetDlgItem(IDC_FILESIZE)->SetWindowText(strMsg);
}

// CPlayNmea 對話方塊

IMPLEMENT_DYNAMIC(CPlayNmea, CDialog)
CPlayNmea::CPlayNmea(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayNmea::IDD, pParent)
{
	m_dialogEvent = 0;
	m_notifyWindow = NULL;
}

CPlayNmea::~CPlayNmea()
{
}

void CPlayNmea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAY_INV_SL, m_playIntervalSlider);
}


BEGIN_MESSAGE_MAP(CPlayNmea, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_PLAY_CONTROL, &CPlayNmea::OnBnClickedPlayControl)
END_MESSAGE_MAP()


// CPlayNmea 訊息處理常式
void CPlayNmea::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CPlayNmea::PostNcDestroy()
{	
	CDialog::PostNcDestroy();
}

UINT CPlayNmea::RegisterEventMessage()
{
	m_dialogEvent = ::RegisterWindowMessage("CPlayNmea-Event");
	return m_dialogEvent;
}

const int IntervalRate[] = { 
	10000, 9000, 8000, 
	7000, 6000, 5000, 
	4000, 3000, 2000, 
	1000, 500, 250, 
	200, 125, 100, 
	50, 40, 25, 
	20 };

const char *IntervalStringTable[] = {
	"10 seconds", "9 seconds", "8 seconds",
	"7 seconds", "6 seconds", "5 seconds",
	"4 seconds", "3 seconds", "2 seconds",
	"1 seconds", "2 Hz", "4 Hz",
    "5 Hz", "8 Hz", "10 Hz",
	"20 Hz", "25 Hz", "40 Hz",
	"50 Hz" };
		
void CPlayNmea::Initialize(LPCSTR nmeaFile)
{
	int defaultRate = 9;
	GetDlgItem(IDC_NMEA_FILE)->SetWindowTextA(nmeaFile);
	GetDlgItem(IDC_PLAY_INV)->SetWindowTextA("1 seconds");
	m_playIntervalSlider.SetRange(0, sizeof(IntervalRate) / sizeof(IntervalRate[0]) - 1);
	m_playIntervalSlider.SetPageSize(1);
	m_playIntervalSlider.SetPos(defaultRate);
	m_isPlayControlInPause = false;
	GetDlgItem(IDC_PLAY_CONTROL)->SetWindowTextA("Pause");
}

int CPlayNmea::GetPlayInterval()
{
	return IntervalRate[m_playIntervalSlider.GetPos()];
}

void CPlayNmea::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	if(pScrollBar->GetDlgCtrlID()==IDC_PLAY_INV_SL)
	{

		CString str = IntervalStringTable[m_playIntervalSlider.GetPos()];
		GetDlgItem(IDC_PLAY_INV)->SetWindowTextA(str);
		switch(nSBCode)
		{
		case TB_LINEUP:
		case TB_LINEDOWN:
		case TB_PAGEUP:
		case TB_PAGEDOWN:
		case TB_THUMBTRACK:
		case TB_TOP:
		case TB_BOTTOM:
			::PostMessage(m_notifyWindow, m_dialogEvent, IntervalChange, IntervalRate[m_playIntervalSlider.GetPos()]);
			break;
		default:
			break;
		}
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPlayNmea::SetLineCount(int count, long size, long total)
{
	CString str;
	str.Format("%d (%d / %d)", count, size, total);
	GetDlgItem(IDC_LINE_COUNT)->SetWindowTextA(str);

	CProgressCtrl* p = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	p->SetRange32(0, (int)total);
	p->SetPos((int)size);
	
}

void CPlayNmea::OnBnClickedPlayControl()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	if(m_isPlayControlInPause)
	{
		m_isPlayControlInPause = false;
		GetDlgItem(IDC_PLAY_CONTROL)->SetWindowTextA("Pause");
		::PostMessage(m_notifyWindow, m_dialogEvent, PauseStatus, FALSE);
	}
	else
	{
		m_isPlayControlInPause = true;
		GetDlgItem(IDC_PLAY_CONTROL)->SetWindowTextA("Resume");
		::PostMessage(m_notifyWindow, m_dialogEvent, PauseStatus, TRUE);
	}
}

void CPlayNmea::OnBnClickedClose()
{
	::PostMessage(m_notifyWindow, m_dialogEvent, ClickClose, 0);
	ShowWindow(SW_HIDE);
}
