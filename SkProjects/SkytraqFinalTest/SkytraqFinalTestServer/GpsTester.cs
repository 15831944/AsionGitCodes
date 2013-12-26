using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace SkytraqFinalTestServer
{
    class GpsTester
    {
        public string DoCommand(string cmd)
        {
            AddMessage("Receive command " + cmd);

            bool b = ParsingCmd(cmd);
            if (!b)
            {
                return "";
            }

            string retCmd = "";
            switch (cmdType)
            {
                case CmdType.Initial:
                    Thread.Sleep(1000);
                    if (ServerForm.testType==ServerForm.TestType.TestException)
                    {
                        retCmd = GetReturnCommand(CmdType.Err1);
                    }
                    else
                    {
                        retCmd = GetReturnCommand(CmdType.Ready);
                    }
                    break;
                case CmdType.Test_Start:
                    for (int i = 0; i < 5; ++i)
                    {
                        AddMessage("Testing..." + i.ToString());
                        Thread.Sleep(1000);
                    }
                    if (ServerForm.testType == ServerForm.TestType.TestException)
                    {
                        retCmd = GetReturnCommand(CmdType.Err2);
                    }
                    else
                    {
                        retCmd = GetReturnCommand(CmdType.Test_End);
                    }
                    break;
                default:
                    return "";
            }

            return retCmd;
        }

        private String GetReturnCommand(CmdType c)
        {
            String cmd;

            if (c == CmdType.Ready || c == CmdType.Test_End)
            {
                string dutResult = "";
                if (ServerForm.testType == ServerForm.TestType.TestNG)
                {
                    Random rd = new Random(Guid.NewGuid().GetHashCode());

                    for (int i = 0; i < duts.Length; ++i)
                    {
                        if (duts[i] == '1' && rd.NextDouble() > 0.5)
                        {
                            dutResult += '1';
                        }
                        else
                        {
                            dutResult += '0';
                        }
                    }
                }
                else
                {
                    dutResult = duts;
                }

                cmd = "@" + module + " " + c.ToString() + " " +
                    Siteno.ToString("D2") + " " + dutResult +
                    "+";
            }
            else if (c == CmdType.Err1 || c == CmdType.Err2)
            {
                cmd = "@" + module + " " + c.ToString() + " " +
                    Siteno.ToString("D2") + "+";
            }
            else
            {
                cmd = "";
            }

            AddMessage("Return : " + cmd);
            return cmd;
        }

        private void AddMessage(string msg)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = msg;
            ServerForm.wp.bw.ReportProgress(0, new WorkerReportParam(r));
        }

        private enum CmdType
        {
            Unknown,
            Initial,
            Test_Start,
            Test_End,
            Ready,
            Err1,
            Err2

        };

        private int Siteno = -1;
        private String duts;
        private String module;
        private CmdType cmdType = CmdType.Unknown;

        private bool ParsingCmd(string cmd)
        {
            if (cmd.Length < 5)
            {
                AddMessage("Error : Invalid command format, length < 5");
                return false;
            }
            if (cmd[0] != '@' && cmd[cmd.Length - 1] != '+')
            {
                AddMessage("Error : Invalid command format, not begin in @ and not end in +");
                return false;
            }

            String[] param = null;
            try
            {
                char[] delimiterChars = { ' ' };
                param = cmd.Substring(1, cmd.Length - 2).Split(delimiterChars);
            }
            catch (Exception e)
            {
                Console.Write(e.ToString());
                //return false;
            }

            if (param == null || param.Length < 4)
            {
                AddMessage("Error : Invalid command format, parameter count < 4");
                return false;
            }
            module = param[0];
            cmdType = CmdType.Unknown;
            if (param[1] == "Initial")
            {
                cmdType = CmdType.Initial;
            }
            else if (param[1] == "Test_Start")
            {
                cmdType = CmdType.Test_Start;
            }
            else if (param[1] == "Test_End")
            {
                cmdType = CmdType.Test_End;
            }
            if (cmdType == CmdType.Unknown)
            {
                AddMessage("Error : Unknown command");
                return false;
            }

            Siteno = -1;
            try
            {
                Siteno = Convert.ToInt32(param[2]);
            }
            catch (Exception e)
            {
                Console.Write(e.ToString());
                //AddMessage(e.ToString());
                //return false;
            }

            if (Siteno < 0 || Siteno > 7)
            {
                AddMessage("Error : Invalid Siteno");
                return false;            
            }

            if (param[3].Length != 8)
            {
                AddMessage("Error : Invalid duts");
                return false;
            }
            duts = param[3];
            for (int i = 0; i < 8; ++i)
            {
                if (duts[i] != '0' && duts[i] != '1')
                {
                    AddMessage("Error : Invalid duts");
                    return false;
                }
            }
            return true;
        }
    }
}
