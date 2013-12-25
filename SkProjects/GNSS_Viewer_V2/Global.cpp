#include "stdafx.h"
#include "Global.h"
#include "WaitReadLog.h"
#include "Monitor_1PPS.h"
#include "NMEA.h"
#include "GPSDlg.h"

const float R2D = 57.2957795131F;
const COLORREF g_panelBkColor = RGB(250, 250, 250);
Setting g_setting;

U08 type;
U08 attribute;
U08 msgid;

U08  slgga;	
U08  slgsa;
U08  slgsv;
U08  slgll;
U08  slrmc;
U08  slvtg;
U08  slzda;
U08  slgns;

HANDLE g_connectEvent = NULL;
HANDLE g_closeEvent = NULL;
CWaitReadLog* WRL = NULL;
HANDLE waitlog = NULL;

double pos_lat, pos_lon;
int m_almanac_no;
CString m_almanac_filename;
CMonitor_1PPS *dia_monitor_1pps = NULL;

int FlashBytes[] = { 8*1024, 16*1024, 24*1024, 32*1024 };


U32 ConvertCharToU32(const char *src)
{
	U32 resultValue = 0;
	U08 charIter;
	const char *ptr = src;
	int start = strlen(src);
	for (int i=start-1; i>=0 ;--i)
	{
		charIter = *ptr;
		if (charIter >= '0' && charIter <= '9')
		{
			resultValue += (*ptr - '0') << (i*4);
		}
		else if (charIter >= 'A' && charIter <= 'F')
		{
			resultValue += (*ptr - 'A' + 0xA) << (i*4);
		}
		else if (charIter >= 'a' && charIter <= 'f')
		{
			resultValue += (*ptr - 'a' + 0xA) << (i*4);
		}
		ptr++;
	}
	return resultValue;
}

UINT WaitLogRead(LPVOID pParam)
{
	if(WRL != NULL)
	{
		WRL->DoModal();
	}

	if(!ResetEvent(waitlog))  
	{
		DWORD error = GetLastError();
	}
	delete WRL; 
	WRL = NULL;
	return 0;
}

bool IsFixed(U16 ggaIndicator)
{
	U08 gpsInd, gnssInd;

	if(ggaIndicator > 0xFF)
	{
		gpsInd = ggaIndicator >> 8;
		gnssInd = ggaIndicator & 0xFF;
	}
	else
	{
		gpsInd = ggaIndicator & 0xFF;
		gnssInd = 0xFF;
	}

	return (gpsInd == '1' || gpsInd == '2' || gpsInd=='6' || 
			gpsInd == 'A' || gpsInd == 'D' || gpsInd == 'E' ||
			gnssInd == '1' || gnssInd == '2' || gnssInd=='6' || 
			gnssInd == 'A' || gnssInd == 'D' || gnssInd == 'E');
}

bool CheckInUse(int id, GPGSA* gsa)
{
	bool noUSed = true;
	for(int i=0; i<MAX_SATELLITE; ++i)
	{
		if(id == gsa->SatelliteID[i])
		{
			return false;
		}
	}
	return noUSed;
}

double Deg2Rad(double degree)
{
	return degree * (PI/180); 
}

double Rad2Deg(double rad)
{
	return rad * (180/PI); 
}