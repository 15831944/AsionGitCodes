using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace FormClient
{
    public partial class FormClient : Form
    {
        public FormClient()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serverIp.Text = "169.254.128.101";
            comboBox1.Text = "@V821 Initial 03 00001111+";

            comboBox1.Items.Add("@V821 Initial 03 00001111+");
            comboBox1.Items.Add("@V821 Test_Start 03 00001111+");
            comboBox1.Items.Add("@V821 Initial 15 11111111+");
            comboBox1.Items.Add("@V821 Test_Start 15 11111111+");
            comboBox1.Items.Add("@V821 Initial 30 11111111+");
            comboBox1.Items.Add("@V821 Test_Start 30 11111111+");



            close.Enabled = false;
            connect.Enabled = true;
            send.Enabled = false;
        }

        private Client client = null;
        private void connect_Click(object sender, EventArgs e)
        {
            if (client != null)
            {
                client.CloseConnect();
                client = null;
            }

            client = new Client();
            client.ConnectToServer(serverIp.Text, 9000);

            close.Enabled = true;
            connect.Enabled = false;
            send.Enabled = true;
        }

        private void close_Click(object sender, EventArgs e)
        {
            if (client != null)
            {
                client.CloseConnect();
                client = null;
            }
            close.Enabled = false;
            connect.Enabled = true;
            send.Enabled = false;
        }

        private void send_Click(object sender, EventArgs e)
        {
            if (client == null)
            {
                return;
            }
            client.Send(comboBox1.Text);
        }
    }
}
