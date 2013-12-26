using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using SocketDemo;

namespace SkytraqFinalTestServer
{
    public class HandleClient
    {
        /// <summary>
        /// private attribute of HandleClient class
        /// </summary>
        private TcpClient mTcpClient;
        public static void AddMessage(string msg)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = msg;
            ServerForm.wp.bw.ReportProgress(0, new WorkerReportParam(r));
        }
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="_tmpTcpClient">傳入TcpClient參數</param>
        public HandleClient(TcpClient _tmpTcpClient)
        {
            this.mTcpClient = _tmpTcpClient;
        }

        /// <summary>
        /// Communicate
        /// </summary>
        public void Communicate()
        {
            try
            {
                CommunicationBase cb = new CommunicationBase();
                //while (this.mTcpClient.Connected)
                {
                    string msg = cb.ReceiveMsg(this.mTcpClient);
                    //Console.WriteLine(msg + "\n");
                    if (msg.Length > 0)
                    {
                        GpsTester g = new GpsTester();
                        //g.DoCommand(msg);
                        string retCmd = g.DoCommand(msg);
                        if (retCmd.Length != 0)
                        {
                            cb.SendMsg(retCmd, this.mTcpClient);
                        }
                    }
                    else
                    {
                        Thread.Sleep(50);
                    }
                }
            }
            catch
            {
                //Console.WriteLine("客戶端強制關閉連線!");
                this.mTcpClient.Close();
                //Console.Read();
            }
            this.mTcpClient.Close();
            AddMessage("TcpClient closed.");
        } // end HandleClient()


    } // end Class

    class TcpServer
    {
        /// <summary>
        /// 等待客戶端連線
        /// </summary>
        ///         
        public static void AddMessage(string msg)
        {
            WorkerReportParam r = new WorkerReportParam();
            r.reportType = WorkerReportParam.ReportType.ShowProgress;
            r.output = msg;
            ServerForm.wp.bw.ReportProgress(0, new WorkerReportParam(r));
        }

        public void ListenToConnection(int port)
        {
            //取得本機名稱
            string hostName = Dns.GetHostName();
            AddMessage("Host Name :" + hostName);

            //取得本機IP
            IPAddress[] ipa = Dns.GetHostAddresses(hostName);

            // 取得所有 IP 位址
            int num = 0;
            foreach (IPAddress ipaddress in ipa)
            {
                if (ipaddress.AddressFamily == AddressFamily.InterNetwork)
                {
                    break;
                }
                //Console.WriteLine(ipaddress.ToString() + ipaddress.AddressFamily.ToString());
                num = num + 1;
            }
            if (num > ipa.Length)
            {
                //Console.WriteLine("無可用網路介面!");
                AddMessage("No available network interface");
                return;
            }

            //Console.WriteLine("本機IP=" + ipa[num].ToString());
            AddMessage("Server IP : " + ipa[num].ToString());

            WorkerReportParam r = new WorkerReportParam();
            r.reportType = WorkerReportParam.ReportType.ShowIP;
            r.output = ipa[num].ToString();
            ServerForm.wp.bw.ReportProgress(0, new WorkerReportParam(r));

            //建立本機端的IPEndPoint物件
            IPEndPoint ipe = new IPEndPoint(ipa[num], port);

            //建立TcpListener物件
            TcpListener tcpListener = new TcpListener(ipe);

            //開始監聽port
            tcpListener.Start();
            //Console.WriteLine("等待客戶端連線中... \n");
            AddMessage("Server Ready...");

            TcpClient tmpTcpClient;
            int numberOfClients = 0;
            while (!ServerForm.wp.bw.CancellationPending)
            {
                try
                {
                    //建立與客戶端的連線
                    if (tcpListener.Pending() == false)
                    {
                        Thread.Sleep(1);
                    }
                    else
                    {
                        tmpTcpClient = tcpListener.AcceptTcpClient();

                        if (tmpTcpClient.Connected)
                        {
                            //Console.WriteLine("連線成功!");
                            AddMessage("Client connected...");
                            HandleClient handleClient = new HandleClient(tmpTcpClient);
                            Thread myThread = new Thread(new ThreadStart(handleClient.Communicate));
                            numberOfClients += 1;
                            myThread.IsBackground = true;
                            myThread.Start();
                            myThread.Name = tmpTcpClient.Client.RemoteEndPoint.ToString();
                        }
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                    //Console.Read();
                }
            } // end while
            tcpListener.Stop();
            AddMessage("Server End");
        } // end ListenToConnect()
    }
}
