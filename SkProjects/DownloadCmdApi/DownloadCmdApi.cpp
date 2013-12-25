// DownloadCmdApi.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <conio.h>

#include "Serial.h"
#include "Download.h"

CSerial g_serial;
AppParameter g_appParam = { 0 };

int GetBaudRateIndex(int b)
{
	const int baudRateTable[] = { 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600 };
	const int tableSize = sizeof(baudRateTable) / sizeof(baudRateTable[0]);
	for(int i = 0; i < tableSize; ++i)
	{
		if(b == baudRateTable[i])
		{
			return i;
		}
	}
	return -1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 4)
	{
		printf("NavSparkDownloadCmd.exe V1.0.2\r\n");
		printf("NavSparkDownloadCmd.exe can update firmware to GNSS device.\r\n");
		printf("\r\n");
		printf("NavSparkDownloadCmd COMPORT BAUDRATE FWPATH [DNBAUDRATE]\r\n");
		printf("\r\n");
		printf(" COMPORT   The com port for GPS GNSS connection, ex : COM5\r\n");
		printf(" BAUDRATE   The baud rate for GNSS device, ex : 9600\r\n");
		printf(" FWPATH    The firmware file to be download into GNSS device. ex : prom.bin\r\n");
		printf(" DNBAUDRATE The baud rate to use for update. ex : 115200\r\n");
		printf("\r\n");
		printf(" Baud rate only support the follow values in BAUDRATE and DNBAUDRATE : \r\n");
		printf("  4800 bps\r\n");
		printf("  9600 bps\r\n");
		printf("  19200 bps\r\n");
		printf("  38400 bps\r\n");
		printf("  57600 bps\r\n");
		printf("  115200 bps\r\n");
		printf("  230400 bps\r\n");
		printf("  460800 bps (DNBAUDRATE default)\r\n");
		printf("  921600 bps\r\n");
		printf("\r\n");
		printf("Example : \r\n");
		printf("  NavSparkDownloadCmd COM3 9600 prom.bin \r\n");

		return 0;
	}

	if(INVALID_FILE_ATTRIBUTES == ::GetFileAttributes(argv[3]))
	{
		printf("Can't find firmware file in \"%s\"\r\n", argv[3]);
		return 1;
	}
	
	g_appParam.baudRateIndex = GetBaudRateIndex(atoi(argv[2]));
	g_appParam.comPort = argv[1];
	g_appParam.fwPath = argv[3];

	if(argc > 4)
	{
		if(0 == strcmp(argv[4], "-v"))
		{
			g_appParam.showProgress = true;
			g_appParam.downloadBaudIndex = 7;
		}
		else
		{
			g_appParam.downloadBaudIndex = GetBaudRateIndex(atoi(argv[4]));
		}

		if((argc > 5) && (0 == strcmp(argv[5], "-v")))
		{
			g_appParam.showProgress = true;
		}
	}
	else
	{
		g_appParam.downloadBaudIndex = 7;	
	}

	if(g_appParam.baudRateIndex < 0 || g_appParam.baudRateIndex > 8 || 
		g_appParam.downloadBaudIndex < 0 || g_appParam.downloadBaudIndex > 8)
	{
		printf("Parameter error, unsupported baud rate for BAUDRATE or DNBAUDRATE.\r\n");
		return 2;
	}

	if(!g_serial.Open2(g_appParam.comPort, CSerial::BaudrateTable[g_appParam.baudRateIndex]))
	{
		printf("Can't open COM port in \"%s\"\r\n", argv[1]);
		return 3;
	}


	Download d;
	int n = d.DoDownload();

	g_serial.Close();
	if(n == 0)
	{
		printf("\r\nFirmware update finished.\r\n");
	}
	else
	{
		printf("\r\nFirmware update failure. Error code : %d\r\n", n);
	}

	return n;
}

