#include "stdafx.h"
#include "GPS.h"
#include "BinaryMSG.h"
#include "Serial.h"
#include "WaitReadLog.h"
#include "Redir.h"
#include "MaskedBitmap.h" 
#include "ConPowerMode.h"
#include "Config_Password.h"
#include "Monitor_1PPS.h"
#include "ConfigSBAS.h"
#include "ConfigSAGPS.h" 
#include "PositionRateDlg.h"
#include "ConfigQZSS.h"
#include "SysRestartDlg.h"
#include "SnrBarChart.h" 
#include "Pic_Earth.h"
#include "Pic_Scatter.h"
#include "Config1ppsPulseWidthDlg.h"
#include "Con1PPS_PulseClkSource.h"
#include "Timing_start.h"
#include "ConfigNoisePowerControl.h"
#include "ConfigInterferenceDetectControl.h"
#include "ConfigNMEABinaryOutputDestination.h"
#include "Config_Proprietary.h"
#include "Config_Powersaving_param.h"
#include "ConAntennaDetection.h"
#include "Con1PPS_Nmea_Delay.h"
#include "Con1PPS_OutputMode.h"
#include "CConfigParameterSearchEngineNumber.h"
#include "ConfigPositionFixNavigationMask.h"
#include "ConfigRefTimeToGpsTimeDlg.h"
#include "ConNMEADlg.h"
#include "ConfigNmeaIntervalDlg.h"
#include "ConDauDlg.h"
#include "ConDOPDlg.h"
#include "ConEleDlg.h"
#include "ConBinOutDlg.h"
#include "ConSrePorDlg.h"
#include "DrMultiHzDlg.h"
#include "Config1ppsFrequenceOutput.h"
#include "ConfigEricssonIntervalDlg.h"

#include "GPSDlg.h"

struct CommandEntry
{
	U08 cmdId;
	U08 cmdSubId;
	U16 cmdSize;
	U08 cmdAck;
	U08 cmdAckSub;
};

static CommandEntry cmdTable[] =
{
	//QueryPositionRateCmd
	{ 0x10, 0xFF, 1, 0x86, 0x00 },
	//QueryPositionPinningCmd
	{ 0x3A, 0xFF, 1, 0xB4, 0x00 },
	//QueryDatumCmd
	{ 0x2D, 0xFF, 1, 0xAE, 0x00 },
	//QuerySwVerRomCmd
	{ 0x02, 0x00, 2, 0x80, 0x00 },
	//QuerySwVerSysCmd
	{ 0x02, 0x01, 2, 0x80, 0x00 },
	//QuerySwCrcRomCmd
	{ 0x03, 0x00, 2, 0x81, 0x00 },
	//QuerySwCrcSysCmd
	{ 0x03, 0x01, 2, 0x81, 0x00 },
	//QueryWaasStatusCmd
	{ 0x38, 0x00, 2, 0xB3, 0x00 },
	//Query1ppsModeCmd
	{ 0x3F, 0xFF, 1, 0xB6, 0x00 },
	//QueryPowerModeCmd
	{ 0x15, 0xFF, 1, 0xB9, 0x00 },
	//QueryPowerSavingParametersCmd
	{ 0x0D, 0xFF, 1, 0x86, 0x00 },
	//QueryProprietaryMessageCmd
	{ 0x4A, 0xFF, 1, 0xBF, 0x00 },
	//QueryTimingCmd
	{ 0x44, 0xFF, 1, 0xC2, 0x00 },
	//QueryDopMaskCmd
	{ 0x2E, 0xFF, 1, 0xAF, 0x00 },
	//QueryElevationAndCnrMaskCmd,
	{ 0x2F, 0xFF, 1, 0xB0, 0x00 },
	//QueryAntennaDetectionCmd,
	{ 0x48, 0xFF, 1, 0xBC, 0x00 },
	//QueryNoisePowerCmd,
	{ 0x40, 0xFF, 1, 0xB7, 0x00 },
	//QueryDrInfoCmd,
	{ 0x7F, 0xFF, 1, 0xF0, 0x00 },
	//QueryDrHwParameterCmd,
	{ 0x7E, 0xFF, 1, 0xF1, 0x00 },
	//QueryGnssSelectionForNavigationSystemCmd,
	{ 0x11, 0xFF, 1, 0x87, 0x00 },
	//QueryGnssKnumberSlotCnrCmd,
	{ 0x12, 0xFF, 1, 0x88, 0x00 },
	//QuerySbasCmd,
	{ 0x62, 0x02, 2, 0x62, 0x80 },
	//QuerySagpsCmd,
	{ 0x63, 0x02, 2, 0x63, 0x80 },
	//QueryQzssCmd,
	{ 0x62, 0x04, 2, 0x62, 0x81 },
	//QueryNoisePowerControlCmd,
	{ 0x64, 0x09, 2, 0x64, 0x84 },
	//QueryInterferenceDetectControlCmd,
	{ 0x64, 0x07, 2, 0x64, 0x83 },
	//QueryNmeaBinaryOutputDestinationCmd,
	{ 0x64, 0x05, 2, 0x64, 0x82 },
	//QueryParameterSearchEngineNumberCmd,
	{ 0x64, 0x0b, 2, 0x64, 0x85 },
	//QueryAgpsStatusCmd,
	{ 0x34, 0xFF, 8, 0xB2, 0x00 },
	//QueryDatalogLogStatusCmd,
	{ 0x17, 0xFF, 8, 0x94, 0x00 },
	//QueryRegisterCmd,
	{ 0x71, 0xFF, 5, 0xC0, 0x00 },
	//QueryPositionFixNavigationMaskCmd,
	{ 0x64, 0x12, 2, 0x64, 0x88 },
	//QueryChannelDopplerCmd,
	{ 0x7B, 0xFF, 9, 0xFE, 0x00 },
	//QueryNavigationModeCmd,
	{ 0x3D, 0xFF, 1, 0xB5, 0x00 },
	//QueryNmeaIntervalV8Cmd,
	{ 0x64, 0x03, 2, 0x64, 0x81 },
	//QueryNmeaInterval2V8Cmd,
	{ 0x7A, 0x01, 3, 0x7A, 0x01 },
	//QueryChannelClockOffsetCmd,
	{ 0x7C, 0xFF, 9, 0xFF, 0x00 },
	//QueryRefTimeSyncToGpsTimeCmd,
	{ 0x64, 0x16, 2, 0x64, 0x8A },
	//QueryNavigationModeV8Cmd,
	{ 0x64, 0x18, 2, 0x64, 0x8B },
	//QueryGnssBootStatusCmd,
	{ 0x64, 0x01, 2, 0x64, 0x80 },
	//QueryDrMultiHzCmd,
	{ 0x6F, 0x02, 2, 0x6F, 0x80 },
	//QueryGnssKnumberSlotCnr2Cmd,
	{ 0x66, 0x7F, 2, 0x66, 0xFF },
	//QueryGnssSelectionForNavigationSystem2Cmd,
	{ 0x66, 0x02, 2, 0x66, 0x80 },
	//QueryV8PowerSaveParameters,
	{ 0x64, 0x0D, 2, 0x64, 0x86 },
	//QueryV8RomPowerSaveParameters,
	{ 0x64, 0x0D, 2, 0x86, 0x00 },
	//QueryGnssNavSolCmd,
	{ 0x64, 0x1A, 2, 0x64, 0x8C },
	//{ 0x64, 0x1A, 2, 0x8C, 0x00 },
	//QueryGnssNmeaTalkIdCmd,
	{ 0x4F, 0xFF, 1, 0x93, 0x00 },
	//QueryCustomerIDCmd,
	{ 0x0D, 0xFF, 1, 0x85, 0x00 },
	//Query1ppsFreqencyOutputCmd,
	{ 0x65, 0x04, 2, 0x65, 0x81 },
	//QueryEricssonIntervalCmd,
	{ 0x7A, 0x04, 3, 0x7A, 0x04 },
	//QQueryBinaryMeasurementDataOutCmd,
	{ 0x1F, 0xFF, 1, 0x89, 0x00 },
};

enum SqBinaryCmd
{
	QueryPositionRateCmd = 0,
	QueryPositionPinningCmd,
	QueryDatumCmd,
	QuerySwVerRomCmd,
	QuerySwVerSysCmd,
	QuerySwCrcRomCmd,
	QuerySwCrcSysCmd,
	QueryWaasStatusCmd,
	Query1ppsModeCmd,
	QueryPowerModeCmd,
	QueryPowerSavingParametersCmd,
	QueryProprietaryMessageCmd,
	QueryTimingCmd,
	QueryDopMaskCmd,
	QueryElevationAndCnrMaskCmd,
	QueryAntennaDetectionCmd,
	QueryNoisePowerCmd,
	QueryDrInfoCmd,
	QueryDrHwParameterCmd,
	QueryGnssSelectionForNavigationSystemCmd,
	QueryGnssKnumberSlotCnrCmd,
	QuerySbasCmd,
	QuerySagpsCmd,
	QueryQzssCmd,
	QueryNoisePowerControlCmd,
	QueryInterferenceDetectControlCmd,
	QueryNmeaBinaryOutputDestinationCmd,
	QueryParameterSearchEngineNumberCmd,
	QueryAgpsStatusCmd,
	QueryDatalogLogStatusCmd,
	QueryRegisterCmd,
	QueryPositionFixNavigationMaskCmd,
	QueryChannelDopplerCmd,
	QueryNavigationModeCmd,
	QueryNmeaIntervalV8Cmd,
	QueryNmeaInterval2V8Cmd,
	QueryChannelClockOffsetCmd,
	QueryRefTimeSyncToGpsTimeCmd,
	QueryNavigationModeV8Cmd,
	QueryGnssBootStatusCmd,
	QueryDrMultiHzCmd,
	QueryGnssKnumberSlotCnr2Cmd,
	QueryGnssSelectionForNavigationSystem2Cmd,
	QueryV8PowerSaveParameters,
	QueryV8RomPowerSaveParameters,
	QueryGnssNavSolCmd,
	QueryGnssNmeaTalkIdCmd,
	QueryCustomerIDCmd,
	Query1ppsFreqencyOutputCmd,
	QueryEricssonIntervalCmd,
	QueryBinaryMeasurementDataOutCmd,
};

bool CGPSDlg::SAVE_EPHEMRIS(U08* buff, U08 id)
{	
	if(Cal_Checksum(buff) == id)
	{
		int len = buff[3] - 1;
		m_ephmsFile.Write(&buff[5], len);     
		return true;
	}
	return false;
}

bool CGPSDlg::SaveEphemeris2(U08* buff, WORD id)
{	
	if(CalChecksum2(buff) == id)
	{	
		int len = buff[3] - 2;	//id and sub id are two bytes.
		m_ephmsFile.Write(&buff[6], len);     
		return true;
	}
	return false;
}

U08  CGPSDlg::IsSuccessful(U08* buff, int tail, bool show_msg)
{	
	if(buff[0]==0xa0 && buff[1]==0xa1 &&
		buff[tail-1]==0x0d && buff[tail]==0x0a && buff[4]==0x83 && buff[5]==0x0)
	{		
		return 0;
	}
	if((buff[0]==0xa0) && (buff[1]==0xa1) &&
		(buff[tail-1]==0x0d)&&(buff[tail]==0x0a) && (buff[4]==0x83))
	{
		_cprintf("Received ACK...\n");			
		return 1;
	}
	else if((buff[0]==0xa0) && (buff[1]==0xa1) &&
		(buff[tail-1]==0x0d)&&(buff[tail]==0x0a) && (buff[4]==0x84))
	{
		_cprintf("Received NACK...\n");		      
		if(show_msg)
		{
			add_msgtolist("Received NACK...");
		}
		return 2;
	}
	return 0;
}

bool CGPSDlg::SendToTarget(U08* message, U16 length, const char* Msg, bool quick)
{	
	time_t start,end;
	start = clock();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}

	ClearQue();
	m_serial->SendData(message, length);	
	while(1)
	{		
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));

		U08 len = buff[2] <<8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		U08 ack = IsSuccessful(buff, k2, !quick);	
		if(ack == 1)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, buff[2] <<8 | buff[3] + 7));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}
			return true;
		}
		else if(ack == 2)
		{
			return false;
		}

		end=clock();
		if(quick)
		{
			if(TIMEOUT_METHOD_QUICK(start, end))
				return false;
		}
		else
		{
			if(TIMEOUT_METHOD(start, end))
				return false;
		}
	}		
}

int CGPSDlg::SendToTargetEph(U08* message,U16 length,char* Msg,bool quick)
{	
	time_t start,end;
	start = clock();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}
	ClearQue();
	m_serial->SendData(message, length, true);	
	while(1)
	{		
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));			
		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		
		U08 ack = IsSuccessful(buff, k2, !quick);	
		if(ack == 1)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}
			return 0;
		}
		else if(ack == 2)
		{
			return 1;
		}
		end=clock();

		if(quick)
		{
			if(TIMEOUT_METHOD_QUICK(start, end))
			{
				return 2;
			}
		}
		else
		{
			if(TIMEOUT_METHOD(start, end))
			{
				return 2;
			}
		}
	}
}

bool CGPSDlg::SendToTargetNoAck(U08* message,U16 length)
{		
//	ScopeTimer t("SendToTargetNoAck()");
	ClearQue();
	m_serial->SendData(message, length, true);	
	return true;
}

bool CGPSDlg::SendToTargetNoWait(U08* message, U16 length, LPCSTR Msg)
{		
	time_t start,end;
	start = clock();		    
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}

	ClearQue();
	m_serial->SendData(message, length);	
	while(1)
	{	
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));

		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		U08 ack = IsSuccessful(buff, k2);		
		if(ack == 1)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}
			return true;
		}
		else if(ack == 2)
		{
			return false;
		}
		
		end=clock();
		if(TIMEOUT_METHOD(start, end))
		{			
			return false;
		}	
	}
}

void CGPSDlg::QueryRtc()
{
	U08 msg[5], checksum=0;
	CString temp;
	U32 data = 0;
	msg[0]=0x71; //msgid
	msg[1]=0x20;
	msg[2]=0x00;
	msg[3]=0x4C;
#if GPS_VIEWER
	msg[4]=0x34;
#else
	msg[4]=0x08;
#endif
	int len = SetMessage(msg, sizeof(msg));
	time_t start,end;

	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg, len, "", 1))
	{
		start = clock();
		while(1)
		{
			U08 buff[1024] = {0};
			m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			int k1 = len + 5;
			int k2 = len + 6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xc0) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a) )
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					if(m_bShowBinaryCmdData)
					{
						add_msgtolist("Return: " + theApp.GetHexString(buff, k2+1));	
					}

					data = buff[5]<<24 | buff[6]<<16 |buff[7]<<8 | buff[8];
					temp.Format("0X%08X",data);
					m_noise.SetWindowText(temp);
					//sprintf_s(binmsg,"0x %x", data);	
					break;
				}
			}
			end=clock();	
			if(TIMEOUT_METHOD_QUICK(start,end))
				return ;		
		}		
	}	
	return ;
}

bool CGPSDlg::CheckGPS(U08* message, U16 length, char* Msg)
{	
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}
	m_serial->SendData(message, length, true);	  
	time_t start,end;
	start = clock();		    
	while(1)
	{		
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));			
		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;			
		if(IsSuccessful(buff, k2))
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Return: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);	
			}		
			return true;
		}
		end=clock();
		//TIMEOUT =  (U32)(end-start);
		if((end-start) > SCAN_TIME_OUT_MS )
		{				
			return false;
		}
	}
}

bool CGPSDlg::SendToTargetNoShow(U08* message,U16 length,char* Msg)
{		
	ClearQue();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}
	m_serial->SendData(message, length);	
	time_t start,end;
	start = clock();		    
	while(1)
	{	
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));				
		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		U08 ack = IsSuccessful(buff,k2);		
		if(ack == 1)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				//TRACE("%s",Msg);
			}
			//pWADlg->IsFinish = true;
			return true;
		}
		else if(ack == 2)
		{
		//	pWADlg->IsFinish = true;
			return false;
		}
		
		end=clock();
		//TIMEOUT = (U32)( end-start);
		if((end-start) > TIME_OUT_MS )
		{			
		//	AfxMessageBox("Timeout: GPS device no response.");
			return false;
		}	
	}
}

U32 CGPSDlg::get_register_zero()
{
	U08 msg[5] ,checksum=0;
	U32 data = 0;
	msg[0]=0x71; //msgid
	msg[1]=0;
	msg[2]=0;
	msg[3]=0;
	msg[4]=0;

	int len = SetMessage(msg, sizeof(msg));
	time_t start,end;
	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg, len, ""))
	{
		start = clock();
		while(1)
		{
			U08 buff[1024] = {0};
			m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			int k1 = len + 5;
			int k2 = len + 6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xc0) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a) )
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					data = buff[5]<<24 | buff[6]<<16 |buff[7]<<8 | buff[8];
			    		//sprintf_s(binmsg,"0x %x", data);	
					break;
				}
			}
			end=clock();	
			if(TIMEOUT_METHOD(start,end))
				return 0x1cba;		
		}		
	}	
	return data;
}

int CGPSDlg::SendToTarget_datalog(U08* message,U16 length,char* Msg)
{	
	//char Str[30];
	time_t start,end;
	start = clock();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(message, length));	
	}
	ClearQue();
	m_serial->SendData(message, length, true);		    
	while(1)
	{		
		U08 buff[1024] = {0};
		m_serial->GetBinary(buff, sizeof(buff));			
		U08 len = buff[2] << 8 | buff[3];
		int k1 = len + 5;
		int k2 = len + 6;		
		U08 ack = IsSuccessful(buff,k2);	
		
		if(ack == 1)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(buff, k2+1));	
			}

			if(strlen(Msg))
			{
				add_msgtolist(Msg);
				//m_binary.InsertItem(bincount++,Msg);	
				//m_binary.Scroll(size);
			}
			return 0;
		}
		else if(ack == 2)
		{
			return 1;
		}
		end=clock();
		if(TIMEOUT_METHOD(start, end))
			return 9;
			
	}		
}

void CGPSDlg::cancel_log_read()
{
	cancel_readlog = 1;
}

void CGPSDlg::UnlockSoarcomm()
{
#if(SOARCOMM)
	//if(!CheckConnect())return;
	////WaitEvent();
	ClearQue();
	U08 message[23];
	U08 msg[16];
	U08 buff[256];
	char binmsg[100];
	msg[0] = 0x45; //msgid
	msg[1] = 's';
	msg[2] = 'k';
	msg[3] = 'y';
	msg[4] = 't';
	msg[5] = 'r';
	msg[6] = 'a';
	msg[7] = 'q';
	msg[8] = 's';
	msg[9] = 'o';
	msg[10] = 'a';
	msg[11] = 'r';
	msg[12] = 'c';
	msg[13] = 'o';
	msg[14] = 'm';
	msg[15] = 'm';
	int msg_len = SetMessage(msg,sizeof(msg));

	if(SendToTarget(CGPSDlg::m_inputMsg,msg_len,"") )
	{
		start = clock();
		while(1)
		{
			m_serial->GetBinary(buff, sizeof(buff));
			if (check_msg_valid(buff) == 0xd0)
			{
				if ( buff[5] == 0 )
					sprintf_s(binmsg, sizeof(binmsg), "Datalog Unlock.");
				else
					sprintf_s(binmsg, sizeof(binmsg), "Datalog Lock.");
				
				add_msgtolist(binmsg);
				break;
			}
			end=clock();	
			if(TIMEOUT_METHOD(start,end))
				break;		
		}
	}
	//CreateGPSThread();
#endif
}

void CGPSDlg::UnlockPolstar()
{
#ifdef POLSTAR
	//if(!CheckConnect())return;
	////WaitEvent();
	ClearQue();
	U08 message[22];
	U08 msg[15];
	U08 buff[256];
	char binmsg[100];
	msg[0] = 0x45; //msgid
	msg[1] = 's';
	msg[2] = 'k';
	msg[3] = 'y';
	msg[4] = 't';
	msg[5] = 'r';
	msg[6] = 'a';
	msg[7] = 'q';
	msg[8] = 'p';
	msg[9] = 'o';
	msg[10] = 'l';
	msg[11] = 's';
	msg[12] = 't';
	msg[13] = 'a';
	msg[14] = 'r';

	int msg_len = SetMessage(msg,sizeof(msg));

	if(SendToTarget(CGPSDlg::m_inputMsg,msg_len,"") )
	{
		start = clock();
		while(1)
		{
			m_serial->GetBinary(buff, sizeof(buff));
			if (check_msg_valid(buff) == 0xd0)
			{
				if ( buff[5] == 0 )
					sprintf_s(binmsg, sizeof(binmsg), "Datalog Unlock.");
				else
					sprintf_s(binmsg, sizeof(binmsg), "Datalog Lock.");
				
				add_msgtolist(binmsg);
				break;
			}
			end=clock();	
			if(TIMEOUT_METHOD(start,end))
				break;		
		}
	}
	//CreateGPSThread();
#endif
}
/*
void CGPSDlg::OnSoarcommLogin()
{
#if SOARCOMM
	CLogin login;

	if(!CheckConnect())return;
	
	if ( login.DoModal() == IDOK)
	{
		CString password = login.password;

	//LPTSTR lpsz;
 //    lpsz = new TCHAR[Msg.GetLength()+1];
 //    _tcscpy(lpsz, Msg);
 //    char * psz;
 //    strcpy(psz,lpsz); 

		LPCTSTR lp_pass = password;
		char char_pass[9];
		memcpy(char_pass,lp_pass,8);
		char_pass[8] = 0;

		ClearQue();
		U08 message[17];
		U08 msg[10];
		U08 buff[256];
		char binmsg[100];
		msg[0] = 0x46; //msgid

		memcpy( &msg[1],char_pass,8);

		msg[9] = 0xa2 ;

		int msg_len = SetMessage(msg,sizeof(msg));

		if(SendToTarget(CGPSDlg::m_inputMsg,msg_len,"") )
		{
			start = clock();
			while(1)
			{
				m_serial->GetBinary(buff, sizeof(buff));
				if (check_msg_valid(buff) == 0xd1)
				{
					if ( buff[5] == 1 )
						sprintf_s(binmsg, sizeof(binmsg), "Password ok.");
					else
						sprintf_s(binmsg, sizeof(binmsg), "Password fail.");
					
					add_msgtolist(binmsg);
					UnlockSoarcomm();
					break;
				}
				end=clock();	
				if(TIMEOUT_METHOD(start,end))
					break;		
			}
		}
		

	}
	CreateGPSThread();
#endif
}

void CGPSDlg::OnSoarcommLogout()
{
	if(!CheckConnect())return;

	ClearQue();
	//U08 message[15];
	U08 msg[8];
	char binmsg[100];

	msg[0] = 0x47; //msgid
	char logoutpass[8] = "skytraq";
	memcpy(&msg[1],logoutpass,7);

	int msg_len = SetMessage(msg,sizeof(msg));

	if(SendToTarget(CGPSDlg::m_inputMsg,msg_len,"") )
	
		sprintf_s(binmsg, sizeof(binmsg), "system logout successful.");
	else
		sprintf_s(binmsg, sizeof(binmsg), "system logout fail.");
					
	add_msgtolist(binmsg);

	CreateGPSThread();
}

void CGPSDlg::OnSoarcommConfigpassword()
{
	if(!CheckConnect())return;

	CConfig_Password dlg_pass;

	if ( dlg_pass.DoModal() == IDOK)
	{
		ClearQue();
		//U08 message[24];
		U08 msg[17];
		//U08 buff[256];
		char binmsg[100];
		msg[0] = 0x48; //msgid
		memcpy(&msg[1],dlg_pass.old_password,8);
		memcpy(&msg[9],dlg_pass.new_password,8);

		int msg_len = SetMessage(msg,sizeof(msg));

		if(SendToTarget(CGPSDlg::m_inputMsg,msg_len,"") )
		{
			sprintf_s(binmsg, sizeof(binmsg), "Config Password ok.");
		}	
		else
		{
			sprintf_s(binmsg, sizeof(binmsg), "Config Password fail.");
		}
		add_msgtolist(binmsg);
	}

	CreateGPSThread();
}



void CGPSDlg::cartesian_to_geodetic( const POS_T* xyz_p, LLA_T* lla_p )
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

U08 CGPSDlg::com_get_register()
{
	U08 msg[5] ,checksum=0;
	U08 buff[100];
	int k1,k2;
	U32 data = 0;
	time_t start,end;
	msg[0]=0x71; //msgid
	msg[1]=0x80;
	msg[2]=0x00;
	msg[3]=0x00;
	msg[4]=0x90;
	int res = -99;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTargetNoAck(m_inputMsg,len))
	{
		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			k1=len+5;
			k2=len+6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xc0) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a) )
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					data = buff[5]<<24 | buff[6]<<16 |buff[7]<<8 | buff[8];
					if(data & 1<<8)
						res = 1;				//uart1
					else
						res = 0;				//uart0
					//sprintf_s(binmsg,"0x %x", data);	
					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD_QUICK(start,end))
			{
				break;
			}	
		}		
	}	
	return res;
}
*/
UINT reset_odometer_thread(LPVOID param)
{
	U08 msg[1] ;
	msg[0] = 0x7F;
	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Reset Odometer Successful...");

	return 0;	
}

void CGPSDlg::OnBinaryResetodometer()
{
	if(CheckConnect())	
	{
		::AfxBeginThread(reset_odometer_thread, 0);
	}
}

UINT query_1pps_timing_cable_thread(LPVOID param)
{
	U08 buff[100];
	U08 msg[1] ;
//	char tmp;
	time_t start,end;
	int res;
//	U08 interval;
	CString tmp_msg;
	int cable_delay;


	msg[0] = 0x46;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();

	if (CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Query 1PPS Cable Delay Successful..."))
	{

		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res) && BINARY_HD1==buff[0] && BINARY_HD2==buff[1])
			{
				if (Cal_Checksum(buff) == 0xBB)
				{
					cable_delay = buff[5]<<24 | buff[6]<<16 | buff[7]<<8 | buff[8];

					tmp_msg.Format("Cable delay: %0.2fns", cable_delay / 100.0);
					CGPSDlg::gpsDlg->add_msgtolist(tmp_msg);
					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD(start,end))
				break;
		}
	}

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}

void CGPSDlg::OnQuery1ppstimingQuerycabledelay()
{
	if(CheckConnect())	
	{
		AfxBeginThread(query_1pps_timing_cable_thread, 0);
	}
}

UINT LogReadBatchControlThread(LPVOID pParam)
{
	CGPSDlg::gpsDlg->LogReadBatchControl();
	return 0;
}

void CGPSDlg::OnDatalogLogReadBatch()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CString fileName("Data.log");	
	CFileDialog dlgFile(false, _T("log"), fileName, OFN_HIDEREADONLY, _T("ALL Files (*.*)|*.*||"), this);
	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();			

	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			datalogFilename = fileName;
			this->m_nDownloadBaudIdx = g_setting.boostBaudIndex;
			::AfxBeginThread(LogReadBatchControlThread, 0);
			//SetThreadPriority(pthread,THREAD_PRIORITY_HIGHEST);
		}		
		else
		{
			SetMode();
			CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
}

bool CGPSDlg::DataLogReadOffsetCtrl(int startId, int totalSector, int offset, U08 *buffer, long bufferSize, long *receiveBytes)
{
	WRL = new CWaitReadLog;
	AfxBeginThread(WaitLogRead, 0);	
	WaitForSingleObject(waitlog, INFINITE);

	memset(buffer, 0, bufferSize);
	for(int i=startId; i<totalSector; i+=offset)
	{
		int step = ((i+offset)>totalSector) ? (totalSector - i) : offset;
		int tryCount = 0;
		while(1)
		{
			++tryCount;
			if(tryCount > 60)
			{
				//OnBinaryCode.fire(9);
				return false;
			}			
			
			long tmp_count = 0;
			if(DatalogReadAll(i, step, &buffer[*receiveBytes], step * 4096, &tmp_count))
			{
				*receiveBytes += tmp_count;
				CString displayMessage;
				displayMessage.Format("Retrieve Log data #%d sector #%d bytes\n", i, *receiveBytes);
				//sprintf_s(tmp_buff, sizeof(tmp_buff), "Retrieve Log data #%d sector #%d bytes\n",i,*receiveBytes);
				WRL->msg.SetWindowText(displayMessage);
				//WRL->msg.SetWindowText("Log read cancel by user!");
				//OnReadProgress.fire(0,*receiveBytes);
				Sleep(150);
				break;
			}
			else
			{
				Utility::LogFatal(__FUNCTION__, "[DataLog] DatalogReadAll fail", __LINE__);
				Sleep(150);
			}
		}
	}
	VerifyDataLogFormat(buffer, receiveBytes);
	return true;
}

bool CGPSDlg::DatalogReadAll(int startId, int offset, U08 *datalog, long size, long *receiveBytes)
{
	U08 cmd[5] = {0x1d, startId >> 8, startId & 0xff, offset >> 8, offset & 0xff};
	U08 message[12] = {0};
	int len = SetMessage2(message, cmd, sizeof(cmd));
	Sleep(50);

	int i = 0;
	for(; i<30; ++i)
	{
		if(SendToTarget(message, len, "", 1))
		{
			break;
		}
		Utility::Log(__FUNCTION__, "[DataLog] SendToTarget retry :", i);
	}

	if(i==30)
	{
		Utility::LogFatal(__FUNCTION__, "[DataLog] SendToTarget fail", __LINE__);
		return false;
	}

	U08 ptr_tmp = 0;
	bool isFinish = false;
//	long show_c = 0;
	DWORD res_c = 0;
	*receiveBytes = 0;
	U08 res_buff[0x1000] = {0};
	while(1)
	{		
		U08 buff[0x2000] = {0};
		//Read 8k one time.
		DWORD readBytes = m_serial->ReadData(buff, sizeof(buff), true);
		if(readBytes <= 0 || readBytes > sizeof(buff))
		{
			continue;
		}
		if(*receiveBytes > 8000)
		{
			int a = 0;
		}
		bool isEnd = VerifyDataLogBuffer(buff, datalog, &ptr_tmp, readBytes, receiveBytes);
		if(isEnd)
		{
			if(ptr_tmp > 0)
			{
				memcpy(res_buff, buff, ptr_tmp);
			}
			isFinish = true;
			break;
		}

		if(*receiveBytes > size + 10)
		{
			Utility::LogFatal(__FUNCTION__, "[DataLog] receiveBytes fail", __LINE__);
			isFinish = false;
			break;
		}

		if(*receiveBytes + readBytes > 8000)
		{
			int a = 0;
		}
		memcpy(&datalog[*receiveBytes], buff, readBytes);

		*receiveBytes += readBytes;
//		show_c += readBytes;
	}

	if(!isFinish)
	{
		Utility::LogFatal(__FUNCTION__, "[DataLog] isFinish fail", __LINE__);
		return false;
	}

	res_c = ptr_tmp;
	int count = 0;
	bool chk_sum_right = false;

	while(1)
	{
		if (res_c > 0)
		{
			if(!strncmp((char*)res_buff,"CHECKSUM=",9) && res_c >= 12)
			{
				U08 chk = (U08)res_buff[9];

				U16 id;
				if ( res_buff[10] == 10 && res_buff[11] == 13 )
					id = startId ;	
				else
					id = (U08)res_buff[11]<<8|(U08)res_buff[10];

				U08 chk_sum = 0;
				for(int i=0; i <*receiveBytes; i++)
					chk_sum = datalog[i]^chk_sum;
				TRACE("chk=%d,chk_sum=%d,*receiveBytes=%d\n", chk, chk_sum, *receiveBytes);
				if(chk == chk_sum && id == startId)
				{
					//OnReadProgress.fire(startId,*receiveBytes);
					return true;
				}
				Utility::LogFatal(__FUNCTION__, "[DataLog] chk_sum fail", __LINE__);
				return false;
			}
			Utility::LogFatal(__FUNCTION__, "[DataLog] CHECKSUM fail", __LINE__);
		}
		U08 buff[1024] = {0};
		DWORD readBytes = m_serial->ReadData(&res_buff[res_c], sizeof(buff), true);
		res_c += readBytes;
		count++;
		if (count > 10000)
		{
			Utility::LogFatal(__FUNCTION__, "[DataLog] count fail", __LINE__);
			return false;
		}
	} //while(1)
	return false;
}

BOOL CGPSDlg::OpenDataLogFile(UINT nOpenFlags)
{
	return dataLogFile.Open(datalogFilename, nOpenFlags);
}

void CGPSDlg::LogReadBatchControl()
{
	if(!dataLogFile.Open(datalogFilename, CFile::modeReadWrite | CFile::modeCreate))
	{
		return;
	}

	U16 endPos, totalBytes;
	if(!QueryDataLogBoundary(&endPos, &totalBytes))
	{
		dataLogFile.Close();
		Utility::LogFatal(__FUNCTION__, "[DataLog] QueryDataLogBoundary fail", __LINE__);
		return;
	}

	BoostBaudrate(FALSE);
	long bufferSize = (endPos + 1) * 0x1000;
	U08* buffer = (U08*)malloc(bufferSize);
	long receiveBytes = 0;
	if(DataLogReadOffsetCtrl(0, endPos, 2, buffer, bufferSize, &receiveBytes))
	{
		dataLogFile.Write(buffer, receiveBytes);
		WRL->msg.SetWindowText("Log read is completed!");
		CGPSDlg::gpsDlg->add_msgtolist("Log Read Successful...");
	}
	else
	{
		WRL->msg.SetWindowText("Log read is failed!");
		CGPSDlg::gpsDlg->add_msgtolist("Log Read failure...");
	}
	//	Flog.Write(datalog,receiveBytesount);
	WRL->IsFinish = true;

	free(buffer);
	
	CGPSDlg::gpsDlg->target_only_restart(0);
	CGPSDlg::gpsDlg->m_serial->ResetPort(CGPSDlg::gpsDlg->GetBaudrate());
	CGPSDlg::gpsDlg->m_BaudRateCombo.SetCurSel(CGPSDlg::gpsDlg->GetBaudrate());
	dataLogFile.Close();
}

DWORD ReadDataLogSector(CSerial* serial, void* buffer, DWORD bufferSize)
{
	DWORD totalSize = 0;
	char* bufferIter = (char*)buffer;
	DWORD nBytesRead = 0;
	do
	{ 
		nBytesRead = serial->ReadData(bufferIter, 1);
		if(nBytesRead <= 0)
		{
			return totalSize;
		}

		if((*bufferIter == 0x0a) && (*(bufferIter - 1) == 0x0d))
		{	//When read 0d, 0a, check pack size to make sure until tail.
			DWORD len = *(bufferIter - totalSize + 3);
			if(totalSize == len + 6)
			{
				break;
			}
		}
		bufferIter += nBytesRead;
		totalSize += nBytesRead;
	}
	while(totalSize < bufferSize);
	return totalSize;	
}

/*
//Datalog read one sector
int CGPSDlg::Datalog_read_one(U08* message,int sector_id,int sector_count)
{
	bool receive_error = false ;
	char write_buff[8192];
	int ptr_buff = 0;
	unsigned char buff[100];
	char BINMSG[1024] = {0};
	ClearQue();
	if(SendToTarget(message,9,""))
	{
		//Sleep(100);
		int i=0;
		//U08 temp[100];
		while(1)
		{		
			i++;
			sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d entry",sector_id,i);
			WRL->msg.SetWindowText(BINMSG);
			
			ReadDataLogSector(m_serial, buff, sizeof(buff));

			//m_serial->getdatalog(buff,sizeof(buff));

			if((buff[8] ==0xff && buff[9] ==0xff && buff[10] ==0xff) || buff[11] ==0xff && buff[12] ==0xff && buff[13] ==0xff)
			{
				break;
			}

			if( Cal_Checksum(buff) == BINMSG_LOG_READ_DATA )
			{
				int len = buff[3]-1;
				memcpy(&write_buff[ptr_buff],&buff[5],len);
				ptr_buff += len;
			}
			else
			{
				if(chk_gp((char*)buff,sizeof(buff)))
					break;
				else
				{
					TRACE(BINMSG,"Retrieve Log data #%d sector #%d entry\n",sector_id,i);
					TRACE("%s\n",buff);
					receive_error = true ;
				}
			}
			
		}
		if(!receive_error)
			dataLogFile.Write(write_buff,ptr_buff);
		return receive_error;
	}
	else
	{
		return 9; 
	}
	
}

int CGPSDlg::Datalog_read_one_srec(U08* message,int sector_id,int sector_count)
{
	char buff[256];
	U08 ptr_tmp =0;
	char res_buff[0x1000];
	int res_c = 0;
	char BINMSG[1024] = {0};

	U08 datalog[0x2000];
	long receive_count =0;
	bool is_finish = false;
	long show_c = 0;

	ClearQue();
#ifdef DEBUG_TEST
	sprintf_s(BINMSG, sizeof(BINMSG), "SendToTarget_datalog id = %d",sector_id);
	WRL->msg.SetWindowText(BINMSG);	
#endif
	int res_target = SendToTarget_datalog(message,9,"");
	if(res_target == 0)
	{


		while(1)
		{		

			int res = m_serial->ReadData(buff, sizeof(buff), true);
			if (res > 0)
			{
				//TRACE("%s\n",buff);
				//TRACE("%d\n",res);
				bool isend = false;
				for(U08 i = 0 ; i < res -2 ; i++)
				{
					if (buff[i] == 'E' && buff[i+1] == 'N' && buff[i+2] == 'D' && buff[i+3] == '\0')
					{
						//WRL->msg.SetWindowText("Enter case 1");	
						isend = true;
						memcpy(&datalog[receive_count],buff,i);
						receive_count+= i ;
						if ( res > i+3)
						{
							memcpy(buff,&buff[i+4],res-(i+4));
							ptr_tmp = res-(i+4);
						}
						sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d bytes\n",sector_id,receive_count);
						WRL->msg.SetWindowText(BINMSG);	
						break;
					}
					else if( receive_count >= 1 && datalog[receive_count-1] == 'E' && buff[i] == 'N' && buff[i+1] == 'D' && buff[i+2] == '\0')
					{
						//WRL->msg.SetWindowText("Enter case 2");	
						//TRACE("Enter case 1\n");
						isend = true;
						receive_count -= 1 ;
						if ( res > i+3)
						{
							memcpy(buff,&buff[i+3],res-(i+3));
							ptr_tmp = res-(i+3);
						}
						sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d bytes\n",sector_id,receive_count);
						WRL->msg.SetWindowText(BINMSG);	
						break;					
					}
					else if ( receive_count >= 2 && datalog[receive_count-2] == 'E' && datalog[receive_count-1] == 'N' && buff[i] == 'D' && buff[i+1] == '\0')
					{
						//WRL->msg.SetWindowText("Enter case 3");	
						//TRACE("Enter case 2\n");
						isend = true;
						receive_count -= 2 ;
						if ( res > i+2)
						{
							memcpy(buff,&buff[i+2],res-(i+2));
							ptr_tmp = res-(i+2);
						}
						sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d bytes\n",sector_id,receive_count);
						WRL->msg.SetWindowText(BINMSG);	
						break;					
					}
					else if ( receive_count >= 3 && datalog[receive_count-3] == 'E' && datalog[receive_count-2] == 'N' && datalog[receive_count-1] == 'D' && buff[i] == '\0')
					{
						//WRL->msg.SetWindowText("Enter case 4");	
						//TRACE("Enter case 3\n");
						isend = true;
						receive_count -= 3 ;
						if ( res > i+1)
						{
							memcpy(buff,&buff[i+1],res-(i+1));
							ptr_tmp = res-(i+1);
						}
						sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d bytes\n",sector_id,receive_count);
						WRL->msg.SetWindowText(BINMSG);	
						break;					
					}
				}
				if (isend)
				{
					if (ptr_tmp>0)
					{
						memcpy(res_buff,buff,ptr_tmp);
					}
					is_finish = true;
					break;
				}
				if ( receive_count > 4100 )
				{
					is_finish = false;
					break;
				}

				memcpy(&datalog[receive_count],buff,res);
				receive_count+= res ;

				show_c += res;
				const int MaxLength = 300;
				if ( show_c > MaxLength )
				{
					sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Log data #%d sector #%d bytes\n",sector_id,receive_count);
					WRL->msg.SetWindowText(BINMSG);	
					show_c -= MaxLength ;
				}

//				sprintf_s(BINMSG,"Retrieve Log data #%d sector #%d bytes",sector_id,receive_count);
//				WRL->msg.SetWindowText(BINMSG);
			}

		}
		if (is_finish)
		{
			res_c = ptr_tmp;
			int count = 0;
			bool chk_sum_right = false;

			while(1)
			{
#ifdef DEBUG_TEST
				sprintf_s(BINMSG, sizeof(BINMSG), "Wait Checksum algorithm(%d), ptr_tmp = %d",count,ptr_tmp);
				WRL->msg.SetWindowText(BINMSG);	
#endif
				if (res_c > 0)
				{
					if(!strncmp(res_buff,"CHECKSUM=",9) && res_c >= 12)
					{
						U08 chk = (U08)res_buff[9];
						U08 id,dst_chk_sum = 0;
						if ( res_buff[10] == 10 && res_buff[11] == 13 )
						{
							//old
							id = sector_id ;	
						}
						else
						{
							id = (U08)res_buff[10];
							dst_chk_sum = (U08)res_buff[11];
							
						}
						U08 chk_sum = 0;
						for(int i = 0 ; i < receive_count ;i++)
							chk_sum = datalog[i]^chk_sum;
						if (chk == chk_sum && id == sector_id)
						{
							//chk_sum_right = true;
							dataLogFile.Write(datalog,receive_count);
							return 0;
						}
						else
						{
							sprintf_s(BINMSG, sizeof(BINMSG), "error srcid = %d, dstid = %d\n", sector_id , id);
							sprintf_s(BINMSG, sizeof(BINMSG), "error srcsum = %d, dstsum = %d\n", message[6] & 0x7f , dst_chk_sum);
						}
						break;
					}
				}

				int res = m_serial->ReadData(&res_buff[res_c], sizeof(buff), true);
				res_c+=res;
				count++;
				if (count > 5)
				{
					sprintf_s(BINMSG, sizeof(BINMSG), "No Recevie Checksum sectorid = %d\n", sector_id);
					break;
				}
			}
		}
		return 1;
	}
	else if (res_target == 1 )
	{
		return 8; 
	}
	else
	{
		return 9;
	}
}
*/
/*
void CGPSDlg::OnDatalogLogreadsrec()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode  = 0;
	CString fileName("Data.log");	
	CFileDialog dlgFile(false, _T("log"), fileName, OFN_HIDEREADONLY, _T("ALL Files (*.*)|*.*||"), this);
  	INT_PTR nResult = dlgFile.DoModal();
	fileName = dlgFile.GetPathName();			

	CFileException ef;
	try
	{
		if(nResult == IDOK)
		{  				
			datalogFilename = fileName;
		    //AfxBeginThread(LogReadSrecControlThread,0,THREAD_PRIORITY_TIME_CRITICAL);	
			//AfxBeginThread(LogReadSrecControlThread,0);	
			HANDLE pthread = (HANDLE)_beginthread(LogReadSrecControlThread,0,this);

			//HANDLE pthread = (HANDLE)_beginthread(LogReadBatchControlThread,0,this);
			SetThreadPriority(pthread,THREAD_PRIORITY_HIGHEST);
		}		
		else
		{
			SetMode();
	        CreateGPSThread();
		}
	}
	catch(CFileException *fe)
	{
		fe->ReportError();
		fe->Delete();
		return;
	}	
	fileName.ReleaseBuffer();
}
*/
void CGPSDlg::VerifyDataLogFormat(U08 *datalog, long *size)
{
	long count = 0;
	U08* buff = new U08[*size];
	for(long i=0; i<*size; i+=0x1000)
	{
		U08 *bufferIter = &datalog[i];
		long tmp_count = 0;
		while(1)
		{
			//type = bufferIter[0] & 0xFC;
			U08 type = bufferIter[0] & 0xE0;
			if(type == 0x40 || type == 0x60)
			{
				bufferIter += 18;
				tmp_count += 18;
			}
			else if(type == 0x80 )
			{
				bufferIter += 8;
				tmp_count += 8;
			}
			else if(type == 0x20 || type == 0xC0)
			{
				bufferIter += 20;
				tmp_count += 20;
			}
			else
			{
				break;
			}
		}
		memcpy(&buff[count], &datalog[i], tmp_count);
		count += tmp_count;
	}
	memcpy(datalog, buff, count);
	*size = count;
	delete [] buff;
}


bool CGPSDlg::VerifyDataLogBuffer(U08 *buff, U08 *datalog, U08 *ptr_last, int size, long *receive_count)
{
	bool isEnd = false;
	//if ( size < 2 ) return false;
	for(int i=0; i<size; ++i)
	{
		if(i < size - 3 && 
			buff[i] == 'E' && buff[i+1] == 'N' && buff[i+2] == 'D' && buff[i+3] == '\0')
		{
			isEnd = true;
			memcpy(&datalog[*receive_count], buff, i);
			*receive_count += i;
			if(size > i + 3)
			{
				memcpy(buff, &buff[i+4], size - (i+4));
				*ptr_last = size - (i + 4);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 1", __LINE__);
			break;
		}
		else if(i < size - 2 && *receive_count >= 1 && 
			datalog[*receive_count-1] == 'E' && buff[i] == 'N' && buff[i+1] == 'D' && buff[i+2] == '\0')
		{
			isEnd = true;
			*receive_count -= 1;
			if(size > i + 3)
			{
				memcpy(buff, &buff[i+3], size - (i + 3));
				*ptr_last = size - (i + 3);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 2", __LINE__);
			break;					
		}
		else if(i < size - 1 && *receive_count >= 2 && 
			datalog[*receive_count-2] == 'E' && datalog[*receive_count-1] == 'N' && buff[i] == 'D' && buff[i+1] == '\0')
		{
			isEnd = true;
			*receive_count -= 2;
			if(size > i + 2)
			{
				memcpy(buff, &buff[i+2], size - (i + 2));
				*ptr_last = size - (i + 2);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 3", __LINE__);
			break;					
		}
		else if(i < size && *receive_count >= 3 && 
			datalog[*receive_count-3] == 'E' && datalog[*receive_count-2] == 'N' && datalog[*receive_count-1] == 'D' && buff[i] == '\0')
		{
			isEnd = true;
			*receive_count -= 3;
			if(size > i + 1)
			{
				memcpy(buff, &buff[i+1], size - (i + 1));
				*ptr_last = size - (i + 1);
			}
			Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer type 4", __LINE__);
			break;					
		}
	}

	//Utility::Log(__FUNCTION__, "[DataLog] VerifyDataLogBuffer isEnd :", (int)isEnd);
	return isEnd;
}

U32 CGPSDlg::GetClockOffsetByRegister()
{
	U08 msg[5] ,checksum=0;
	CString temp;
	U32 data = 0;
	msg[0]=0x71; //msgid
	msg[1]=0x00;
	msg[2]=0x00;
	msg[3]=0x00;
	msg[4]=0x01;

	int len = SetMessage(msg,sizeof(msg));

	time_t start,end;
	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg,len,"",1))
	{
		start = clock();
		while(1)
		{
			U08 buff[1024] = {0};
			m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			int k1 = len + 5;
			int k2 = len + 6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xc0) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a) )
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					data = buff[5]<<24 | buff[6]<<16 |buff[7]<<8 | buff[8];
					break;
				}
			}
			end=clock();	
			if(TIMEOUT_METHOD_QUICK(start,end))
			{
				return data;
			}
		}		
	}	
	return data;
}

U08 CGPSDlg::MinihomerQuerytag()
{
	U08 msg[1] ,checksum=0;
	CString temp;
	U32 data = 0;
	U08 buff[100];
	int k1,k2;
	time_t start,end;

	msg[0]=0x7D; //msgid

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Query miniHomer tag Successful.",1))
	{
		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));			
			len = buff[2]<<8|buff[3];		
			k1=len+5;
			k2=len+6;
			if((buff[0]==0xa0) && (buff[1]==0xa1) && (buff[4]==0xD1) && (buff[k2-1]==0x0d)&&(buff[k2]==0x0a) )
			{
				for(int i=0;i<(int) buff[3];i++)
					checksum^=buff[i+4];			
				if(checksum == buff[k2-2])
				{
					U08 size = buff[5];
					
					if (buff[5] == 0xFF)
					{
						CGPSDlg::gpsDlg->add_msgtolist("No Tag");
					}else
					{
						temp.Append("Tag = ");
						for (int i=0;i<size;i++)
						{
							temp.AppendFormat("0x%02X ",buff[6+i]);
						}
						CGPSDlg::gpsDlg->add_msgtolist(temp);
					}

					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD_QUICK(start,end))
				break;	
		}		
	}else
		CGPSDlg::gpsDlg->add_msgtolist("Query DR Info Fail.");

	CGPSDlg::gpsDlg->SetMode();
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

UINT MinihomerQuerytagThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->MinihomerQuerytag();
	return TRUE;	
}

void CGPSDlg::OnMinihomerQuerytag()
{
	if(!CheckConnect())return;

	AfxBeginThread(MinihomerQuerytagThread, 0);	

}

UINT ActivateminiHomerThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->activate_minihomer();
	return TRUE;	
}

void CGPSDlg::set_minihomerid(U08* id,int id_len)
{
	U08 msg[11] ,checksum=0;
	CString temp;
	U32 data = 0;
//	U08 buff[100];
//	int k1,k2;
//	time_t start,end;

	msg[0]=0x74; // set device_id;
	memcpy(&msg[1],id,id_len);

	int len = SetMessage(msg,sizeof(msg));

	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg,len,"Set miniHomer Device ID Successful.",1) != 1)
		add_msgtolist("Set miniHomer Device ID Fail.");	
}

void CGPSDlg::set_minihomerkey(U08* key,int key_len)
{
	U08 msg[65] ,checksum=0;
	CString temp;
	U32 data = 0;
//	U08 buff[100];
//	int k1,k2;
//	time_t start,end;

	msg[0]=0x75; // set device_id;
	memcpy(&msg[1],key,key_len);

	int len = SetMessage(msg,sizeof(msg));

	ClearQue();
	if(SendToTarget(CGPSDlg::m_inputMsg,len,"Set miniHomer Device Key Successful.",1) != 1)
		add_msgtolist("Set miniHomer Device Key Fail.");	
}

void CGPSDlg::activate_minihomer()
{
	CRedirector m_redir;
	char cmd_path[1024];
	GetCurrentDirectory(1024,cmd_path);
	strcat_s(cmd_path, sizeof(cmd_path), "\\Create_miniHomer_Activate_Code.exe -c -s");
//	NMEA nmea;

	m_redir.Close();
	m_redir.Open(cmd_path);

	m_redir.Wait();

	TRACE("%s",m_redir.std_output);
	m_redir.Close();

	U08 id[10];
	U08 key[64];

	CString retval = m_redir.std_output;
		
	int start = retval.Find("id=")+3;
	for (int i=0;i<10;i++)
	{
		//id[i] = Utility::MSB(retval[start+i*2]) + Utility::LSB(retval[start+i*2+1]);
		id[i] = Utility::GetOctValue(retval[start+i*2], retval[start+i*2+1]);
	}

	start = retval.Find("signature=")+10;
	for (int i=0;i<64;i++)
	{
		//key[i] = Utility::MSB(retval[start+i*2]) + Utility::LSB(retval[start+i*2+1]);
		key[i] = Utility::GetOctValue(retval[start+i*2], retval[start+i*2+1]);
	}
	
	set_minihomerid(id,sizeof(id));
	set_minihomerkey(key,sizeof(key));

	SetMode();
	CreateGPSThread();
	return ;	
}

void CGPSDlg::OnMinihomerActivate()
{
	if(!CheckConnect())return;

	AfxBeginThread(ActivateminiHomerThread, 0);	
}

void CGPSDlg::OnBnClickedSetorigin()
{
	g_scatterData.SetOrigin();
}

//parsing the gps message
void CGPSDlg::MSG_PROC()
{	
	int length = 0;	
	static int message_mode =0;
	ClearQue();
	m_lastNmeaToken = MSG_Unknown;
	while(m_isConnectOn)
	{	
		U08 buffer[1024] = {0};
		switch(m_inputMode)
		{
		case NMEA_Mode:				
			length = m_serial->GetBinary(buffer, sizeof(buffer));			
			if(!m_isConnectOn)
			{
				break;
			}
			if(ReadOK(length))
			{
				if(SetFirstDataIn(true))
				{
					NmeaOutput((LPCSTR)buffer, length);
					continue;
				}
				if(buffer[0] == 0xA0 && buffer[1] == 0xA1)
				{				
					if(length > 4 && 0x83==buffer[4])	//Query ACK
					{
						if(m_bShowBinaryCmdData)
						{
							add_msgtolist("Ack : " + theApp.GetHexString(buffer, length));	
						}	
						continue;
					}
					if(length > 4 && 0x84==buffer[4])	//Query NACK
					{
						if(m_bShowBinaryCmdData)
						{
							add_msgtolist("Ack : " + theApp.GetHexString(buffer, length));	
						}
						continue;
					}
					if(length >= 21 && 0x80==buffer[4])	//Query Version Return
					{
						if(m_bShowBinaryCmdData)
						{
							add_msgtolist("Return : " + theApp.GetHexString(buffer, length));	
						}
						m_versionInfo.ReadFromMemory(buffer, length);
						continue;
					}
					if(length >= 11 && 0x64==buffer[4] && 0x80==buffer[5])	//Query Version Return
					{
						if(m_bShowBinaryCmdData)
						{
							add_msgtolist("Return : " + theApp.GetHexString(buffer, length));	
						}
						m_bootInfo.ReadFromMemory(buffer, length);
						continue;
					}
					if (message_mode==0)
					{
					//	m_nmeaCount = 0;
					//	m_nmeaList.DeleteAllItems();
						m_nmeaList.ClearAllText();
					} 
					message_mode = 1;
					
					BinaryProc(buffer,length+1);
					Copy_NMEA_Memery();		    
					//_GETNMEA0183CS.Unlock();
					SetNmeaWriting(true);
				}
				else
				{
					if (message_mode==1)
					{
						//m_nmeaCount = 0;
						//m_nmeaList.DeleteAllItems();
						m_nmeaList.ClearAllText();
					}
					if(!m_isConnectOn)
					{
						break;
					}
					if (NmeaProc((const char*)buffer,length))
					{		
						Copy_NMEA_Memery();		    
						//_GETNMEA0183CS.Unlock();
						SetNmeaWriting(true);
					}
					message_mode = 0;
				}
			}
			else
			{
				if(!m_isConnectOn)
				{
					break;
				}
				Sleep(10);
			}
		    break;
		case Binary_Mode:
			length = m_serial->GetBinary(buffer, sizeof(buffer));
			if(ReadOK(length))
			{
				BinaryProc(buffer,length);						
			}
			break;
		default:
			break;
		}
	}
	m_isConnectOn = false;	
}

bool CGPSDlg::QueryDataLogBoundary(U16 *end, U16 *total)
{
	int nackTimes = 0;

	U08 cmd[1] = {0x17};	//query logstaus
	U08 message[8] = {0};
	int len = SetMessage2(message, cmd, sizeof(cmd));

	for(int i=0; i<30; ++i)
	{
		if(!SendToTarget(message, len, "", true))
		{
			Utility::LogFatal(__FUNCTION__, "[DataLog] SendToTarget fail i = ", i);
			continue;
		}

		while(1)
		{
			U08 buff[512] = {0};
			m_serial->GetBinary(buff, sizeof(buff));
			if(Cal_Checksum(buff) != BINMSG_REPLY_LOG_STATUS)
			{
				Utility::LogFatal(__FUNCTION__, "[DataLog] Cal_Checksum error", __LINE__);
				continue;
			}

			U16 left;
			memcpy(&left, &buff[9], sizeof(U16));
			memcpy(total, &buff[11], sizeof(U16));
			if(left == 0)
			{
				*end = *total - left;
			}
			else
			{
				*end = *total - left + 1;
			}
			return true;
		}
		Utility::LogFatal(__FUNCTION__, "[DataLog] impossible fail i = ", i);
		break;
	} //for(int i=0; i<30; ++i)
	Utility::LogFatal(__FUNCTION__, "[DataLog] return false", __LINE__);
	return false;
}
/*
UINT query_multi_mode(LPVOID param)
{

	U08 buff[100];
//	char msg_buf[100];
//	char tmp_buff[100];;
	U08 msg[1] = {0};
//	char tmp;
	time_t start,end;
	int res;

	msg[0] = 0x3D;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	CGPSDlg::gpsDlg->ClearQue();

	if (CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Query Navigation Mode Successful..."))
	{

		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res))
			{
				if (Cal_Checksum(buff) == 0xB5)
				{
					if(CGPSDlg::gpsDlg->GetShowBinaryCmdData())
					{
						CGPSDlg::gpsDlg->add_msgtolist("Return: " + theApp.GetHexString(buff,  buff[2] <<8 | buff[3] + 7));	
					}
#ifdef LG
					switch(buff[5])
					{
					case 0:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Car");
						break;
					case 1:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Pedestrain");
						break;
					case 2:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Stationary");
						break;
					}

#else
					switch(buff[5])
					{
					case 0:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Car");
						break;
					case 1:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Pedestrain");
						break;
					case 2:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Bike");
						break;
					case 3:
						CGPSDlg::gpsDlg->add_msgtolist("Navigation Mode: Boat");
						break;
					}
#endif
					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD(start,end))
				break;
		}
	}

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}
*/
void CGPSDlg::GetAlmanac_tmp()
{
	int wait = 0;
	U08 msg[2] ;
	char BINMSG[1024] = {0};

	msg[0] = 0x7f;
	msg[1] = m_almanac_no;
	int res_len;
	int len = SetMessage(msg,sizeof(msg));
	FILE *f;

	ClearQue();
	if (SendToTarget(CGPSDlg::m_inputMsg,len,"Get Almance start...") == 1)
	{
		if(WRL == NULL )
		{
			WRL = new CWaitReadLog;
		}
		AfxBeginThread(WaitLogRead, 0);
		WaitForSingleObject(waitlog,INFINITE);
		WRL->SetWindowText("Wait for get almanac");
		WRL->msg.SetWindowText("Please wait for get almanac!");

		U08 NumsOfEphemeris = 0;

		fopen_s(&f, m_almanac_filename, "wb+");
		fclose(f);

		while(1)
		{	
			wait++;
			if(wait == 50 )
			{ 
				WRL->msg.SetWindowText("Retrieve almanac data is Failed!");
				Sleep(500);
				WRL->IsFinish = true;
				Sleep(500);
				add_msgtolist("Retrieve almanac Failed...");	
				//goto TheLast;
			}
			U08 buff[1024] = {0};
			res_len = m_serial->GetBinary(buff, sizeof(buff));

			if(res_len==0x3C)
			{
				fopen_s(&f, m_almanac_filename, "ab");
				fwrite(&buff[8],1,res_len-10,f);
				fclose(f);
			}else
				break;

			NumsOfEphemeris = buff[5];
			//	}
			sprintf_s(BINMSG, sizeof(BINMSG), "Retrieve Satellite ID # %d almanac",NumsOfEphemeris);
			WRL->msg.SetWindowText(BINMSG);

			if(NumsOfEphemeris==32)break;
			//}
		}	
		Sleep(500);
		WRL->msg.SetWindowText("Retrieve almanac data is completed!");
		Sleep(500);
		WRL->IsFinish = true;
		add_msgtolist("Retrieve almanac Successful...");	
		//WRL = NULL;
	}
	SetMode();  
	CreateGPSThread();

	//	return TRUE;	
}
/*
U08 CGPSDlg::query_clock_offset(S32 *clock_offset)
{
	U08 buff[100];
	U08 msg[1] ;
	CString tmp;
	time_t start,end;
	int res;

	msg[0] = 0x79;

	int len = SetMessage(msg,sizeof(msg));

	//WaitEvent();
	ClearQue();

	if (SendToTarget(CGPSDlg::m_inputMsg,len,""))
	{

		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			res = m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res))
			{
				if (Cal_Checksum(buff) == 0xFD)
				{
					*clock_offset = buff[5]<<24 | buff[6]<<16 | buff[7]<<8 | buff[8];
					return 1;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD(start,end))
				return 9;
		}
	}

	return FALSE;	

}
*/
/*
void CGPSDlg::OnMultimodeQuerymode()
{
	if(CheckConnect())	
	{
		AfxBeginThread(query_multi_mode, 0);
	}
}
*/
U16 m_power_mode;
U16 m_power_attribute;
UINT ConfigurePowerMode(LPVOID param)
{
	U08 msg[3] ;
	msg[0] = 0xC ;
	msg[1] = (U08)m_power_mode;
	msg[2] = (U08)m_power_attribute;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure PowerSave Successful...");
	return 0;	
}

void CGPSDlg::OnBinaryConfigurepowermode()
{
	if(CheckConnect())	
	{
		CConPowerMode dia;

		if (dia.DoModal()== IDOK)
		{
			m_power_mode = dia.mode;
			m_power_attribute = dia.attribute;

			AfxBeginThread(ConfigurePowerMode, 0);
		}
		else
		{
			CGPSDlg::gpsDlg->SetMode();  
			CGPSDlg::gpsDlg->CreateGPSThread();
		}
	}	
}

void CGPSDlg::Rad2Deg(double& degree)
{
	degree*=(180/PI); 
}

void CGPSDlg::WriteKMLPath(CFile& Fnmea,double lon,double lat)
{
	string temp;	
	char CoordinateToString[50];
	char t[]="      ";
	Fnmea.Write(t,sizeof(t)-1);
	sprintf_s(CoordinateToString,"%f",lon);
	temp=CoordinateToString;
	Fnmea.Write(CoordinateToString,(U16)temp.length());
	char title[]=",";
	Fnmea.Write(title,sizeof(title)-1);
	sprintf_s(CoordinateToString,"%f",lat);
	temp=CoordinateToString;
	Fnmea.Write(CoordinateToString,(U16)temp.length());
	char title1[]=",2\r\n";
	Fnmea.Write(title1,sizeof(title1)-1);
}

void CGPSDlg::WriteKMLini(CFile& Fnmea,double lon,double lat)
{
	CString temp;

	temp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<kml xmlns=\"http://earth.google.com/kml/2.1\">\r\n";
	Fnmea.Write(temp, temp.GetLength());
	
	temp = "<Style id=\"POI_STYLE\">\r\n    <IconStyle>\r\n    <color>ff00ff00</color>\r\n    <scale>1.1</scale>\r\n";
	Fnmea.Write(temp, temp.GetLength());

	temp = "    <Icon><href>http://maps.google.com/mapfiles/kml/pal3/icon21.png</href></Icon>\r\n    </IconStyle>\r\n</Style>\r\n";
	Fnmea.Write(temp, temp.GetLength());

	temp = "<Folder id=\"Data logger\">\r\n    <name>Data logger</name>\r\n";
	Fnmea.Write(temp, temp.GetLength());

	temp = "<Placemark id=\"logger\">\r\n  <styleUrl>#lineStyle</styleUrl>\r\n   <description>Plot Your Traveling Path</description>\r\n  <name>Trajectory</name>\r\n";
	Fnmea.Write(temp, temp.GetLength());

	temp = "    <visibility>1</visibility>\r\n  <open>0</open>\r\n    <Style>\r\n    <LineStyle>\r\n      <color>ff0000ff</color>\r\n    </LineStyle>\r\n    </Style>\r\n    <LineString>\r\n    <extrude>1</extrude>\r\n    <tessellate>1</tessellate>\r\n    <coordinates>\r\n";
	Fnmea.Write(temp, temp.GetLength());
}

BOOL CGPSDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tip.RelayEvent(pMsg);	

	if(pMsg->message==WM_KEYDOWN && 
		(pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN))
	{
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}	


inline const char *go_next_dot(const char *buff)
{
	while(1)
	{
		if (*buff == 0 || *buff=='*') return NULL;
		if (*buff == ',') return buff+1;
		buff++;
	}
}

void CGPSDlg::parse_sti_20_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;
	CString temp;

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	temp.Format("%d",atoi(ptr));
	m_odo_meter.SetWindowText(temp);

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	temp.Format("%d",atoi(ptr));
	m_backward_indicator.SetWindowText(temp);

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	temp.Format("%.2f",atof(ptr));
	m_gyro_data.SetWindowText(temp);
	
}

void CGPSDlg::parse_sti_03_message(const char *buff,int len) // for timing module
{
	const char *ptr = buff;

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;

	CString temp;
	temp.Format("%d",atoi(ptr));
	m_odo_meter.SetWindowText(temp);
}

void CGPSDlg::parse_sti_message(const char *buff,int len)
{
	//	int mode,survery_len;
	//	float error;
	const char *ptr = buff;
	int psti_id;

	ptr = go_next_dot(ptr);
	if (ptr == NULL) 
		return;

	psti_id = atoi(ptr);
	if(psti_id == 50)  // glonass k-number
	{
		parse_psti_50(ptr);
	} 
	else if(psti_id == 0)
	{
		parse_sti_0_message(buff,len);
	}
	else if (psti_id == 20)		// for dr
	{
		parse_sti_20_message(buff,len);
	}
	else if (psti_id == 3)		// for jamming interference
	{
		parse_sti_03_message(buff,len);
	}

}

#if (LG || TIMING_MODE)
void CGPSDlg::parse_sti_0_message(const char *buff,int len) // for timing module
{
	int mode,survery_len;
	float error;
	int set_std,now_std;
//	int psti_id;
	const char *ptr = buff;


	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	mode = atoi(ptr);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	survery_len = atoi(ptr);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	error = (float)atof(ptr);

	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	set_std = atoi(ptr);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	now_std = atoi(ptr);

	if(dia_monitor_1pps != NULL)
		dia_monitor_1pps->Show1PPSTiming(mode,survery_len,error,set_std,now_std);
}
#else
void CGPSDlg::parse_sti_0_message(const char *buff,int len) // for timing module
{
	int mode,survery_len;
	float error;
//	int psti_id;
	const char *ptr = buff;


	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	mode = atoi(ptr);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	survery_len = atoi(ptr);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	error = (float)atof(ptr);

	if(dia_monitor_1pps != NULL)
		dia_monitor_1pps->Show1PPSTiming(mode,survery_len,error);
}
#endif

void CGPSDlg::parse_rtoem_message(const char *buff,int len)
{
//	int mode,survery_len;
//	float error;
	const char *ptr = buff;
	CString temp;


	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	//temp.Format("%c",*(ptr+3));
	//m_backward_indicator.SetWindowText(temp);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	//temp.Format("%.2f",atof(ptr));
	//m_gyro_data.SetWindowText(temp);
	ptr = go_next_dot(ptr);
	if (ptr == NULL) return;
	temp.Format("%d",atoi(ptr));
	m_odo_meter.SetWindowText(temp);
	

}


void CGPSDlg::On1ppstimingMonitoring1pps()
{

	if(dia_monitor_1pps == NULL)
	{
		dia_monitor_1pps = new CMonitor_1PPS();
		dia_monitor_1pps->Create(IDD_1PPS_MONITOR);
		dia_monitor_1pps->ShowWindow(SW_SHOW);
	}
	else
	{
		dia_monitor_1pps->SetFocus();
	}

}

void CGPSDlg::close_minitor_1pps_window()
{
	dia_monitor_1pps->DestroyWindow();
	dia_monitor_1pps = NULL;
}

void CGPSDlg::parse_psti_50(const char *buff)		// gnss
{
	if(!GNSS_VIEWER)
	{
		return;
	}

	const char *ptr = buff;
	U08 psti_num, seq_num;
	U08 total_gnss;

	ptr = go_next_dot(ptr);

	psti_num = atoi(ptr);
	ptr = go_next_dot(ptr);
	seq_num = atoi(ptr);
	ptr = go_next_dot(ptr);
	total_gnss = atoi(ptr);
	ptr = go_next_dot(ptr);

	if(seq_num == 1)
	{
		memset(&m_gnssTemp,0,sizeof(GNSS_T));
	}

	while(ptr && *ptr != ',')
	{ 
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].k_num = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].slot_num = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.sate[m_gnssTemp.gnss_in_view].snr = atoi(ptr);
		ptr=go_next_dot(ptr);
		m_gnssTemp.gnss_in_view++;
	}

	if(seq_num == psti_num && m_gnssTemp.gnss_in_view == total_gnss)
	{
		memcpy(&m_gnss, &m_gnssTemp,sizeof(GNSS_T));
		memset(&m_gnssTemp,0,sizeof(GNSS_T));	
	}

	ptr = NULL;
}

UINT AFX_CDECL ConfigureSBASThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 16, "Configure SBAS Successful");
	return 0;
}

void CGPSDlg::OnBinaryConfigureSBAS()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigSBAS dlg;
	if(dlg.DoModal() == IDOK) 
	{

		U08 msg[9] = {0};
		msg[0] = 0x62;
		msg[1] = 0x01;
		msg[2] = (U08)dlg.m_bEnable;
		msg[3] = (U08)dlg.m_bRanging;
		msg[4] = (U08)dlg.m_nUraMask;
		msg[5] = (U08)dlg.m_bCorrection;
		msg[6] = (U08)dlg.m_nTrackingChannel;
		msg[7] = (U08)(dlg.m_bWAAS | (dlg.m_bEGNOS << 1) | (dlg.m_bMSAS << 2));
		msg[8] = (U08)dlg.m_nAttribute;
		SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureSBASThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}
/*
void CGPSDlg::ConfigSBAS()
{
	ClearQue();
	SendToTarget(m_inputMsg, 16, "Configure SBAS Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
UINT AFX_CDECL ConfigureSAGPSThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 11, "Configure SAEE Successful");
	return 0;
}

void CGPSDlg::OnBinaryConfigureSAGPS()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigSAGPS dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		if(dlg.m_nEnable != 2)
		{	//not disable must be checked position update rate first.
			U08 data = 0;
			if(Ack == QueryPositionRate(Return, &data))
			{
				if(data > 1)
				{	//Now is in multi-hz position update rate.
					add_msgtolist("Configure SAEE Cancel...");	
					::AfxMessageBox("Please disable multi-hz position update rate before enable SAEE.");

					SetMode();  
					CreateGPSThread();
					return;
				}
			}
		}

		U08 msg[4] = {0};
		msg[0] = 0x63;
		msg[1] = 0x01;
		msg[2] = (U08)dlg.m_nEnable;
		msg[3] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));

        AfxBeginThread(ConfigureSAGPSThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

/*
void CGPSDlg::ConfigSAGPS()
{
	ClearQue();
	SendToTarget(m_inputMsg, 11, "Configure SAEE Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/

int position_update_rate;
int position_update_attr;
int com_baudrate;
UINT Configurepositionrate(LPVOID param)
{
	U08 msg[3];

	msg[0] = 0xE;
	msg[1] = position_update_rate;
	msg[2] = position_update_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Configure Position Rate Successful..."))
	{
		Sleep(200);
		if(position_update_rate>10 && com_baudrate<115200)
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(5, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>2 && com_baudrate<38400)	//Boost to 115200 when update rate > 2Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(3, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>1 && com_baudrate<9600)	//Boost to 115200 when update rate > 1Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(1, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else
		{
			CGPSDlg::gpsDlg->target_restart();
		}
	}

	//CGPSDlg::gpsDlg->OnQuerypositionrate();
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();

	return 0;	
}
UINT ConfigureDrMultiHz(LPVOID param)
{
	U08 msg[4] = {0};

	msg[0] = 0x6F;
	msg[1] = 0x01;
	msg[2] = position_update_rate;
	msg[3] = position_update_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Configure DR Multi-Hz Successful..."))
	{
		Sleep(200);
		if(position_update_rate > 10 && com_baudrate < 115200)
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(5, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>2 && com_baudrate<38400)	//Boost to 115200 when update rate > 2Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(3, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else if(position_update_rate>1 && com_baudrate<9600)	//Boost to 115200 when update rate > 1Hz.
		{
			CGPSDlg::gpsDlg->ConfigBaudrate(1, position_update_attr);		//Boost to 115200 when update rate > 10Hz.
		}
		else
		{
			CGPSDlg::gpsDlg->target_restart();
		}
	}

	//CGPSDlg::gpsDlg->OnQuerypositionrate();
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}
void CGPSDlg::OnBinaryConfigurepositionrate()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode = 0;

	CPositionRateDlg *dlg = new CPositionRateDlg(this);

	if(dlg->DoModal() == IDOK)
	{
		position_update_rate = dlg->rate;
		position_update_attr = dlg->attr;
		com_baudrate = m_serial->GetBaudRate();
		if(position_update_rate > 1)
		{
			U08 data = 0;
			U32 data2 = 0;
			if(Ack == QuerySagps(Return, &data))
			{
				if(data != 2)
				{	//AGPS not disable or ROM version.
					if(Ack == QueryGnssBootStatus(Return, &data2))
					{
						if((LOBYTE(data2) != 1) && HIBYTE(data2) != 0)
						{	// Not boot from ROM.
							add_msgtolist("Configure Position Rate Cancel...");	
							::AfxMessageBox("Please disable SAEE before using multi-hz update rate.");

							SetMode();  
							CreateGPSThread();
							return;
						}
					}
				}
			}
		}
		AfxBeginThread(Configurepositionrate, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}

}

void CGPSDlg::OnBinaryConfigDrMultiHz()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode = 0;	

	DrMultiHzDlg *dlg = new DrMultiHzDlg(this);

	if(dlg->DoModal() == IDOK)
	{
		position_update_rate = dlg->rate;
		position_update_attr = dlg->attr;
		com_baudrate = m_serial->GetBaudRate();
		AfxBeginThread(ConfigureDrMultiHz, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}

}

UINT AFX_CDECL ConfigureQZSSThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 12, "Configure QZSS Successful");
	return 0;
}

void CGPSDlg::OnBinaryConfigureQZSS()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigQZSS dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[5] = {0};
		msg[0] = 0x62;
		msg[1] = 0x03;
		msg[2] = (U08)dlg.m_bEnable;
		msg[3] = (U08)dlg.m_nTrackingChannel;
		msg[4] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureQZSSThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

/*
void CGPSDlg::ConfigQZSS()
{
	ClearQue();
	SendToTarget(m_inputMsg, 12, "Configure QZSS Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/

void CGPSDlg::OnBnClickedECompassCalibration()
{
	if(!CheckConnect())
	{
		return;
	}

	//WaitEvent();
	ClearQue();

	U08 message[8];
	U08 msg[1];

	msg[0] = 0x54; //msgid
	int msg_len = SetMessage2(message, msg, sizeof(msg));

	int res = SendToTarget(message, msg_len, "E-Compass calibration successful...");
	if(res)
	{
		GetDlgItem(IDC_ECOM_COUNTER)->SetWindowText("20");
		SetTimer(ECOM_CALIB_TIMER, 1000, NULL);
	}
	else
	{
		::AfxMessageBox("No E-Compass for calibration.");
	}
	CreateGPSThread();
}

void CGPSDlg::OnBinarySystemRestart()
{	
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	CSysRestartDlg dlg;
	dlg.ReStartType = 0;
	INT_PTR nResult = dlg.DoModal();	
	if(nResult == IDOK)
	{		
		m_ttffCount = 0;
		m_initTtff = false;
		SetTTFF(0);
		DeleteNmeaMemery();
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

//���}��
void CGPSDlg::OnBnClickedHotstart()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	m_ttffCount      = 0;	
	m_initTtff = false;
	if( IS_DEBUG == FALSE)
	{
		target_only_restart(1);
		//CreateGPSThread();
	}
	else
	{
		CSysRestartDlg* dlg = new CSysRestartDlg;
		dlg->ReStartType = 1;
		INT_PTR nResult = dlg->DoModal();	
		if(nResult == IDOK)
		{		
	//		CreatWaitAck();
			//m_ttffCount      = 0;	
			//m_initTtff = false;
			//m_ttff.SetWindowText("0");
			SetTTFF(0);
		}
		else
		{
			SetMode();  
			Sleep(500);
			CreateGPSThread();
		}
		delete dlg;	dlg= NULL;
	}
	DeleteNmeaMemery();
	m_ttffCount      = 0;	
	m_initTtff = false;
	//m_ttff.SetWindowText("0");
	SetTTFF(0);

#if GNSS_VIEWER
	ClearGlonass();
#endif
}


//warm start
void CGPSDlg::OnBnClickedWarmstart()
{
	if(!CheckConnect())return;
	m_inputMode = 0;	

	m_ttffCount      = 0;	
	m_initTtff = false;
	if(IS_DEBUG == FALSE)
	{

		target_only_restart(2);

		//CreateGPSThread();
	}
	else
	{
		CSysRestartDlg* dlg = new CSysRestartDlg;
		dlg->ReStartType = 2;
		INT_PTR nResult = dlg->DoModal();	
		if(nResult == IDOK)
		{		
			//CreatWaitAck();
			//m_ttffCount      = 0;	
			//m_initTtff = false;
			//m_ttff.SetWindowText("0");
			SetTTFF(0);
		}
		else
		{
			SetMode();  
			Sleep(500);
			CreateGPSThread();
		}
		delete dlg;	dlg= NULL;	
	}
	DeleteNmeaMemery();
#if GNSS_VIEWER
	ClearGlonass();
#endif
}

void CGPSDlg::target_only_restart(int mode)
{
	SendRestartCommand(mode);
	SetMode(); 
	CreateGPSThread();
}

void CGPSDlg::SendRestartCommand(int mode)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	U08 msg[15] = {0};
	msg[0] = 0x1; //msgid
	msg[1] = mode; //mode

	if(mode!=1 && mode!=5)	
	{
		msg[2] = HIBYTE(now.wYear);
		msg[3] = LOBYTE(now.wYear);	
		msg[4] = (U08)now.wMonth;
		msg[5] = (U08)now.wDay;
		msg[6] = (U08)now.wHour;
		msg[7] = (U08)now.wMinute;
		msg[8] = (U08)now.wSecond;
		
		if(mode!=4)
		{
			if(m_gpggaMsgBk.Latitude == 0.0)
			{
				m_gpggaMsgBk.Latitude = warmstart_latitude;
			}
			if(m_gpggaMsgBk.Longitude == 0.0)
			{
				m_gpggaMsgBk.Longitude = warmstart_longitude;
			}
			if(m_gpggaMsgBk.Altitude == 0.0)
			{
				m_gpggaMsgBk.Altitude = (F32)warmstart_altitude;
			}

			S16 lat = (S16)(m_gpggaMsgBk.Latitude / 100) * 100;
			lat += (S16)((m_gpggaMsgBk.Latitude - lat)* 100 / 60 + 0.5);
			if(m_gpggaMsgBk.Latitude_N_S == 'S')
			{
				lat *= -1;
			}
			msg[9] = HIBYTE(lat);
			msg[10]= LOBYTE(lat);	

			S16 lon = (S16)((m_gpggaMsgBk.Longitude / 100) * 100);
			lon += (S16)((m_gpggaMsgBk.Longitude - lon)* 100 / 60 + 0.5);
			if(m_gpggaMsgBk.Longitude_E_W == 'W')
			{
				lon *= -1;
			}
			msg[11]= HIBYTE(lon);
			msg[12]= LOBYTE(lon);

			msg[13]= HIBYTE((int)m_gpggaMsgBk.Altitude);
			msg[14]= LOBYTE((int)m_gpggaMsgBk.Altitude);
		}
	}

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	SendToTarget(CGPSDlg::m_inputMsg, len, "System Restart Successful...", true);
}

void CGPSDlg::Restart(U08* messages)
{
	m_CloseBtn.ShowWindow(0);
	//WaitEvent();
	ClearQue();
	SendToTarget(messages,22,"System Restart Successful...");	
	DeleteNmeaMemery();
	SetMode(); 
	CreateGPSThread();	

	gpsSnrBar->Invalidate(FALSE);
	bdSnrBar->Invalidate(FALSE);
	gaSnrBar->Invalidate(FALSE);

	pic_earth->Invalidate(FALSE);
	pic_scatter->Invalidate(FALSE);
	
//	Sleep(1000);
	m_CloseBtn.ShowWindow(1);
}

UINT RestartThread(LPVOID pParam)
{
	BinaryData binData(15);
	*(binData.GetBuffer(0)) = 1;
	*(binData.GetBuffer(1)) = CGPSDlg::gpsDlg->GetRestartMode();
	BinaryCommand binCmd(binData);
	CGPSDlg::gpsDlg->Restart(binCmd.GetBuffer());

/*
	int i;
	U08 messages[22];
	memset(messages, 0, 22);   		    
	messages[0] = (U08)0xa0;
	messages[1] = (U08)0xa1;
	messages[2] = 0;
	messages[3] = 15;
	messages[4] = 1; //msgid
	messages[5] = CGPSDlg::gpsDlg->startmode; //msgid
	messages[6] = 0;
	messages[7] = 0;	
	messages[8] = 0;
	messages[9] = 0;
	messages[10]= 0;
	messages[11]= 0;
	messages[12]= 0;
	messages[13]= 0;
	messages[14]= 0;	
	messages[15]= 0;
	messages[16]= 0;	
	messages[17]= 0;
	messages[18]= 0;
	//messages[5] = CGPSDlg::gpsDlg->startmode; //msgid
	//messages[6] = CGPSDlg::gpsDlg->SRutcyear>>8 &0xff;
	//messages[7] = CGPSDlg::gpsDlg->SRutcyear    &0xff;		
	//messages[8] = CGPSDlg::gpsDlg->SRutcmonth;
	//messages[9] = CGPSDlg::gpsDlg->SRutcday;
	//messages[10]= CGPSDlg::gpsDlg->SRutchour;
	//messages[11]= CGPSDlg::gpsDlg->SRutcmin;
	//messages[12]= CGPSDlg::gpsDlg->SRutcsec;
	//messages[13]= CGPSDlg::gpsDlg->SRlat>>8     &0xff;
	//messages[14]= CGPSDlg::gpsDlg->SRlat        &0xff;		
	//messages[15]= CGPSDlg::gpsDlg->SRlon>>8     &0xff;
	//messages[16]= CGPSDlg::gpsDlg->SRlon        &0xff;		
	//messages[17]= CGPSDlg::gpsDlg->SRalt>>8     &0xff;
	//messages[18]= CGPSDlg::gpsDlg->SRalt        &0xff;		

	U08 checksum = 0;
	for(i=0;i<15;i++)
		checksum^=messages[i+4];

	messages[19]=checksum; //checksum right	    
	messages[20]=(U08)0x0d;
	messages[21]=(U08)0x0a;
	CGPSDlg::gpsDlg->Restart(messages );
*/
	//AfxEndThread(0);	
	return 0;
}

void CGPSDlg::OnBnClickedColdstart()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	m_restartMode = 3;	
	m_initTtff = false;
	m_ttffCount = 0;
	//m_ttff.SetWindowText("0");
	SetTTFF(0);

	AfxBeginThread(RestartThread, 0);

#if GNSS_VIEWER
	ClearGlonass();
#endif
}

void CGPSDlg::target_restart()
{
	SYSTEMTIME	now;		
	GetSystemTime(&now);

	int i;
	U08 messages[22];
	memset(messages, 0, 22);   		    
	messages[0] = (U08)0xa0;
	messages[1] = (U08)0xa1;
	messages[2] = 0;
	messages[3] = 15;
	messages[4] = 1; //msgid
	messages[5] = 1; //msgid
	messages[6] = now.wYear >>8 &0xff;
	messages[7] = now.wYear    &0xff;		
	messages[8] = (U08)now.wMonth;
	messages[9] = (U08)now.wDay;
	messages[10]= (U08)now.wHour;
	messages[11]= (U08)now.wMinute;
	messages[12]= (U08)now.wSecond;
	messages[13]= (24*100) >>8     &0xff;
	messages[14]= (24*100)        &0xff;		
	messages[15]= (121*100) >>8     &0xff;
	messages[16]= (121*100)        &0xff;		
	messages[17]= (100) >>8     &0xff;
	messages[18]= (100)        &0xff;		
	unsigned char checksum = 0;
	for(i=0;i<15;i++)
		checksum^=messages[i+4];

	messages[19]=checksum; //checksum right	    
	messages[20]=(unsigned char)0x0d;
	messages[21]=(unsigned char)0x0a;

	//for(int i=0;i<22;i++)	_cprintf("%x ",messages[i]);
	ClearQue();
	//SendToTarget(messages, 22,"System Restart Successful...",true);
	SendToTargetNoAck(messages, 22);	
	SetMode(); 
	//CreateGPSThread();
}

UINT AFX_CDECL Configure1ppsPulseWidthThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 14, "Configure 1PPS Pulse Width Successful");
	return 0;
}

void CGPSDlg::On1ppstimingConfigurePulseWidth()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfig1ppsPulseWidthDlg dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[7] = {0};
		msg[0] = 0x65;
		msg[1] = 0x01;
		msg[2] = dlg.m_nPulseWidth >> 24 & 0xFF;
		msg[3] = dlg.m_nPulseWidth >> 16 & 0xFF;
		msg[4] = dlg.m_nPulseWidth >> 8 & 0xFF;
		msg[5] = dlg.m_nPulseWidth & 0xFF;
		msg[6] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));

        AfxBeginThread(Configure1ppsPulseWidthThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

/*
void CGPSDlg::Configure1ppsPulseWidth()
{
	ClearQue();
	SendToTarget(m_inputMsg, 14, "Configure 1PPS Pulse Width Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
UINT AFX_CDECL Query1ppsPulseWidth(LPVOID param)
{
	U08 msg[2];
	msg[0] = 0x65;
	msg[1] = 0x02;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();

	U08 buff[100] = {0};
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg, len, "Query 1PPS Pulse Width Successful..."))
	{
		ScopeTimer timer;

		while(1)
		{
			memset(buff, 0, 100);
			DWORD res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res))
			{
				if (Cal_Checksum(buff) == 0x65 && buff[5] == 0x80)
				{
					if(CGPSDlg::gpsDlg->GetShowBinaryCmdData())
					{
						CGPSDlg::gpsDlg->add_msgtolist("Return: " + theApp.GetHexString(buff,  buff[2] <<8 | buff[3] + 7));	
					}
					CString strTxt;
					UINT32 nPulseWidth = 0;
					nPulseWidth = buff[6] << 24 & 0xff000000 | 
								buff[7] << 16 & 0xff0000 | 
								buff[8] << 8 &0xff00 | 
								buff[9] & 0xff;
					strTxt.Format("1PPS Pulse Width : %dus", nPulseWidth);
					CGPSDlg::gpsDlg->add_msgtolist(strTxt);
					break;
				}
			}
			if(timer.GetDuration() > TIME_OUT_MS)
			{
				AfxMessageBox("Timeout: GPS device no response.");
				break;
			}
		} //while(1)
	} //if(CGPSDlg::gpsDlg->SendToTarget(messages,len,"Query Position Pinning Successful..."))

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

void CGPSDlg::On1ppsTimingQuery1ppsPulseWidth()
{
	if(!CheckConnect())
	{
		return;
	}
	//Utility::Log(__FUNCTION__, "start QuerySBAS thread", __LINE__);
	AfxBeginThread(Query1ppsPulseWidth, 0);	
}

#if WITH_CONFIG_USB_BAUDRATE	
/*
void CGPSDlg::Config_silab_baudrate(HANDLE *m_DeviceHandle)
{
	BAUD_CONFIG_DATA m_DefaultBaudConfigData;	

	m_DefaultBaudConfigData[0].BaudRate = 15000000;
	m_DefaultBaudConfigData[0].BaudGen = 0xFFF0;
	m_DefaultBaudConfigData[0].Prescaler = 1;
	m_DefaultBaudConfigData[0].Timer0Reload = 0xFFFA;

	m_DefaultBaudConfigData[1].BaudRate = 15000000;
	m_DefaultBaudConfigData[1].BaudGen = 0xFFF0;
	m_DefaultBaudConfigData[1].Prescaler = 1;
	m_DefaultBaudConfigData[1].Timer0Reload = 0xFFFA;

	m_DefaultBaudConfigData[2].BaudRate = 12000000;
	m_DefaultBaudConfigData[2].BaudGen = 0xFFEC;
	m_DefaultBaudConfigData[2].Prescaler = 1;
	m_DefaultBaudConfigData[2].Timer0Reload = 0xFFF8;

	m_DefaultBaudConfigData[3].BaudRate = 921600;
	m_DefaultBaudConfigData[3].BaudGen = 0xFFE8;
	m_DefaultBaudConfigData[3].Prescaler = 1;
	m_DefaultBaudConfigData[3].Timer0Reload = 0xFFF6;

	m_DefaultBaudConfigData[4].BaudRate = 576000;
	m_DefaultBaudConfigData[4].BaudGen = 0xFFD6;
	m_DefaultBaudConfigData[4].Prescaler = 1;
	m_DefaultBaudConfigData[4].Timer0Reload = 0xFFF0;

	m_DefaultBaudConfigData[5].BaudRate = 500000;
	m_DefaultBaudConfigData[5].BaudGen = 0xFFD0;
	m_DefaultBaudConfigData[5].Prescaler = 1;
	m_DefaultBaudConfigData[5].Timer0Reload = 0xFFEE;

	m_DefaultBaudConfigData[6].BaudRate = 460800;
	m_DefaultBaudConfigData[6].BaudGen = 0xFFD1;
	m_DefaultBaudConfigData[6].Prescaler = 1;
	m_DefaultBaudConfigData[6].Timer0Reload = 0xFFEC;

	m_DefaultBaudConfigData[7].BaudRate = 256000;
	m_DefaultBaudConfigData[7].BaudGen = 0xFFA2;
	m_DefaultBaudConfigData[7].Prescaler = 1;
	m_DefaultBaudConfigData[7].Timer0Reload = 0xFFDC;

	m_DefaultBaudConfigData[8].BaudRate = 250000;
	m_DefaultBaudConfigData[8].BaudGen = 0xFFA0;
	m_DefaultBaudConfigData[8].Prescaler = 1;
	m_DefaultBaudConfigData[8].Timer0Reload = 0xFFDC;

	m_DefaultBaudConfigData[9].BaudRate = 230400;
	m_DefaultBaudConfigData[9].BaudGen = 0xFF98;
	m_DefaultBaudConfigData[9].Prescaler = 1;
	m_DefaultBaudConfigData[9].Timer0Reload = 0xFFD9;

	m_DefaultBaudConfigData[10].BaudRate = 153600;
	m_DefaultBaudConfigData[10].BaudGen = 0xFF64;
	m_DefaultBaudConfigData[10].Prescaler = 1;
	m_DefaultBaudConfigData[10].Timer0Reload = 0xFFC5;

	m_DefaultBaudConfigData[11].BaudRate = 128000;
	m_DefaultBaudConfigData[11].BaudGen = 0xFF44; //Originally FF45 in firmware, but changed to FF44 Sep 7 2004
	m_DefaultBaudConfigData[11].Prescaler = 1;
	m_DefaultBaudConfigData[11].Timer0Reload = 0xFFB9;

	m_DefaultBaudConfigData[12].BaudRate = 115200;
	m_DefaultBaudConfigData[12].BaudGen = 0xFF30;
	m_DefaultBaudConfigData[12].Prescaler = 1;
	m_DefaultBaudConfigData[12].Timer0Reload = 0xFFB2;

	m_DefaultBaudConfigData[13].BaudRate = 76800;
	m_DefaultBaudConfigData[13].BaudGen = 0xFEC8;
	m_DefaultBaudConfigData[13].Prescaler = 1;
	m_DefaultBaudConfigData[13].Timer0Reload = 0xFF8B;

	m_DefaultBaudConfigData[14].BaudRate = 64000;
	m_DefaultBaudConfigData[14].BaudGen = 0xFE89;
	m_DefaultBaudConfigData[14].Prescaler = 1;
	m_DefaultBaudConfigData[14].Timer0Reload = 0xFF73;

	m_DefaultBaudConfigData[15].BaudRate = 57600;
	m_DefaultBaudConfigData[15].BaudGen = 0xFE5F;
	m_DefaultBaudConfigData[15].Prescaler = 1;
	m_DefaultBaudConfigData[15].Timer0Reload = 0xFF63;

	m_DefaultBaudConfigData[16].BaudRate = 56000;
	m_DefaultBaudConfigData[16].BaudGen = 0xFE53;
	m_DefaultBaudConfigData[16].Prescaler = 1;
	m_DefaultBaudConfigData[16].Timer0Reload = 0xFF5F;

	m_DefaultBaudConfigData[17].BaudRate = 51200;
	m_DefaultBaudConfigData[17].BaudGen = 0xFE2B;
	m_DefaultBaudConfigData[17].Prescaler = 1;
	m_DefaultBaudConfigData[17].Timer0Reload = 0xFF50;

	m_DefaultBaudConfigData[18].BaudRate = 38400;
	m_DefaultBaudConfigData[18].BaudGen = 0xFD8F;
	m_DefaultBaudConfigData[18].Prescaler = 1;
	m_DefaultBaudConfigData[18].Timer0Reload = 0xFF15;

	m_DefaultBaudConfigData[19].BaudRate = 28800;
	m_DefaultBaudConfigData[19].BaudGen = 0xFCBF;
	m_DefaultBaudConfigData[19].Prescaler = 1;
	m_DefaultBaudConfigData[19].Timer0Reload = 0xFEC7;

	m_DefaultBaudConfigData[20].BaudRate = 19200;
	m_DefaultBaudConfigData[20].BaudGen = 0xFB1E;
	m_DefaultBaudConfigData[20].Prescaler = 1;
	m_DefaultBaudConfigData[20].Timer0Reload = 0xFE2B;

	m_DefaultBaudConfigData[21].BaudRate = 16000;
	m_DefaultBaudConfigData[21].BaudGen = 0xFA24;
	m_DefaultBaudConfigData[21].Prescaler = 1;
	m_DefaultBaudConfigData[21].Timer0Reload = 0xFDCD;

	m_DefaultBaudConfigData[22].BaudRate = 14400;
	m_DefaultBaudConfigData[22].BaudGen = 0xF97D;
	m_DefaultBaudConfigData[22].Prescaler = 1;
	m_DefaultBaudConfigData[22].Timer0Reload = 0xFD8E;

	m_DefaultBaudConfigData[23].BaudRate = 9600;
	m_DefaultBaudConfigData[23].BaudGen = 0xF63C;
	m_DefaultBaudConfigData[23].Prescaler = 1;
	m_DefaultBaudConfigData[23].Timer0Reload = 0xFC56;

	m_DefaultBaudConfigData[24].BaudRate = 7200;
	m_DefaultBaudConfigData[24].BaudGen = 0xF2FB;
	m_DefaultBaudConfigData[24].Prescaler = 1;
	m_DefaultBaudConfigData[24].Timer0Reload = 0xFB1E;

	m_DefaultBaudConfigData[25].BaudRate = 4800;
	m_DefaultBaudConfigData[25].BaudGen = 0xEC78;
	m_DefaultBaudConfigData[25].Prescaler = 1;
	m_DefaultBaudConfigData[25].Timer0Reload = 0xF8AD;

	m_DefaultBaudConfigData[26].BaudRate = 4000;
	m_DefaultBaudConfigData[26].BaudGen = 0xE890;
	m_DefaultBaudConfigData[26].Prescaler = 1;
	m_DefaultBaudConfigData[26].Timer0Reload = 0xF736;

	m_DefaultBaudConfigData[27].BaudRate = 2400;
	m_DefaultBaudConfigData[27].BaudGen = 0xD8F0;
	m_DefaultBaudConfigData[27].Prescaler = 1;
	m_DefaultBaudConfigData[27].Timer0Reload = 0xF15A;

	m_DefaultBaudConfigData[28].BaudRate = 1800;
	m_DefaultBaudConfigData[28].BaudGen = 0xCBEB;
	m_DefaultBaudConfigData[28].Prescaler = 1;
	m_DefaultBaudConfigData[28].Timer0Reload = 0xEC78;

	m_DefaultBaudConfigData[29].BaudRate = 1200;
	m_DefaultBaudConfigData[29].BaudGen = 0xB1E0;
	m_DefaultBaudConfigData[29].Prescaler = 1;
	m_DefaultBaudConfigData[29].Timer0Reload = 0xE2B4;

	m_DefaultBaudConfigData[30].BaudRate = 600;
	m_DefaultBaudConfigData[30].BaudGen = 0x63C0;
	m_DefaultBaudConfigData[30].Prescaler = 1;
	m_DefaultBaudConfigData[30].Timer0Reload = 0xC568;

	m_DefaultBaudConfigData[31].BaudRate = 300;
	m_DefaultBaudConfigData[31].BaudGen = 0xB1E0;
	m_DefaultBaudConfigData[31].Prescaler = 4;
	m_DefaultBaudConfigData[31].Timer0Reload = 0x8AD0;

	CP210x_STATUS r = CP210x_SetBaudRateConfig(*m_DeviceHandle, m_DefaultBaudConfigData);
}
*/
#endif 

long int g_1ppsFrequencyOutput = 0;
U08 g_1ppsFrequencyOutputAttr = 0;
UINT Config1ppsFrequencyOutputThread(LPVOID param)
{
	U08 msg[7] ;

	msg[0] = 0x65;
	msg[1] = 0x03;
	msg[2] = (U08)HIBYTE(HIWORD(g_1ppsFrequencyOutput));
	msg[3] = (U08)LOBYTE(HIWORD(g_1ppsFrequencyOutput));
	msg[4] = (U08)HIBYTE(LOWORD(g_1ppsFrequencyOutput));
	msg[5] = (U08)LOBYTE(LOWORD(g_1ppsFrequencyOutput));
	msg[6] = g_1ppsFrequencyOutputAttr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure 1PPS Frequency Output Successful...");
	return 0;	
}

void CGPSDlg::OnConfig1ppsFrequencyOutput()
{
	CConfig1ppsFrequenceOutput dlg;

	if(!CheckConnect())	
	{
		return;
	}

	if(dlg.DoModal()!=IDOK)
	{
		SetMode();  
		CreateGPSThread();
		return;
	}

	g_1ppsFrequencyOutput = dlg.freqOutput;
	g_1ppsFrequencyOutputAttr = dlg.freqOutputAttr;
	AfxBeginThread(Config1ppsFrequencyOutputThread, 0);

}

void CGPSDlg::On1ppstimingEnterreferenceposition32977()
{
	CTiming_start dlg;

	if(CheckConnect())	
	{
		dlg.DoModal();
		SetMode();  
		CreateGPSThread();
	}
}

UINT query_1PPS_pulse_clksrc_thread(LPVOID param)
{
	U08 buff[100];
	U08 msg[1] ;
	CString tmp;
	time_t start,end;
	int res;
	msg[0] = 0x58;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();

	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Query 1PPS Pulse Clk Src Successful..."))
	{

		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res) && BINARY_HD1==buff[0] && BINARY_HD2==buff[1])
			{
				if(Cal_Checksum(buff) == 0xC4)
				{
					if(buff[5] == 0)
						CGPSDlg::gpsDlg->add_msgtolist("PPS Pulse Clock Source : x1");
					else if(buff[5] == 1)
						CGPSDlg::gpsDlg->add_msgtolist("PPS Pulse Clock Source : x2");
					else if(buff[5] == 2)
						CGPSDlg::gpsDlg->add_msgtolist("PPS Pulse Clock Source : x4");
					else if(buff[5] == 3)
						CGPSDlg::gpsDlg->add_msgtolist("PPS Pulse Clock Source : x8");
					else
                        CGPSDlg::gpsDlg->add_msgtolist("PPS Pulse Clock Source :Out of Range");
					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD(start,end))
				break;
		}
	}
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	

}

void CGPSDlg::On1ppstimingQueryppspulseclksrc()
{
	if(CheckConnect())	
	{
		AfxBeginThread(query_1PPS_pulse_clksrc_thread, 0);
	}
}

UINT AFX_CDECL ConfigureNoisePowerControlThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 11, "Configure Noise Power Control Successful");
	return 0;
}

void CGPSDlg::OnConfigureNoisePowerControl()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigNoisePowerControl dlg;
	if(dlg.DoModal() == IDOK) 
	{
		U08 msg[4] = {0};
		msg[0] = 0x64;
		msg[1] = 0x08;
		msg[2] = (U08)dlg.m_nMode;
		msg[3] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureNoisePowerControlThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

/*
void CGPSDlg::ConfigNoisePowerControl()
{
	ClearQue();
	SendToTarget(m_inputMsg, 11, "Configure Noise Power Control Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
UINT AFX_CDECL ConfigureInterferenceDetectControlThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 11, "Configure Interference Detect Control Successful");
	return 0;
}

void CGPSDlg::OnConfigureInterferenceDetectControl()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigInterferenceDetectControl dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[4] = {0};
		msg[0] = 0x64;
		msg[1] = 0x06;
		msg[2] = (U08)dlg.m_nMode;
		msg[3] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureInterferenceDetectControlThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}
/*
void CGPSDlg::ConfigInterferenceDetectControl()
{
	ClearQue();
	SendToTarget(m_inputMsg, 11, "Configure Interference Detect Control Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}

UINT AFX_CDECL QueryNMEABinaryOutputDestination(LPVOID param)
{
	U08 msg[2];
	msg[0] = 0x64;
	msg[1] = 0x05;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();

	U08 buff[100] = {0};
	if(CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg, len, "Query NMEABinaryOutputDestination Successful..."))
	{
		ScopeTimer timer;

		while(1)
		{
			memset(buff, 0, 100);
			DWORD res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res))
			{
				if (Cal_Checksum(buff) == 0x64 && buff[5] == 0x82)
				{
					if(CGPSDlg::gpsDlg->GetShowBinaryCmdData())
					{
						CGPSDlg::gpsDlg->add_msgtolist("Return: " + theApp.GetHexString(buff,  buff[2] <<8 | buff[3] + 7));	
					}

					if(buff[7] & 0x0001)
					{
						CGPSDlg::gpsDlg->add_msgtolist("UART Port");
					}
					if(buff[7] & 0x0002)
					{
						CGPSDlg::gpsDlg->add_msgtolist("SPI Slave Port");
					}
					if(buff[7] & 0x0004)
					{
						CGPSDlg::gpsDlg->add_msgtolist("I2C Slave Port");
					}
					break;
				}
			}
			if(timer.GetDuration() > TIME_OUT_MS)
			{
				AfxMessageBox("Timeout: GPS device no response.");
				break;
			}
		} //while(1)
	} //if(CGPSDlg::gpsDlg->SendToTarget(messages,len,"Query Position Pinning Successful..."))

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

void CGPSDlg::OnQueryNMEABinaryOutputDestination()
{
	if(!CheckConnect())
	{
		return;
	}
	Utility::Log(__FUNCTION__, "start NMEABinaryOutputDestination thread", __LINE__);
	AfxBeginThread(QueryNMEABinaryOutputDestination, 0);	
}
*/
UINT AFX_CDECL ConfigureNMEABinaryOutputDestinationThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 12, "Configure NMEABinaryOutputDestination Successful");
	return 0;
}

void CGPSDlg::OnConfigNMEABinaryOutputDestination()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigNMEABinaryOutputDestination dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[5] = {0};
		msg[0] = 0x64;
		msg[1] = 0x04;
		msg[2] = 0;
		msg[3] = (U08)dlg.m_nMode;
		msg[4] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureNMEABinaryOutputDestinationThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}
/* 
void CGPSDlg::ConfigNMEABinaryOutputDestination()
{
	ClearQue();
	SendToTarget(m_inputMsg, 12, "Configure NMEABinaryOutputDestination Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
U32 m_propri_enable;
int m_propri_attr;
UINT ConfigProprietaryMessageThread(LPVOID param)
{
	U08 msg[6] = { 0 };

	msg[0] = 0x49;
	msg[1] = m_propri_enable>>24 & 0xFF;
	msg[2] = m_propri_enable>>16 & 0xFF;
	msg[3] = m_propri_enable>>8 & 0xFF;
	msg[4] = m_propri_enable & 0xFF;
	msg[5] = m_propri_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Proprietary NMEA Successful...");
/*
	CGPSDlg::gpsDlg->ClearQue();
	CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Configure Proprietary NMEA Successful...");
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
*/
	return 0;	
}

void CGPSDlg::OnConfigProprietaryMessage()
{
	if(CheckConnect())	
	{
		return;
	}

	CConfig_Proprietary dlg;
	if(dlg.DoModal()==IDOK)
	{
		m_propri_enable = dlg.enable;
		m_propri_attr = dlg.attr;
		AfxBeginThread(ConfigProprietaryMessageThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

int m_powersaving_param[9],  m_powersaving_attr;
UINT ConfigPowerSavingParametersThread(LPVOID param)
{
	U08 msg[21] = { 0 };

	msg[0] = 0x64;
	msg[1] = 0x0C;
	msg[2] = m_powersaving_param[0]>>8 & 0xFF;
	msg[3] = m_powersaving_param[0] & 0xFF;
	msg[4] = m_powersaving_param[1]>>8 & 0xFF;
	msg[5] = m_powersaving_param[1] & 0xFF;
	msg[6] = m_powersaving_param[2]>>8 & 0xFF;
	msg[7] = m_powersaving_param[2] & 0xFF;
	msg[8] = m_powersaving_param[3]>>8 & 0xFF;
	msg[9] = m_powersaving_param[3] & 0xFF;
	msg[10] = m_powersaving_param[4]>>8 & 0xFF;
	msg[11] = m_powersaving_param[4] & 0xFF;
	msg[12] = m_powersaving_param[5]>>8 & 0xFF;
	msg[13] = m_powersaving_param[5] & 0xFF;
	msg[14] = m_powersaving_param[6]>>8 & 0xFF;
	msg[15] = m_powersaving_param[6] & 0xFF;
	msg[16] = m_powersaving_param[7]>>8 & 0xFF;
	msg[17] = m_powersaving_param[7] & 0xFF;
	msg[18] = m_powersaving_param[8]>>8 & 0xFF;
	msg[19] = m_powersaving_param[8] & 0xFF;
	msg[20] = m_powersaving_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Power Saving Parameters Successful...");
	return 0;	
}

UINT ConfigPowerSavingParametersRomThread(LPVOID param)
{
	U08 msg[21] = { 0 };

	msg[0] = 0x64;
	msg[1] = 0x0C;
	msg[2] = m_powersaving_param[0]>>8 & 0xFF;
	msg[3] = m_powersaving_param[0] & 0xFF;
	msg[4] = m_powersaving_param[1]>>8 & 0xFF;
	msg[5] = m_powersaving_param[1] & 0xFF;
	msg[6] = m_powersaving_param[2]>>8 & 0xFF;
	msg[7] = m_powersaving_param[2] & 0xFF;
	msg[8] = m_powersaving_param[3]>>8 & 0xFF;
	msg[9] = m_powersaving_param[3] & 0xFF;
	msg[10] = m_powersaving_param[4]>>8 & 0xFF;
	msg[11] = m_powersaving_param[4] & 0xFF;
	msg[12] = m_powersaving_param[5]>>8 & 0xFF;
	msg[13] = m_powersaving_param[5] & 0xFF;
	msg[14] = m_powersaving_param[6]>>8 & 0xFF;
	msg[15] = m_powersaving_param[6] & 0xFF;
	msg[16] = m_powersaving_param[7]>>8 & 0xFF;
	msg[17] = m_powersaving_param[7] & 0xFF;
	msg[18] = m_powersaving_param[8]>>8 & 0xFF;
	msg[19] = m_powersaving_param[8] & 0xFF;
	msg[20] = m_powersaving_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg, sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Power Saving Parameters Successful...");
	return 0;	
}

void CGPSDlg::OnConfigPowerSavingParameters()
{
	if(!CheckConnect())	
	{
		return;
	}

	CConfig_Powersaving_param dlg;
	if (dlg.DoModal()!=IDOK)
	{
		SetMode();  
		CreateGPSThread();
		return;
	}

	m_powersaving_param[0] = dlg.param[0];
	m_powersaving_param[1] = dlg.param[1];
	m_powersaving_param[2] = dlg.param[2];
	m_powersaving_param[3] = dlg.param[3];
	m_powersaving_param[4] = dlg.param[4];
	m_powersaving_param[5] = dlg.param[5];
	m_powersaving_param[6] = dlg.param[6];
	m_powersaving_param[7] = dlg.param[7];
	m_powersaving_param[8] = dlg.param[8];
	m_powersaving_attr = dlg.attr;
	AfxBeginThread(ConfigPowerSavingParametersThread, 0);
}

void CGPSDlg::OnConfigPowerSavingParametersRom()
{
	if(!CheckConnect())	
	{
		return;
	}

	CConfig_Powersaving_param dlg;
	dlg.SetRomMode(true);
	if (dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
		return;
	}

	m_powersaving_param[0] = dlg.param[0];
	m_powersaving_param[1] = dlg.param[1];
	m_powersaving_param[2] = dlg.param[2];
	m_powersaving_param[3] = dlg.param[3];
	m_powersaving_param[4] = dlg.param[4];
	m_powersaving_param[5] = dlg.param[5];
	m_powersaving_param[6] = dlg.param[6];
	m_powersaving_param[7] = dlg.param[7];
	m_powersaving_param[8] = dlg.param[8];
	m_powersaving_attr = dlg.attr;
	AfxBeginThread(ConfigPowerSavingParametersRomThread, 0);
}

int m_antenna_control,m_antenna_attr;
UINT configy_antenna_detection_thread(LPVOID param)
{
	U08 msg[3] ;

	memset(msg,0,sizeof(msg));

	msg[0] = 0x47;
	msg[1] = m_antenna_control;
	msg[2] = m_antenna_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure Antenna Detection Successful...");
/*
	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();
	CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Configure Antenna Detection Successful...");
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
*/
	return 0;	
}


void CGPSDlg::OnBinaryConfigureantennadetection()
{
	CConAntennaDetection dlg;

	if(CheckConnect())	
	{
		if (dlg.DoModal()==IDOK)
		{
			m_antenna_control = dlg.antenna_control;
			m_antenna_attr = dlg.attr;
			AfxBeginThread(configy_antenna_detection_thread, 0);
		}
		else
		{
			SetMode();  
			CreateGPSThread();
		}
	}	
}
/*
int m_1pps_nmea_delay,m_1pps_nmea_attr;

UINT configy_1PPS_NMEA_Delay_thread(LPVOID param)
{
	U08 msg[3] ;

	memset(msg,0,sizeof(msg));

	msg[0] = 0x4D;
	msg[1] = m_1pps_nmea_delay;
	msg[2] = m_1pps_nmea_attr;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, len, "Configure 1PPS NMEA Delay Successful...");

	////CGPSDlg::gpsDlg->WaitEvent();
	//CGPSDlg::gpsDlg->ClearQue();
	//CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Configure 1PPS NMEA Delay Successful...");
	//CGPSDlg::gpsDlg->SetMode();  
	//CGPSDlg::gpsDlg->CreateGPSThread();

	return 0;	
}
*/
/*
void CGPSDlg::On1ppstimingConfigure1ppsnmeadelay()
{
	if(!CheckConnect())	
	{
		return;
	}

	CCon1PPS_Nmea_Delay dlg;
	if (dlg.DoModal()!=IDOK)
	{
		SetMode();  
		CreateGPSThread();
		return;
	}
	m_1pps_nmea_delay = dlg.nmea_delay;
	m_1pps_nmea_attr = dlg.attr;
	AfxBeginThread(configy_1PPS_NMEA_Delay_thread, 0);
}

*/
/*
UINT query_1PPS_NMEA_Delay_thread(LPVOID param)
{
	U08 buff[100];
	U08 msg[1] ;
	CString tmp;
	time_t start,end;
	int res;

	msg[0] = 0x4E;

	int len = CGPSDlg::gpsDlg->SetMessage(msg,sizeof(msg));

	//CGPSDlg::gpsDlg->WaitEvent();
	CGPSDlg::gpsDlg->ClearQue();

	if (CGPSDlg::gpsDlg->SendToTarget(CGPSDlg::m_inputMsg,len,"Query 1PPS NMEA Delay Successful..."))
	{

		start = clock();
		while(1)
		{
			memset(buff, 0, 100);
			res = CGPSDlg::gpsDlg->m_serial->GetBinary(buff, sizeof(buff));	

			if(ReadOK(res) && BINARY_HD1==buff[0] && BINARY_HD2==buff[1])
			{
				if (Cal_Checksum(buff) == 0xC1)
				{
					tmp.Format("NMEA Delay Time:%d ms",buff[5]);
					CGPSDlg::gpsDlg->add_msgtolist(tmp);
					break;
				}
			}
			end=clock();	
			if(CGPSDlg::gpsDlg->TIMEOUT_METHOD(start,end))
				break;
		}
	}

	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return 0;	
}

void CGPSDlg::On1ppstimingQuery1ppsnmeadelay()
{
	if(CheckConnect())	
	{
		AfxBeginThread(query_1PPS_NMEA_Delay_thread, 0);
	}
}
//*/
/*
U08 CGPSDlg::set_clock_offset(S32 clk_offset)
{
	U08 msg[5] ;

	msg[0] = 0x7A;
	msg[1] = clk_offset>>24 & 0xFF;
	msg[2] = clk_offset>>16 & 0xFF;
	msg[3] = clk_offset>>8 & 0xFF;
	msg[4] = clk_offset & 0xFF;

	int len = SetMessage(msg,sizeof(msg));

	//WaitEvent();
	ClearQue();

	return SendToTarget(CGPSDlg::m_inputMsg,len,"",1);
}
*/

void CGPSDlg::On1ppstimingEnterreferenceposition32961()
{
	CTiming_start dlg;

	if(CheckConnect())	
	{
		dlg.DoModal();
		SetMode();  
		CreateGPSThread();
	}
}
/* 
void CGPSDlg::ConfigParameterSearchEngineNumber()
{
	ClearQue();
	SendToTarget(m_inputMsg, 11, "ConfigureParameterSearchEngineNumber Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
UINT AFX_CDECL ConfigureParameterSearchEngineNumberThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 11, "ConfigureParameterSearchEngineNumber Successful");
	return 0;
}

void CGPSDlg::OnConfigParameterSearchEngineNumber()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	CConfigParameterSearchEngineNumber dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[4] = {0};
		msg[0] = 0x64;
		msg[1] = 0x0A;
		msg[2] = (U08)dlg.m_nMode;
		msg[3] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigureParameterSearchEngineNumberThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}
/* 
void CGPSDlg::ConfigPositionFixNavigationMask()
{
	ClearQue();
	SendToTarget(m_inputMsg, 12, "ConfigPositionFixNavigationMask Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}

void CGPSDlg::ConfigRefTimeSyncToGpsTime()
{
	ClearQue();
	SendToTarget(m_inputMsg, 15, "ConfigRefTimeSyncToGpsTime Successful");	
	SetMode();
	CreateGPSThread();
	return ;
}
*/
UINT AFX_CDECL ConfigurePositionFixNavigationMaskThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 12, "ConfigPositionFixNavigationMask Successful");
	return 0;
}

void CGPSDlg::OnConfigPositionFixNavigationMask()
{
	if(!CheckConnect())
	{
		return;
	}


	m_inputMode = 0;
	CConfigPositionFixNavigationMask dlg;
	INT_PTR nResult = dlg.DoModal();
	if(nResult == IDOK) 
	{
		U08 msg[5] = {0};
		msg[0] = 0x64;
		msg[1] = 0x11;
		msg[2] = (U08)dlg.m_nMask1;
		msg[3] = (U08)dlg.m_nMask2;
		msg[4] = (U08)dlg.m_nAttribute;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigurePositionFixNavigationMaskThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}

}

UINT AFX_CDECL ConfigGnssDozeModeThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 9, "ConfigGnssDozeMode Successful");
	return 0;
}

void CGPSDlg::OnConfigGnssDozeMode()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
//	CConfigPositionFixNavigationMask dlg;
//	INT_PTR nResult = dlg.DoModal();
//	if(nResult == IDOK) 
	{
		U08 msg[2] = {0};
		msg[0] = 0x64;
		msg[1] = 0x1C;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigGnssDozeModeThread, 0);
	}
	//else
	//{
	//	SetMode();  
	//	CreateGPSThread();
	//}
}

UINT AFX_CDECL ConfigRefTimeSyncToGpsTimeThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 15, "ConfigRefTimeSyncToGpsTime Successful");
	return 0;
}

void CGPSDlg::OnConfigRefTimeSyncToGpsTime()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	ConfigRefTimeToGpsTimeDlg dlg;
	if(dlg.DoModal() == IDOK) 
	{
		U08 msg[8] = {0};
		msg[0] = 0x64;
		msg[1] = 0x15;
		msg[2] = (U08)dlg.m_isEnable;
		msg[3] = (U08)HIBYTE(dlg.m_nYear);
		msg[4] = (U08)LOBYTE(dlg.m_nYear);
		msg[5] = (U08)dlg.m_nMonth;
		msg[6] = (U08)dlg.m_nDay;
		//msg[7] = (U08)0;

		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigRefTimeSyncToGpsTimeThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT AFX_CDECL ConfigQueryGnssNavSolThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 12, "ConfigGnssNavSol Successful");
	return 0;
}

void CGPSDlg::OnConfigQueryGnssNavSol()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	ConfigQueryGnssNavSolDlg dlg;
	if(dlg.DoModal() == IDOK) 
	{
		U08 msg[5] = {0};
		msg[0] = 0x64;
		msg[1] = 0x19;
		msg[2] = (U08)HIBYTE(dlg.m_mode);
		msg[3] = (U08)LOBYTE(dlg.m_mode);
		msg[4] = (U08)dlg.m_attribute;
		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigQueryGnssNavSolThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}

UINT AFX_CDECL ConfigBinaryMeasurementDataOutThread(LPVOID param)
{
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::gpsDlg->m_inputMsg, 15, "ConfigBinaryMeasurementDataOut Successful...");
	return 0;
}

void CGPSDlg::OnConfigBinaryMeasurementDataOut()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;
	ConfigBinaryMeasurementDataOutDlg dlg;
	if(dlg.DoModal() == IDOK) 
	{
		U08 msg[8] = {0};
		msg[0] = 0x1E;
		msg[1] = dlg.m_rate;
		msg[2] = dlg.m_measTime;
		msg[3] = dlg.m_rawMeas;
		msg[4] = dlg.m_svChStatus;
		msg[5] = dlg.m_rcvChStatus;
		msg[6] = dlg.m_subFrame;
		msg[7] = dlg.m_attribute;
		int len = SetMessage(msg, sizeof(msg));
        AfxBeginThread(ConfigBinaryMeasurementDataOutThread, 0);
	}
	else
	{
		SetMode();  
		CreateGPSThread();
	}
}
U08 CGPSDlg::QueryChanelFreq(int chanel,U16 *prn,double *freq)
{
	CmdErrorCode ack;
	U32 v;
	long int nco_freq;
	int clk_base_reg = 0x60005A24;
	int clk_struct_size = 0x5b0;
  //ack = query_register(0x60005A24+4 + chanel*0x420,&v); ROM Code D
  // ack = query_register(0x600057f0+4 + chanel*0x5b0,&v);  ROM Code E
//  ack = query_register(clk_base_reg+4 + chanel*clk_struct_size,&v);
	m_regAddress = clk_base_reg+4 + chanel*clk_struct_size;
	ack = QueryRegister(Return, &v);

	if (ack != Ack) 
	{
	return 0;
	}
	//return 0;

	*prn = v>>16;

	m_regAddress = 0x2000401C + chanel*0x100;
	ack = QueryRegister(Return, &v);
  if (ack != Ack) 
  {
    return 0;
  }
  v = v<<9;
  memcpy(&nco_freq,&v,4);
  nco_freq = nco_freq / 512;
  *freq = nco_freq * 0.030487 - 32051.25 ;

  //TRACE("chanel=%d, prn=%d, freq=%f\r\n",chanel,*prn,*freq);

  return 1;
}

U08 CGPSDlg::PredictClockOffset(double *clk_offset)
{
	U08 ack;
	U16 prn;
	double nco_freq;
	int i;

	for(i=0;i<12;i++)
	{ 
		ack = QueryChanelFreq(i,&prn,&nco_freq);
		//return ack; // Test
		if (ack == 1)
		{       
			if (prn == 1)
			{
				*clk_offset = nco_freq - 173.3276;
				return 1;
			}     
		}
	}
	return 0;
}

void CGPSDlg::OnClockOffsetPredict()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode = 0;
///////////////////////////////////////////////////////////////////////
//	double ck;
//	PredictClockOffset(&ck);
	int ChannelTable[GSA_MAX_SATELLITE] = {0};
	QueryChannelDoppler(Return, ChannelTable);
	QueryClockOffset(Display, ChannelTable);
///////////////////////////////////////////////////////////////////////
	SetMode();
	CreateGPSThread();
}

void CGPSDlg::OnClockOffsetPredictOld()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode = 0;
///////////////////////////////////////////////////////////////////////
//	double ck;
//	PredictClockOffset(&ck);
	QueryChannelDoppler(Display, NULL);
///////////////////////////////////////////////////////////////////////
	SetMode();
	CreateGPSThread();
}

CGPSDlg::CmdErrorCode CGPSDlg::ExcuteBinaryCommand(int cmdIdx, BinaryCommand* cmd, BinaryData* ackCmd, DWORD timeOut, bool silent)
{
	CommandEntry binMsg = cmdTable[cmdIdx];
	U08* pCmd = cmd->GetBuffer();
	int inSize = cmd->Size();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(pCmd, inSize));	
	}
	ackCmd->Alloc(1024);
	m_serial->ClearQueue();
	m_serial->SendData(pCmd, inSize);
	ScopeTimer t;
	bool alreadyAck = false;
	while(1)
	{		
		ackCmd->Clear();
		DWORD len = m_serial->GetBinary(ackCmd->GetBuffer(), ackCmd->Size());
		if(CGPSDlg::gpsDlg->CheckTimeOut(t.GetDuration(), timeOut, silent))
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
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("NACK: " + theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			add_msgtolist("Received NACK...");
			return NACK;
		}
		if( (*ackCmd)[4] == 0x83 )
		{	//Get ACK
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Ack: " + theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			alreadyAck = true;
			continue;
		}
		if(alreadyAck && Cal_Checksum((*ackCmd).GetBuffer()) && (*ackCmd)[4]==binMsg.cmdAck && binMsg.cmdAckSub==0x00)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Out: "  +theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			return Ack;
		}
		if(alreadyAck && Cal_Checksum((*ackCmd).GetBuffer()) && (*ackCmd)[4]==binMsg.cmdAck && (*ackCmd)[5]==binMsg.cmdAckSub)
		{
			if(m_bShowBinaryCmdData)
			{
				add_msgtolist("Out: "  +theApp.GetHexString(ackCmd->Ptr(), len));	
			}
			return Ack;
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::ExcuteBinaryCommandNoWait(int cmdIdx, BinaryCommand* cmd)
{
	CommandEntry binMsg = cmdTable[cmdIdx];
	U08* pCmd = cmd->GetBuffer();
	int inSize = cmd->Size();
	if(m_bShowBinaryCmdData)
	{
		add_msgtolist("In : " + theApp.GetHexString(pCmd, inSize));	
	}

//	m_serial->ClearQueue();
	m_serial->SendData(pCmd, inSize);
	return Timeout;
}

CGPSDlg::CmdErrorCode (CGPSDlg::*queryFunction)(CGPSDlg::CmdExeMode, void*) = NULL;
UINT GenericQueryThread(LPVOID param)
{
	(*CGPSDlg::gpsDlg.*queryFunction)(CGPSDlg::Display, NULL);
	CGPSDlg::gpsDlg->SetMode();  
	CGPSDlg::gpsDlg->CreateGPSThread();
	return TRUE;	
}

void CGPSDlg::GenericQuery(QueryFunction pfn)
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	queryFunction = pfn;
	::AfxBeginThread(GenericQueryThread, 0);
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionRate(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPositionRateCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionRateCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryPositionRateCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[5];
			return Ack;
		}

		CString strMsg = "Query Position Rate Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Position Rate = %d Hz", ackCmd[5]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

const TEL el[] = 
{
	{0,0},
	{6377563.396,	299.3249646},
	{6377340.189,	299.3249646},
	{6378160,	    298.25},
	{6377483.865,	299.1528128},
	{6377397.155,	299.1528128},
	{6378206.4,  	294.9786982},
	{6378249.145,	293.465},
	{6377276.345,	300.8017},
	{6377298.556,	300.8017},
	{6377301.243,	300.8017},
	{6377295.664,	300.8017},
	{6377304.063,	300.8017},
	{6377309.613,	300.8017},
	{6378155,	    298.3},
	{6378200,	    298.3},
	{6378270,	    297},
	{6378160,	    298.247},
	{6378388,	    297},
	{6378245,	    298.3},
	{6378137,	    298.257222101},
	{6378160,	    298.25},
	{6378135,	    298.26},
	{6378137,	    298.257223563}
};

const TDRL datum[] =
{
	{   0,    0,    0, el[23].a, el[23].I_F,23},
	{-118,  -14,  218, el[7].a,  el[7].I_F ,23},   
	{-134,   -2,  210, el[7].a,  el[7].I_F,7 },
	{-165,  -11,  206, el[7].a,  el[7].I_F ,7},	
	{-123,  -20,  220, el[7].a,  el[7].I_F ,7},
	{-166,  -15,  204, el[7].a,  el[7].I_F ,7},	
	{-128,  -18,  224, el[7].a,  el[7].I_F ,7},
	{-161,  -14,  205, el[7].a,  el[7].I_F ,7},
	{ -43, -163,   45, el[19].a, el[19].I_F,19},
	{-150, -250,   -1, el[18].a, el[18].I_F,18},	
	{-143, -236,    7, el[18].a, el[18].I_F,18},
	{-115,	118,  426, el[6].a,  el[6].I_F ,6},
	{-491,  -22,  435, el[3].a,  el[3].I_F ,3},  
	{-270,	 13,   62, el[7].a,  el[7].I_F ,7},	
	{-138, -105, -289, el[7].a, el[7].I_F,7},
	{-153,	 -5, -292, el[7].a, el[7].I_F ,7},	
	{-125, -108, -295, el[7].a, el[7].I_F,7}, 
	{-161,  -73, -317, el[7].a, el[7].I_F ,7},	
	{-143,	-90, -294, el[7].a, el[7].I_F,7},
	{-134, -105, -295, el[7].a, el[7].I_F ,7},	
	{-169,	-19, -278, el[7].a, el[7].I_F,7},
	{-147,  -74, -283, el[7].a, el[7].I_F ,7},	
	{-142,	-96, -293, el[7].a, el[7].I_F,7},
	{-160,   -6, -302, el[7].a, el[7].I_F ,7},	
	{-157,	 -2, -299, el[7].a, el[7].I_F,7},
	{-175,  -23, -303, el[7].a, el[7].I_F ,7},
	{-205,	107,   53, el[18].a, el[18].I_F,18},   
	{ 145,	 75, -272, el[18].a, el[18].I_F,18},	
	{-320,	550, -494, el[18].a, el[18].I_F,18},	
	{ 114, -116, -333,  el[18].a, el[18].I_F,18},	
	{ 124, -234,  -25, el[18].a, el[18].I_F,18},   
	{-133,  -48,  148, el[3].a,  el[3].I_F ,3},	
	{-134,	-48,  149, el[3].a, el[3].I_F,3},
	{ -79, -129,  145, el[7].a,  el[7].I_F,7},
	{-127, -769,  472, el[18].a, el[18].I_F,18},   
	{ -73,  213,  296, el[6].a, el[6].I_F,6},
	{-173,	253,   27, el[18].a, el[18].I_F,18},   
	{ 307,  304, -318, el[18].a, el[18].I_F,18},
	{-384,	664,  -48, el[5].a,  el[5].I_F,5},
	{-104, -129,  239, el[18].a, el[18].I_F,18},	
	{-148,	136,   90, el[18].a, el[18].I_F,18},
	{ 298, -304, -375, el[18].a, el[18].I_F,18},
	{-136, -108, -292, el[7].a, el[7].I_F,7},
	{  -2,  151,  181, el[6].a, el[6].I_F,6},
	{-263,	  6,  431, el[7].a, el[7].I_F,7},
	{ 175,  -38,  113, el[18].a, el[18].I_F,18},	
	{-134,	229,  -29, el[18].a, el[18].I_F,18},
	{-206,  172,   -6, el[18].a, el[18].I_F,18},
	{ -83,	 37,  124, el[7].a, el[7].I_F,7},     
	{ 260,   12, -147, el[7].a, el[7].I_F,7},
	{-377,	681,  -50, el[5].a, el[5].I_F,5},
	{ 230, -199, -752, el[18].a, el[18].I_F,18},	
	{ 211,	147,  111, el[18].a, el[18].I_F,18},
	{ 374,  150,  588, el[5].a, el[5].I_F,5},
	{-104, -101, -140, el[18].a, el[18].I_F,18},   
	{-130, -117, -151, el[18].a, el[18].I_F,18},
	{ -86,  -96, -120, el[18].a, el[18].I_F,18},   
	{ -86,  -96, -120, el[18].a, el[18].I_F,18},
	{ -87,	-95, -120, el[18].a, el[18].I_F,18},   
	{ -84,  -95, -130, el[18].a, el[18].I_F,18},
	{-117, -132, -164, el[18].a, el[18].I_F,18},   
	{ -97, -103, -120, el[18].a, el[18].I_F,18},
	{ -97,	-88, -135, el[18].a, el[18].I_F,18},   
	{-107,  -88, -149, el[18].a, el[18].I_F,18},
	{ -87,	-98, -121, el[18].a, el[18].I_F,18},   
	{ -87,  -96, -120, el[18].a, el[18].I_F,18},
	{-103, -106, -141, el[18].a, el[18].I_F,18},   
	{ -84, -107, -120, el[18].a, el[18].I_F,18},
	{-112,	-77, -145, el[18].a, el[18].I_F,18},   
	{ -86,  -98, -119, el[18].a, el[18].I_F,18},
	{  -7,	215,  225, el[7].a,  el[7].I_F,7},
	{-133, -321,   50, el[18].a,  el[18].I_F,18},	
	{  84,	-22,  209, el[18].a,  el[18].I_F,18},
	{-104,  167,  -38, el[18].a,  el[18].I_F ,18},
	{-100, -248,  259, el[6].a,  el[6].I_F,6},
	{-403,  684,   41, el[5].a,  el[5].I_F,5},
	{ 252, -209, -751, el[18].a,  el[18].I_F,18},  
	{-333, -222,  114, el[18].a,  el[18].I_F,18},
	{ 653, -212,  449, el[4].a,  el[4].I_F,4},
	{ -73,   46,  -86, el[18].a,  el[18].I_F,18},	
	{-156, -271, -189, el[18].a,  el[18].I_F,18},
	{-637, -549, -203, el[18].a,  el[18].I_F,18},
	{ 282,  726,  254, el[8].a,  el[8].I_F,8},
	{ 295,  736,  257, el[10].a,  el[10].I_F,10},
	{ 283,	682,  231, el[13].a,  el[13].I_F,13},
	{ 217,  823,  299, el[8].a,  el[8].I_F,8},	
	{ 182,	915,  344, el[8].a,  el[8].I_F,8},
	{ 198,  881,  317, el[8].a,  el[8].I_F,8},	
	{ 210,	814,  289, el[8].a,  el[8].I_F,8},
	{ -24,  -15,    5, el[17].a,  el[17].I_F,17},
	{ 506, -122,  611, el[2].a,  el[2].I_F,2},
	{-794,	119, -298, el[18].a,  el[18].I_F,18},	
	{ 208, -435, -229, el[18].a,  el[18].I_F,18},
	{ 189,	-79, -202, el[18].a,  el[18].I_F,18},
	{ -97,	787,   86,  el[8].a,  el[8].I_F,8},
	{ 145, -187,  103, el[18].a,  el[18].I_F,18},
	{ -11,	851,	5, el[12].a,  el[12].I_F,12},
	{ 647, 1777,-1124, el[18].a,  el[18].I_F,18},
	{   0,	  0,	0,  el[20].a,  el[20].I_F,20},
	{  42,  124,  147, el[6].a,  el[6].I_F,6},
	{-130,	 29,  364, el[7].a,  el[7].I_F,7},    
	{ -90,   40,   88, el[7].a,  el[7].I_F,7},
	{-133,	-77,  -51, el[6].a,  el[6].I_F,6},    
	{-133,  -79,  -72, el[6].a,  el[6].I_F,6},
	{ -74, -130,   42, el[7].a,  el[7].I_F,7},    
	{  41, -220, -134, el[7].a,  el[7].I_F,7},
	{ 639,	405,   60, el[5].a,  el[5].I_F,5},    
	{  31,  146,   47, el[7].a,  el[7].I_F,7},
	{ 912,	-58, 1227, el[18].a,  el[18].I_F,18},
	{ -81,  -84,  115, el[7].a,  el[7].I_F,7},	
	{ -92,  -93,  122, el[7].a,  el[7].I_F,7},
	{ 174,  359,  365, el[7].a,  el[7].I_F},	
	{-247, -148,  369, el[7].a,  el[7].I_F,7},
	{-243, -192,  477, el[7].a,  el[7].I_F},	
	{-249, -156,  381, el[7].a,  el[7].I_F,7},
	{ -10,  375,  165, el[18].a,  el[18].I_F,18},
	{  -5,	135,  172, el[6].a,  el[6].I_F,6},    
	{  -2,  152,  149, el[6].a,  el[6].I_F,6},
	{   2,	204,  105, el[6].a,  el[6].I_F,6},    
	{  -4,  154,  178, el[6].a,  el[6].I_F,6},
	{   1,	140,  165, el[6].a,  el[6].I_F,6},    
	{  -7,  162,  188, el[6].a,  el[6].I_F,6},
	{  -9,	157,  184, el[6].a,  el[6].I_F,6},    
	{ -22,  160,  190, el[6].a,  el[6].I_F,6},
	{   4,	159,  188, el[6].a,  el[6].I_F,6},    
	{  -7,	139,  181, el[6].a,  el[6].I_F,6},
	{   0,	125,  201, el[6].a,  el[6].I_F,6},    
	{  -9,	152,  178, el[6].a,  el[6].I_F,6},
	{  11,	114,  195, el[6].a,  el[6].I_F,6},    
	{  -3,	142,  183, el[6].a,  el[6].I_F,6},
	{   0,	125,  194, el[6].a,  el[6].I_F,6},    
	{ -10,	158,  187, el[6].a,  el[6].I_F,6},
	{  -8,	160,  176, el[6].a,  el[6].I_F,6},    
	{  -9,	161,  179, el[6].a,  el[6].I_F,6},
	{  -8,	159,  175, el[6].a,  el[6].I_F,6},    
	{ -12,	130,  190, el[6].a,  el[6].I_F,6},
	{   0,	  0,    0, el[20].a, el[20].I_F,20},   
	{  -2,	  0,    4, el[20].a,  el[20].I_F,20},
	{   0,	  0,	0, el[20].a, el[20].I_F,20},   
	{   0,	  0,    0, el[20].a,  el[20].I_F,20},
	{   1,	  1,   -1, el[20].a, el[20].I_F,20},   
	{   0,	  0,    0, el[20].a,  el[20].I_F,20},
	{-186,	-93,  310, el[7].a,  el[7].I_F,7},  
	{-425, -169,   81,  el[18].a,  el[18].I_F,18},
	{-130,	110,  -13,  el[15].a,  el[15].I_F,15},
	{  89, -279, -183,  el[6].a,  el[6].I_F,6},  
	{  45, -290, -172,  el[6].a,  el[6].I_F,6},
	{  65, -290, -190,  el[6].a,  el[6].I_F,6},	
	{  61, -285, -181,  el[6].a,  el[6].I_F,6}, 
	{  58, -283, -182,  el[6].a,  el[6].I_F,6},
	{-346,	 -1,  224,  el[7].a,  el[7].I_F,7},
	{ 371, -112,  434,  el[1].a,  el[1].I_F,1}, 	
	{ 371, -111,  434,  el[1].a,  el[1].I_F,1},
	{ 375, -111,  431,  el[1].a,  el[1].I_F,1},	
	{ 384, -111,  425,  el[1].a,  el[1].I_F,1}, 
	{ 370, -108,  434,  el[1].a,  el[1].I_F,1}, 	
	{-307,  -92,  127,  el[18].a,  el[18].I_F,18}, 
	{ 185,  165,   42,  el[18].a,  el[18].I_F,18}, 
	{-106, -129,  165,  el[7].a,  el[7].I_F,7},
	{-148,	 51, -291,  el[18].a,  el[18].I_F,18}, 
	{-499, -249,  314,  el[18].a,  el[18].I_F,18}, 
	{-270,  188, -388,  el[18].a,  el[18].I_F,18}, 
	{-270,	183, -390,  el[18].a,  el[18].I_F,18}, 
	{-305,	243, -442,  el[18].a,  el[18].I_F,18}, 
	{-282,	169, -371,  el[18].a,  el[18].I_F,18}, 
	{-278,	171, -367,  el[18].a,  el[18].I_F,18}, 
	{-298,	159, -369,  el[18].a,  el[18].I_F,18}, 
	{-288,	175, -376,  el[18].a,  el[18].I_F,18}, 
	{-279,	175, -379,  el[18].a,  el[18].I_F,18}, 
	{-295,	173, -371,  el[18].a,  el[18].I_F,18}, 
	{  16,	196,   93,  el[18].a,  el[18].I_F,18}, 
	{  11,	 72, -101,  el[6].a,  el[6].I_F,6},
	{  28, -130,  -95,  el[19].a,  el[19].I_F,19}, 
	{-128, -283,   22,  el[18].a,  el[18].I_F,18}, 
	{ 164,	138, -189,  el[18].a,  el[18].I_F,18}, 
	{  94, -948,-1262,  el[18].a,  el[18].I_F,18}, 
	{-225,	-65,	9,  el[18].a,  el[18].I_F,18},
	{  28, -121,  -77,  el[19].a,  el[19].I_F,19},
	{  23, -124,  -82,  el[19].a,  el[19].I_F,19},
	{  26, -121,  -78,   el[19].a,  el[19].I_F,19}, 
	{  24, -124,  -82,    el[19].a,  el[19].I_F,19}, 
	{  15, -130,  -84,    el[19].a,  el[19].I_F,19}, 
	{  24, -130,  -92,    el[19].a,  el[19].I_F,19}, 
	{  28, -121,  -77,   el[19].a,  el[19].I_F,19}, 
	{ 589,	 76,  480,   el[5].a,  el[5].I_F,5}, 
	{ 170,   42,   84,   el[18].a,  el[18].I_F,18}, 
	{-203,	141,   53,   el[18].a,  el[18].I_F,5}, 
	{-355,   21,   72,   el[18].a,  el[18].I_F,18}, 
	{ 616,	 97, -251,   el[4].a,  el[4].I_F,4}, 
	{-289, -124,   60,   el[18].a,  el[18].I_F,18}, 
	{ -88,	  4,  101,   el[7].a,  el[7].I_F,7}, 
	{ -62,   -1,  -37,   el[21].a,  el[21].I_F,21}, 
	{ -61,    2,  -48,  el[21].a,  el[21].I_F,21}, 
	{ -60,   -2,  -41,   el[21].a,  el[21].I_F,21}, 
	{ -75,	 -1,  -44,   el[21].a,  el[21].I_F,21}, 
	{ -44,    6,  -36,   el[21].a,  el[21].I_F,21}, 
	{ -48,    3,  -44,  el[21].a,  el[21].I_F,21}, 
	{ -47,   26,  -42,   el[21].a,  el[21].I_F,21}, 
	{ -53,	  3,  -47,   el[21].a,  el[21].I_F,21}, 
	{ -57,    1,  -41,   el[21].a,  el[21].I_F,21}, 
	{ -61,	  2,  -33,   el[21].a,  el[21].I_F,21}, 
	{ -58,    0,  -44,   el[21].a,  el[21].I_F,21}, 
	{ -45,	 12,  -33,   el[21].a,  el[21].I_F,21}, 
	{ -45,    8,  -33,   el[21].a,  el[21].I_F,21}, 
	{   7,	-10,  -26,  el[14].a,  el[14].I_F,14}, 
	{-189, -242,  -91,  el[18].a,  el[18].I_F,18}, 
	{-679,	669,  -48,   el[9].a,  el[7].I_F,7},
	{-148,  507,  685, el[5].a,  el[5].I_F,5},	
	{-148,	507,  685,  el[5].a,  el[5].I_F,5},
	{-158,  507,  676,  el[5].a,  el[5].I_F,5},	
	{-147,	506,  687,  el[5].a,  el[5].I_F,5},
	{-632,  438, -609,  el[18].a,  el[18].I_F,18},
	{  51,	391,  -36,  el[7].a,  el[7].I_F,7},
	{-123, -206,  219, el[7].a,  el[7].I_F,7},
	{ 276,	-57,  149,  el[18].a,  el[18].I_F,18},
	{ 102,   52,  -38, el[16].a,  el[16].I_F,16},
	{   0,	  0,	0,  el[22].a,  el[22].I_F,22},
	{-155,  171,   37,  el[18].a,  el[18].I_F,18},
	{-265,	120, -358,  el[18].a,  el[18].I_F,22},     	
};

CGPSDlg::CmdErrorCode CGPSDlg::QueryDatum(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDatumCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDatumCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryDatumCmd, &cmd, &ackCmd))
	{
		U16 datumIdx = MAKEWORD(ackCmd[6], ackCmd[5]);
		CString strMsg = "Query Datum Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("DeltaX: %d, DeltaY: %d, DeltaZ: %d", 
			datum[datumIdx].DeltaX, datum[datumIdx].DeltaY, datum[datumIdx].DeltaZ);
		add_msgtolist(strMsg);
		strMsg.Format("Semi_Major_Axis: %.3f", datum[datumIdx].Semi_Major_Axis);
		add_msgtolist(strMsg);
		strMsg.Format("Inversed_Flattening: %.9f", datum[datumIdx].Inversd_Flattening);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareVersionRomCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwVerRomCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwVerRomCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwVerRomCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySwVerRomCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Version Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Kernel Version ", ackCmd[7], ackCmd[8], ackCmd[9]);
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Software Version ", ackCmd[11], ackCmd[12], ackCmd[13]);
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Revision ", ackCmd[15] + 2000, ackCmd[16], ackCmd[17]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareVersionSystemCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwVerSysCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwVerSysCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwVerSysCmd].cmdSubId);

	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QuerySwVerSysCmd, &cmd);
		return Timeout;
	}
	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySwVerSysCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Version Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Kernel Version ", ackCmd[7], ackCmd[8], ackCmd[9]);
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Software Version ", ackCmd[11], ackCmd[12], ackCmd[13]);
		add_msgtolist(strMsg);
		strMsg.Format("%s%d.%d.%d", "Revision ", ackCmd[15] + 2000, ackCmd[16], ackCmd[17]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareCrcRomCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwCrcRomCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwCrcRomCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwCrcRomCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySwCrcRomCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query CRC Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("%s%02x%02x", "Rom CRC: ", ackCmd[6], ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySoftwareCrcSystemCode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySwCrcSysCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySwCrcSysCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySwCrcSysCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySwCrcSysCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U16*)outputData) = (ackCmd[6]<<8) | ackCmd[7];
			return Ack;
		}

		CString strMsg = "Query CRC Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("%s%02x%02x", "System CRC: ", ackCmd[6], ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryWaasStatus(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryWaasStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryWaasStatusCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryWaasStatusCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryWaasStatusCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Waas Status Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("WAAS status: %s", (ackCmd[5]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionPinning(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPositionPinningCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionPinningCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryPositionPinningCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Position Pinning Successful...";
		add_msgtolist(strMsg);
		strMsg = "Position Pinning: ";
		if(0==ackCmd[5])
		{
			strMsg += "Default";
		}
		else if(1==ackCmd[5])
		{
			strMsg += "Enable";
		}
		else if(2==ackCmd[5])
		{
			strMsg += "Disable";
		}
		add_msgtolist(strMsg);
		strMsg.Format("Pinning Speed: %d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("Pinning Cnt: %d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Speed: %d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Cnt: %d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("Unpinning Distance: %d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::Query1ppsMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[Query1ppsModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[Query1ppsModeCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(Query1ppsModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GPS Measurement Mode Successful...";
		add_msgtolist(strMsg);
		strMsg = "GPS Measurement Mode: ";
		if(0==ackCmd[5])
		{
			strMsg += "Not sync to UTC second";
		}
		else if(1==ackCmd[5])
		{
			strMsg += "Sync to UTC second when 3D fix";
		}
		else if(2==ackCmd[5])
		{
			strMsg += "On when 1 SV";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPowerMode(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPowerModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPowerModeCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryPowerModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Mode Successful...";
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[5])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPowerSavingParameters(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryPowerSavingParametersCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPowerSavingParametersCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryPowerSavingParametersCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Tunnel full power time:%d", MAKEWORD(ackCmd[21], ackCmd[20]));
		add_msgtolist(strMsg);
		strMsg.Format("SV no diff time:%d", MAKEWORD(ackCmd[23], ackCmd[22]));
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[24])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryV8PowerSavingParametersRom(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryV8RomPowerSaveParameters].cmdSize);
	cmd.SetU08(1, cmdTable[QueryV8RomPowerSaveParameters].cmdId);
	cmd.SetU08(2, cmdTable[QueryV8RomPowerSaveParameters].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryV8RomPowerSaveParameters, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		//strMsg.Format("Tunnel full power time:%d", MAKEWORD(ackCmd[21], ackCmd[20]));
		//add_msgtolist(strMsg);
		//strMsg.Format("SV no diff time:%d", MAKEWORD(ackCmd[23], ackCmd[22]));
		//add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryV8PowerSavingParameters(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryV8PowerSaveParameters].cmdSize);
	cmd.SetU08(1, cmdTable[QueryV8PowerSaveParameters].cmdId);
	cmd.SetU08(2, cmdTable[QueryV8PowerSaveParameters].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryV8PowerSaveParameters, &cmd, &ackCmd))
	{
		CString strMsg = "Query Power Saving Parameters Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE no fix cycle time:%d", MAKEWORD(ackCmd[7], ackCmd[6]));
		add_msgtolist(strMsg);
		strMsg.Format("2 PSE fix cycle time:%d", MAKEWORD(ackCmd[9], ackCmd[8]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE no fix cycle time:%d", MAKEWORD(ackCmd[11], ackCmd[10]));
		add_msgtolist(strMsg);
		strMsg.Format("4 PSE fix cycle time:%d", MAKEWORD(ackCmd[13], ackCmd[12]));
		add_msgtolist(strMsg);
		strMsg.Format("First boot up full power time:%d", MAKEWORD(ackCmd[15], ackCmd[14]));
		add_msgtolist(strMsg);
		strMsg.Format("Power save entering wait:%d", MAKEWORD(ackCmd[17], ackCmd[16]));
		add_msgtolist(strMsg);
		strMsg.Format("Cold start full power time:%d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Tunnel full power time:%d", MAKEWORD(ackCmd[21], ackCmd[20]));
		add_msgtolist(strMsg);
		strMsg.Format("SV no diff time:%d", MAKEWORD(ackCmd[23], ackCmd[22]));
		add_msgtolist(strMsg);
		strMsg = "Power Mode: ";
		if(1==ackCmd[24])
		{
			strMsg += "Power Save";
		}
		else
		{
			strMsg += "Normal";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryProprietaryMessage(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryProprietaryMessageCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryProprietaryMessageCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryProprietaryMessageCmd, &cmd, &ackCmd))
	{
		add_msgtolist("Query Proprietary Message Successful...");
		DWORD enable = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
		if(enable & 0x00000001)
		{
			add_msgtolist("epe enable");
		}
		else
		{
			add_msgtolist("epe disable");
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryTiming(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryTimingCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryTimingCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryTimingCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Timing Successful...";
		add_msgtolist(strMsg);

		_1PPS_Timing_T t;
		t.Timing_mode = ackCmd[5];
		t.Survey_Length = MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]), MAKEWORD(ackCmd[7], ackCmd[6]));
		t.Standard_deviation = MAKELONG(MAKEWORD(ackCmd[13], ackCmd[12]), MAKEWORD(ackCmd[11], ackCmd[10]));
		for(int i=0; i<sizeof(t.latitude); ++i)
		{
			((BYTE*)(&t.latitude))[7 - i] = ackCmd[14 + i];
			((BYTE*)(&t.longitude))[7 - i] = ackCmd[22 + i];
			if(i < sizeof(t.altitude))
			{
				((BYTE*)(&t.altitude))[3 - i] = ackCmd[30 + i];
			}
		}
		t.RT_Timing_mode = ackCmd[34];
		t.RT_Survey_Length = MAKELONG(MAKEWORD(ackCmd[38], ackCmd[37]), MAKEWORD(ackCmd[36], ackCmd[35]));

		if(t.Timing_mode==0)
		{
			add_msgtolist("Timing Mode: PVT Mode");
		}
		else if(t.Timing_mode==1)
		{
			add_msgtolist("Timing Mode: Survey Mode");
			strMsg.Format("Survey Length: %d",t.Survey_Length);
			add_msgtolist(strMsg);
			strMsg.Format("Standard deviation: %d",t.Standard_deviation);
			add_msgtolist(strMsg);
		}
		else if(t.Timing_mode==2)
		{
			add_msgtolist("Timing Mode: Static Mode");
			strMsg.Format("Saved Latitude: %.6f",t.latitude);
			add_msgtolist(strMsg);
			strMsg.Format("Saved Longitude: %.6f",t.longitude);
			add_msgtolist(strMsg);
			strMsg.Format("Saved Altitude: %.6f",t.altitude);
			add_msgtolist(strMsg);
		}

		if(t.RT_Timing_mode==0)
		{
			add_msgtolist("Run-time Timing Mode: PVT Mode");
		}
		else if(t.RT_Timing_mode==1)
		{
			add_msgtolist("Run-time Timing Mode: Survey Mode");
			strMsg.Format("Run-time Survey Length: %d",t.RT_Survey_Length);
			add_msgtolist(strMsg);
		}
		else if(t.RT_Timing_mode==2)
		{
			add_msgtolist("Run-time Timing Mode: Static Mode");
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDopMask(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDopMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDopMaskCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryDopMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DOP Mask Successful...";
		add_msgtolist(strMsg);

		strMsg.Format("DOP Mode: %d", ackCmd[5]);
		add_msgtolist(strMsg);
		if(ackCmd[5]==1)
		{
			strMsg.Format("PDOP: %.1f", MAKEWORD(ackCmd[7], ackCmd[6]) / 10.0);
			add_msgtolist(strMsg);
			strMsg.Format("HDOP: %.1f", MAKEWORD(ackCmd[9], ackCmd[8]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 2)
		{
			strMsg.Format("PDOP: %.1f", MAKEWORD(ackCmd[7], ackCmd[6]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if (ackCmd[5] == 3)
		{
			strMsg.Format("HDOP: %.1f", MAKEWORD(ackCmd[9], ackCmd[8]) / 10.0);
			add_msgtolist(strMsg);
		}
		else if (ackCmd[5] == 4)
		{
			strMsg.Format("GDOP: %.1f", MAKEWORD(ackCmd[11], ackCmd[10]) / 10.0);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryElevationAndCnrMask(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryElevationAndCnrMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryElevationAndCnrMaskCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryElevationAndCnrMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Elevation and CNR Mask Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Elevation and CNR Mask Mode: %d", ackCmd[5]);
		add_msgtolist(strMsg);
		if(ackCmd[5]==1)
		{
			strMsg.Format("Elevation Mask: %d", ackCmd[6]);
			add_msgtolist(strMsg);
			strMsg.Format("CNR Mask: %d", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 2)
		{
			strMsg.Format("Elevation Mask: %d", ackCmd[6]);
			add_msgtolist(strMsg);
		}
		else if(ackCmd[5] == 3)
		{
			strMsg.Format("CNR Mask: %d", ackCmd[7]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryAntennaDetection(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryAntennaDetectionCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryAntennaDetectionCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryAntennaDetectionCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Antenna Detection Successful...";
		add_msgtolist(strMsg);

		if(ackCmd[5] & 0x01)
		{
			add_msgtolist("Short Circuit Detection On");
		}
		else
		{
			add_msgtolist("Short Circuit Detection Off");
		}

		if(ackCmd[5] & 0x02)
		{
			add_msgtolist("Antenna Detection On");
		}
		else
		{
			add_msgtolist("Antenna Detection Off");
		}

		strMsg.Format("Antenna Mode Status:0x%02X", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNoisePower(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNoisePowerCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNoisePowerCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNoisePowerCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Noise Power Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Noise Power: %d", MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}


CGPSDlg::CmdErrorCode CGPSDlg::QueryDrInfo(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDrInfoCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrInfoCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryDrInfoCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DR Info Successful...";
		add_msgtolist(strMsg);
		//data = buff[7]<<24 | buff[8]<<16 | buff[9]<<8 | buff[10];
		F32 gyroData;
		for(int i=0; i<sizeof(gyroData); ++i)
		{
			((BYTE*)(&gyroData))[sizeof(gyroData) - i - 1] = ackCmd[7 + i];

		}
		strMsg.Format("Temperature = %f",(F32)MAKEWORD(ackCmd[6], ackCmd[5]) / 10.0);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro Data = %f",gyroData);
		add_msgtolist(strMsg);
		strMsg.Format("Odo Data = %d", MAKEWORD(ackCmd[12], ackCmd[11]));
		add_msgtolist(strMsg);
		strMsg.Format("Odo backward indicator =  %s", (ackCmd[13]) ? "TRUE" : "FALSE");
		add_msgtolist(strMsg);

	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDrHwParameter(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryDrHwParameterCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrHwParameterCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryDrHwParameterCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query DR HW Parameter Successful...";
		add_msgtolist(strMsg);

		F32 gyroOutputUnit, gyroReference, gyroAdcReference, gyroAdcGain, defaultOdoScale;
		for(int i=0; i<sizeof(gyroOutputUnit); ++i)
		{
			((BYTE*)(&gyroOutputUnit))[sizeof(F32) - i - 1] = ackCmd[5 + i];
			((BYTE*)(&gyroReference))[sizeof(F32) - i - 1] = ackCmd[9 + i];
			((BYTE*)(&gyroAdcReference))[sizeof(F32) - i - 1] = ackCmd[14 + i];
			((BYTE*)(&gyroAdcGain))[sizeof(F32) - i - 1] = ackCmd[21 + i];
			((BYTE*)(&defaultOdoScale))[sizeof(F32) - i - 1] = ackCmd[25 + i];

		}
		strMsg.Format("Gyro output unit = %f mV/dps", gyroOutputUnit);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro reference voltage = %f", gyroReference);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC type = %s", (ackCmd[13]) ? "unipolar" : "differential");
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC reference voltage = %f", gyroAdcReference);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC sampling freq = %d", MAKEWORD(ackCmd[19], ackCmd[18]));
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC NBITS = %d", ackCmd[20]);
		add_msgtolist(strMsg);
		strMsg.Format("Gyro ADC gain = %f", gyroAdcGain);
		add_msgtolist(strMsg);
		strMsg.Format("Default ODO scale = %f", defaultOdoScale);
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[29]) ? "ODO has backward" : "ODO no backward");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[30]) ? "ODO invert FW/BW" : "ODO not invert FW/BW");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[31]) ? "ODO in period mode" : "ODO in hz mode");
		add_msgtolist(strMsg);
		strMsg.Format("ODO period in = %d us", MAKEWORD(ackCmd[33], ackCmd[32]));
		add_msgtolist(strMsg);
		strMsg.Format("DR firmware version = %d.%d.%d", ackCmd[35], ackCmd[36], ackCmd[37]);
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[38]) ? "DR enable temperature" : "DR not enable temperature");
		add_msgtolist(strMsg);
		strMsg.Format("%s", (ackCmd[39]) ? "DR enable tunnel table" : "DR not enable tunnel table");
		add_msgtolist(strMsg);
		strMsg = "DR table region = ";
		if(ackCmd[43] & 0x01)
		{
			strMsg.Append("Taiwan,");
		}
		if(ackCmd[43] & 0x02)
		{
			strMsg.Append("Korea,");
		}
		if(ackCmd[43] & 0x04)
		{
			strMsg.Append("Japan,");
		}
		if(ackCmd[43] & 0x08)
		{
			strMsg.Append("China,");
		}
		add_msgtolist(strMsg);

	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssSelectionForNavigationSystem(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssSelectionForNavigationSystemCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssSelectionForNavigationSystemCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssSelectionForNavigationSystemCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Gnss Selection for Navigation System Successful...";
		add_msgtolist(strMsg);
		strMsg = "GNSS Navigation Selection: ";

		if(ackCmd[5]==0)
		{
			strMsg.Append("None");
		}
		else if(ackCmd[5]==1)
		{
			strMsg.Append("GPS Only");
		}
		else if(ackCmd[5]==2)
		{
			strMsg.Append("Glonass Only");
		}
		else if(ackCmd[5]==3)
		{
			strMsg.Append("Combined");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssKnumberSlotCnr(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssKnumberSlotCnrCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssKnumberSlotCnrCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssKnumberSlotCnrCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GLONASS K-Num, Slot, CNR Successful...";
		add_msgtolist(strMsg);
		for(int i=0; i<ackCmd[5]; ++i)
		{
			strMsg.Format("K-Num=%d, Slot=%d, CNR=%d", (int)(char)ackCmd[6+i*3], ackCmd[7+i*3], ackCmd[8+i*3]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySbas(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySbasCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySbasCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySbasCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySbasCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query SBAS Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("SBAS system: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		if(ackCmd[7]==0)
		{
			strMsg.Format("Ranging : %s", "Disable");
		}
		else if(ackCmd[7]==1)
		{
			strMsg.Format("Ranging : %s", "Enable");
		}
		else
		{
			strMsg.Format("Ranging : %s", "Auto");
		}
		add_msgtolist(strMsg);
		strMsg.Format("Ranking URA Mask: %d", ackCmd[8]);
		add_msgtolist(strMsg);
		strMsg.Format("Correction: %s", ((ackCmd[9]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("Number of tracking channels: %d", ackCmd[10]);
		add_msgtolist(strMsg);
		strMsg.Format("WAAS: %s", ((ackCmd[11] & 0x01) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("EGNOS: %s", ((ackCmd[11] & 0x02) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("MSAS: %s", ((ackCmd[11] & 0x04) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QuerySagps(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QuerySagpsCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QuerySagpsCmd].cmdId);
	cmd.SetU08(2, cmdTable[QuerySagpsCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QuerySagpsCmd, &cmd, &ackCmd))
	{
		if(nMode==Return)
		{
			*((U08*)outputData) = ackCmd[6];
			return Ack;
		}

		CString strMsg = "Query SAEE Successful...";
		add_msgtolist(strMsg);
		strMsg = "SAEE System: ";
		if(0==(ackCmd[6]))
		{
			strMsg += "Default";
		}
		else if(1==(ackCmd[6]))
		{
			strMsg += "Enable";
		}
		else if(2==(ackCmd[6]))
		{
			strMsg += "Disable";
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryQzss(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryQzssCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryQzssCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryQzssCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryQzssCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query QZSS Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("QZSS System: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("Number of Tracking Channels: %d", ackCmd[7]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNoisePowerControl(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNoisePowerControlCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNoisePowerControlCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNoisePowerControlCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNoisePowerControlCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Noise Power Control Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Noise Power Control: %s", ((ackCmd[6]) ? "Dynamic" : "Static"));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryInterferenceDetectControl(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryInterferenceDetectControlCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryInterferenceDetectControlCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryInterferenceDetectControlCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryInterferenceDetectControlCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Interference Detect Control Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Interference Control Detect: %s", ((ackCmd[6]) ? "Enable" : "Disable"));
		add_msgtolist(strMsg);
		strMsg.Format("Interference Status: ");
		if(0==ackCmd[7])
		{
			strMsg.Append("Unknown");
		}
		else if(1==ackCmd[7])
		{
			strMsg.Append("No Interference");
		}
		else if(2==ackCmd[7])
		{
			strMsg.Append("Lite");
		}
		else 
		{
			strMsg.Append("Critical");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaBinaryOutputDestination(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaBinaryOutputDestinationCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNmeaBinaryOutputDestinationCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Binary Output Dest. Successful...";
		add_msgtolist(strMsg);
		if(ackCmd[7] & 0x0001)
		{
			add_msgtolist("UART Port");
		}
		if(ackCmd[7] & 0x0002)
		{
			add_msgtolist("SPI Slave Port");
		}
		if(ackCmd[7] & 0x0004)
		{
			add_msgtolist("I2C Slave Port");
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryParameterSearchEngineNumber(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryParameterSearchEngineNumberCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryParameterSearchEngineNumberCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryParameterSearchEngineNumberCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryParameterSearchEngineNumberCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Param. Search Engine Num. Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Search Engine Mode: ");
		if(0==ackCmd[6])
		{
			strMsg.Append("Default");
		}
		else if(1==ackCmd[6])
		{
			strMsg.Append("Low");
		}
		else if(2==ackCmd[6])
		{
			strMsg.Append("Middle");
		}
		else if(3==ackCmd[6])
		{
			strMsg.Append("High");
		}
		else if(4==ackCmd[6])
		{
			strMsg.Append("Full");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssKnumberSlotCnr2(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssKnumberSlotCnr2Cmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssKnumberSlotCnr2Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GLONASS K-Num, Slot, CNR Successful...";
		add_msgtolist(strMsg);
		for(int i=0; i<ackCmd[6]; ++i)
		{
			strMsg.Format("K-Num=%d, Slot=%d, CNR=%d", (int)(char)ackCmd[7+i*3], ackCmd[8+i*3], ackCmd[9+i*3]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryAgpsStatus(CmdExeMode nMode, void* outputData)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	BinaryCommand cmd(cmdTable[QueryAgpsStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryAgpsStatusCmd].cmdId);
	cmd.SetU08(2, HIBYTE(now.wYear));
	cmd.SetU08(3, LOBYTE(now.wYear));
	cmd.SetU08(4, (U08)now.wMonth);
	cmd.SetU08(5, (U08)now.wDay);
	cmd.SetU08(6, (U08)now.wHour);
	cmd.SetU08(7, (U08)now.wMinute);
	cmd.SetU08(8, (U08)now.wSecond);

	BinaryData ackCmd;
	CmdErrorCode ret = ExcuteBinaryCommand(QueryAgpsStatusCmd, &cmd, &ackCmd);
	if(Return==nMode)
	{
		return ret;
	}

	if(!ret)
	{
		CString strMsg = "---------  AGPS Status  ---------";
		add_msgtolist(strMsg);
		strMsg.Format("Aiding days: %d days %d hours", MAKEWORD(ackCmd[5], ackCmd[6]) / 24, MAKEWORD(ackCmd[5], ackCmd[6]) % 24);
		add_msgtolist(strMsg);
		strMsg.Format("AGPS: %s", (ackCmd[7]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
	}
	return ret;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssSelectionForNavigationSystem2(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssSelectionForNavigationSystem2Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssSelectionForNavigationSystem2Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssSelectionForNavigationSystem2Cmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssSelectionForNavigationSystem2Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Gnss Sel for Nav System Successful...";
		add_msgtolist(strMsg);
		strMsg = "GNSS Nav Sel: ";

		if(ackCmd[6]==0)
		{
			strMsg.Append("None");
		}
		else if(ackCmd[6]==1)
		{
			strMsg.Append("GPS Only");
		}
		else if(ackCmd[6]==2)
		{
			strMsg.Append("Glonass Only");
		}
		else if(ackCmd[6]==3)
		{
			strMsg.Append("Both GPS and Glonass");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssNmeaTalkId(CmdExeMode nMode, void* outputData)
{
	BinaryCommand cmd(cmdTable[QueryGnssNmeaTalkIdCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssNmeaTalkIdCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssNmeaTalkIdCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Gnss NMEA Talk ID Successful...";
		add_msgtolist(strMsg);
		strMsg = "NMEA Talk ID: ";

		if(ackCmd[5]==0)
		{
			strMsg.Append("GP Mode");
		}
		else if(ackCmd[5]==1)
		{
			strMsg.Append("GN Mode");
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDatalogLogStatus(CmdExeMode nMode, void* outputData)
{
	SYSTEMTIME	now; 
	GetSystemTime(&now);

	BinaryCommand cmd(cmdTable[QueryDatalogLogStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDatalogLogStatusCmd].cmdId);

	BinaryData ackCmd;
	CmdErrorCode ret = ExcuteBinaryCommand(QueryDatalogLogStatusCmd, &cmd, &ackCmd);
	if(Return==nMode)
	{
		return ret;
	}

	if(!ret)
	{
		LogFlashInfo1 logInfo = {0};
//		memcpy(&logInfo.log_flash_current_prt, &ackCmd[5], sizeof(U32));
		logInfo.log_flash_current_prt = MAKELONG(MAKEWORD(ackCmd[5], ackCmd[6]), MAKEWORD(ackCmd[7], ackCmd[8]));
//		memcpy(&logInfo.sector_left, &ackCmd[9], sizeof(U16));
		logInfo.sector_left = MAKEWORD(ackCmd[9], ackCmd[10]);
//		memcpy(&logInfo.total_sector, &ackCmd[11], sizeof(U16));
		logInfo.total_sector = MAKEWORD(ackCmd[11], ackCmd[12]);
//		memcpy(&logInfo.max_time, &ackCmd[13], sizeof(U32));
		logInfo.max_time = MAKELONG(MAKEWORD(ackCmd[13], ackCmd[14]), MAKEWORD(ackCmd[15], ackCmd[16]));
//		memcpy(&logInfo.min_time, &ackCmd[17], sizeof(U32));
		logInfo.min_time = MAKELONG(MAKEWORD(ackCmd[17], ackCmd[18]), MAKEWORD(ackCmd[19], ackCmd[20]));
//		memcpy(&logInfo.max_distance, &ackCmd[21], sizeof(U32));
		logInfo.max_distance = MAKELONG(MAKEWORD(ackCmd[21], ackCmd[22]), MAKEWORD(ackCmd[23], ackCmd[24]));
//		memcpy(&logInfo.min_distance, &ackCmd[25], sizeof(U32));
		logInfo.min_distance = MAKELONG(MAKEWORD(ackCmd[25], ackCmd[26]), MAKEWORD(ackCmd[27], ackCmd[28]));
//		memcpy(&logInfo.max_speed, &ackCmd[29], sizeof(U32));
		logInfo.max_speed = MAKELONG(MAKEWORD(ackCmd[29], ackCmd[30]), MAKEWORD(ackCmd[31], ackCmd[32]));
//		memcpy(&logInfo.min_speed, &ackCmd[33], sizeof(U32));
		logInfo.min_speed = MAKELONG(MAKEWORD(ackCmd[33], ackCmd[34]), MAKEWORD(ackCmd[35], ackCmd[36]));
//		memcpy(&logInfo.datalog_enable, &ackCmd[37], sizeof(U08));
		logInfo.datalog_enable = ackCmd[37];
//		memcpy(&logInfo.fifo_mode, &ackCmd[38], sizeof(U08));
		logInfo.fifo_mode = ackCmd[38];
#if DATA_POI
//		memcpy(&logInfo.poi_entry, &ackCmd[39], sizeof(U32));
		logInfo.poi_entry = MAKELONG(MAKEWORD(ackCmd[39], ackCmd[40]), MAKEWORD(ackCmd[41], ackCmd[42]));
//		memcpy(&logInfo.autolog_full, &ackCmd[43], sizeof(U08));
		logInfo.autolog_full = ackCmd[43];
//		memcpy(&logInfo.poi_full, &ackCmd[44], sizeof(U08));
		logInfo.poi_full = ackCmd[44];
#endif

		add_msgtolist("Get Log Status Successful...");
		add_msgtolist("---------  Log Status  ---------");
		CString strMsg;
		if(logInfo.total_sector)
		{
			if(logInfo.sector_left == 0x0)
			{
				strMsg = "Sector Full!";
			}
			else if((logInfo.sector_left &0x80000000) != 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Circular Sector left: %d / %d", logInfo.sector_left, logInfo.total_sector);
			}
			else if((logInfo.sector_left &0x80000000) == 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Sector left: %d / %d", logInfo.sector_left, logInfo.total_sector);
			}
		}
		else
		{
			if(logInfo.sector_left == 0x0)
			{
				strMsg = "Sector Full!";
			}
			else if((logInfo.sector_left &0x80000000) != 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Circular Sector left: %d", logInfo.sector_left);
			}
			else if((logInfo.sector_left &0x80000000) == 0)
			{
				logInfo.sector_left = logInfo.sector_left << 1 >> 1;
				strMsg.Format("Sector left: %d", logInfo.sector_left);
			}
		}
		add_msgtolist(strMsg);
		strMsg.Format("max T: %d, min T: %d", logInfo.max_time, logInfo.min_time);
		add_msgtolist(strMsg);
		strMsg.Format("max D: %d, min D: %d", logInfo.max_distance, logInfo.min_distance);
		add_msgtolist(strMsg);
		strMsg.Format("max V: %d, min V: %d", logInfo.max_speed, logInfo.min_speed);
		add_msgtolist(strMsg);
		strMsg.Format("Datalog: %s", (logInfo.datalog_enable) ? "Enable" : "Disable");
		add_msgtolist(strMsg);
		strMsg.Format("FIFO mode: %s", (logInfo.fifo_mode) ? "Circular" : "Oneway");
		
		if(DATA_POI && TWIN_DATALOG)
		{
			strMsg.Format("POI entry Datalogger1 : %d", logInfo.poi_entry & 0xffff);
			add_msgtolist(strMsg);
			strMsg.Format("POI entry Datalogger2 : %d", logInfo.poi_entry >> 16);
			add_msgtolist(strMsg);
			strMsg.Format("Autolog full : %d", logInfo.autolog_full);
			add_msgtolist(strMsg);
			strMsg.Format("POIlog full : %d", logInfo.poi_full);
			add_msgtolist(strMsg);
		}
		else if(DATA_POI && !TWIN_DATALOG)
		{
			strMsg.Format("POI entry Datalogger1 : %d", logInfo.poi_entry);
			add_msgtolist(strMsg);
			strMsg.Format("Autolog full : %d", logInfo.autolog_full);
			add_msgtolist(strMsg);
			strMsg.Format("POIlog full : %d", logInfo.poi_full);
			add_msgtolist(strMsg);
		}
	}
	return ret;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRegister(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRegisterCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRegisterCmd].cmdId);
	cmd.SetU08(2, HIBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(3, LOBYTE(HIWORD(m_regAddress)));
	cmd.SetU08(4, HIBYTE(LOWORD(m_regAddress)));
	cmd.SetU08(5, LOBYTE(LOWORD(m_regAddress)));

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryRegisterCmd, &cmd, &ackCmd))
	{
		U32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
		if(nMode==Return)
		{
			*((U32*)outputData) = data;
			return Ack;
		}
		CString strMsg;
		strMsg.Format("Get Register in 0x%08X", m_regAddress);
		add_msgtolist(strMsg);
		strMsg.Format("0x%08X (%d)", data, data);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryPositionFixNavigationMask(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryPositionFixNavigationMaskCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryPositionFixNavigationMaskCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryPositionFixNavigationMaskCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryPositionFixNavigationMaskCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Position Fix Navigation Mask Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("First fix navigation mask : %s", (ackCmd[6]) ? "2D" : "3D");
		add_msgtolist(strMsg);
		strMsg.Format("Subsequent fix navigation mask : %s", (ackCmd[7]) ? "2D" : "3D");
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNavigationMode(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNavigationModeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNavigationModeCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNavigationModeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Navigation Mode Successful";
		add_msgtolist(strMsg);
		strMsg.Format("Navigation Mode : %d", ackCmd[5]);
		switch(ackCmd[5])
		{
		case 0:
			strMsg.SetString("Navigation Mode: Car");
			break;
		case 1:
			strMsg.SetString("Navigation Mode: Pedestrain");
			break;
		case 2:
			strMsg.SetString("Navigation Mode: Bike");
			break;
		case 3:
			strMsg.SetString("Navigation Mode: Marine");
			break;
		case 4:
			strMsg.SetString("Navigation Mode: Balloon");
			break;
		case 5:
			strMsg.SetString("Navigation Mode: Portable");
			break;
		case 6:
			strMsg.SetString("Navigation Mode: Airborne");
			break;
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNavigationModeV8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNavigationModeV8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNavigationModeV8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNavigationModeV8Cmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNavigationModeV8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Navigation Mode Successful";
		add_msgtolist(strMsg);
		switch(ackCmd[6])
		{
		case 0:
			strMsg.SetString("Navigation Mode: Auto");
			break;
		case 1:
			strMsg.SetString("Navigation Mode: Pedestrain");
			break;
		case 2:
			strMsg.SetString("Navigation Mode: Car");
			break;
		case 3:
			strMsg.SetString("Navigation Mode: Marine");
			break;
		case 4:
			strMsg.SetString("Navigation Mode: Balloon");
			break;
		case 5:
			strMsg.SetString("Navigation Mode: Airborne");
			break;
		default:
			strMsg.Format("Navigation Mode: %d", ackCmd[6]);
			break;
		}
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssBootStatus(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryGnssBootStatusCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssBootStatusCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssBootStatusCmd].cmdSubId);
	
	if(NoWait==nMode)
	{
		ExcuteBinaryCommandNoWait(QueryGnssBootStatusCmd, &cmd);
		return Timeout;
	}
	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssBootStatusCmd, &cmd, &ackCmd))
	{
		if(nMode == Return)
		{
			*((U32*)outputData) = MAKEWORD(ackCmd[6], ackCmd[7]);
			return Ack;
		}

		CString strMsg = "Get GNSS ROM Boot Status Successful";
		add_msgtolist(strMsg);
		switch(ackCmd[6])
		{
		case 0:
			strMsg.SetString("Status : Boot flash OK");
			break;
		case 1:
			strMsg.SetString("Status : Fail over to boot from ROM");
			break;
		default:
			strMsg.Format("Status : %d", ackCmd[6]);
			break;
		}
		add_msgtolist(strMsg);
		switch(ackCmd[7])
		{
		case 0:
			strMsg.SetString("Flash Type : ROM");
			break;
		case 1:
			strMsg.SetString("Flash Type : QSPI Flash Type 1");		//Winbond-type 
			break;
		case 2:
			strMsg.SetString("Flash Type : QSPI Flash Type 2");		// EON-type 
			break;
		case 4:
			strMsg.SetString("Flash Type : Parallel Flash");
			break;
		default:
			strMsg.Format("Flash Type : %d", ackCmd[7]);
			break;
		}
		add_msgtolist(strMsg);	
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryDrMultiHz(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryDrMultiHzCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryDrMultiHzCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryDrMultiHzCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryDrMultiHzCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Get DR Multi-Hz Successful";
		strMsg.Format("DR Update Rate = %d Hz", ackCmd[6]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaIntervalV8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNmeaIntervalV8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaIntervalV8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaIntervalV8Cmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNmeaIntervalV8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Message Interval Successful";
		add_msgtolist(strMsg);
		if(ackCmd[6])
		{
			strMsg.Format("GGA Interval : %d second(s)", ackCmd[6]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[7])
		{
			strMsg.Format("GSA Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GSV Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GLL Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("RMC Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[11])
		{
			strMsg.Format("VTG Interval : %d second(s)", ackCmd[11]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[12])
		{
			strMsg.Format("ZDA Interval : %d second(s)", ackCmd[12]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[13])
		{
			strMsg.Format("GNS Interval : %d second(s)", ackCmd[13]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[14])
		{
			strMsg.Format("GBS Interval : %d second(s)", ackCmd[14]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[15])
		{
			strMsg.Format("GRS Interval : %d second(s)", ackCmd[15]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[16])
		{
			strMsg.Format("DTM Interval : %d second(s)", ackCmd[16]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[17])
		{
			strMsg.Format("GST Interval : %d second(s)", ackCmd[17]);
			add_msgtolist(strMsg);
		}

	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryNmeaInterval2V8(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryNmeaInterval2V8Cmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryNmeaInterval2V8Cmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryNmeaInterval2V8Cmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryNmeaInterval2V8Cmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query NMEA Message Interval Successful";
		add_msgtolist(strMsg);
		if(ackCmd[7])
		{
			strMsg.Format("GGA Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GSA Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GSV Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("GPtps Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[11])
		{
			strMsg.Format("GPanc Interval : %d second(s)", ackCmd[11]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryEricssonInterval(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryEricssonIntervalCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryEricssonIntervalCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryEricssonIntervalCmd].cmdSubId);
	cmd.SetU08(3, 0x02);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryEricssonIntervalCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Ericsson Sentence Interval Successful";
		add_msgtolist(strMsg);
		if(ackCmd[7])
		{
			strMsg.Format("GPppr Interval : %d second(s)", ackCmd[7]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[8])
		{
			strMsg.Format("GPsts Interval : %d second(s)", ackCmd[8]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[9])
		{
			strMsg.Format("GPver Interval : %d second(s)", ackCmd[9]);
			add_msgtolist(strMsg);
		}
		if(ackCmd[10])
		{
			strMsg.Format("GPavp Interval : %d second(s)", ackCmd[10]);
			add_msgtolist(strMsg);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryChannelDoppler(CmdExeMode nMode, void* outputData)
{	    
	const int ChannelCount = GSA_MAX_SATELLITE;
	CString strMsg("PRN, DOPPLER");
	if(nMode==Display)
	{
		add_msgtolist(strMsg);
	}
	for(int i=0; i<ChannelCount; ++i)
	{
		BinaryCommand cmd(cmdTable[QueryChannelDopplerCmd].cmdSize);
		cmd.SetU08(1, cmdTable[QueryChannelDopplerCmd].cmdId);
		cmd.SetU08(2, (U08)i);

		BinaryData ackCmd;
		if(!ExcuteBinaryCommand(QueryChannelDopplerCmd, &cmd, &ackCmd))
		{
			U32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
			S16 prn = MAKEWORD(ackCmd[6], ackCmd[5]);
			S16 doppler = MAKEWORD(ackCmd[8], ackCmd[7]);
			if(nMode==Return)
			{
				int* ChannelTable = (int*)outputData;
				ChannelTable[i] = prn;
			}
			else if(nMode==Display)
			{
				strMsg.Format(" %02d, %d", prn, doppler);
				add_msgtolist(strMsg);
			}
		}
	}
	if(nMode==Return)
	{
		return Ack;
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryClockOffset(CmdExeMode nMode, void* outputData)
{	    
	const int ChannelCount = GSA_MAX_SATELLITE;
	CString strMsg("PRN, Clock Offset, ppm");
	if(nMode==Display)
	{
		add_msgtolist(strMsg);
	}
	int* ChannelTable = (int*)outputData;

	int totalCount = 0;
	int totalOffset = 0;
	for(int i=0; i<ChannelCount; ++i)
	{
		if(-1==ChannelTable[i])
		{
			continue;
		}
		BinaryCommand cmd(cmdTable[QueryChannelClockOffsetCmd].cmdSize);
		cmd.SetU08(1, cmdTable[QueryChannelClockOffsetCmd].cmdId);
		cmd.SetU08(6, HIBYTE(ChannelTable[i]));
		cmd.SetU08(7, LOBYTE(ChannelTable[i]));

		BinaryData ackCmd;
		if(!ExcuteBinaryCommand(QueryChannelClockOffsetCmd, &cmd, &ackCmd))
		{
			S32 data = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]), MAKEWORD(ackCmd[6], ackCmd[5]));
			strMsg.Format(" %3d, %5d, %2.6fppm", ChannelTable[i], data, data / (96.25 * 16.367667));
			add_msgtolist(strMsg);
			++totalCount;
			totalOffset += data;
		}
	}
	if(nMode==Display)
	{
		strMsg.Format("Avg : %5d, %2.6fppm", totalOffset / totalCount, (totalOffset / totalCount) / (96.25 * 16.367667));
		add_msgtolist(strMsg);
	}

	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryRefTimeSyncToGpsTime(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryRefTimeSyncToGpsTimeCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryRefTimeSyncToGpsTimeCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Ref Time Sync To Gps Time Successful";
		add_msgtolist(strMsg);
		strMsg.Format("Enable : %d", ackCmd[6]);
		add_msgtolist(strMsg);
		strMsg.Format("Date : %d/%d/%d", MAKEWORD(ackCmd[8], ackCmd[7]), ackCmd[9], ackCmd[10]);
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryGnssNavSol(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryGnssNavSolCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryGnssNavSolCmd].cmdId);
	cmd.SetU08(2, cmdTable[QueryGnssNavSolCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryGnssNavSolCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query GNSS constellation type Successful";
		add_msgtolist(strMsg);
		U16 mode = MAKEWORD(ackCmd[7], ackCmd[6]);
		add_msgtolist("Navigation Solution : ");

		CString strOutput;
		if(mode & 0x0001)
		{
			strOutput += "GPS + ";
		}
		if(mode & 0x0002)
		{
			strOutput += "GLONASS + ";
		}
		if(mode & 0x0004)
		{
			strOutput += "Galileo + ";
		}
		if(mode & 0x0008)
		{
			strOutput += "Beidou + ";
		}

		if(mode==0)
		{
			add_msgtolist("None");
		}
		else
		{
			strOutput = strOutput.Left(strOutput.GetLength() - 2);
			add_msgtolist(strOutput);
		}
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryCustomerID(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryCustomerIDCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryCustomerIDCmd].cmdId);

	BinaryData ackCmd;
	if(Return==nMode && outputData)
	{
		if(!ExcuteBinaryCommand(QueryCustomerIDCmd, &cmd, &ackCmd, 300, true))
		{
			*((U32*)outputData) = MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]),MAKEWORD(ackCmd[6], ackCmd[5]));
		}
		return Timeout;
	}
	if(!ExcuteBinaryCommand(QueryCustomerIDCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Customer ID Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Customer ID : %d",
			MAKELONG(MAKEWORD(ackCmd[8], ackCmd[7]),MAKEWORD(ackCmd[6], ackCmd[5])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::Query1ppsFreqencyOutput(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[Query1ppsFreqencyOutputCmd].cmdSize);
	cmd.SetU08(1, cmdTable[Query1ppsFreqencyOutputCmd].cmdId);
	cmd.SetU08(2, cmdTable[Query1ppsFreqencyOutputCmd].cmdSubId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(Query1ppsFreqencyOutputCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query 1PPS Freqency Output Successful...";
		add_msgtolist(strMsg);
		strMsg.Format("Freqency : %d",
			MAKELONG(MAKEWORD(ackCmd[9], ackCmd[8]),MAKEWORD(ackCmd[7], ackCmd[6])));
		add_msgtolist(strMsg);
	}
	return Timeout;
}

CGPSDlg::CmdErrorCode CGPSDlg::QueryBinaryMeasurementDataOut(CmdExeMode nMode, void* outputData)
{	    
	BinaryCommand cmd(cmdTable[QueryBinaryMeasurementDataOutCmd].cmdSize);
	cmd.SetU08(1, cmdTable[QueryBinaryMeasurementDataOutCmd].cmdId);

	BinaryData ackCmd;
	if(!ExcuteBinaryCommand(QueryBinaryMeasurementDataOutCmd, &cmd, &ackCmd))
	{
		CString strMsg = "Query Binary Measurement Data Out Successful...";
		add_msgtolist(strMsg);
		if(ackCmd[5]==0)
		{
			strMsg.Format("Output Rate : 1Hz");	
		}
		else if(ackCmd[5]==1)
		{
			strMsg.Format("Output Rate : 2Hz");
		}
		else if(ackCmd[5]==2)
		{
			strMsg.Format("Output Rate : 4Hz");
		}
		else if(ackCmd[5]==3)
		{
			strMsg.Format("Output Rate : 5Hz");
		}
		else if(ackCmd[5]==4)
		{
			strMsg.Format("Output Rate : 10Hz");
		}
		else
		{
			strMsg.Format("Output Rate : 20Hz");
		}
		add_msgtolist(strMsg);

		strMsg.Format("Meas Time : %s", (ackCmd[6]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);

		strMsg.Format("Raw Meas : %s", (ackCmd[7]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);

		strMsg.Format("SV CH Status : %s", (ackCmd[8]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);

		strMsg.Format("RCV State : %s", (ackCmd[9]) ? "Enable" : "Disable");
		add_msgtolist(strMsg);

		CString strOutput;
		if(ackCmd[10] & 0x01)
		{
			strOutput += "GPS + ";
		}
		if(ackCmd[10] & 0x02)
		{
			strOutput += "GLONASS + ";
		}
		if(ackCmd[10] & 0x04)
		{
			strOutput += "Galileo + ";
		}
		if(ackCmd[10] & 0x08)
		{
			strOutput += "Beidou + ";
		}

		if(ackCmd[10]==0)
		{
			add_msgtolist("None");
		}
		else
		{
			add_msgtolist("Subframe for different constellation :");
			strOutput = strOutput.Left(strOutput.GetLength() - 2);
			add_msgtolist(strOutput);
		}

	}
	return Timeout;
}

UINT SetFacMsgThread(LPVOID pParam)
{	
	CGPSDlg::gpsDlg->ExecuteConfigureCommand(CGPSDlg::m_inputMsg, 9, "Configure Successful...");
	return 0;
}

void CGPSDlg::SetFactoryDefault(bool isReboot)
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode = 0;	
	memset(m_inputMsg, 0, 9);   		    
	m_inputMsg[0]=(U08)0xa0;
	m_inputMsg[1]=(U08)0xa1;
	m_inputMsg[2]=0;
	m_inputMsg[3]=2;
	m_inputMsg[4]=4; //msgid
	if(isReboot)
	{
		m_inputMsg[5]=1;
		m_inputMsg[6]=5; //checksum right	 
	}
	else
	{
		m_inputMsg[5]=0;
		m_inputMsg[6]=4; //checksum right	 
	}
	m_inputMsg[7]=(U08)0x0d;
	m_inputMsg[8]=(U08)0x0a;
	slgsv = 6;
	AfxBeginThread(SetFacMsgThread, 0);
}

void CGPSDlg::OnSetFactoryDefaultNoReboot()
{		
	SetFactoryDefault(false);
}

void CGPSDlg::OnSetFactoryDefaultReboot()
{	
	SetFactoryDefault(true);
}

void CGPSDlg::OnConfigureoutputmessagetypeNooutput()
{	
	if(!CheckConnect())return;
	m_inputMode = 0;	
	memset(m_inputMsg, 0, 9);   		    
	m_inputMsg[0]=(U08)0xa0;
	m_inputMsg[1]=(U08)0xa1;
	m_inputMsg[2]=0;
	m_inputMsg[3]=2;
	m_inputMsg[4]=9; //msgid
	m_inputMsg[5]=0;
	m_inputMsg[6]=9; //checksum right	    
	m_inputMsg[7]=(U08)0x0d;
	m_inputMsg[8]=(U08)0x0a;
	SetMsgType(Nooutput_Mode);
	m_no_output.EnableWindow(0);
	m_nmea0183msg.EnableWindow(1);
	m_binarymsg.EnableWindow(1);
	AfxBeginThread(SetFacMsgThread,0);
	//InvalidateRect(CRect(30,64,320,114),TRUE);	
}

void CGPSDlg::OnConfigureoutputmessagetypeNmeamessage()
{	
	if(!CheckConnect())return;
	m_inputMode = 0;	
	memset(m_inputMsg, 0, 9);   		    
	m_inputMsg[0]=(U08)0xa0;
	m_inputMsg[1]=(U08)0xa1;
	m_inputMsg[2]=0;
	m_inputMsg[3]=2;
	m_inputMsg[4]=9; //msgid
	m_inputMsg[5]=1;
	m_inputMsg[6]=8; //checksum right	    
	m_inputMsg[7]=(U08)0x0d;
	m_inputMsg[8]=(U08)0x0a;
	SetMsgType(NMEA_Mode);
	m_no_output.EnableWindow(1);
	m_nmea0183msg.EnableWindow(0);
	m_binarymsg.EnableWindow(1);
	AfxBeginThread(SetFacMsgThread,0);
}

void CGPSDlg::OnConfigureoutputmessagetypeBinarymessage()
{		
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	memset(m_inputMsg, 0, 9);   		    
	m_inputMsg[0]=(U08)0xa0;
	m_inputMsg[1]=(U08)0xa1;
	m_inputMsg[2]=0;
	m_inputMsg[3]=2;
	m_inputMsg[4]=9; //msgid
	m_inputMsg[5]=2;
	m_inputMsg[6]=11; //checksum right	    
	m_inputMsg[7]=(U08)0x0d;
	m_inputMsg[8]=(U08)0x0a;
	SetMsgType(Binary_Mode);
	m_no_output.EnableWindow(1);
	m_nmea0183msg.EnableWindow(1);
	m_binarymsg.EnableWindow(0);
	AfxBeginThread(SetFacMsgThread,0);
}

void CGPSDlg::OnBinaryConfigurenmeaoutput()
{   
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode  = 0;
	CConNMEADlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureNmeaIntervalV8()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode  = 0;

	CConfigNmeaIntervalDlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureEricssonSentecneInterval()
{
	if(!CheckConnect())
	{
		return;
	}
	m_inputMode  = 0;

	ConfigEricssonIntervalDlg dlg;
	if(dlg.DoModal() != IDOK)		
	{
		SetMode();  
		CreateGPSThread();
	}
}
void CGPSDlg::OnBinaryConfiguredatum()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode  = 0;
	CConDauDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfiguredopmask()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode  = 0;
	CConDOPDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfigureelevationmask()
{	
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode  = 0;
	CConEleDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnBinaryConfigurebinarydata()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode  = 0;	
	CConBinOutDlg dlg;
	if(dlg.DoModal() != IDOK)
	{
		SetMode();  
		CreateGPSThread();
	}
}

void CGPSDlg::OnConfigureSerialPort()
{
	if(!CheckConnect())
	{
		return;
	}

	m_inputMode = 0;	
	CConSrePorDlg dlg;
	if(dlg.DoModal() != IDOK) 	
	{
		SetMode();  
		CreateGPSThread();
	}
}