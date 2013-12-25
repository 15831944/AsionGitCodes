#pragma once

class BinaryCommand;
class BinaryData;

struct AppParameter
{
	LPCSTR comPort;
	int	baudRateIndex;
	LPCSTR fwPath;
	int	downloadBaudIndex;
	bool showProgress;
};
extern AppParameter g_appParam;

class Download
{
public:
	enum CmdErrorCode {
		Ack = 0,
		NACK,
		Timeout,
		Ok,
		End,
		Error,
	};

	Download(void);
	~Download(void);

	int DoDownload();

protected:
	BinaryData* fwFile;
	CmdErrorCode ExcuteBinaryCommand(BinaryCommand* cmd, BinaryData* ackCmd, DWORD timeOut = 2000);
	CmdErrorCode ExcuteTextCommand(const CString& strCmd, CString& strAckCmd, DWORD timeOut = 2000);
	CmdErrorCode GetTextAck(CString& strAckCmd, DWORD timeOut);

	CmdErrorCode SendBinsizeCmd();
	CmdErrorCode SendFwBuffer();
	void ShowProgress(int prog, int total);
	void ShowStartProgress();
	void ShowEndProgress();
};
