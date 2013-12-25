#pragma once

#include "Utility.h"
#include <vector>
#include "matrix.h"
using namespace math;
using namespace std;

#include "CxImage/ximage.h"
#if defined(_DEBUG)
 #pragma comment( lib, "CxImage/DebugLib/cximage.lib" )
 #pragma comment( lib, "CxImage/DebugLib/png.lib" )
 #pragma comment( lib, "CxImage/DebugLib/zlib.lib" )
#else
 #pragma comment( lib, "CxImage/ReleaseLib/cximage.lib" )
 #pragma comment( lib, "CxImage/ReleaseLib/png.lib" )
 #pragma comment( lib, "CxImage/ReleaseLib/zlib.lib" )
#endif

class CWaitReadLog;
struct GPGSA;
typedef matrix<float> Matrix;


#include "Registry.h"
struct Setting
{
	Setting()
	{
		Load();
	}

	void Save()
	{
		CRegistry reg;
		reg.SetRootKey(HKEY_CURRENT_USER);
		if(reg.SetKey("Software\\GNSSViewer\\GPS", false))
		{
			reg.WriteInt("setting_delayBeforeBinsize", delayBeforeBinsize);
			reg.WriteInt("setting_boostBaudrateIndex", boostBaudIndex);
			reg.WriteInt("setting_autoQueryVersion", autoQueryVersion);
			reg.WriteInt("setting_boostEphemeris", boostEphemeris);
		}	
	}

	void Load()
	{
		CRegistry reg;
		reg.SetRootKey(HKEY_CURRENT_USER);
		if(reg.SetKey("Software\\GNSSViewer\\GPS", true))
		{
			delayBeforeBinsize = reg.ReadInt("setting_delayBeforeBinsize", 0);
			boostBaudIndex = reg.ReadInt("setting_boostBaudrateIndex", BAUDRATE_DEFAULT);
			autoQueryVersion = reg.ReadInt("setting_autoQueryVersion", TRUE);
			boostEphemeris = reg.ReadInt("setting_boostEphemeris", FALSE);
		}
		else
		{
			delayBeforeBinsize = 0;
			boostBaudIndex = BAUDRATE_DEFAULT;
			autoQueryVersion = TRUE;
			autoQueryVersion = FALSE;
		}
	}

	int delayBeforeBinsize;
	int boostBaudIndex;
	BOOL autoQueryVersion;
	BOOL boostEphemeris;
};

extern Setting g_setting;

#if GNSS_VIEWER
 #define MAX_SATELLITE				32
#else
 #define MAX_SATELLITE				16
#endif
#define MyMaxPath		(MAX_PATH * 8)

extern const float R2D;
extern const COLORREF g_panelBkColor;

extern U08 type;
extern U08 attribute;
extern U08 msgid;

extern U08  slgga;	
extern U08  slgsa;
extern U08  slgsv;
extern U08  slgll;
extern U08  slrmc;
extern U08  slvtg;
extern U08  slzda;
extern U08  slgns;

extern HANDLE g_connectEvent;
extern HANDLE g_closeEvent;
extern CWaitReadLog* WRL;
extern HANDLE	waitlog;

extern double pos_lat, pos_lon;
extern int m_almanac_no;
extern CString m_almanac_filename;

class CMonitor_1PPS;
extern CMonitor_1PPS *dia_monitor_1pps;
//extern bool g_doesNotChangeBaudrate;
extern int FlashBytes[];

U32 ConvertCharToU32(const char *src);
UINT WaitLogRead(LPVOID pParam);
bool IsFixed(U16 gga_indicator);
bool CheckInUse(int id, GPGSA* gsa);
double Deg2Rad(double degree);
double Rad2Deg(double rad);
