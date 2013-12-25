#pragma once

class ScopeTimer
{
public:
	ScopeTimer(LPCTSTR subject = NULL, bool dump = false) : 
		startTickCount(::GetTickCount()), endTickCount(0), logSubject(subject) 
	{
		isDump = dump;
	}

	~ScopeTimer()
	{
		if(isDump)
		{
			Dump();
		}
	}
	DWORD GetDuration()
	{
		endTickCount = ::GetTickCount();
		return endTickCount - startTickCount;
	}

protected:
	DWORD startTickCount;
	DWORD endTickCount;
	CString logSubject;
	bool isDump;

	void Dump()
	{
		CString logText;
		logText.Format(_T("%s spent %u ms."), logSubject, GetDuration());
		::OutputDebugString(logText);
	}
};


// Serial.h

#define ASCII_CR        0x0D
#define ASCII_LF        0x0A

#define BINARY_HD1      0xA0
#define BINARY_HD2      0xA1

#define READ_ERROR		((DWORD)(-1))
#define ReadOK(len)			((len==READ_ERROR) ?0 :len)

class CSerial
{
public:
	CSerial();
	~CSerial();

	static int BaudrateTable[];
	static const int BaudrateTableSize;

	enum { InQueueSize = 1024 };
	enum { OutQueueSize = 1024 };
	enum { MaxQueueLoop = 128 };
	enum { IoPendingTimeOut = 2000 };

	//Getter and Setter
	bool IsOpened() { return m_isOpened; }
	int GetComPort() { return m_comPort; }
	int GetBaudRate() { return m_baudRate; }
	HANDLE GetHandle() { return m_comDeviceHandle; };
	void SetSentUnit(DWORD s) { m_sendUnit = s; };
	void SetLogStatus(DWORD s) { m_logStatus = s; };

	//Flow control
	bool Open(int port = 2, int baudIndex = 5);
	bool Open2(LPCSTR comPort, int baudRate);
	bool OpenByBaudrate(LPCSTR comPort, int baudrate = 115200);
	void Close();
	void CancelTransmission() { TRACE("CancelTransmission()\r\n"); m_cancelTransmission = true; }
	void ResetTransmission() { m_cancelTransmission = false; }
	bool GetCancelTransmission() { return m_cancelTransmission; }
	bool ResetPort(int baudIndex);
	void ClearQueue();

	DWORD ReadData(void* buffer, DWORD bufferSize, bool once = false);
	DWORD GetString(void* buffer, DWORD bufferSize, DWORD timeOut);
	DWORD GetBinary(void* buffer, DWORD bufferSize);
	DWORD GetBinaryBlock(void* buffer, DWORD bufferSize, DWORD blockSize);
	DWORD SendData(const void* buffer, DWORD bufferSize, bool blockTransfer = false, int delayDuration = 0);
	DWORD GetBinaryBlockInTime(void* buffer, DWORD bufferSize, DWORD timeout);
protected:
	HANDLE m_comDeviceHandle;
	int m_comPort;
	int m_baudRate;
	bool m_cancelTransmission;
	bool m_isOpened;
	OVERLAPPED m_OverlappedRead;
	OVERLAPPED m_OverlappedWrite;
	DWORD m_sendUnit;
	DWORD m_logStatus;


	int ComInitial();
	int WaitingDataIn();
	bool WriteCommBytes(char* buffer, int bufferSize);
	bool ResetPortNoDelay(int baud);
};
