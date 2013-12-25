#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CSavenmea ��ܤ��

class CSaveNmea : public CDialog
{
	DECLARE_DYNAMIC(CSaveNmea)

public:
	CSaveNmea(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CSaveNmea();

	enum EventType
	{
		StopWriteNmea,
		ClickClose,
	};
	// ��ܤ�����
	enum { IDD = IDD_SAVENMEA };
	UINT RegisterEventMessage();
	void SetNotifyWindow(HWND h) { m_notifyWindow = h; };

	void Initialize(LPCSTR nmeaFile);
	void DisplaySize(int size);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedContinue();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedClose();
	
protected:
	UINT m_dialogEvent;
	HWND m_notifyWindow;

//public:
//	afx_msg void OnClose();
};

class CPlayNmea : public CDialog
{
	DECLARE_DYNAMIC(CPlayNmea)

public:
	CPlayNmea(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~CPlayNmea();

	enum EventType
	{
		IntervalChange,
		PauseStatus,
		ClickClose,
	};
// ��ܤ�����
	enum { IDD = IDD_PLAY_NMEA };

	UINT RegisterEventMessage();
	void SetNotifyWindow(HWND h) { m_notifyWindow = h; };

	void Initialize(LPCSTR nmeaFile);
	int GetPlayInterval();
	void SetLineCount(int count, long size, long total);

protected:
	UINT m_dialogEvent;
	HWND m_notifyWindow;

	CSliderCtrl m_playIntervalSlider;
	bool m_isPlayControlInPause;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedPlayControl();
};
