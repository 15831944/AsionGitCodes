namespace SkytraqFinalTestServer
{
    partial class ServerForm
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置 Managed 資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改這個方法的內容。
        ///
        /// </summary>
        private void InitializeComponent()
        {
            this.consoleMsg = new System.Windows.Forms.ListBox();
            this.start = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.ip = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.port = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.testException = new System.Windows.Forms.RadioButton();
            this.testNg = new System.Windows.Forms.RadioButton();
            this.allPass = new System.Windows.Forms.RadioButton();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // consoleMsg
            // 
            this.consoleMsg.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.consoleMsg.FormattingEnabled = true;
            this.consoleMsg.ItemHeight = 12;
            this.consoleMsg.Location = new System.Drawing.Point(12, 114);
            this.consoleMsg.Name = "consoleMsg";
            this.consoleMsg.Size = new System.Drawing.Size(672, 316);
            this.consoleMsg.TabIndex = 1;
            // 
            // start
            // 
            this.start.Location = new System.Drawing.Point(577, 12);
            this.start.Name = "start";
            this.start.Size = new System.Drawing.Size(107, 96);
            this.start.TabIndex = 2;
            this.start.Text = "Start";
            this.start.UseVisualStyleBackColor = true;
            this.start.Click += new System.EventHandler(this.start_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(10, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(54, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Server IP :";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // ip
            // 
            this.ip.AutoSize = true;
            this.ip.Location = new System.Drawing.Point(72, 13);
            this.ip.Name = "ip";
            this.ip.Size = new System.Drawing.Size(0, 12);
            this.ip.TabIndex = 3;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(175, 13);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 12);
            this.label2.TabIndex = 3;
            this.label2.Text = "Port :";
            // 
            // port
            // 
            this.port.Location = new System.Drawing.Point(211, 7);
            this.port.Name = "port";
            this.port.Size = new System.Drawing.Size(100, 22);
            this.port.TabIndex = 4;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.testException);
            this.panel1.Controls.Add(this.testNg);
            this.panel1.Controls.Add(this.allPass);
            this.panel1.Location = new System.Drawing.Point(12, 38);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(559, 70);
            this.panel1.TabIndex = 6;
            // 
            // testException
            // 
            this.testException.AutoSize = true;
            this.testException.Location = new System.Drawing.Point(4, 49);
            this.testException.Name = "testException";
            this.testException.Size = new System.Drawing.Size(70, 16);
            this.testException.TabIndex = 2;
            this.testException.Text = "Exception";
            this.testException.UseVisualStyleBackColor = true;
            this.testException.CheckedChanged += new System.EventHandler(this.testException_CheckedChanged);
            // 
            // testNg
            // 
            this.testNg.AutoSize = true;
            this.testNg.Location = new System.Drawing.Point(4, 26);
            this.testNg.Name = "testNg";
            this.testNg.Size = new System.Drawing.Size(61, 16);
            this.testNg.TabIndex = 1;
            this.testNg.Text = "Test NG";
            this.testNg.UseVisualStyleBackColor = true;
            this.testNg.CheckedChanged += new System.EventHandler(this.testNg_CheckedChanged);
            // 
            // allPass
            // 
            this.allPass.AutoSize = true;
            this.allPass.Checked = true;
            this.allPass.Location = new System.Drawing.Point(3, 3);
            this.allPass.Name = "allPass";
            this.allPass.Size = new System.Drawing.Size(59, 16);
            this.allPass.TabIndex = 0;
            this.allPass.TabStop = true;
            this.allPass.Text = "All Pass";
            this.allPass.UseVisualStyleBackColor = true;
            this.allPass.CheckedChanged += new System.EventHandler(this.allPass_CheckedChanged);
            // 
            // ServerForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(696, 450);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.port);
            this.Controls.Add(this.ip);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.start);
            this.Controls.Add(this.consoleMsg);
            this.Name = "ServerForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.ServerForm_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox consoleMsg;
        private System.Windows.Forms.Button start;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label ip;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox port;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.RadioButton testException;
        private System.Windows.Forms.RadioButton testNg;
        private System.Windows.Forms.RadioButton allPass;

    }
}

