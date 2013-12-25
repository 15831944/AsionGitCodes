using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace ModuleTestV8
{
    public class WorkerParam
    {
        [Flags] public enum ErrorType : ulong
        {
            NoError = 0,
            WaitingPositionFixTimeout = (ulong)1 << 1,
            GetClockOffsetFail = (ulong)1 << 2,
            ColdStartTimeOut = (ulong)1 << 3,
            ColdStartNack = (ulong)1 << 4,
            QueryVersionTimeOut = (ulong)1 << 5,
            QueryVersionNack = (ulong)1 << 6,
            QueryRtcTimeOut = (ulong)1 << 7,
            QueryRtcNack = (ulong)1 << 8,

            ConfigMessageOutputTimeOut = (ulong)1 << 9,
            ConfigMessageOutputNack = (ulong)1 << 10,
            ConfigNmeaOutputTimeOut = (ulong)1 << 11,
            ConfigNmeaOutputNack = (ulong)1 << 12,
            FactoryResetTimeOut = (ulong)1 << 13,
            FactoryResetNack = (ulong)1 << 14,

            FirmwareVersionError = (ulong)1 << 15,
            CheckRtcError = (ulong)1 << 16,
            GpsSnrError = (ulong)1 << 17,
            GlonassSnrError = (ulong)1 << 18,
            BeidouSnrError = (ulong)1 << 19,
            GetClockOffsetTimeout = (ulong)1 << 20,
            GetClockOffsetNack = (ulong)1 << 21,
            CheckClockOffsetFail = (ulong)1 << 22,
            SetPsti50Timeout = (ulong)1 << 23,
            SetPsti50Nack = (ulong)1 << 24,

            NoUsed = (ulong)1 << 25,

            DownloadCmdTimeOut = (ulong)1 << 26,
            DownloadCmdNack = (ulong)1 << 27,
            BinsizeCmdTimeOut = (ulong)1 << 28,
            DownloadEndTimeOut = (ulong)1 << 29,
            DownloadWriteFail = (ulong)1 << 30,

            TestNotComplete = (ulong)1 << 31,
            OpenPortFail = (ulong)1 << 32,

            ChangeBaudRateFail = (ulong)1 << 33,
            LoaderDownloadFail = (ulong)1 << 34,
            UploadLoaderFail = (ulong)1 << 35,
            UnsupportTagType = (ulong)1 << 36,
            QueryCrcTimeOut = (ulong)1 << 37,
            QueryCrcNack = (ulong)1 << 38,
            FirmwareCrcError = (ulong)1 << 39,

            TestIcacheError = (ulong)1 << 40,
            ResetDetectError = (ulong)1 << 41,
            NmeaDelayDetectError = (ulong)1 << 42,

            TestIcacheTimeout = (ulong)1 << 43,
            TestErr10 = (ulong)1 << 44,
        }
        public const int ErrorCount = 43;

        public static String GetErrorString(ErrorType er)
        {
            if (ErrorType.NoError == er)
            {
                return "";
            }

            StringBuilder sb = new StringBuilder();
            UInt64 nErr = (UInt64)er;
            bool first = true;
            for (byte i = 0; i < 64; i++)
            {
                UInt64 tt = nErr & ((UInt64)1 << i);

                if ((nErr & ((UInt64)1 << i)) != 0)
                {
                    if (!first)
                    {
                        sb.Append(", ");
                    }
                    sb.Append(i.ToString());
                    first = false;
                }
            }
            return sb.ToString();
        }

        public int index;
        public String comPort;
        public BackgroundWorker bw;
        public SkytraqGps gps;
        public GpsMsgParser parser;
        public ModuleTestProfile profile;
        public double gpSnrOffset;
        public double glSnrOffset;
        public double bdSnrOffset;
        public ErrorType error;
        //for Report
        //public DateTime startTime;
        public long duration;
        public StringBuilder log;
    }

    public class WorkerReportParam
    {

        public enum ReportType
        {
            ShowProgress,
            GoldenSampleReady,
            UpdateSnrChart,
            ShowError,
            ShowFinished,
            ShowWaitingGoldenSample,
            HideWaitingGoldenSample,
            AllTaskFinished,
        }

        public WorkerReportParam()
        {

        }

        public WorkerReportParam(WorkerReportParam r)
        {
            index = r.index;
            output = r.output;
            reportType = r.reportType;
        }
        public int index { get; set; }
        public String output { get; set; }
        //public ErrorType error { get; set; }
        public ReportType reportType { get; set; }
    }
}
