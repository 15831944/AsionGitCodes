#pragma once


// ConfigRefTimeToGpsTimeDlg ��ܤ��

class ConfigRefTimeToGpsTimeDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigRefTimeToGpsTimeDlg)

public:
	ConfigRefTimeToGpsTimeDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~ConfigRefTimeToGpsTimeDlg();

// ��ܤ�����
//	enum { IDD = IDD_REF_TIME_TO_GPS_TIME };
	virtual BOOL OnInitDialog();

	BOOL m_isEnable;
	int m_nYear;
	int m_nMonth;
	int m_nDay;
	int m_nAttribute;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

class ConfigQueryGnssNavSolDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigQueryGnssNavSolDlg)

public:
	ConfigQueryGnssNavSolDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~ConfigQueryGnssNavSolDlg();

// ��ܤ�����
//	enum { IDD = IDD_REF_TIME_TO_GPS_TIME };
	virtual BOOL OnInitDialog();

	U16 m_mode;
	U08 m_attribute;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


class ConfigBinaryMeasurementDataOutDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfigBinaryMeasurementDataOutDlg)

public:
	ConfigBinaryMeasurementDataOutDlg(CWnd* pParent = NULL);   // �зǫغc�禡
	virtual ~ConfigBinaryMeasurementDataOutDlg();

// ��ܤ�����
//	enum { IDD = IDD_REF_TIME_TO_GPS_TIME };
	virtual BOOL OnInitDialog();

	U08 m_rate;
	U08 m_measTime;
	U08 m_rawMeas;
	U08 m_svChStatus;
	U08 m_rcvChStatus;
	U08 m_subFrame;
	U08 m_attribute;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


