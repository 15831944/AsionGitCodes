#include "stdafx.h"
#include "Download.h"
#include "Serial.h"

extern CSerial g_serial;
extern AppParameter g_appParam;

class BinaryData
{
private:
	U08* dataPtr;
	int dataSize;
public:
	BinaryData() 
	{ 
		dataSize = 0; 
		dataPtr = NULL; 
	};

	BinaryData(LPCSTR filepath) 
	{ 
		dataSize = 0; 
		dataPtr = NULL;
		ReadFromFile(filepath);
	};

	BinaryData(U08 *data, int size) 
	{ 
		ReadFromMemory(data, size);
	};

	BinaryData(const BinaryData& src) 
	{ 
		dataSize = 0; 
		dataPtr = NULL;

		if(src.Ptr())
		{
			Alloc(src.Size());
			memcpy(dataPtr, src.Ptr(), dataSize);
		}
	};

	BinaryData(UINT id, LPCSTR type) 
	{ 
		dataSize = 0; 
		dataPtr = NULL;
		ReadFromResource(id, type);	
	};

	BinaryData(int size) 
	{ 
		dataSize = 0; 
		dataPtr = NULL;
		Alloc(size);
	};

	virtual ~BinaryData() 
	{ 
		Free(); 
	};

	void Free()
	{
		delete [] dataPtr;
		dataPtr = NULL;
		dataSize = 0;
	}

	void Alloc(int size)
	{
		if(dataPtr)
		{
			Free();
		}
		dataSize = size; 
		if(dataSize)
		{
			dataPtr = new U08[dataSize];
			memset(dataPtr, 0, dataSize);
		}
	}

	int ReadFromFile(LPCSTR filepath)
	{
		CFile f;
		if(FALSE==f.Open(filepath, CFile::modeRead))
		{
			return 0;
		}
		dataSize = (int)f.GetLength();
		if(dataSize > 0 && dataPtr)
		{
			Free();
		}
		dataPtr = new U08[dataSize];
		f.Read(dataPtr, dataSize);
		f.Close();
		return dataSize;
	};

	int ReadFromResource(UINT id, LPCSTR type)
	{
		HRSRC hres = NULL;
		hres = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(id), type);
		int newDataSize = SizeofResource(AfxGetResourceHandle(), hres);
		ASSERT(hres && newDataSize);
		if(dataSize > 0 && dataPtr)
		{
			Free();
		}
		dataSize = newDataSize;
		dataPtr = new U08[dataSize];

		HGLOBAL	hbytes = LoadResource(AfxGetResourceHandle(), hres);
		U08* ptrSource = (U08*)LockResource(hbytes);
		memcpy(dataPtr, ptrSource, dataSize);
//		sData = (U08*)LockResource(hbytes);
		UnlockResource(ptrSource);
		::FreeResource(hbytes);
		return dataSize;
	}

	int ReadFromMemory(U08 *data, int size)
	{
		if(dataSize > 0 && dataPtr)
		{
			Free();
		}
		dataPtr = new U08[size];
		dataSize = size;
		memcpy(dataPtr, data, size);
		return dataSize;
	};

	U08 operator[](int iChar) const
	{
		ATLASSERT((iChar >= 0) && (iChar <= dataSize - 1));  // Indexing the '\0' is OK
		
		if((iChar < 0) || (iChar > dataSize - 1))
		{
			AtlThrow(E_INVALIDARG);
		}
			
		return(dataPtr[iChar]);
	}
	BinaryData& operator=(const BinaryData& src)
	{
		dataSize = 0; 
		dataPtr = NULL;

		if(src.Ptr())
		{
			Alloc(src.Size());
			memcpy(dataPtr, src.Ptr(), dataSize);
		}

		return(*this);
	}

	int Size() const { return dataSize; }
	const U08* Ptr(int index = 0) const { return (dataPtr + index); }
	U08* GetBuffer(int index = 0) { return (dataPtr + index); }
	void Clear() { if(dataSize) memset(dataPtr, 0, dataSize); };
};

class BinaryCommand
{
private:
	enum { CommandExtraSize = 7, CommandHeaderSize = 4 };
	BinaryData	m_commandData;

public:
	BinaryCommand() {};
	BinaryCommand(int size) 
	{
		m_commandData.Alloc(CommandExtraSize + size);
	}
	BinaryCommand(const BinaryData& data)
	{ SetData(data); }

	void SetData(const BinaryData& data)
	{
		m_commandData.Alloc(CommandExtraSize + data.Size());
		memcpy(m_commandData.GetBuffer(CommandHeaderSize), data.Ptr(), data.Size());
	}
	U08* GetBuffer() 
	{ 
		U08 checkSum = 0;
		for(int i = 0; i < m_commandData.Size() - CommandExtraSize; ++i)
		{
			checkSum ^= *(m_commandData.Ptr(i + CommandHeaderSize));	
		}

		*m_commandData.GetBuffer(0) = 0xA0;
		*m_commandData.GetBuffer(1) = 0xA1;
		*m_commandData.GetBuffer(2) = HIBYTE(m_commandData.Size() - CommandExtraSize);
		*m_commandData.GetBuffer(3) = LOBYTE(m_commandData.Size() - CommandExtraSize);
		*m_commandData.GetBuffer(m_commandData.Size() - 3) = checkSum;
		*m_commandData.GetBuffer(m_commandData.Size() - 2) = 0x0D;
		*m_commandData.GetBuffer(m_commandData.Size() - 1) = 0x0A;

		return m_commandData.GetBuffer(0); 
	}
	void SetU08(int index, U08 data)
	{
		*m_commandData.GetBuffer(index + CommandHeaderSize - 1) = data;
	}
	void SetU16(int index, U16 data)
	{
		*m_commandData.GetBuffer(index + CommandHeaderSize - 1) = HIBYTE(data);
		*m_commandData.GetBuffer(index + CommandHeaderSize + 0) = LOBYTE(data);
	}	
	void SetU32(int index, U32 data)
	{
		*m_commandData.GetBuffer(index + CommandHeaderSize - 1) = HIBYTE(HIWORD(data));
		*m_commandData.GetBuffer(index + CommandHeaderSize + 0) = LOBYTE(HIWORD(data));
		*m_commandData.GetBuffer(index + CommandHeaderSize + 1) = HIBYTE(LOWORD(data));
		*m_commandData.GetBuffer(index + CommandHeaderSize + 2) = LOBYTE(LOWORD(data));
	}	
	int Size() const
	{ return m_commandData.Size(); }
};

Download::Download(void)
{
	fwFile = new BinaryData;
}

Download::~Download(void)
{
	delete fwFile;
}

void Download::ShowProgress(int prog, int total)
{
	if(!g_appParam.showProgress)
	{
		return;
	}
	printf("\r%d%%...   ", 1 + (int)((double)prog / total * 98));
}

void Download::ShowStartProgress()
{
	if(!g_appParam.showProgress)
	{
		return;
	}
	printf("Start update firmware \"%s\" to %s using %d bps. \r\n", 
		g_appParam.fwPath, g_appParam.comPort, 
		CSerial::BaudrateTable[g_appParam.downloadBaudIndex]);
}

void Download::ShowEndProgress()
{
	if(!g_appParam.showProgress)
	{
		return;
	}
	printf("\r%d%%...   \r\n", 100);
}

int Download::DoDownload()
{
	ShowStartProgress();

	BinaryCommand cmd(6);
	cmd.SetU08(1, 0x0B);
	cmd.SetU08(2, g_appParam.downloadBaudIndex);		//Baud Rate Index
	cmd.SetU08(3, 0);
	cmd.SetU08(4, 0);
	cmd.SetU08(5, 0);

	BinaryData ackCmd;
	if(Ack != ExcuteBinaryCommand(&cmd, &ackCmd, 2000))
	{
		return 4;
	}

	g_serial.Close();
	g_serial.Open2(g_appParam.comPort, CSerial::BaudrateTable[g_appParam.downloadBaudIndex]);
	Sleep(200);
	ShowProgress(0, 100);
	
	if(0 == fwFile->ReadFromFile(g_appParam.fwPath))
	{
		return 5;
	}

	if(Ok != SendBinsizeCmd())
	{
		return 6;
	}
	ShowProgress(1, 100);

	if(Ok != SendFwBuffer())
	{
		return 7;
	}

	CString strAckCmd;
	if(End != GetTextAck(strAckCmd, 10000))
	{
		return 8;
	}
	ShowEndProgress();
	return 0;
}

Download::CmdErrorCode Download::SendBinsizeCmd()
{
	CString strBinsizeCmd;
	CString strAckCmd;
	
	U08 fwCheck = 0;
	for(int i=0; i<fwFile->Size(); ++i)
	{
		fwCheck += *(fwFile->Ptr(i));
	}

	U32 checkCode = fwFile->Size() + fwCheck;
	strBinsizeCmd.Format("BINSIZE = %d Checksum = %d %u ", fwFile->Size(), fwCheck, checkCode);
	return ExcuteTextCommand(strBinsizeCmd, strAckCmd, 10000);
}

Download::CmdErrorCode Download::SendFwBuffer()
{
	const DWORD bufferSize = 8192;
	DWORD leftSize = fwFile->Size();
	DWORD sentSize = 0;
	while(leftSize)
	{ 
		DWORD sendSize = (leftSize >= bufferSize) ? bufferSize : leftSize;
		g_serial.ClearQueue();
		if(sendSize != g_serial.SendData(fwFile->Ptr(sentSize), sendSize, true))
		{
			return Error;
		}

		CString strAckCmd;
		CmdErrorCode err = GetTextAck(strAckCmd, 3000);
		if(err != Ok)
		{
			return err;
		}
		sentSize += sendSize;
		leftSize -= sendSize;
		ShowProgress(sentSize, fwFile->Size());
	}
	return Ok;
}

Download::CmdErrorCode Download::ExcuteBinaryCommand(BinaryCommand* cmd, BinaryData* ackCmd, DWORD timeOut)
{
	//CommandEntry binMsg = cmdTable[cmdIdx];
	U08* pCmd = cmd->GetBuffer();
	int inSize = cmd->Size();

	ackCmd->Alloc(1024);
	g_serial.ClearQueue();
	g_serial.SendData(pCmd, inSize);
	ScopeTimer t;
	bool alreadyAck = false;
	while(1)
	{		
		ackCmd->Clear();
		DWORD len = g_serial.GetBinary(ackCmd->GetBuffer(), ackCmd->Size());
		if(t.GetDuration() > timeOut)
		{	//Time Out
			return Timeout;
		}
		if(!ReadOK(len))
		{	
			continue;
		}
		int cmdSize = MAKEWORD((*ackCmd)[3], (*ackCmd)[2]);
		if(cmdSize != len - 7)
		{	//Packet Size Error
			continue;
		}
		if( (*ackCmd)[0] != 0xa0 || (*ackCmd)[1] != 0xa1 ||
			(*ackCmd)[len-2] != 0x0d || (*ackCmd)[len-1] != 0x0a )
		{	//Format Error
			continue;
		}
		if( (*ackCmd)[4] == 0x83 && (*ackCmd)[5] == 0x0 )
		{	//ACK0
			continue;
		}
		if( (*ackCmd)[4] == 0x84 )
		{	//NACK
			return NACK;
		}
		if( (*ackCmd)[4] == 0x83 )
		{	//Get ACK
			//alreadyAck = true;
			//continue;
			return Ack;
		}
	}
	
	return Timeout;
}	

Download::CmdErrorCode Download::ExcuteTextCommand(const CString& strCmd, CString& strAckCmd, DWORD timeOut)
{
	g_serial.ClearQueue();
	g_serial.SendData(strCmd, strCmd.GetLength() + 1);
	return GetTextAck(strAckCmd, timeOut);
}	

Download::CmdErrorCode Download::GetTextAck(CString& strAckCmd, DWORD timeOut)
{
	ScopeTimer t;
	bool alreadyAck = false;
	while(1)
	{		
		if(t.GetDuration() > timeOut)
		{	//Time Out
			return Timeout;
		}

		strAckCmd.Empty();
		DWORD len = g_serial.GetString(strAckCmd.GetBuffer(1024), 1024, timeOut - t.GetDuration());
		strAckCmd.ReleaseBuffer();

		if(!ReadOK(len))
		{	
			continue;
		}
		if(len!=0)
		{
			if(0 == strAckCmd.Compare(CString("OK")))
			{
				return Ok;
			}
			if(0 == strAckCmd.Compare(CString("END")))
			{
				return End;
			}
		}
		Sleep(50);
	}
	
	return Timeout;
}	