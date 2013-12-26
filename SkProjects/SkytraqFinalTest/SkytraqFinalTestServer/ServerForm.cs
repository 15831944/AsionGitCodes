using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace SkytraqFinalTestServer
{
    public partial class ServerForm : Form
    {
        public ServerForm()
        {
            InitializeComponent();
        }
        private BackgroundWorker bw = new BackgroundWorker();
        public static WorkerParam wp = new WorkerParam();
        private void ServerForm_Load(object sender, EventArgs e)
        {
            bw.WorkerReportsProgress = true;
            bw.WorkerSupportsCancellation = true;
            bw.DoWork += new DoWorkEventHandler(bw_DoWork);
            bw.ProgressChanged += new ProgressChangedEventHandler(bw_ProgressChanged);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(bw_RunWorkerCompleted);
            wp.bw = bw;

            port.Text = "9000";

        }
        //背景執行
        TcpServer server = null;
        private void bw_DoWork(object sender, DoWorkEventArgs e)
        {
            Stopwatch w = new Stopwatch();
            w.Start();

            server = new TcpServer();
            server.ListenToConnection(Convert.ToInt32(port.Text));
            e.Cancel = true;

            //p.duration = w.ElapsedMilliseconds;
            w.Stop();
        }

        //處理進度
        private void bw_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            WorkerReportParam r = e.UserState as WorkerReportParam;

            if (r.reportType == WorkerReportParam.ReportType.ShowIP)
            {
                ip.Text = r.output;
            }
            else if (r.reportType == WorkerReportParam.ReportType.ShowProgress)
            {
                consoleMsg.Items.Add(r.output);
            }
        }

        //執行完成
        private void bw_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            BackgroundWorker b = (sender as BackgroundWorker);
        }

        private bool serverStart = false;
        private void start_Click(object sender, EventArgs e)
        {
            if (serverStart)
            {   //Go stop
                start.Text = "Start";
                bw.CancelAsync();
                serverStart = false;
            }
            else
            {   //Go start
                start.Text = "Stop";
                bw.RunWorkerAsync();
                serverStart = true;
            }
        }
        public enum TestType
        {
            AllPass,
            TestNG,
            TestException
        }
        public static TestType testType = TestType.AllPass;


        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void allPass_CheckedChanged(object sender, EventArgs e)
        {
            testType = TestType.AllPass;
        }

        private void testNg_CheckedChanged(object sender, EventArgs e)
        {
            testType = TestType.TestNG;
        }

        private void testException_CheckedChanged(object sender, EventArgs e)
        {
            testType = TestType.TestException;
        }
    }
}
