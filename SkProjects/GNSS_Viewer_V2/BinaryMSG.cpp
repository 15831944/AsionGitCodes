#include "stdafx.h"
//#include "Utility.h"
#include "GPS.h"
#include "BinaryMSG.h"
#include "GPSDlg.h"



U16 gn_wn = 0xFFFF;
U32 gn_tow = 0xFFFFFFFF;

extern void add2message(char* buffer,int offset);

unsigned char Cal_Checksum(U08* pt)
{
	unsigned char checksum=0;
	int len = *(pt+2);
	len = len<<8 | *(pt+3);

	unsigned char ID = *(pt+4);
	for(int i=0;i<len;i++)
	{
//		_cprintf("%x ",pt[i+4]);
		checksum^=pt[i+4];
	}
//	_cprintf("\n");	
	if(checksum == pt[len+4])
	{		
		return ID;
	}
	else
	{
		Utility::LogFatal(__FUNCTION__, "Cal_Checksum error!!!", __LINE__);
	}
	return BINMSG_ERROR;
}

U16 CalChecksum2(U08* pt)
{
	unsigned char checksum=0;
	int len = *(pt+2);
	len = len<<8 | *(pt+3);

	unsigned char ID = *(pt+4);
	for(int i=0;i<len;i++)
	{
//		_cprintf("%x ",pt[i+4]);
		checksum^=pt[i+4];
	}
//	_cprintf("\n");	
	if(checksum == pt[len+4])
	{		
		return MAKEWORD(*(pt + 4), *(pt + 5));
	}
	else
	{
		Utility::LogFatal(__FUNCTION__, "Cal_Checksum error!!!", __LINE__);
	}
	return BINMSG_ERROR;
}
void ECEF_USER_PVT_PROC(ECEF_USER_PVT& ecef_user_pvt,U08* pt)
{
	memcpy(&ecef_user_pvt.MessageID    , &pt[4],  sizeof(U08));
	memcpy(&ecef_user_pvt.X            , &pt[5],  sizeof(S32));
	memcpy(&ecef_user_pvt.Y            , &pt[9],  sizeof(S32));
	memcpy(&ecef_user_pvt.Z            , &pt[13], sizeof(S32));
	memcpy(&ecef_user_pvt.XV           , &pt[17], sizeof(S16));
	memcpy(&ecef_user_pvt.YV           , &pt[19], sizeof(S16));
	memcpy(&ecef_user_pvt.ZV           , &pt[21], sizeof(S16));
	memcpy(&ecef_user_pvt.PositionMode , &pt[23], sizeof(U08));
	memcpy(&ecef_user_pvt.FixMode      , &pt[24], sizeof(U08));
	memcpy(&ecef_user_pvt.GPSWeek      , &pt[25], sizeof(U16));
	memcpy(&ecef_user_pvt.TOW          , &pt[27], sizeof(U32));
	memcpy(&ecef_user_pvt.Year         , &pt[31], sizeof(U16));
	memcpy(&ecef_user_pvt.Month        , &pt[33], sizeof(U08));
	memcpy(&ecef_user_pvt.Day          , &pt[34], sizeof(U08));
	memcpy(&ecef_user_pvt.Hour         , &pt[35], sizeof(U16));
	memcpy(&ecef_user_pvt.Min          , &pt[37], sizeof(U08));
	memcpy(&ecef_user_pvt.Sec          , &pt[38], sizeof(U08));
	memcpy(&ecef_user_pvt.NumOfSV      , &pt[39], sizeof(U08));
	memcpy(&ecef_user_pvt.NumOfSVinFix , &pt[41], sizeof(U08));
	memcpy(&ecef_user_pvt.GDOP         , &pt[41], sizeof(U08));
	memcpy(&ecef_user_pvt.PDOP         , &pt[42], sizeof(U08));
	memcpy(&ecef_user_pvt.HDOP         , &pt[43], sizeof(U08));
	memcpy(&ecef_user_pvt.VDOP         , &pt[44], sizeof(U08));
    memcpy(&ecef_user_pvt.TDOP         , &pt[45], sizeof(U08));    
}

void GEODETIC_USER_PVT_PROC(GEODETIC_USER_PVT& geod_user_pvt,U08* pt)
{
	memcpy(&geod_user_pvt.MessageID    , &pt[4],  sizeof(U08));
	memcpy(&geod_user_pvt.Latitude     , &pt[5],  sizeof(S32));	
	memcpy(&geod_user_pvt.Longitude    , &pt[9],  sizeof(S32));
	memcpy(&geod_user_pvt.Altitude     , &pt[13], sizeof(S16));
	memcpy(&geod_user_pvt.Heading      , &pt[15], sizeof(U16));
	memcpy(&geod_user_pvt.Speed        , &pt[17], sizeof(U16));
	memcpy(&geod_user_pvt.PositionMode , &pt[19], sizeof(U08));
	memcpy(&geod_user_pvt.FixMode      , &pt[20], sizeof(U08));
	memcpy(&geod_user_pvt.GPSWeek      , &pt[21], sizeof(U16));
	memcpy(&geod_user_pvt.TOW          , &pt[23], sizeof(U32));
	memcpy(&geod_user_pvt.Year         , &pt[27], sizeof(U16));
	memcpy(&geod_user_pvt.Month        , &pt[29], sizeof(U08));
	memcpy(&geod_user_pvt.Day          , &pt[30], sizeof(U08));
	memcpy(&geod_user_pvt.Hour         , &pt[31], sizeof(U16));
	memcpy(&geod_user_pvt.Min          , &pt[33], sizeof(U08));
	memcpy(&geod_user_pvt.Sec          , &pt[34], sizeof(U08));
	memcpy(&geod_user_pvt.NumOfSV      , &pt[35], sizeof(U08));
	memcpy(&geod_user_pvt.NumOfSVinFix , &pt[36], sizeof(U08));
	memcpy(&geod_user_pvt.GDOP         , &pt[37], sizeof(U08));
	memcpy(&geod_user_pvt.PDOP         , &pt[38], sizeof(U08));
	memcpy(&geod_user_pvt.HDOP         , &pt[39], sizeof(U08));
	memcpy(&geod_user_pvt.VDOP         , &pt[40], sizeof(U08));
	memcpy(&geod_user_pvt.TDOP         , &pt[41], sizeof(U08));   
	geod_user_pvt.Lat = (float)geod_user_pvt.Latitude/10000000;
	geod_user_pvt.Lon = (float)geod_user_pvt.Longitude /10000000;
}

void USER_SATELLITE_INFOMATION_PROC(USER_SATELLITE_INFOMATION& sv_info,U08* pt)
{
	memcpy(&sv_info.MessageID    , &pt[4],   sizeof(U08));
	int index=5;
	for(int i=0;i<12;i++)
	{
		memcpy(&sv_info.sv[i].prn    , &pt[index], sizeof(U08));
		index+=sizeof(U08);
	    memcpy(&sv_info.sv[i].health , &pt[index], sizeof(U08));
		index+=sizeof(U08);
	    memcpy(&sv_info.sv[i].azm    , &pt[index], sizeof(U16));
		index+=sizeof(U16);
	    memcpy(&sv_info.sv[i].elv    , &pt[index], sizeof(U08));
		index+=sizeof(U08);
	    memcpy(&sv_info.sv[i].cn0    , &pt[index], sizeof(U08));
		index+=sizeof(U08);
	    memcpy(&sv_info.sv[i].stu    , &pt[index], sizeof(U16));
		index+=sizeof(U16);	
	}	
	memcpy(&sv_info.GPSWeek       , &pt[101], sizeof(U16));
	memcpy(&sv_info.TOW           , &pt[103], sizeof(U32));
}

void SATELLITE_MEASUREMENT_DATA_PROC(SATELLITE_MEASUREMENT_DATA& sm_data,U08* pt)
{
	memcpy(&sm_data.MessageID     , &pt[4],    sizeof(U08));

	int index=5;
	for(int i=0;i<12;i++)
	{
		memcpy(&sm_data.smd[i].prn, &pt[index], sizeof(U16));
		index+=sizeof(U16);
	    memcpy(&sm_data.smd[i].PR,  &pt[index], sizeof(S32));
		index+=sizeof(S32);
	    memcpy(&sm_data.smd[i].DR,  &pt[index], sizeof(S32));
		index+=sizeof(S32);
	    memcpy(&sm_data.smd[i].Dp,  &pt[index], sizeof(U16));
		index+=sizeof(S16);
	}
	memcpy(&sm_data.GPSWeek       , &pt[149],  sizeof(U16));
	memcpy(&sm_data.TOW           , &pt[151],  sizeof(U32));
	memcpy(&sm_data.ClockOffset   , &pt[155],  sizeof(U16));
}

void SHOW_ECEF_USER_PVT(ECEF_USER_PVT& ecef_user_pvt,U08* pt,int len)
{
	ECEF_USER_PVT_PROC(ecef_user_pvt,pt);
#if 0
	TRACE("\r\nECEF USER PVT-------------------------------------\r\n");
	TRACE("X : %d , Y : %d , Z : %d\r\n",ecef_user_pvt.X,ecef_user_pvt.Y,ecef_user_pvt.Z);
	TRACE("XV: %d , YV: %d , ZV: %d\r\n",ecef_user_pvt.XV,ecef_user_pvt.YV,ecef_user_pvt.ZV);

	TRACE("Position Mode: ");
	if(ecef_user_pvt.PositionMode == 0)
		TRACE("Autonomous Mode\r\n");
	else if(ecef_user_pvt.PositionMode == 1)
		TRACE("Differential Mode\r\n");
	else if(ecef_user_pvt.PositionMode == 2)
		TRACE("Dead Reckoning\r\n");
	else if(ecef_user_pvt.PositionMode == 3)
		TRACE("Manual Mode\r\n");
	else if(ecef_user_pvt.PositionMode == 4)
		TRACE("Simulator Mode\r\n");
	else if(ecef_user_pvt.PositionMode == 5)
		TRACE("Data Invalid\r\n");

	TRACE("Fix Mode: ");
	if(ecef_user_pvt.FixMode == 0)
		TRACE("No Fix\r\n");
	else if(ecef_user_pvt.FixMode == 1)
		TRACE("2D\r\n");
	else if(ecef_user_pvt.FixMode == 2)
		TRACE("3D\r\n");
	else if(ecef_user_pvt.FixMode == 3)
		TRACE("3D+DGPS\r\n");
	TRACE("GPS Week : %d\r\n",ecef_user_pvt.GPSWeek);
	TRACE("Data : %d/%d/%d  ",ecef_user_pvt.Year+2000,ecef_user_pvt.Month,ecef_user_pvt.Day);
	TRACE("Time : %d:%d:%d\r\n",ecef_user_pvt.Hour,ecef_user_pvt.Min,ecef_user_pvt.Sec);
	TRACE("Number of SV: %d\r\n",ecef_user_pvt.NumOfSV);
	TRACE("Number of SV in fix: %d\r\n",ecef_user_pvt.NumOfSVinFix);
	TRACE("GDOP: %d ",ecef_user_pvt.GDOP);
	TRACE("PDOP: %d ",ecef_user_pvt.PDOP);
	TRACE("HDOP: %d ",ecef_user_pvt.HDOP);
	TRACE("VDOP: %d ",ecef_user_pvt.VDOP);
	TRACE("TDOP: %d ",ecef_user_pvt.TDOP);
	TRACE("\r\n---------------------------------------------------\r\n");
#endif
}


void SHOW_GEODETIC_USER_PVT(GEODETIC_USER_PVT& geod_user_pvt,U08* pt,int len)
{	
	GEODETIC_USER_PVT_PROC(geod_user_pvt,pt);
#if 0	
	TRACE("\r\nGEODETIC_USER_PVT-------------------------------------\r\n");
	TRACE("Latitude : %f ",geod_user_pvt.Lat);
	TRACE("Longitude: %f\r\n",geod_user_pvt.Lon);
	TRACE("Altitude : %d ",geod_user_pvt.Altitude);
	TRACE("Speed : %d ",geod_user_pvt.Speed);
	TRACE("Heading: %d\r\n",geod_user_pvt.Heading);

	TRACE("Position Mode: ");
	if(geod_user_pvt.PositionMode == 0)
		TRACE("Autonomous Mode\r\n");
	else if(geod_user_pvt.PositionMode == 1)
		TRACE("Differential Mode\r\n");
	else if(geod_user_pvt.PositionMode == 2)
		TRACE("Dead Reckoning\r\n");
	else if(geod_user_pvt.PositionMode == 3)
		TRACE("Manual Mode\r\n");
	else if(geod_user_pvt.PositionMode == 4)
		TRACE("Simulator Mode\r\n");
	else if(geod_user_pvt.PositionMode == 5)
		TRACE("Data Invalid\r\n");

	TRACE("Fix Mode: ");
	if(geod_user_pvt.FixMode == 0)
		TRACE("No Fix\r\n");
	else if(geod_user_pvt.FixMode == 1)
		TRACE("2D\r\n");
	else if(geod_user_pvt.FixMode == 2)
		TRACE("3D\r\n");
	else if(geod_user_pvt.FixMode == 3)
		TRACE("3D+DGPS\r\n");
	TRACE("GPS Week : %d\r\n",geod_user_pvt.GPSWeek);
	TRACE("Data : %d/%d/%d  ",geod_user_pvt.Year+2000,geod_user_pvt.Month,geod_user_pvt.Day);
	TRACE("Time : %d:%d:%d\r\n",geod_user_pvt.Hour,geod_user_pvt.Min,geod_user_pvt.Sec);
	TRACE("Number of SV: %d\r\n",geod_user_pvt.NumOfSV);
	TRACE("Number of SV in fix: %d\r\n",geod_user_pvt.NumOfSVinFix);
	TRACE("GDOP: %d ",geod_user_pvt.GDOP);
	TRACE("PDOP: %d ",geod_user_pvt.PDOP);
	TRACE("HDOP: %d ",geod_user_pvt.HDOP);
	TRACE("VDOP: %d ",geod_user_pvt.VDOP);
	TRACE("TDOP: %d ",geod_user_pvt.TDOP);
	TRACE("\r\n---------------------------------------------------\r\n");
#endif
}

void SHOW_USER_SATELLITE_INFOMATION(USER_SATELLITE_INFOMATION& sv_info,U08* pt,int len)
{   
	USER_SATELLITE_INFOMATION_PROC(sv_info,pt);
#if 0
	TRACE("\r\nUSER_SATELLITE_INFOMATION----------------------------\r\n");

	for(int i=0;i<12;i++)
	{
		TRACE("PRN: [%d], Health: %d,",sv_info.sv[i].prn,sv_info.sv[i].health);
        TRACE("Azimuth: %d, Elv: %d, C/NO: %d",sv_info.sv[i].azm,sv_info.sv[i].elv,sv_info.sv[i].cn0);
        TRACE("Status: %d \r\n",sv_info.sv[i].stu);
	}
	TRACE("GPS Week: %d , TOW: %d",sv_info.GPSWeek, sv_info.TOW);
	TRACE("\r\n-----------------------------------------------------\r\n");
#endif
}

void SHOW_SATELLITE_MEASUREMENT_DATA(SATELLITE_MEASUREMENT_DATA& sm_data,U08* pt,int len)
{	
	SATELLITE_MEASUREMENT_DATA_PROC(sm_data,pt);
#if 0
	TRACE("\r\nSATELLITE_MEASUREMENT_DATA----------------------------\r\n");

	for(int i=0;i<12;i++)
	{
		TRACE("PRN: [%d], PseudoRange: %d,",sm_data.smd[i].prn,sm_data.smd[i].PR);
		TRACE("DeltaRange: %d, Doppler: %d\r\n",sm_data.smd[i].DR,sm_data.smd[i].Dp);      
	}
	TRACE("GPS Week: %d, TOW: %d, Clock Offset: %d",sm_data.GPSWeek, sm_data.TOW,sm_data.ClockOffset);
	TRACE("\r\n-----------------------------------------------------\r\n");
#endif
}



void UTC_convert_gps_to_utc_time( S16 wn, D64 tow, UTC_TIME_T *utc_time_p )
{
	const S16 DEFAULT_LEAP_SECS = 15;           // 2006.01.01 : GPS-UTC = +14 seconds  
	const S16 INIT_UTC_YEAR = 1980;             // GPS Time start at 1980 Jan. 5/6 mid-night    
	const S16 DAYS_PER_YEAR = 365;    
	const S16 DAYS_PER_4_YEARS = ( DAYS_PER_YEAR*4 + 1 ); // plus one day for leap year
	const S16 day_of_year_table[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const S16 day_of_leap_year_table[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
	const S16* month_tbl_p = day_of_year_table;           // default is not the leap year              

	S16 i;
	S32 passed_leap_days, passed_utc_years, day_of_utc_year, leap_days_of_passed_utc_years;  
//	S32 ref_time ;              // ICD-200C : p122 ~ p124, decide situation a. or b. or c.             
	D64 tE = wn*604800 + tow;   // GPS time as estimated by the user.
	D64 delta_t_UTC = 0;        // leap sec calculated from sub4-p18
	D64 double_total_utc_sec;   
	S32 int_total_utc_sec;
	D64 tow_frac;
	S32 total_utc_day;
	S32 sec_of_day;  

	// ( ICD-200C p123, situation b. ) DN + 3/4 ~ DN +5/4
//	D64 W;
//	D64 t_UTC;

	// step 1 : decide leap sec, total_utc_day, sec of day 

		double_total_utc_sec = tE - DEFAULT_LEAP_SECS;
		int_total_utc_sec = (S32)double_total_utc_sec;
		tow_frac = double_total_utc_sec - (D64)int_total_utc_sec;
		total_utc_day = int_total_utc_sec / 86400L;
		sec_of_day = int_total_utc_sec - 86400L*total_utc_day;          

	// step 2 : calculate utc time    
	passed_leap_days = 1 + ( (total_utc_day + 5 - day_of_leap_year_table[2] ) / DAYS_PER_4_YEARS );
	passed_utc_years = (total_utc_day + 5 - passed_leap_days) / 365;
	leap_days_of_passed_utc_years = (passed_utc_years + 3) / 4;

	day_of_utc_year = total_utc_day + 5 - passed_utc_years*DAYS_PER_YEAR
		- leap_days_of_passed_utc_years;
	utc_time_p->day_of_year = (S16)day_of_utc_year + 1;                  
	utc_time_p->year = INIT_UTC_YEAR + (S16)passed_utc_years;

	if( (utc_time_p->year & 3) == 0 )        
		month_tbl_p = day_of_leap_year_table; // this year is leap year

	for( i = 1 ; i < 13 ; i++ )
		if( day_of_utc_year < month_tbl_p[i] )
			break;

	utc_time_p->month = i;
	utc_time_p->day   = (S16)day_of_utc_year - month_tbl_p[i-1] + 1; // + 1 : because of having sec_of_day

	utc_time_p->hour = (S16)(sec_of_day / 3600);
	if( utc_time_p->hour > 23 )
		utc_time_p->hour = 23;   

	utc_time_p->minute = (S16)((sec_of_day - utc_time_p->hour * 3600) / 60);
	if( utc_time_p->minute > 59 )
		utc_time_p->minute = 59;   

	utc_time_p->sec = (F32)(sec_of_day - utc_time_p->hour*3600L - utc_time_p->minute*60L) + (F32)tow_frac;
}

void COO_cartesian_to_geodetic( const POS_T* xyz_p, LLA_T* lla_p )
{
	D64 p;
	D64 theta;
	D64 temp;
	D64 s_theta, c_theta, s_phi, c_phi;
	D64 phi; // latitude

	p = sqrt( (xyz_p->px)*(xyz_p->px) + (xyz_p->py)*(xyz_p->py) );

	if( p <= 0.01f )
	{
		lla_p->lat = (xyz_p->pz >= 0)?(ICD_PI/2.0):(-ICD_PI/2.0);
		lla_p->lon = 0.0;
		lla_p->alt = (F32)(fabs(xyz_p->pz) - RB);
		return;
	}

	theta = atan2( ( xyz_p->pz*RA ), ( p*RB ) );
	s_theta = sin(theta);
	c_theta = cos(theta);    

	temp = ( xyz_p->pz + E2P*RB*s_theta*s_theta*s_theta ); 
	phi = atan2( temp, ( p - E2*RA*c_theta*c_theta*c_theta ) );

	s_phi = sin(phi);
	c_phi = cos(phi);

	lla_p->lat = phi;    
	lla_p->lon = atan2( xyz_p->py, xyz_p->px );
	lla_p->alt = (F32)( (p / c_phi) - ( RA / sqrt(1.0 - E2*s_phi*s_phi ) ) );
}

U08 *decode_4bytes(U08 *src,U32 *dst)
{
	*dst = src[0]<<24 | src[1]<<16 | src[2]<<8 | src[3];
	src+=4;
	return src;
}

U08 *decode_2bytes(U08 *src,U16 *dst)
{
	*dst = src[0]<<8 | src[1];
	src+=2;
	return src;
}
U08 *decode_1bytes(U08 *src,U08 *dst)
{
	*dst = src[0];
	src+=1;
	return src;
}

void Show_Measurement_time(unsigned char *src)
{
	U16 wn;
	U32 tow;
	U16 m_period;
	F32 ftow;
	char msg[512];

	U08 *ptr = &src[6];
	ptr = decode_2bytes(ptr,&wn);
	ptr = decode_4bytes(ptr,&tow);

	gn_wn = wn;
	gn_tow = tow;
	ptr = decode_2bytes(ptr,&m_period);
	ftow = (F32)(tow / 1000.0);
	
	int len = sprintf_s(msg, 512, "$wn=%d,tow=%d,period=%d", wn,tow, m_period);

	add2message(msg,len);

	UTC_TIME_T utc;
	UTC_convert_gps_to_utc_time(wn,ftow,&utc);

	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Hour = utc.hour;
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Min = utc.minute;
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Sec = utc.sec;

	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Year = utc.year;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Month = utc.month;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Day = utc.day;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Hour = utc.hour;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Min = utc.minute;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Sec = utc.sec;
}

void Show_Measurement_channel(unsigned char *src)
{
	U08 nmeas;
	CHANNEL_DATA_T channel;
	//char msg[512];
	U08 *ptr;
	char msg[512];
	U32 tmp[2];
//	F32 carrier_cycle;
	
	//nmeas = src[6]<<8 | src[7];
	nmeas = src[6];

	//channel = new CHANNEL_DATA_T[nmeas];

	ptr = &src[7];
	for (int i=0;i<nmeas;i++)
	{
		ptr = decode_1bytes(ptr,&channel.prn);
		ptr = decode_1bytes(ptr,&channel.cn0);
		ptr = decode_4bytes(ptr,&tmp[1]);
		ptr = decode_4bytes(ptr,&tmp[0]);
		memcpy(&channel.pseudo_range,tmp,sizeof(D64));

		ptr = decode_4bytes(ptr,&tmp[1]);
		ptr = decode_4bytes(ptr,&tmp[0]);
		memcpy(&channel.carrier_cycle,tmp,sizeof(D64));
		//memcpy(&channel.carrier_cycle,tmp,sizeof(S32));
		//carrier_cycle = channel.carrier_cycle / 1024.0;

		ptr = decode_4bytes(ptr,&tmp[0]);
		memcpy(&channel.doppler_freq,tmp,sizeof(F32));

		ptr = decode_1bytes(ptr,&channel.indicator);

		int len = sprintf_s(msg, sizeof(msg), "$SVID=%d,cn0=%d,pseudo_range=%.3f,carrier_cycle=%.3f,doppler_freq=%.0f,indicator=%d",
			channel.prn,channel.cn0,channel.pseudo_range,channel.carrier_cycle,channel.doppler_freq,channel.indicator);
		add2message(msg,len);

		//ptr += 19;
	}

//	delete [] channel;
}

extern Satellite satellites_gps[MAX_SATELLITE];
extern Satellite satellites_gnss[MAX_SATELLITE];
void Show_Measurement_SV(unsigned char *src)
{
	U08 svs;
	SV_CH_DATA_T sv;
	//char msg[512];
	U08 *ptr;
	char msg[512];
//	U32 tmp[2];
	int fixed_gps_c=0, fixed_glonass_c=0;
	int gps_c=0,glonass_c=0;

	svs = src[6];

	memset(CGPSDlg::gpsDlg->m_bdgsaMsgCopy.SatelliteID, 0, 24 * sizeof(U16));
	memset(CGPSDlg::gpsDlg->m_glgsaMsgCopy.SatelliteID, 0, 24 * sizeof(U16));
	memset(CGPSDlg::gpsDlg->m_gpgsaMsgCopy.SatelliteID, 0, 24 * sizeof(U16));
	//channel = new CHANNEL_DATA_T[nmeas];

	ptr = &src[7];
	for (int i=0;i<svs;i++)
	{
		ptr = decode_1bytes(ptr,&sv.channel_id);
		ptr = decode_1bytes(ptr,&sv.prn);
		ptr = decode_1bytes(ptr,&sv.SV_status);
		ptr = decode_1bytes(ptr,&sv.URA);
		ptr = decode_1bytes(ptr,(U08*)&sv.cn0);
		ptr = decode_2bytes(ptr,(U16*)&sv.elevation);
		ptr = decode_2bytes(ptr,(U16*)&sv.azimuth);
		ptr = decode_1bytes(ptr,&sv.channel_status);


		int len = sprintf_s(msg, sizeof(msg), "$channel_id=%d,SVID=%d,SV_status=%d,URA=%d,cn0=%d,elevation=%d,azimuth=%d,channel_status=%d",
			sv.channel_id,sv.prn,sv.SV_status,sv.URA,sv.cn0,sv.elevation,sv.azimuth,sv.channel_status);
		add2message(msg,len);

		if (sv.prn>0x40)
		{
			if (sv.channel_status & 0x30)
			{
				CGPSDlg::gpsDlg->m_glgsaMsgCopy.SatelliteID[fixed_glonass_c] = sv.prn;
				fixed_glonass_c++;
			}
			satellites_gnss[glonass_c].SatelliteID = sv.prn;
			satellites_gnss[glonass_c].SNR = sv.cn0;
			satellites_gnss[glonass_c].Elevation = sv.elevation;
			satellites_gnss[glonass_c].Azimuth = sv.azimuth;
			glonass_c++;
		}
		else
		{
			if (sv.channel_status & 0x30)
			{
				CGPSDlg::gpsDlg->m_gpgsaMsgCopy.SatelliteID[fixed_gps_c] = sv.prn;
				fixed_gps_c++;
			}

			satellites_gps[gps_c].SatelliteID = sv.prn;
			satellites_gps[gps_c].SNR = sv.cn0;
			satellites_gps[gps_c].Elevation = sv.elevation;
			satellites_gps[gps_c].Azimuth = sv.azimuth;

			gps_c++;
		}
	}
	
	CGPSDlg::gpsDlg->m_gpgsvMsgCopy.NumOfSate = gps_c;
	CGPSDlg::gpsDlg->m_gpgsvMsgCopy.NumOfMessage = (gps_c+3)/4;
	CGPSDlg::gpsDlg->m_gpgsvMsgCopy.SequenceNum = (gps_c+3)/4;
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.NumsOfSatellites = fixed_gps_c+fixed_glonass_c;

	CGPSDlg::gpsDlg->m_glgsvMsgCopy.NumOfSate = glonass_c;
	CGPSDlg::gpsDlg->m_glgsvMsgCopy.NumOfMessage = (glonass_c+3)/4;
	CGPSDlg::gpsDlg->m_glgsvMsgCopy.SequenceNum = (glonass_c+3)/4;

	//	delete [] channel;
}

void Show_Receiver_nav(unsigned char *src)
{
//	U08 svs;
	RECEIVER_NAV_DATA_T receiver;
	//char msg[512];
	U08 *ptr;
	char msg[1024];
	U32 tmp[2];

	//svs = src[6];

	//channel = new CHANNEL_DATA_T[nmeas];
	ptr = &src[6];
	ptr = decode_1bytes(ptr,&receiver.nav_status);
	ptr = decode_2bytes(ptr,&receiver.wn);
	
	ptr = decode_4bytes(ptr,&tmp[1]);
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.tow,tmp,sizeof(D64));
	ptr = decode_4bytes(ptr,&tmp[1]);
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_x,tmp,sizeof(D64));
	ptr = decode_4bytes(ptr,&tmp[1]);
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_y,tmp,sizeof(D64));
	ptr = decode_4bytes(ptr,&tmp[1]);
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_z,tmp,sizeof(D64));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_vx,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_vy,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.ecef_vz,tmp,sizeof(F32));

	ptr = decode_4bytes(ptr,&tmp[1]);
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.clock_bias,tmp,sizeof(D64));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.clock_drift,tmp,sizeof(F32));

	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.gdop,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.pdop,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.hdop,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.vdop,tmp,sizeof(F32));
	ptr = decode_4bytes(ptr,&tmp[0]);
	memcpy(&receiver.tdop,tmp,sizeof(F32));


	int len = sprintf_s(msg, sizeof(msg), "$navigation_status=%d,wn=%d,tow=%f,ecef_x=%f,ecef_y=%f,ecef_z=%f,ecef_vx=%f,ecef_vy=%f,ecef_vz=%f,clock_bias=%f,clock_drift=%f,gdop=%f,pdop=%f,hdop=%f,vdop=%f,tdop=%f",
		receiver.nav_status,receiver.wn,receiver.tow,receiver.ecef_x,receiver.ecef_y,receiver.ecef_z,
		receiver.ecef_vx,receiver.ecef_vy,receiver.ecef_vz,receiver.clock_bias,receiver.clock_drift,
		receiver.gdop,receiver.pdop,receiver.hdop,receiver.vdop,receiver.tdop);
	add2message(msg,len);

	switch(receiver.nav_status)
	{
	case 0:
		CGPSDlg::gpsDlg->m_gpggaMsgCopy.GPSQualityIndicator = '0';
		CGPSDlg::gpsDlg->m_gpgsaMsgCopy.Mode = 1;
		CGPSDlg::gpsDlg->m_gprmcMsgCopy.ModeIndicator = 'V';
		break;
	case 1:
		CGPSDlg::gpsDlg->m_gpggaMsgCopy.GPSQualityIndicator = '1';
		CGPSDlg::gpsDlg->m_gpgsaMsgCopy.Mode = 2;
		CGPSDlg::gpsDlg->m_gprmcMsgCopy.ModeIndicator = 'A';
		break;
	case 2:
		CGPSDlg::gpsDlg->m_gpggaMsgCopy.GPSQualityIndicator = '1';
		CGPSDlg::gpsDlg->m_gpgsaMsgCopy.Mode = 2;
		CGPSDlg::gpsDlg->m_gprmcMsgCopy.ModeIndicator = 'A';
		break;
	case 3:
		CGPSDlg::gpsDlg->m_gpggaMsgCopy.GPSQualityIndicator = '1';
		CGPSDlg::gpsDlg->m_gpgsaMsgCopy.Mode = 3;
		CGPSDlg::gpsDlg->m_gprmcMsgCopy.ModeIndicator = 'A';
		break;
	case 4:
		CGPSDlg::gpsDlg->m_gpggaMsgCopy.GPSQualityIndicator = '2';
		CGPSDlg::gpsDlg->m_gpgsaMsgCopy.Mode = 3;
		CGPSDlg::gpsDlg->m_gprmcMsgCopy.ModeIndicator = 'D';
		break;
	}

	POS_T pos;
	LLA_T lla;
	pos.px = receiver.ecef_x;
	pos.py = receiver.ecef_y;
	pos.pz = receiver.ecef_z;
	COO_cartesian_to_geodetic(&pos,&lla);

	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Latitude = lla.lat * R2D;
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Latitude_N_S = lla.lat >= 0 ? 'N' : 'S';
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Longitude = lla.lon * R2D;
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Longitude_E_W= lla.lon >= 0 ? 'E' : 'W';
	CGPSDlg::gpsDlg->m_gpggaMsgCopy.Altitude = lla.alt;

	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Latitude = lla.lat * R2D;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Latitude_N_S = lla.lat >= 0 ? 'N' : 'S';
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Longitude = lla.lon * R2D;
	CGPSDlg::gpsDlg->m_gprmcMsgCopy.Longitude_E_W= lla.lon >= 0 ? 'E' : 'W';

	CGPSDlg::gpsDlg->m_gpggaMsgCopy.HDOP = receiver.hdop;

	CGPSDlg::gpsDlg->m_gpgsaMsgCopy.PDOP = receiver.pdop;
	CGPSDlg::gpsDlg->m_gpgsaMsgCopy.HDOP = receiver.hdop;
	CGPSDlg::gpsDlg->m_gpgsaMsgCopy.VDOP = receiver.vdop;

	//	delete [] channel;
}

void Show_subframe(unsigned char *src)
{
	U08 prn;
	U08 subframe_id;
	//char msg[512];
//	U08 *ptr;
	char msg[1024];
//	U32 tmp[2];
	char temp_buff[10];
	int len;

	//svs = src[6];

	//channel = new CHANNEL_DATA_T[nmeas];
	len = src[2]<<8 | src[3];
	prn = src[5];
	subframe_id = src[6];

	sprintf_s(msg, sizeof(msg), "$SVID=%d,subframe id=%d,data=",prn,subframe_id);
	for(int i=3;i<len;i++)
	{
		sprintf_s(temp_buff, sizeof(temp_buff), "%02X ",src[4+i]);
		strcat_s(msg, sizeof(msg), temp_buff);
	}

	add2message(msg,strlen(msg));

}

void Show_glonass_string_buffer(unsigned char *src)
{
	U08 prn;
	U08 string_num;
	//char msg[512];
//	U08 *ptr;
	char msg[1024];
//	U32 tmp[2];
	char temp_buff[10];
	int len;

	//svs = src[6];

	//channel = new CHANNEL_DATA_T[nmeas];
	len = src[2]<<8 | src[3];
	prn = src[5];
	string_num = src[6];

	sprintf_s(msg, 1024, "$SVID=%d,string_number=%d,data=",prn,string_num);
	for(int i=3;i<len;i++)
	{
		sprintf_s(temp_buff, 10, "%02X ",src[4+i]);
		strcat_s(msg, 1024, temp_buff);
	}

	add2message(msg,strlen(msg));
}