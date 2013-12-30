using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;
using System.Threading;
using System.Diagnostics;

namespace ModuleTestV8
{
    class TestModule
    {
       // private static byte boostBaudrateIndex = 5;      //115200
        const int DefaultCmdTimeout = 1000;
        public static GpsMsgParser.ParsingStatus[] dvResult;
        public static UInt32 gdClockOffset = 0;
        public static void ClearResult()
        {
            if (dvResult == null)
            {
                return;
            }

            for (int i = 0; i < Form1.ModuleCount; i++)
            {
                dvResult[i].ClearAllSate();
                dvResult[i].positionFixResult = 0;
            }        
        }

        public TestModule()
        {
            if(dvResult == null)
            {
                dvResult = new GpsMsgParser.ParsingStatus[Form1.ModuleCount];
                for (int i = 0; i < Form1.ModuleCount; i++)
                {
                    dvResult[i] = new GpsMsgParser.ParsingStatus();
                }
            }
        }

        const int GpsCompareCount = 3;
        const int GlonassCompareCount = 2;
        const int BeidouCompareCount = 2;
        private bool CompareResult(WorkerParam p, ref WorkerReportParam r, GpsMsgParser.SateType t)
        {
            GpsMsgParser.ParsingStatus.sateInfo[] dvSate = null;
            int snrLimit = 0;
            String usingSnrTxt;
            String dvSnrTxt;
            String gdSnrTxt;
            String dvSnrAvgTxt;
            String gdSnrAvgTxt;
            String gdSnrLimitErrorTxt;
            String dvSnrLimitErrorTxt;
            //int passSel = 0;
            int testSnrUpper = 0;
            int testSnrLower = 0;
            String dvTestPassTxt;
            double snrOffset = 0.0;
            int CompareCount = 0;
            switch (t)
            {
                case GpsMsgParser.SateType.Gps:
                    dvSate = dvResult[p.index].GetSortedGpsSateArray();
                    snrLimit = p.profile.gpSnrLimit;
                    usingSnrTxt = "Using GPS PRN : ";
                    dvSnrTxt = "Device GPS SNR : ";
                    gdSnrTxt = "Golden GPS SNR : ";
                    dvSnrAvgTxt = "Device average GPS SNR" + "(" + p.gpSnrOffset.ToString() + ") : ";
                    gdSnrAvgTxt = "Golden average GPS SNR : ";
                    gdSnrLimitErrorTxt = "Golden sample GPS SNR over the limit.";
                    dvSnrLimitErrorTxt = "Device sample GPS SNR over the limit.";
                    //passSel = p.profile.gpPassSel;
                    testSnrUpper = p.profile.gpSnrUpper;
                    testSnrLower = p.profile.gpSnrLower;
                    dvTestPassTxt = "Device GPS SNR test pass.";
                    snrOffset = p.gpSnrOffset;
                    CompareCount = GpsCompareCount;
                    break;
                case GpsMsgParser.SateType.Glonass:
                    dvSate = dvResult[p.index].GetSortedGlonassSateArray();
                    snrLimit = p.profile.glSnrLimit;
                    usingSnrTxt = "Using Glonass PRN : ";
                    dvSnrTxt = "Device Glonass SNR : ";
                    gdSnrTxt = "Golden Glonass SNR : ";
                    dvSnrAvgTxt = "Device average Glonass SNR" + "(" + p.glSnrOffset.ToString() + ") : ";
                    gdSnrAvgTxt = "Golden average Glonass SNR : ";
                    gdSnrLimitErrorTxt = "Golden sample Glonass SNR over the limit.";
                    dvSnrLimitErrorTxt = "Device sample Glonass SNR over the limit.";
                    //passSel = p.profile.glPassSel;
                    testSnrUpper = p.profile.glSnrUpper;
                    testSnrLower = p.profile.glSnrLower;
                    dvTestPassTxt = "Device Glonass SNR test pass.";
                    snrOffset = p.glSnrOffset;
                    CompareCount = GlonassCompareCount;
                    break;
                case GpsMsgParser.SateType.Beidou:
                    dvSate = dvResult[p.index].GetSortedBeidouSateArray();
                    snrLimit = p.profile.bdSnrLimit;
                    usingSnrTxt = "Using Beidou PRN : ";
                    dvSnrTxt = "Device Beidou SNR : ";
                    gdSnrTxt = "Golden Beidou SNR : ";
                    dvSnrAvgTxt = "Device average Beidou SNR" + "(" + p.bdSnrOffset.ToString() + ") : ";
                    gdSnrAvgTxt = "Golden average Beidou SNR : ";
                    gdSnrLimitErrorTxt = "Golden sample Beidou SNR over the limit.";
                    dvSnrLimitErrorTxt = "Device sample Beidou SNR over the limit.";
                    //passSel = p.profile.bdPassSel;
                    testSnrUpper = p.profile.bdSnrUpper;
                    testSnrLower = p.profile.bdSnrLower;
                    dvTestPassTxt = "Device Beidou test pass.";
                    snrOffset = p.bdSnrOffset;
                    CompareCount = BeidouCompareCount;
                    break;
                default:
                    dvSate = dvResult[p.index].GetSortedGpsSateArray();
                    snrLimit = p.profile.gpSnrLimit;
                    usingSnrTxt = "Using GPS PRN : ";
                    dvSnrTxt = "Device GPS SNR : ";
                    gdSnrTxt = "Golden GPS SNR : ";
                    dvSnrAvgTxt = "Device average GPS SNR" + "(" + p.gpSnrOffset.ToString() + ") : ";
                    gdSnrAvgTxt = "Golden average GPS SNR : ";
                    gdSnrLimitErrorTxt = "Golden sample Gps SNR over the limit.";
                    dvSnrLimitErrorTxt = "Device sample Gps SNR over the limit.";
                    //passSel = p.profile.gpPassSel;
                    testSnrUpper = p.profile.gpSnrUpper;
                    testSnrLower = p.profile.gpSnrLower;
                    dvTestPassTxt = "Device Gps SNR test pass.";
                    snrOffset = p.gpSnrOffset;
                    CompareCount = GpsCompareCount;
                    break;
            }

            if (dvSate[2].snr <= 0 || dvSate[1].snr <= 0 || dvSate[0].snr <= 0)
            {
                //return false;
            }

            GpsMsgParser.ParsingStatus.sateInfo[] selGd = new GpsMsgParser.ParsingStatus.sateInfo[CompareCount];
            GpsMsgParser.ParsingStatus.sateInfo[] selDv = new GpsMsgParser.ParsingStatus.sateInfo[CompareCount];
            int idxGd = 0;
            int idxDv = 0;
            do
            {
                if (dvSate[idxDv].snr < 0)
                {
                    break;
                }

                if (t== GpsMsgParser.SateType.Glonass &&
                    (dvSate[idxDv].prn == 6 || dvSate[idxDv].prn == -7))
                {   //Pass glonass satellite with off-peak frequency.
                    ++idxDv;
                    continue;
                }

                if (dvResult[0].GetSnr(dvSate[idxDv].prn, t).snr > 0)
                {
                    selGd[idxGd] = dvResult[0].GetSnr(dvSate[idxDv].prn, t);
                    selDv[idxGd] = dvSate[idxDv];
                    ++idxGd;
                }
                ++idxDv;
            }
            while (idxGd < CompareCount);
            if (idxGd < CompareCount)
            {
                return false;
            }

            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            StringBuilder sb = new StringBuilder(128);
            sb.Append(usingSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selDv[i].prn.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            sb.Remove(0, sb.Length);
            sb.Append(dvSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selDv[i].snr.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            sb.Remove(0, sb.Length);
            sb.Append(gdSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selGd[i].snr.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            double gdAverage = 0, dvAverage = 0;
            for (int i = 0; i < CompareCount; ++i)
            {
                gdAverage += selGd[i].snr;
                dvAverage += selDv[i].snr;
            }
            gdAverage /= CompareCount;
            dvAverage /= CompareCount;
            dvAverage += snrOffset;

            r.output = dvSnrAvgTxt + dvAverage.ToString("F2");
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            r.output = gdSnrAvgTxt + gdAverage.ToString("F2");
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            if (gdAverage > snrLimit)
            {
                r.output = gdSnrLimitErrorTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return false;
            }

            if (dvAverage > p.profile.gpSnrLimit)
            {
                r.output = dvSnrLimitErrorTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return false;
            }

            double diff = dvAverage - gdAverage;
            if (diff <= testSnrUpper &&
                diff >= testSnrLower)
            {
                r.output = dvTestPassTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return true;
            }
            return false;
        }

        private bool CompareBeidouResult(WorkerParam p, ref WorkerReportParam r)
        {
            GpsMsgParser.ParsingStatus.sateInfo[] dvSate = null;
            int snrLimit = 0;
            String usingSnrTxt;
            String dvSnrTxt;
            String gdSnrTxt;
            String dvSnrAvgTxt;
            String gdSnrAvgTxt;
            String gdSnrLimitErrorTxt;
            String dvSnrLimitErrorTxt;
            //int passSel = 0;
            int testSnrUpper = 0;
            int testSnrLower = 0;
            String dvTestPassTxt;
            double snrOffset = 0.0;
            int CompareCount = 0;

            dvSate = dvResult[p.index].GetSortedBeidouSateArray();
            snrLimit = p.profile.bdSnrLimit;
            usingSnrTxt = "Using Beidou PRN : ";
            dvSnrTxt = "Device Beidou SNR : ";
            gdSnrTxt = "Golden Beidou SNR : ";
            dvSnrAvgTxt = "Device average Beidou SNR" + "(" + p.bdSnrOffset.ToString() + ") : ";
            gdSnrAvgTxt = "Golden average Beidou SNR : ";
            gdSnrLimitErrorTxt = "Golden sample Beidou SNR over the limit.";
            dvSnrLimitErrorTxt = "Device sample Beidou SNR over the limit.";
            //passSel = p.profile.bdPassSel;
            testSnrUpper = p.profile.bdSnrUpper;
            testSnrLower = p.profile.bdSnrLower;
            dvTestPassTxt = "Device Beidou test pass.";
            snrOffset = p.bdSnrOffset;
            CompareCount = BeidouCompareCount;

            if (dvSate[2].snr <= 0 || dvSate[1].snr <= 0 || dvSate[0].snr <= 0)
            {
                //return false;
            }

            GpsMsgParser.ParsingStatus.sateInfo[] selGd = new GpsMsgParser.ParsingStatus.sateInfo[CompareCount];
            GpsMsgParser.ParsingStatus.sateInfo[] selDv = new GpsMsgParser.ParsingStatus.sateInfo[CompareCount];
            int idxGd = 0;
            int idxDv = 0;
            do
            {
                if (dvSate[idxDv].snr < 0)
                {
                    break;
                }

                if (dvResult[0].GetRealBeidouSnr(dvSate[idxDv].prn, GpsMsgParser.SateType.Beidou).snr > 0)
                {
                    selGd[idxGd] = dvResult[0].GetRealBeidouSnr(dvSate[idxDv].prn, GpsMsgParser.SateType.Beidou);
                    selDv[idxGd] = dvSate[idxDv];
                    ++idxGd;
                }
                ++idxDv;
            }
            while (idxGd < CompareCount);
            if (idxGd < CompareCount)
            {
                return false;
            }

            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            StringBuilder sb = new StringBuilder(128);
            sb.Append(usingSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selDv[i].prn.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            sb.Remove(0, sb.Length);
            sb.Append(dvSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selDv[i].snr.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            sb.Remove(0, sb.Length);
            sb.Append(gdSnrTxt);
            for (int i = 0; i < CompareCount; ++i)
            {
                sb.Append(selGd[i].snr.ToString());
                if (i < CompareCount - 1)
                {
                    sb.Append(", ");
                }
            }
            r.output = sb.ToString();
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            double gdAverage = 0, dvAverage = 0;
            for (int i = 0; i < CompareCount; ++i)
            {
                gdAverage += selGd[i].snr;
                dvAverage += selDv[i].snr;
            }
            gdAverage /= CompareCount;
            dvAverage /= CompareCount;
            dvAverage += snrOffset;

            r.output = dvSnrAvgTxt + dvAverage.ToString("F2");
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            r.output = gdSnrAvgTxt + gdAverage.ToString("F2");
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            if (gdAverage > snrLimit)
            {
                r.output = gdSnrLimitErrorTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return false;
            }

            if (dvAverage > p.profile.gpSnrLimit)
            {
                r.output = dvSnrLimitErrorTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return false;
            }

            double diff = dvAverage - gdAverage;
            if (diff <= testSnrUpper &&
                diff >= testSnrLower)
            {
                r.output = dvTestPassTxt;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                return true;
            }
            return false;
        }

        public bool DoTest(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep;

            rep = p.gps.Open(p.comPort,
                GpsBaudRateConverter.BaudRate2Index(p.profile.fwProfile.dvBaudRate));
            if (GPS_RESPONSE.UART_FAIL == rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Open " + p.comPort + " in " + 
                    p.gps.GetBaudRate().ToString() + " success.";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            rep = p.gps.SendColdStart(3);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ColdStartNack : WorkerParam.ErrorType.ColdStartTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Cold start success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                Thread.Sleep(500);  //For venus 6 testing.
            }

            String kVer = "";
            String sVer = "";
            String rev = "";
            rep = p.gps.QueryVersion(DefaultCmdTimeout, ref kVer, ref sVer, ref rev);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.QueryVersionNack : WorkerParam.ErrorType.QueryVersionTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                if (!p.profile.fwProfile.kVersion.Equals(kVer) ||
                    !p.profile.fwProfile.sVersion.Equals(sVer) ||
                    !p.profile.fwProfile.rVersion.Equals(rev))
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.FirmwareVersionError;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Check version pass";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
            }

            if (p.profile.checkPromCrc)
            {
                uint crc = 0;
                rep = p.gps.QueryCrc(DefaultCmdTimeout, ref crc);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.QueryCrcNack : WorkerParam.ErrorType.QueryCrcTimeOut;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    if (p.profile.fwProfile.crc != crc)
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowError;
                        p.error = WorkerParam.ErrorType.FirmwareCrcError;
                        p.bw.ReportProgress(0, new WorkerReportParam(r));
                        EndProcess(p);
                        return false;
                    }
                    else
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowProgress;
                        r.output = "Check CRC pass";
                        p.bw.ReportProgress(0, new WorkerReportParam(r));
                    }
                }
            }

            UInt32 rtc1 = 0, rtc2 = 0;
            rep = p.gps.QueryRtc(ref rtc1);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.QueryRtcNack : WorkerParam.ErrorType.QueryRtcTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Get RTC1 " + rtc1.ToString();
                p.bw.ReportProgress(0, new WorkerReportParam(r));

                Thread.Sleep(1000);
                rep = p.gps.QueryRtc(ref rtc2);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.QueryRtcNack : WorkerParam.ErrorType.QueryRtcTimeOut;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Get RTC2 " + rtc2.ToString();
                p.bw.ReportProgress(0, new WorkerReportParam(r));

                if ((rtc2 - rtc1) > 2 || (rtc2 - rtc1) < 1)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.CheckRtcError;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Check rtc pass";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
  
            }

            if (!p.profile.testGpSnr && !p.profile.testGlSnr && !p.profile.testBdSnr)
            {
                r.reportType = WorkerReportParam.ReportType.ShowFinished;
                p.error = WorkerParam.ErrorType.NoError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return true;
            }

            if (p.profile.testGlSnr)
            {
                rep = p.gps.SetRegister(1000, 0x90000000, 0x01);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK)
                        ? WorkerParam.ErrorType.SetPsti50Nack
                        : WorkerParam.ErrorType.SetPsti50Timeout;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Set PSTI 50 Interval success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
            }

            rep = p.gps.ConfigMessageOutput(0x01);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ConfigMessageOutputNack : WorkerParam.ErrorType.ConfigMessageOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config Message Output success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            rep = p.gps.ConfigNmeaOutput(1, 1, 1, 0, 1, 1, 0, 0);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ConfigNmeaOutputNack : WorkerParam.ErrorType.ConfigNmeaOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config NMEA Interval success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            bool gpSnrPass = !p.profile.testGpSnr;
            bool glSnrPass = !p.profile.testGlSnr;
            bool bdSnrPass = !p.profile.testBdSnr;
            //bool allSnrPass = false;
            do
            {
                byte[] buff = new byte[256];
                int l = p.gps.ReadLineNoWait(buff, 256);
                string line = Encoding.UTF8.GetString(buff, 0, l);
                if (GpsMsgParser.CheckNmea(line))
                {
                    GpsMsgParser.ParsingResult ps = p.parser.ParsingNmea(line);
                    if (GpsMsgParser.ParsingResult.UpdateSate == ps)
                    {   //Update SNR Chart
                        p.parser.parsingStat.CopyTo(dvResult[p.index]);
                        r.reportType = WorkerReportParam.ReportType.UpdateSnrChart;
                        p.bw.ReportProgress(0, new WorkerReportParam(r));
                    }

                    if (!gpSnrPass && p.profile.testGpSnr && GpsMsgParser.ParsingResult.UpdateSate == ps)
                    {
                        gpSnrPass = CompareResult(p, ref r, GpsMsgParser.SateType.Gps);
                    }
                    if (!glSnrPass && p.profile.testGlSnr && GpsMsgParser.ParsingResult.UpdateSate == ps)
                    {
                        glSnrPass = CompareResult(p, ref r, GpsMsgParser.SateType.Glonass);
                    }
                    if (!bdSnrPass && p.profile.testBdSnr && GpsMsgParser.ParsingResult.UpdateSate == ps)
                    {
                        bdSnrPass = CompareBeidouResult(p, ref r);
                    }
                }
            } while (!(gpSnrPass && glSnrPass && bdSnrPass) && !p.bw.CancellationPending);

            if (!p.bw.CancellationPending && p.profile.testClockOffset)
            {
                int tryCount = 3;
                int sumOfClockOffset = 0;
                int prnCount = 0;
                do 
                {
                    for (int i = 0; i < GpsMsgParser.ParsingStatus.MaxSattellite; i++)
                    {
                        GpsMsgParser.ParsingStatus.sateInfo s = TestModule.dvResult[r.index].GetGpsSate(i);
                        if (s.snr <= 30)
                        {
                            continue;
                        }
                        UInt32 prn = 0, freq = 0;
                        rep = p.gps.QueryChannelDoppler((byte)i, ref prn, ref freq);
                        if (GPS_RESPONSE.ACK != rep)
                        {
                            r.reportType = WorkerReportParam.ReportType.ShowError;
                            p.error = WorkerParam.ErrorType.GetClockOffsetFail;
                            p.bw.ReportProgress(0, new WorkerReportParam(r));
                            EndProcess(p);
                            return false;
                        }

                        UInt32 clkData = 0;
                        rep = p.gps.QueryChannelClockOffset(gdClockOffset, prn, freq, ref clkData);
                        if (GPS_RESPONSE.ACK != rep)
                        {
                            r.reportType = WorkerReportParam.ReportType.ShowError;
                            p.error = WorkerParam.ErrorType.GetClockOffsetFail;
                            p.bw.ReportProgress(0, new WorkerReportParam(r));
                            EndProcess(p);
                            return false;
                        }

                        sumOfClockOffset += unchecked((int)clkData);
                        prnCount++;
                    }

                    if (prnCount < 3)
                    {
                        sumOfClockOffset = 0;
                        prnCount = 0;
                    }
                    else
                    {
                        break;
                    }
                } while (++tryCount < 3);

                if (tryCount >= 3)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.GetClockOffsetFail;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                double avgClockOffset = sumOfClockOffset / prnCount;
                double clkPpm = avgClockOffset / (96.25 * 16.367667);

                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Device clock offset " + avgClockOffset.ToString("F0") + "(" + clkPpm.ToString("F2") + " ppm)";
                p.bw.ReportProgress(0, new WorkerReportParam(r));


                if (clkPpm < 0)
                {
                    clkPpm = -clkPpm;
                }
                if (clkPpm > p.profile.clockOffsetThreshold)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.CheckClockOffsetFail;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
            }

            if (!p.bw.CancellationPending)
            {
                rep = p.gps.FactoryReset();
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.FactoryResetNack : WorkerParam.ErrorType.FactoryResetTimeOut;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Factory Reset success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
            }

            if (gpSnrPass && glSnrPass && bdSnrPass)
            {
                r.reportType = WorkerReportParam.ReportType.ShowFinished;
                p.error = WorkerParam.ErrorType.NoError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.NoError;
                if (!gpSnrPass)
                {
                    p.error |= WorkerParam.ErrorType.GpsSnrError;
                }
                if (!glSnrPass)
                {
                    p.error |= WorkerParam.ErrorType.GlonassSnrError;
                }
                if (!bdSnrPass)
                {
                    p.error |= WorkerParam.ErrorType.BeidouSnrError;
                }
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            EndProcess(p);
            return true;
        }
        public bool OpenPortTest(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep;

            System.Diagnostics.Stopwatch w = new System.Diagnostics.Stopwatch();
            while(!p.bw.CancellationPending)
            {
                w.Reset();
                w.Start();
                rep = p.gps.Open(p.comPort, 1);
                if (GPS_RESPONSE.UART_FAIL == rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.OpenPortFail;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    w.Stop();
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open spend " + w.ElapsedMilliseconds + " ms";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    p.gps.Close();
                }
                Thread.Sleep(500);
            }
            return true;
        }

        private byte CalcCheckSum16(byte[] data, int start, int len)
        {
            UInt16 checkSum = 0;
            //const U08* ptr = dataPtr;
            //int loopCount = len / sizeof(UInt16);
            //int i;

            for (int i = 0; i < len; i += sizeof(UInt16))
            {
                UInt16 word = Convert.ToUInt16(data[start + i + 1] | data[start + i] << 8);
	            checkSum += word;
            }
            return Convert.ToByte(((checkSum >> 8) + (checkSum & 0xFF)) & 0xFF);
        }

        private int ScanBaudRate(WorkerParam p, int first)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;

            GPS_RESPONSE rep;
            int TestDeviceTimeout = 500;
            int[] testingOrder = { 1, 0, 3, 5, 2, 4 };

            if (first != -1)
            {
                rep = p.gps.Open(p.comPort, first);
                if (GPS_RESPONSE.UART_OK != rep)
                {   //This com port can't open.
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " fail!";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));

                    return -1;
                }

                TestDeviceTimeout = (first < 2) ? 1500 : 1000;
                rep = p.gps.TestDevice(TestDeviceTimeout, 2);
                if (GPS_RESPONSE.NACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Baud rate " + GpsBaudRateConverter.Index2BaudRate(first).ToString() + " invalid.";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    p.gps.Close();
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Found working baud rate " + GpsBaudRateConverter.Index2BaudRate(first).ToString() + ".";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    return first;
                }
            }

            foreach (int i in testingOrder)
            {
                if (i == first)
                {
                    continue;
                }
                if (p.bw.CancellationPending)
                {
                    return -1;
                }

                rep = p.gps.Open(p.comPort, i);
                if (GPS_RESPONSE.UART_OK != rep)
                {   //This com port can't open.
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " fail!";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    return -1;
                }

                TestDeviceTimeout = (i < 2) ? 1500 : 1000;
                rep = p.gps.TestDevice(TestDeviceTimeout, 3);
                if (GPS_RESPONSE.NACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Baud rate " + GpsBaudRateConverter.Index2BaudRate(i).ToString() + " invalid!";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    p.gps.Close();
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Found working baud rate " + GpsBaudRateConverter.Index2BaudRate(i).ToString() + ".";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    return i;
                }
            }
            return -1;
        }

        private static int lastDeviceBaudIdx = -1;
        private static int lastRomBaudIdx = 1;
        public bool DoDownload(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep = GPS_RESPONSE.UART_OK;
            
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = "Scanning " + p.comPort + " baud rate...";
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            // Retry three times for disable auto uart firmware, it'll 
            // change uart output baud rate after 5 seconds.
            int baudIdx = -1;
            for (int i = 0; i < 3; ++i)
            {
                baudIdx = ScanBaudRate(p, lastDeviceBaudIdx);
                if (-1 != baudIdx)
                {
                    lastDeviceBaudIdx = baudIdx;
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " in " +
                        GpsBaudRateConverter.Index2BaudRate(baudIdx).ToString() +
                        " success.";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    break;
                }
                Thread.Sleep(50);
            }

            if (-1 == baudIdx)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            //Reboot to ROM Code
            rep = p.gps.SetRegister(2000, 0x2000F050, 0x00000000);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ColdStartNack : WorkerParam.ErrorType.ColdStartTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Reboot from ROM success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                p.gps.Close();
                Thread.Sleep(3000);  //Waiting for reboot
            }

            // Retry three times for disable auto uart firmware, it'll 
            // change uart output baud rate after 5 seconds.
            baudIdx = -1;
            for (int i = 0; i < 3; ++i)
            {
                baudIdx = ScanBaudRate(p, lastRomBaudIdx);
                if (-1 != baudIdx)
                {
                    lastRomBaudIdx = baudIdx;
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " in " +
                        GpsBaudRateConverter.Index2BaudRate(baudIdx).ToString() +
                        " success.";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    break;
                }
                Thread.Sleep(50);
            }

            if (-1 == baudIdx)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }

            if ((p.profile.fwProfile.tagAddress == 0 && p.profile.fwProfile.tagContent == 0) ||
                (p.profile.fwProfile.tagAddress == 0xAAAAAAAA && p.profile.fwProfile.tagContent == 0x55555555))
            {   //No tag, using rom loader command
                rep = p.gps.StartDownload((byte)p.profile.dlBaudSel);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK)
                        ? WorkerParam.ErrorType.DownloadCmdNack
                        : WorkerParam.ErrorType.DownloadCmdTimeOut;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }
                else
                {
                    p.gps.Close();
                    rep = p.gps.Open(p.comPort, p.profile.dlBaudSel);
                    if (GPS_RESPONSE.UART_FAIL == rep)
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowError;
                        p.error = (rep == GPS_RESPONSE.NACK)
                            ? WorkerParam.ErrorType.DownloadCmdNack
                            : WorkerParam.ErrorType.DownloadCmdTimeOut;
                        p.bw.ReportProgress(0, r);
                        //EndProcess(p);
                        return false;
                    }
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Download command success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));

                    Thread.Sleep(1000);
                }
            }
            else
            {   //Need tag, using "$LOADER DOWNLOAD" command
/*
                if (((UInt32)p.profile.fwProfile.tagAddress ^ 0xAAAAAAAA) != 0x000FCFFC ||
                    ((UInt32)p.profile.fwProfile.tagContent ^ 0x55555555) != 0x00000A01)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.UnsupportTagType;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }
*//*
                rep = p.gps.NoNmeaOutput();
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.ChangeBaudRateFail;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Set NMEA Output success";
                    p.bw.ReportProgress(0, r);
                }
                */
                rep = p.gps.ChangeBaudrate((byte)p.profile.dlBaudSel, 2);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.ChangeBaudRateFail;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Change baud rate success";
                    p.bw.ReportProgress(0, r);
                }

                rep = p.gps.SendLoaderDownload();
                if (GPS_RESPONSE.OK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.LoaderDownloadFail;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Loader Download success";
                    p.bw.ReportProgress(0, r);
                    
                    rep = p.gps.UploadLoader(LoaderData.v8TagLoader);
                    if (GPS_RESPONSE.OK != rep)
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowError;
                        p.error = WorkerParam.ErrorType.UploadLoaderFail;
                        p.bw.ReportProgress(0, r);
                        EndProcess(p);
                        return false;
                    }  

                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Upload Loader success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    Thread.Sleep(1000);
                }
            }

            if ((p.profile.fwProfile.tagAddress == 0 && p.profile.fwProfile.tagContent == 0) ||
                (p.profile.fwProfile.tagAddress == 0xAAAAAAAA && p.profile.fwProfile.tagContent == 0x55555555))
            {
                rep = p.gps.SendRomBinSize(p.profile.fwProfile.promRaw.Length,
                    p.profile.fwProfile.CalcPromRawCheckSum());
            }
            else
            {
                rep = p.gps.SendTagBinSize(p.profile.fwProfile.promRaw.Length, p.profile.fwProfile.CalcPromRawCheckSum(),
                    p.profile.dlBaudSel, p.profile.fwProfile.tagAddress, p.profile.fwProfile.tagContent);
            }
            if (GPS_RESPONSE.OK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.BinsizeCmdTimeOut;
                p.bw.ReportProgress(0, r);
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Start update firmware";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            const int nFlashBytes = 8 * 1024;
            const int headerSize = 3;

            byte[] header = new byte[headerSize];
            int lSize = p.profile.fwProfile.promRaw.Length;
            int sentBytes = 0;
            int totalByte = 0;
            UInt16 sequence = 0;
            int rawItr = 0;

            int failCount = 0;
            while (lSize > 0)
            {
                sentBytes = (lSize >= nFlashBytes) ? nFlashBytes : lSize;
                totalByte += sentBytes;

                header[0] = (byte)(sequence >> 24 & 0xFF);
                header[1] = (byte)(sequence & 0xff);
                header[2] = CalcCheckSum16(p.profile.fwProfile.promRaw, rawItr, sentBytes);

                //p.gps.SendDataNoWait(header, headerSize);
                rep = p.gps.SendDataWaitStringAck(p.profile.fwProfile.promRaw, rawItr, sentBytes, 10000, "OK\0");

                if (rep == GPS_RESPONSE.OK)
                {
                    sequence++;
                    lSize -= sentBytes;
                    rawItr += nFlashBytes;
                }
                else
                {
                    if (++failCount > 0)
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowError;
                        p.error = WorkerParam.ErrorType.DownloadWriteFail;
                        p.bw.ReportProgress(0, r);
                        EndProcess(p);
                        return false;
                    }
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Write block respone " + rep.ToString() + ", retry " + failCount.ToString();
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    continue;
                }
                failCount = 0;
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "left " + lSize.ToString() + " bytes";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                //Thread.Sleep(100);
            }

            rep = p.gps.WaitStringAck(10000, "END\0");
            if (GPS_RESPONSE.OK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.DownloadEndTimeOut;
                p.bw.ReportProgress(0, r);
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Total time : " + (sw.ElapsedMilliseconds / 1000).ToString() + " seconds";
                p.bw.ReportProgress(0, new WorkerReportParam(r));

                r.reportType = WorkerReportParam.ReportType.ShowFinished;
                p.error = WorkerParam.ErrorType.NoError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }
            EndProcess(p);
            return true;
        }

        public bool DoiCacheTest(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep = GPS_RESPONSE.UART_OK;
            
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = "Scanning " + p.comPort + " baud rate...";
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch();
            sw.Start();

            // Retry three times for disable auto uart firmware, it'll 
            // change uart output baud rate after 5 seconds.
            int baudIdx = -1;
            for (int i = 0; i < 3; ++i)
            {   
                baudIdx = ScanBaudRate(p, lastDeviceBaudIdx);
                if (-1 != baudIdx)
                {
                    lastDeviceBaudIdx = baudIdx;
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " in " +
                        GpsBaudRateConverter.Index2BaudRate(baudIdx).ToString() +
                        " success.";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    break;
                }
                Thread.Sleep(50);
            }

            if (-1 == baudIdx)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            //Reboot to ROM Code
            rep = p.gps.SetRegister(2000, 0x2000F050, 0x00000000);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ColdStartNack : WorkerParam.ErrorType.ColdStartTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Reboot from ROM success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                p.gps.Close();
                Thread.Sleep(3000);  //Waiting for reboot
            }

            // Retry three times for disable auto uart firmware, it'll 
            // change uart output baud rate after 5 seconds.
            baudIdx = -1;
            for (int i = 0; i < 3; ++i)
            {
                baudIdx = ScanBaudRate(p, lastRomBaudIdx);
                if (-1 != baudIdx)
                {
                    lastRomBaudIdx = baudIdx;
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Open " + p.comPort + " in " +
                        GpsBaudRateConverter.Index2BaudRate(baudIdx).ToString() +
                        " success.";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    break;
                }
                Thread.Sleep(50);
            }

            if (-1 == baudIdx)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }

            rep = p.gps.ChangeBaudrate((byte)5, 2);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.ChangeBaudRateFail;
                p.bw.ReportProgress(0, r);
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Change baud rate success";
                p.bw.ReportProgress(0, r);
            }

            rep = p.gps.SendLoaderDownload();
            if (GPS_RESPONSE.OK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.LoaderDownloadFail;
                p.bw.ReportProgress(0, r);
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Loader Download success";
                p.bw.ReportProgress(0, r);

                rep = p.gps.UploadLoader(Properties.Resources.iCacheTesterSrec);
                if (GPS_RESPONSE.OK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.UploadLoaderFail;
                    p.bw.ReportProgress(0, r);
                    EndProcess(p);
                    return false;
                }  

                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Upload Loader success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                Thread.Sleep(1000);
            }

            bool testPass = true;
            byte[] buff = new byte[1024];
            System.Diagnostics.Stopwatch w = new System.Diagnostics.Stopwatch();
            p.error = WorkerParam.ErrorType.NoError;
            w.Reset();
            w.Start();
            do
            {
                buff.Initialize();
                int l = p.gps.ReadLineNoWait(buff, 1024);
                string line = Encoding.UTF8.GetString(buff, 0, l);

                if (line.Length <= 0)
                {
                    if (w.ElapsedMilliseconds > 7000 && testPass)
                    {
                        p.error |= WorkerParam.ErrorType.TestIcacheTimeout;
                        testPass = false;
                        r.reportType = WorkerReportParam.ReportType.ShowProgress;
                        r.output = "i cache timeout defected.";
                        p.bw.ReportProgress(0, new WorkerReportParam(r));

                    }
                    continue;
                }
                w.Reset();
                w.Start();

                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = line;
                p.bw.ReportProgress(0, new WorkerReportParam(r));

                if (line.Contains("$ER:"))
                {
                    p.error |= WorkerParam.ErrorType.TestIcacheError;
                    testPass = false;
                }
            } while (!p.bw.CancellationPending);

            if (testPass && p.error == WorkerParam.ErrorType.NoError)
            {
                r.reportType = WorkerReportParam.ReportType.ShowFinished;
                p.error = WorkerParam.ErrorType.NoError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                //p.error |= WorkerParam.ErrorType.TestIcacheError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }

            EndProcess(p);
            return true;
        }
        
        public bool DoResetTest(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep;

            rep = p.gps.Open(p.comPort, resetTesterLogin.BootBaudRate);
            if (GPS_RESPONSE.UART_FAIL == rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Open " + p.comPort + " in " + 
                    p.gps.GetBaudRate().ToString() + " success.";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            rep = p.gps.SendColdStart(3);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ColdStartNack : WorkerParam.ErrorType.ColdStartTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Cold start success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                Thread.Sleep(500);  //For venus 6 testing.
            }

            rep = p.gps.ConfigMessageOutput(0x01);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ConfigMessageOutputNack : WorkerParam.ErrorType.ConfigMessageOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config Message Output success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            rep = p.gps.ConfigNmeaOutput(1, 1, 1, 0, 1, 1, 0, 0);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.ConfigNmeaOutputNack : WorkerParam.ErrorType.ConfigNmeaOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config NMEA Interval success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            Thread.Sleep(500);
            bool testResetError = false;
            System.Diagnostics.Stopwatch swNmea = new System.Diagnostics.Stopwatch();
            System.Diagnostics.Stopwatch swResetTest = new System.Diagnostics.Stopwatch();
            swNmea.Start();
            swResetTest.Start();
            do
            {
                byte[] buff = new byte[256];
                int l = p.gps.ReadLineNoWait(buff, 256);
                string line = Encoding.UTF8.GetString(buff, 0, l);
                if (line.Length > 0)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = line;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));

                    if(line.Contains("$SkyTraq"))
                    {
                        r.reportType = WorkerReportParam.ReportType.ShowError;
                        p.error = WorkerParam.ErrorType.ResetDetectError;
                        p.bw.ReportProgress(0, new WorkerReportParam(r));
                        EndProcess(p);
                        return false;
                    }

                    if (GpsMsgParser.CheckNmea(line))
                    {
                        swNmea.Reset();
                        swNmea.Start();
                    }
                }

                if (swNmea.ElapsedMilliseconds > resetTesterLogin.CheckInterval)
                {
                    long a = swNmea.ElapsedMilliseconds;
                    testResetError = true;
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.CheckRtcError;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
            } while (!testResetError && !p.bw.CancellationPending);

            //if (!p.bw.CancellationPending)
            {
                rep = p.gps.FactoryReset();
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK) ? WorkerParam.ErrorType.FactoryResetNack : WorkerParam.ErrorType.FactoryResetTimeOut;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Factory Reset success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
            }

            if (!testResetError)
            {
                r.reportType = WorkerReportParam.ReportType.ShowFinished;
                p.error = WorkerParam.ErrorType.NoError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.NoError;
                p.error |= WorkerParam.ErrorType.FirmwareVersionError;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            EndProcess(p);
            return true;
        }

        public bool DoGolden(WorkerParam p)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.index = p.index;
            GPS_RESPONSE rep;

            rep = p.gps.Open(p.comPort, p.profile.gdBaudSel);
            if (GPS_RESPONSE.UART_FAIL == rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = WorkerParam.ErrorType.OpenPortFail;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Open " + p.comPort + " success ------------------------------------";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            if (p.profile.testGlSnr)
            {
                rep = p.gps.SetRegister(1000, 0x90000000, 0x01);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = (rep == GPS_RESPONSE.NACK)
                        ? WorkerParam.ErrorType.SetPsti50Nack
                        : WorkerParam.ErrorType.SetPsti50Timeout;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Set PSTI 50 Interval success";
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }
            }

            if (p.profile.testClockOffset)
            {
                p.parser.parsingStat.positionFixResult = 0;
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Waiting for position fix to get clock offset...";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                r.reportType = WorkerReportParam.ReportType.ShowWaitingGoldenSample;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                do
                {
                    byte[] buff = new byte[256];
                    int l = p.gps.ReadLineNoWait(buff, 256);
                    string line = Encoding.UTF8.GetString(buff, 0, l);
                    if (GpsMsgParser.CheckNmea(line))
                    {
                        if (GpsMsgParser.ParsingResult.UpdateSate == p.parser.ParsingNmea(line))
                        {
                            p.parser.parsingStat.CopyTo(dvResult[0]);
                            r.reportType = WorkerReportParam.ReportType.UpdateSnrChart;
                            p.bw.ReportProgress(0, new WorkerReportParam(r));
                        }
                    }
                } while (p.parser.parsingStat.positionFixResult <= 2 && !p.bw.CancellationPending);
                r.reportType = WorkerReportParam.ReportType.HideWaitingGoldenSample;
                p.bw.ReportProgress(0, new WorkerReportParam(r));

                if (p.bw.CancellationPending)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;
                    p.error = WorkerParam.ErrorType.TestNotComplete;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }

                rep = p.gps.GetRegister(DefaultCmdTimeout, 0x00000001, ref gdClockOffset);
                if (GPS_RESPONSE.ACK != rep)
                {
                    r.reportType = WorkerReportParam.ReportType.ShowError;

                    p.error = (rep == GPS_RESPONSE.NACK)
                        ? WorkerParam.ErrorType.GetClockOffsetNack
                        : WorkerParam.ErrorType.GetClockOffsetTimeout;
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                    EndProcess(p);
                    return false;
                }
                else
                {
                    r.reportType = WorkerReportParam.ReportType.ShowProgress;
                    r.output = "Get clock offset : " + unchecked((int)gdClockOffset).ToString();
                    p.bw.ReportProgress(0, new WorkerReportParam(r));
                }

            }

            rep = p.gps.ConfigMessageOutput(0x01);
            if (GPS_RESPONSE.ACK != rep)
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;

                p.error = (rep == GPS_RESPONSE.NACK) 
                    ? WorkerParam.ErrorType.ConfigMessageOutputNack
                    : WorkerParam.ErrorType.ConfigMessageOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config Message Output success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            rep = p.gps.ConfigNmeaOutput(1, 1, 1, 0, 1, 1, 0, 0);
            if (GPS_RESPONSE.ACK != p.gps.ConfigNmeaOutput(1, 1, 1, 0, 1, 1, 0, 0))
            {
                r.reportType = WorkerReportParam.ReportType.ShowError;
                p.error = (rep == GPS_RESPONSE.NACK)
                    ? WorkerParam.ErrorType.ConfigNmeaOutputNack
                    : WorkerParam.ErrorType.ConfigNmeaOutputTimeOut;
                p.bw.ReportProgress(0, new WorkerReportParam(r));
                EndProcess(p);
                return false;
            }
            else
            {
                r.reportType = WorkerReportParam.ReportType.ShowProgress;
                r.output = "Config NMEA Interval success";
                p.bw.ReportProgress(0, new WorkerReportParam(r));
            }

            r.reportType = WorkerReportParam.ReportType.GoldenSampleReady;
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            do
            {
                byte[] buff = new byte[256];
                int l = p.gps.ReadLineNoWait(buff, 256);
                string line = Encoding.UTF8.GetString(buff, 0, l);
                if (GpsMsgParser.CheckNmea(line))
                {
                    if (GpsMsgParser.ParsingResult.UpdateSate == p.parser.ParsingNmea(line))
                    {
                        p.parser.parsingStat.CopyTo(dvResult[0]);
                        r.reportType = WorkerReportParam.ReportType.UpdateSnrChart;
                        p.bw.ReportProgress(0, new WorkerReportParam(r));
                    }
                }
            } while (p.bw.CancellationPending != true);

            EndProcess(p);
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = "Closed UART";
            p.bw.ReportProgress(0, new WorkerReportParam(r));

            return false;
        }

        private void EndProcess(WorkerParam p)
        {
            p.gps.Close();
        }    
    }
}
