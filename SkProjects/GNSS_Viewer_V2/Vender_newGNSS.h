#pragma once

#define SW_FUN_DATALOG			0x0001
#define SW_FUN_AGPS				0x0002
#define SW_FUN_DR				0x0004

// .17 Change Beidou Ephemeris format. Modify V8 AGPS Command.
// .18 Fix altitude parsing error when its value is negative.
// .18 Add warning message when download image is not exist.
// .18 [Set Ephemeris] will use the previous saved file automatically.
// .18 Earth View can hide satellite in the specified system.
// .19 Fix rmc speed display error.
// .19 Add setting dialog for adjust delay before binsize.
// .20 Add new download command, fix rmc speed display, set no nmea then download will no nmea output.
// .21 Mutex between [Configure Position Update Rate] and [Configure SAEE].
// .22 Show 12 Channel GPGSV info in theirs GNSS system.
// .23 Fix NMEA Parser error when it has two GNGSA statements.
// .24 Fix NMEA Parser error when it has no GPGGA statements (Unicore pure BD mode).
// .25 Fix [Query Power Saving Parameter] timeout issue.
// .26 Add tag loader process. 
// .26 Hide com port selection in [Configure Serial Port] dialog when using Customer Release version.
// .27 Add [Query NMEA Talker ID] in menu [Binary]. Modify VTG interval default to 1 second.
// .28 Support new loader srec BINSIZE / BINSIZ2 / BINSIZ3 command.
// .28 Fix lon, lat, alt dispear issue after reconnection.
// .29 Remove Query / Configure 1PPS NMEA Delay, and remove AGPS item in general version.
// .30 Add Query / Configure Power Saving Parameter(Rom), and modified Query / Configure Power Saving Parameter in Binary menu.
// .31 NMEA Player added PreprocessInputLine function for normalizing the nmea string.
// .32 Fixed Beidou nmea status bugs, the gsa information is not clear after reconnection.
// .33 Change some strings and menu items based on meeting conclusions. 
// .34 Change some strings and menu items based on meeting conclusions. 
// .35 Fixed Log Decompress and KML Conver open file dialog style. 
// .36 Added GNSS Doze Mode command in Venus 8.
// .37 Added SWCFG_VENDOR_GNSS_GENERAL_OLINKSTAR_PROTECT.
// .38 Modify CheckOlinkstarFirmware() rule.
// .39 Modify "Skytraq" string for customer.
// .39 Remove 1PPS clock pluse and modify Set GLONASS Time Correction.
// .40 Modify AGPS function in General version.
// .41 Add support add tag version for OLinkStart version
// .42 Remove "Skytraq" in About dialog.
// .43 Using "OLINKSTAR IMAGE" for external loader download.
// .44 Fixed CSerial::GetString in Prolific can't download issue.
// .45 Add [Configure / Query 1PPS Frequency Output].
// .46~.47 Add support add tag version for SWID version.
// .48 Add SPI download option and modify GetString time interval.
// .49 Add setting [Boost Get/Set Ephemeris] for disable baud rate change in Get / Set Ephemeris.
// .50 Modify [Query/Configure SBAS] function, using combo box for Ranging options.
// .51 Fix [Query Cable Delay] in wrong unit issue.
// .52 Add SWCFG_VENDOR_GNSS_GENERAL_FKHL, and new NMEA_PRN_TYPE define.
// .53 Fix Query Navigation Mode issue.
// .54 Fix ShowGPGSVmsg2 parsing bugs.
// .55 Add Ericsson customer branch.
// .56 Fix first nmea no output issue.
// .57 Add TIMING_MONITORING,TIMING_OUTPUT_ALIGN, SHOW_ELEV_AND_CNR_MASK_IN_GEN for customize version.

//#define SOFTWARE_FUNCTION		(SW_FUN_DATALOG | SW_FUN_AGPS | SW_FUN_DR)
#define SOFTWARE_FUNCTION		(SW_FUN_DATALOG | SW_FUN_AGPS)
#define IS_DEBUG				0
#define APP_CAPTION				"GNSS Viewer"
#define APP_VERSION				"2.0.057"
#define APP_TITLE				""
#define GNSS_VIEWER				0
#define GPS_VIEWER				0

enum CustomerId	{
	Sktyraq = 0x0000,
	OlinkStar = 0x4F4C,
	Ericsson = 0x0004,


};

//application init default
#define BAUDRATE				5		//5=115200,4=57600,3=38400,2=19200,1=9600,0=4800
//change baudrate default
#define BAUDRATE_DEFAULT		5		//5=115200,4=57600,3=38400,2=19200,1=9600,0=4800
//如果是藍芽要定義這個
//#define BLUE_TOOTH
#define FIRMWARE_DOWNLOAD			1
//#define CHECK_COM_REGISTER		0
//#define LIMIT_SCATTER_PLOT		1
#define MAX_SCATTER_COUNT			500

#define NOFIXED_CHECK				0
#define DRAW_COURSE					0

#define DATALOG_TRIP				0
#define ODOMETER_SUPPORT			0
#define BINARY_MESSAGE_SUPPORT		0
#define BINARY_MESSAGE_INTERVAL		0
//#define BINARY_MESSAGE_NI			1

#define NEW_ALMANAC				1		// normal almanac function
#define TMP_ALMANAC				0		// for andrew used only
#define ACTIVATE_MINIHOMER		0
#define NMEA_INPUT				0

//#define RESTORE_CLK_OFFSET	0

#define GSA_MAX_SATELLITE		12
//#define	GNSS_VIEWER			1
//PRE_LOADER defined the prom.bin is merged with smaller loader
//#define PRE_LOADER			1
//Download using resend protocol
#define RESEND_DOWNLOAD			1
//Display binary command in and ack in respond view.
#define _SHOW_BINARY_DATA_		1

//Using external SREC file directly, no prompt.
#define _ALWAYS_USE_EXTERNAL_SREC_	0
//Does not use pre-loader in download firmware.
#define _DIRECTLY_DOWNLOAD_		0
//Add a tag for DR Firmware.
#define _CREATE_LICENSE_TAG_	0
//for [Reset Motion Sensor] Command
#define RESET_MOTION_SENSOR		0
#define ECOMPASS_CALIBRATION	0
#define TIMING_MODE				0

//#define _V8_DOWNLOAD_			0
#define _BAUTRATE_IDX_6_		891200
#define PACIFIC					0
//#define SJA						0
#define OPEN_PINNING_RESERVE	0
#define RESET_MOTION_SENSOR		0
#define AVID					0
#define NI						0
#define SOARCOMM				0
#define LG						0
#define IQIBLA					0
#define _V8_SUPPORT				0
#define GG12A					0
//#define SUPPORT_CLEAR_LOGIN_PASSWORD	0
#define WITH_CONFIG_USB_BAUDRATE	0
#define SHOW_CLOCK_OFFSET		0
#define SHOW_NOISE				0
#define DATA_POI				0
#define TWIN_DATALOG			0
//#define _UNIT_TESTING_			1
#define _TEST_KERNEL_			0
#define _UNICORE_NMEA_			1
#define CUSTOMER_DOWNLOAD		0
#define CUSTOMER_ID				0x0000
#define NMEA_PRN_TYPE			0
#define TIMING_MONITORING	1
#define TIMING_OUTPUT_ALIGN	1
#define	SHOW_ELEV_AND_CNR_MASK_IN_GEN	0

#if defined(SWCFG_VENDOR_GNSS_GENERAL_OLINKSTAR_PROTECT)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef CUSTOMER_DOWNLOAD
 #undef CUSTOMER_ID

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"OLinkStar Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1
 #define CUSTOMER_DOWNLOAD		1
 #define CUSTOMER_ID			0x4F4C

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_ERICSSON)
//ERICSSON from 重慶西南集成電路設計
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef CUSTOMER_DOWNLOAD
 #undef CUSTOMER_ID
 #undef TIMING_MONITORING
 #undef TIMING_OUTPUT_ALIGN
 #undef SHOW_ELEV_AND_CNR_MASK_IN_GEN

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Ericsson Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1
 #define CUSTOMER_DOWNLOAD		0
 #define CUSTOMER_ID			0x0002
 #define TIMING_MONITORING	0
 #define TIMING_OUTPUT_ALIGN	0
 #define SHOW_ELEV_AND_CNR_MASK_IN_GEN	1

#elif defined(SWCFG_VENDOR_GNSS_GENERAL_SWID_PROTECT)
//SWID 重慶西南集成電路設計
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef CUSTOMER_DOWNLOAD
 #undef CUSTOMER_ID
 #undef TIMING_MONITORING
 #undef TIMING_OUTPUT_ALIGN
 #undef SHOW_ELEV_AND_CNR_MASK_IN_GEN

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"SWID Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1
 #define CUSTOMER_DOWNLOAD		0
 #define CUSTOMER_ID			0x0001
 #define TIMING_MONITORING	0
 #define TIMING_OUTPUT_ALIGN	0
 #define SHOW_ELEV_AND_CNR_MASK_IN_GEN 1

#elif defined(SWCFG_VENDOR_GPS_INTERNALUSE)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GPS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL

 #define APP_CAPTION			"GPS Viewer"
 #define APP_TITLE				"Internal Use"
 #define GPS_VIEWER				1
 #define IS_DEBUG				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1

#elif defined(SWCFG_VENDOR_GPS_DR_INTERNALUSE)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GPS_VIEWER
 #undef SOFTWARE_FUNCTION
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL

 #define APP_CAPTION			"GPS Viewer"
 #define APP_TITLE				"DR Internal Use"
 #define GPS_VIEWER				1
 #define SOFTWARE_FUNCTION		(SW_FUN_DATALOG | SW_FUN_AGPS | SW_FUN_DR)

 #define IS_DEBUG				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1

#elif defined(SWCFG_VENDOR_GNSS_INTERNALUSE)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
 #undef WITH_CONFIG_USB_BAUDRATE

 #define APP_CAPTION		"GNSS Viewer"
 #define APP_TITLE			"Internal Use"
 #define GNSS_VIEWER			1
 #define IS_DEBUG			1
 #define BAUDRATE			5
 #define BAUDRATE_DEFAULT		5
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 #define WITH_CONFIG_USB_BAUDRATE	1

#elif defined(SWCFG_VENDOR_GPS_GENERAL)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GPS_VIEWER
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL

 #define APP_CAPTION			"GPS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GPS_VIEWER				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1

#elif defined(SWCFG_VENDOR_GNSS_GENERAL)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GNSS_VIEWER
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef OPEN_PINNING_RESERVE
 #undef WITH_CONFIG_USB_BAUDRATE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GNSS_VIEWER			1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define OPEN_PINNING_RESERVE	1
 #define WITH_CONFIG_USB_BAUDRATE	1

#elif defined(SWCFG_VENDOR_GPS_V8)
 #undef APP_CAPTION	
 #undef APP_TITLE	
 #undef GPS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef OPEN_PINNING_RESERVE
 #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef TIMING_MODE
 //#undef _V8_DOWNLOAD_
 //#undef _BAUTRATE_IDX_6_
 #undef _V8_SUPPORT
 #undef BINARY_MESSAGE_INTERVAL

 #define APP_CAPTION			"GPS Viewer"
 #define APP_TITLE				"Internal Use"
 #define GPS_VIEWER				1
 #define IS_DEBUG				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define OPEN_PINNING_RESERVE	1
 #define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define TIMING_MODE			1
 //#define _V8_DOWNLOAD_			1
 //#define _BAUTRATE_IDX_6_		460800
 #define _V8_SUPPORT				1
 #define BINARY_MESSAGE_INTERVAL	1

#elif defined(SWCFG_VENDOR_GNSS_INTERNALUSE_BDV8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
// #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT

 #define APP_CAPTION			"GNSS Viewer"
#if(_TEST_KERNEL_)
 #define APP_TITLE				"Test Kernel"
#else
 #define APP_TITLE				"Internal Use"
#endif
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 //#define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1

#elif defined(SWCFG_VENDOR_GNSS_BDV8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
// #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 //#define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1

#elif defined(SWCFG_VENDOR_GNSS_FKHL)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
// #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef NMEA_PRN_TYPE

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Customer Release FKHL"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 //#define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1
 #define NMEA_PRN_TYPE			1

#elif defined(SWCFG_VENDOR_GNSS_BDV8_NMEAPLAYER)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef NMEA_INPUT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"NMEA Player"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				0
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		7
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1
 #define NMEA_INPUT				1

#elif defined(SWCFG_VENDOR_GNSS_V8)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
// #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Internal Use"
 #define GNSS_VIEWER			1
 #define IS_DEBUG				1
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		6
 #define TIMING_MODE			1
 #define OPEN_PINNING_RESERVE	1
 #define BINARY_MESSAGE_INTERVAL	1
 //#define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_	1
 #define _V8_SUPPORT			1

#elif defined(SWCFG_VENDOR_CREATETAG)
 #undef APP_CAPTION
 #undef APP_TITLE
 #undef GNSS_VIEWER
 #undef IS_DEBUG
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef TIMING_MODE
 #undef OPEN_PINNING_RESERVE
 #undef BINARY_MESSAGE_INTERVAL
// #undef _ALWAYS_USE_EXTERNAL_SREC_
 #undef _DIRECTLY_DOWNLOAD_
 #undef _V8_SUPPORT
 #undef _CREATE_LICENSE_TAG_

 #define APP_CAPTION			"GNSS Viewer"
 #define APP_TITLE				"Create Tag"
 #define GNSS_VIEWER				1
 #define IS_DEBUG					0
 #define BAUDRATE					5
 #define BAUDRATE_DEFAULT			7
 #define TIMING_MODE				1
 #define OPEN_PINNING_RESERVE		1
 #define BINARY_MESSAGE_INTERVAL	1
 //#define _ALWAYS_USE_EXTERNAL_SREC_	1
 #define _DIRECTLY_DOWNLOAD_		1
 #define _V8_SUPPORT				1
 #define _CREATE_LICENSE_TAG_		1

#elif defined(SWCFG_VENDOR_GENERAL_DR)
 #undef APP_TITLE				
 #undef BAUDRATE
 #undef BAUDRATE_DEFAULT
 #undef SOFTWARE_FUNCTION
 #undef OPEN_PINNING_RESERVE
 #undef WITH_CONFIG_USB_BAUDRATE

 #define APP_TITLE				"Customer Release DR"
 #define BAUDRATE				5
 #define BAUDRATE_DEFAULT		5
 #define OPEN_PINNING_RESERVE	1
 #define WITH_CONFIG_USB_BAUDRATE	1
 #define SOFTWARE_FUNCTION		(SW_FUN_DATALOG | SW_FUN_AGPS | SW_FUN_DR)

#else
 #error Unknow SWCFG_VENDER_TYPE!!!
#endif

#if ((GNSS_VIEWER || GPS_VIEWER)==0)
 #error Unknow GNSS_VIEWER or GPS_VIEWER!!!
#endif
