using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

// 組件的一般資訊是由下列的屬性集控制。
// 變更這些屬性的值即可修改組件的相關
// 資訊。
[assembly: AssemblyTitle("ModuleTestV8")]
[assembly: AssemblyDescription("")]
[assembly: AssemblyConfiguration("")]
[assembly: AssemblyCompany("")]
[assembly: AssemblyProduct("ModuleTestV8")]
[assembly: AssemblyCopyright("Copyright ©  2013")]
[assembly: AssemblyTrademark("")]
[assembly: AssemblyCulture("")]

// 將 ComVisible 設定為 false 會使得這個組件中的型別
// 對 COM 元件而言為不可見。如果您需要從 COM 存取這個組件中
// 的型別，請在該型別上將 ComVisible 屬性設定為 true。
[assembly: ComVisible(false)]

// 下列 GUID 為專案公開 (Expose) 至 COM 時所要使用的 typelib ID
[assembly: Guid("28386710-c315-42f3-9c0d-64c536f084f9")]

// 組件的版本資訊是由下列四項值構成:
//
//      主要版本
//      次要版本 
//      組建編號
//      修訂編號
//
// 您可以指定所有的值，也可以依照以下的方式，使用 '*' 將組建和修訂編號
// 指定為預設值:
// [assembly: AssemblyVersion("1.0.*")]
[assembly: AssemblyVersion("1.0.0.28")]
[assembly: AssemblyFileVersion("1.0.0.28")]

// 1.0.0.28 - Modify download function, boot from ROM code before downloading.
// 1.0.0.27 - Add retry in get clock offset.
// 1.0.0.27 - Add timeout check for i cache tester.
// 1.0.0.26 - Change CPU clock to pllclk/2 or pllclk/3.
// 1.0.0.25 - Adjust i-cache srec on / off PSE frequency.
// 1.0.0.24 - change i-cache srec test pattern.
// 1.0.0.23 - Add i-cache Tester branch.
// 1.0.0.22 - Fix write tag srec problem for OLink Star customize firmware.
// 1.0.0.21 - Show login setting in main windows in Reset Tester.
// 1.0.0.21 - Show correct error code list in Reset Tester.
// 1.0.0.21 - Change default value in Reset Tester.
// 1.0.0.20 - Remove Check Version in Module Test.
// 1.0.0.19 - Show NMEA Delay Detect in different message.
// 1.0.0.18 - ResetTester will show trigger time.
// 1.0.0.17 - NMEA Parser compatible with Olinkstar NMEA.
// 1.0.0.17 - Add check crc, and fixed compare snr error on beidou / glonass only use 2 satellites.
// 1.0.0.16 - Support tag download.
// 1.0.0.15 - Catalog log file by working number. Add period duration time to log file.
// 1.0.0.14 - Show error code number and retry three times in scan baud rate. Add slot yield information.
// 1.0.0.13 - It's only compare 2 satellites in Beidou.
// 1.0.0.12 - Add test report log in xml. Change Download button position.
// 1.0.0.11 - Show total time after download finished. Show download baud rate in main panel.
// 1.0.0.10 - Glonass snr only test 2 satellites.
// 1.0.0.09 - Add retry loop in first time command after open com port.
// 1.0.0.08 - Try Ready() function to wait serial port data come in.
// 1.0.0.07 - add download error processing.
// 1.0.0.06 - add log and report function.
// 1.0.0.05 - doesn't use -7 and 6 in k num to compare glonass snr.
// 1.0.0.04 - Compare glonass SNR by K-Number, Change icon.
// 1.0.0.03 - Add app / dialog icon, wait golden dialog, change setting dialog.
// 1.0.0.02 - Add clock offset test.
// 1.0.0.01 - first version.