namespace ModuleTestV8
{
    partial class SettingForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SettingForm));
            this.OK = new System.Windows.Forms.Button();
            this.Cancel = new System.Windows.Forms.Button();
            this.dvBaudSel = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.gpModule = new System.Windows.Forms.RadioButton();
            this.glModule = new System.Windows.Forms.RadioButton();
            this.gpModuleSel = new System.Windows.Forms.ComboBox();
            this.glModuleSel = new System.Windows.Forms.ComboBox();
            this.bdModule = new System.Windows.Forms.RadioButton();
            this.bdModuleSel = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.gpSnrLimit = new System.Windows.Forms.TextBox();
            this.glSnrLimit = new System.Windows.Forms.TextBox();
            this.bdSnrLimit = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.testBootStatus = new System.Windows.Forms.CheckBox();
            this.testPromCrc = new System.Windows.Forms.CheckBox();
            this.testClockOffset = new System.Windows.Forms.CheckBox();
            this.label7 = new System.Windows.Forms.Label();
            this.clockOffsetThreshold = new System.Windows.Forms.TextBox();
            this.writeClockOffset = new System.Windows.Forms.CheckBox();
            this.testEcompass = new System.Windows.Forms.CheckBox();
            this.testMiniHommer = new System.Windows.Forms.CheckBox();
            this.testDrCyro = new System.Windows.Forms.CheckBox();
            this.label8 = new System.Windows.Forms.Label();
            this.testDrDuration = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.uslClockWise = new System.Windows.Forms.TextBox();
            this.lslClockWise = new System.Windows.Forms.TextBox();
            this.label13 = new System.Windows.Forms.Label();
            this.uslAnticlockWise = new System.Windows.Forms.TextBox();
            this.lslAnticlockWise = new System.Windows.Forms.TextBox();
            this.gpPassCriteria = new System.Windows.Forms.CheckBox();
            this.glPassCriteria = new System.Windows.Forms.CheckBox();
            this.bdPassCriteria = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.label12 = new System.Windows.Forms.Label();
            this.gdBaudSel = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.gaModuleSel = new System.Windows.Forms.ComboBox();
            this.gaModule = new System.Windows.Forms.RadioButton();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.browseIni = new System.Windows.Forms.Button();
            this.moduleName = new System.Windows.Forms.Label();
            this.snrTestPeriod = new System.Windows.Forms.TextBox();
            this.gaSnrLower = new System.Windows.Forms.TextBox();
            this.bdSnrLower = new System.Windows.Forms.TextBox();
            this.glSnrLower = new System.Windows.Forms.TextBox();
            this.gaSnrUpper = new System.Windows.Forms.TextBox();
            this.bdSnrUpper = new System.Windows.Forms.TextBox();
            this.glSnrUpper = new System.Windows.Forms.TextBox();
            this.gpSnrLower = new System.Windows.Forms.TextBox();
            this.gpSnrUpper = new System.Windows.Forms.TextBox();
            this.gaPassCriteria = new System.Windows.Forms.CheckBox();
            this.gaSnrLimit = new System.Windows.Forms.TextBox();
            this.iniFileName = new System.Windows.Forms.TextBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label18 = new System.Windows.Forms.Label();
            this.dlBaudSel = new System.Windows.Forms.ComboBox();
            this.writeTag = new System.Windows.Forms.CheckBox();
            this.enableDownload = new System.Windows.Forms.CheckBox();
            this.label17 = new System.Windows.Forms.Label();
            this.label19 = new System.Windows.Forms.Label();
            this.thresholdCogWise = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.browseFw = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.promFileName = new System.Windows.Forms.TextBox();
            this.saveAs = new System.Windows.Forms.Button();
            this.loadFrom = new System.Windows.Forms.Button();
            this.checkPromCrc = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.SuspendLayout();
            // 
            // OK
            // 
            this.OK.Location = new System.Drawing.Point(423, 503);
            this.OK.Name = "OK";
            this.OK.Size = new System.Drawing.Size(75, 23);
            this.OK.TabIndex = 1;
            this.OK.Text = "OK";
            this.OK.UseVisualStyleBackColor = true;
            this.OK.Click += new System.EventHandler(this.OK_Click);
            // 
            // Cancel
            // 
            this.Cancel.Location = new System.Drawing.Point(504, 503);
            this.Cancel.Name = "Cancel";
            this.Cancel.Size = new System.Drawing.Size(75, 23);
            this.Cancel.TabIndex = 2;
            this.Cancel.Text = "Cancel";
            this.Cancel.UseVisualStyleBackColor = true;
            this.Cancel.Click += new System.EventHandler(this.Cancel_Click);
            // 
            // dvBaudSel
            // 
            this.dvBaudSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.dvBaudSel.FormattingEnabled = true;
            this.dvBaudSel.Location = new System.Drawing.Point(129, 570);
            this.dvBaudSel.Name = "dvBaudSel";
            this.dvBaudSel.Size = new System.Drawing.Size(144, 20);
            this.dvBaudSel.TabIndex = 2;
            this.dvBaudSel.Visible = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(29, 575);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(89, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "Device Baud Rate";
            this.label1.Visible = false;
            // 
            // gpModule
            // 
            this.gpModule.AutoSize = true;
            this.gpModule.Location = new System.Drawing.Point(12, 22);
            this.gpModule.Name = "gpModule";
            this.gpModule.Size = new System.Drawing.Size(82, 16);
            this.gpModule.TabIndex = 0;
            this.gpModule.TabStop = true;
            this.gpModule.Text = "GPS Module";
            this.gpModule.UseVisualStyleBackColor = true;
            this.gpModule.CheckedChanged += new System.EventHandler(this.gpModule_CheckedChanged);
            // 
            // glModule
            // 
            this.glModule.AutoSize = true;
            this.glModule.Location = new System.Drawing.Point(12, 52);
            this.glModule.Name = "glModule";
            this.glModule.Size = new System.Drawing.Size(98, 16);
            this.glModule.TabIndex = 1;
            this.glModule.TabStop = true;
            this.glModule.Text = "Glonass Module";
            this.glModule.UseVisualStyleBackColor = true;
            this.glModule.CheckedChanged += new System.EventHandler(this.glModule_CheckedChanged);
            // 
            // gpModuleSel
            // 
            this.gpModuleSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gpModuleSel.FormattingEnabled = true;
            this.gpModuleSel.Location = new System.Drawing.Point(142, 22);
            this.gpModuleSel.Name = "gpModuleSel";
            this.gpModuleSel.Size = new System.Drawing.Size(144, 20);
            this.gpModuleSel.TabIndex = 4;
            this.gpModuleSel.SelectedIndexChanged += new System.EventHandler(this.gpModuleSel_SelectedIndexChanged);
            // 
            // glModuleSel
            // 
            this.glModuleSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.glModuleSel.FormattingEnabled = true;
            this.glModuleSel.Location = new System.Drawing.Point(142, 52);
            this.glModuleSel.Name = "glModuleSel";
            this.glModuleSel.Size = new System.Drawing.Size(144, 20);
            this.glModuleSel.TabIndex = 5;
            this.glModuleSel.SelectedIndexChanged += new System.EventHandler(this.glModuleSel_SelectedIndexChanged);
            // 
            // bdModule
            // 
            this.bdModule.AutoSize = true;
            this.bdModule.Location = new System.Drawing.Point(12, 82);
            this.bdModule.Name = "bdModule";
            this.bdModule.Size = new System.Drawing.Size(96, 16);
            this.bdModule.TabIndex = 2;
            this.bdModule.TabStop = true;
            this.bdModule.Text = "Beidou Module";
            this.bdModule.UseVisualStyleBackColor = true;
            this.bdModule.CheckedChanged += new System.EventHandler(this.bdModule_CheckedChanged);
            // 
            // bdModuleSel
            // 
            this.bdModuleSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.bdModuleSel.FormattingEnabled = true;
            this.bdModuleSel.Location = new System.Drawing.Point(142, 82);
            this.bdModuleSel.Name = "bdModuleSel";
            this.bdModuleSel.Size = new System.Drawing.Size(144, 20);
            this.bdModuleSel.TabIndex = 6;
            this.bdModuleSel.SelectedIndexChanged += new System.EventHandler(this.bdModuleSel_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(10, 102);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(82, 12);
            this.label2.TabIndex = 6;
            this.label2.Text = "SNR Test Period";
            // 
            // gpSnrLimit
            // 
            this.gpSnrLimit.Enabled = false;
            this.gpSnrLimit.Location = new System.Drawing.Point(235, 151);
            this.gpSnrLimit.MaxLength = 3;
            this.gpSnrLimit.Name = "gpSnrLimit";
            this.gpSnrLimit.Size = new System.Drawing.Size(48, 22);
            this.gpSnrLimit.TabIndex = 5;
            this.gpSnrLimit.TextChanged += new System.EventHandler(this.gpSnrLimit_TextChanged);
            // 
            // glSnrLimit
            // 
            this.glSnrLimit.Enabled = false;
            this.glSnrLimit.Location = new System.Drawing.Point(235, 181);
            this.glSnrLimit.MaxLength = 3;
            this.glSnrLimit.Name = "glSnrLimit";
            this.glSnrLimit.Size = new System.Drawing.Size(48, 22);
            this.glSnrLimit.TabIndex = 8;
            this.glSnrLimit.TextChanged += new System.EventHandler(this.glSnrLimit_TextChanged);
            // 
            // bdSnrLimit
            // 
            this.bdSnrLimit.Enabled = false;
            this.bdSnrLimit.Location = new System.Drawing.Point(235, 211);
            this.bdSnrLimit.MaxLength = 3;
            this.bdSnrLimit.Name = "bdSnrLimit";
            this.bdSnrLimit.Size = new System.Drawing.Size(48, 22);
            this.bdSnrLimit.TabIndex = 11;
            this.bdSnrLimit.TextChanged += new System.EventHandler(this.bdSnrLimit_TextChanged);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(61, 553);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(57, 12);
            this.label6.TabIndex = 6;
            this.label6.Text = "Prom CRC";
            this.label6.Visible = false;
            // 
            // testBootStatus
            // 
            this.testBootStatus.AutoSize = true;
            this.testBootStatus.Location = new System.Drawing.Point(14, 459);
            this.testBootStatus.Name = "testBootStatus";
            this.testBootStatus.Size = new System.Drawing.Size(137, 16);
            this.testBootStatus.TabIndex = 2;
            this.testBootStatus.Text = "Test boot from SPI flash";
            this.testBootStatus.UseVisualStyleBackColor = true;
            this.testBootStatus.Visible = false;
            this.testBootStatus.CheckedChanged += new System.EventHandler(this.testBootStatus_CheckedChanged);
            // 
            // testPromCrc
            // 
            this.testPromCrc.AutoSize = true;
            this.testPromCrc.Location = new System.Drawing.Point(313, 600);
            this.testPromCrc.Name = "testPromCrc";
            this.testPromCrc.Size = new System.Drawing.Size(98, 16);
            this.testPromCrc.TabIndex = 8;
            this.testPromCrc.Text = "Test prom CRC";
            this.testPromCrc.UseVisualStyleBackColor = true;
            this.testPromCrc.Visible = false;
            this.testPromCrc.CheckedChanged += new System.EventHandler(this.testPromCrc_CheckedChanged);
            // 
            // testClockOffset
            // 
            this.testClockOffset.AutoSize = true;
            this.testClockOffset.Location = new System.Drawing.Point(14, 71);
            this.testClockOffset.Name = "testClockOffset";
            this.testClockOffset.Size = new System.Drawing.Size(100, 16);
            this.testClockOffset.TabIndex = 3;
            this.testClockOffset.Text = "Test clock offset";
            this.testClockOffset.UseVisualStyleBackColor = true;
            this.testClockOffset.CheckedChanged += new System.EventHandler(this.testClockOffset_CheckedChanged);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(29, 95);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(52, 12);
            this.label7.TabIndex = 6;
            this.label7.Text = "Threshold";
            // 
            // clockOffsetThreshold
            // 
            this.clockOffsetThreshold.Enabled = false;
            this.clockOffsetThreshold.Location = new System.Drawing.Point(92, 90);
            this.clockOffsetThreshold.MaxLength = 10;
            this.clockOffsetThreshold.Name = "clockOffsetThreshold";
            this.clockOffsetThreshold.Size = new System.Drawing.Size(77, 22);
            this.clockOffsetThreshold.TabIndex = 4;
            this.clockOffsetThreshold.TextChanged += new System.EventHandler(this.clockOffsetThreshold_TextChanged);
            // 
            // writeClockOffset
            // 
            this.writeClockOffset.AutoSize = true;
            this.writeClockOffset.Location = new System.Drawing.Point(31, 120);
            this.writeClockOffset.Name = "writeClockOffset";
            this.writeClockOffset.Size = new System.Drawing.Size(157, 16);
            this.writeClockOffset.TabIndex = 5;
            this.writeClockOffset.Text = "Write clock offset to module";
            this.writeClockOffset.UseVisualStyleBackColor = true;
            this.writeClockOffset.CheckedChanged += new System.EventHandler(this.writeClockOffset_CheckedChanged);
            // 
            // testEcompass
            // 
            this.testEcompass.AutoSize = true;
            this.testEcompass.Location = new System.Drawing.Point(14, 194);
            this.testEcompass.Name = "testEcompass";
            this.testEcompass.Size = new System.Drawing.Size(94, 16);
            this.testEcompass.TabIndex = 6;
            this.testEcompass.Text = "Test e-compass";
            this.testEcompass.UseVisualStyleBackColor = true;
            this.testEcompass.CheckedChanged += new System.EventHandler(this.testEcompass_CheckedChanged);
            // 
            // testMiniHommer
            // 
            this.testMiniHommer.AutoSize = true;
            this.testMiniHommer.Location = new System.Drawing.Point(14, 219);
            this.testMiniHommer.Name = "testMiniHommer";
            this.testMiniHommer.Size = new System.Drawing.Size(155, 16);
            this.testMiniHommer.TabIndex = 7;
            this.testMiniHommer.Text = "Check miniHommer actived";
            this.testMiniHommer.UseVisualStyleBackColor = true;
            this.testMiniHommer.CheckedChanged += new System.EventHandler(this.testMiniHommer_CheckedChanged);
            // 
            // testDrCyro
            // 
            this.testDrCyro.AutoSize = true;
            this.testDrCyro.Location = new System.Drawing.Point(14, 244);
            this.testDrCyro.Name = "testDrCyro";
            this.testDrCyro.Size = new System.Drawing.Size(89, 16);
            this.testDrCyro.TabIndex = 8;
            this.testDrCyro.Text = "Test DR Cyro";
            this.testDrCyro.UseVisualStyleBackColor = true;
            this.testDrCyro.CheckedChanged += new System.EventHandler(this.testDrCyro_CheckedChanged);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(29, 268);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(46, 12);
            this.label8.TabIndex = 6;
            this.label8.Text = "Duration";
            // 
            // testDrDuration
            // 
            this.testDrDuration.Enabled = false;
            this.testDrDuration.Location = new System.Drawing.Point(85, 263);
            this.testDrDuration.MaxLength = 4;
            this.testDrDuration.Name = "testDrDuration";
            this.testDrDuration.Size = new System.Drawing.Size(84, 22);
            this.testDrDuration.TabIndex = 9;
            this.testDrDuration.TextChanged += new System.EventHandler(this.testDrDuration_TextChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(55, 310);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(26, 12);
            this.label9.TabIndex = 6;
            this.label9.Text = "USL";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(54, 336);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(25, 12);
            this.label10.TabIndex = 6;
            this.label10.Text = "LSL";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(83, 292);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(53, 12);
            this.label11.TabIndex = 6;
            this.label11.Text = "Clockwise";
            // 
            // uslClockWise
            // 
            this.uslClockWise.Enabled = false;
            this.uslClockWise.Location = new System.Drawing.Point(85, 307);
            this.uslClockWise.MaxLength = 10;
            this.uslClockWise.Name = "uslClockWise";
            this.uslClockWise.Size = new System.Drawing.Size(62, 22);
            this.uslClockWise.TabIndex = 10;
            this.uslClockWise.TextChanged += new System.EventHandler(this.uslClockWise_TextChanged);
            // 
            // lslClockWise
            // 
            this.lslClockWise.Enabled = false;
            this.lslClockWise.Location = new System.Drawing.Point(85, 333);
            this.lslClockWise.MaxLength = 10;
            this.lslClockWise.Name = "lslClockWise";
            this.lslClockWise.Size = new System.Drawing.Size(62, 22);
            this.lslClockWise.TabIndex = 12;
            this.lslClockWise.TextChanged += new System.EventHandler(this.lslClockWise_TextChanged);
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(150, 292);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(70, 12);
            this.label13.TabIndex = 6;
            this.label13.Text = "Anticlockwise";
            // 
            // uslAnticlockWise
            // 
            this.uslAnticlockWise.Enabled = false;
            this.uslAnticlockWise.Location = new System.Drawing.Point(154, 307);
            this.uslAnticlockWise.MaxLength = 10;
            this.uslAnticlockWise.Name = "uslAnticlockWise";
            this.uslAnticlockWise.Size = new System.Drawing.Size(62, 22);
            this.uslAnticlockWise.TabIndex = 11;
            this.uslAnticlockWise.TextChanged += new System.EventHandler(this.uslAnticlockWise_TextChanged);
            // 
            // lslAnticlockWise
            // 
            this.lslAnticlockWise.Enabled = false;
            this.lslAnticlockWise.Location = new System.Drawing.Point(154, 333);
            this.lslAnticlockWise.MaxLength = 10;
            this.lslAnticlockWise.Name = "lslAnticlockWise";
            this.lslAnticlockWise.Size = new System.Drawing.Size(62, 22);
            this.lslAnticlockWise.TabIndex = 13;
            this.lslAnticlockWise.TextChanged += new System.EventHandler(this.lslAnticlockWise_TextChanged);
            // 
            // gpPassCriteria
            // 
            this.gpPassCriteria.AutoSize = true;
            this.gpPassCriteria.Location = new System.Drawing.Point(12, 153);
            this.gpPassCriteria.Name = "gpPassCriteria";
            this.gpPassCriteria.Size = new System.Drawing.Size(91, 16);
            this.gpPassCriteria.TabIndex = 3;
            this.gpPassCriteria.Text = "Test GPS SNR";
            this.gpPassCriteria.UseVisualStyleBackColor = true;
            this.gpPassCriteria.CheckedChanged += new System.EventHandler(this.gpPassCriteria_CheckedChanged);
            // 
            // glPassCriteria
            // 
            this.glPassCriteria.AutoSize = true;
            this.glPassCriteria.Location = new System.Drawing.Point(12, 183);
            this.glPassCriteria.Name = "glPassCriteria";
            this.glPassCriteria.Size = new System.Drawing.Size(107, 16);
            this.glPassCriteria.TabIndex = 6;
            this.glPassCriteria.Text = "Test Glonass SNR";
            this.glPassCriteria.UseVisualStyleBackColor = true;
            this.glPassCriteria.CheckedChanged += new System.EventHandler(this.glPassCriteria_CheckedChanged);
            // 
            // bdPassCriteria
            // 
            this.bdPassCriteria.AutoSize = true;
            this.bdPassCriteria.Location = new System.Drawing.Point(12, 213);
            this.bdPassCriteria.Name = "bdPassCriteria";
            this.bdPassCriteria.Size = new System.Drawing.Size(105, 16);
            this.bdPassCriteria.TabIndex = 9;
            this.bdPassCriteria.Text = "Test Beidou SNR";
            this.bdPassCriteria.UseVisualStyleBackColor = true;
            this.bdPassCriteria.CheckedChanged += new System.EventHandler(this.bdPassCriteria_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label12);
            this.groupBox1.Controls.Add(this.gdBaudSel);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(299, 52);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Golden Sample";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(10, 22);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(91, 12);
            this.label12.TabIndex = 3;
            this.label12.Text = "Golden Baud Rate";
            // 
            // gdBaudSel
            // 
            this.gdBaudSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gdBaudSel.FormattingEnabled = true;
            this.gdBaudSel.Location = new System.Drawing.Point(142, 17);
            this.gdBaudSel.Name = "gdBaudSel";
            this.gdBaudSel.Size = new System.Drawing.Size(144, 20);
            this.gdBaudSel.TabIndex = 0;
            this.gdBaudSel.SelectedIndexChanged += new System.EventHandler(this.gdBaudSel_SelectedIndexChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.gpModule);
            this.groupBox2.Controls.Add(this.glModule);
            this.groupBox2.Controls.Add(this.gaModuleSel);
            this.groupBox2.Controls.Add(this.bdModuleSel);
            this.groupBox2.Controls.Add(this.gaModule);
            this.groupBox2.Controls.Add(this.bdModule);
            this.groupBox2.Controls.Add(this.glModuleSel);
            this.groupBox2.Controls.Add(this.gpModuleSel);
            this.groupBox2.Location = new System.Drawing.Point(12, 70);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(299, 141);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Test Module";
            // 
            // gaModuleSel
            // 
            this.gaModuleSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gaModuleSel.FormattingEnabled = true;
            this.gaModuleSel.Location = new System.Drawing.Point(142, 112);
            this.gaModuleSel.Name = "gaModuleSel";
            this.gaModuleSel.Size = new System.Drawing.Size(144, 20);
            this.gaModuleSel.TabIndex = 7;
            this.gaModuleSel.SelectedIndexChanged += new System.EventHandler(this.gaModuleSel_SelectedIndexChanged);
            // 
            // gaModule
            // 
            this.gaModule.AutoSize = true;
            this.gaModule.Location = new System.Drawing.Point(12, 112);
            this.gaModule.Name = "gaModule";
            this.gaModule.Size = new System.Drawing.Size(95, 16);
            this.gaModule.TabIndex = 3;
            this.gaModule.TabStop = true;
            this.gaModule.Text = "Galileo Module";
            this.gaModule.UseVisualStyleBackColor = true;
            this.gaModule.CheckedChanged += new System.EventHandler(this.gaModule_CheckedChanged);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label16);
            this.groupBox3.Controls.Add(this.label14);
            this.groupBox3.Controls.Add(this.label20);
            this.groupBox3.Controls.Add(this.label15);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.browseIni);
            this.groupBox3.Controls.Add(this.moduleName);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.snrTestPeriod);
            this.groupBox3.Controls.Add(this.gaSnrLower);
            this.groupBox3.Controls.Add(this.bdSnrLower);
            this.groupBox3.Controls.Add(this.glSnrLower);
            this.groupBox3.Controls.Add(this.gaSnrUpper);
            this.groupBox3.Controls.Add(this.bdSnrUpper);
            this.groupBox3.Controls.Add(this.glSnrUpper);
            this.groupBox3.Controls.Add(this.gpSnrLower);
            this.groupBox3.Controls.Add(this.gpSnrUpper);
            this.groupBox3.Controls.Add(this.gpSnrLimit);
            this.groupBox3.Controls.Add(this.gaPassCriteria);
            this.groupBox3.Controls.Add(this.bdPassCriteria);
            this.groupBox3.Controls.Add(this.glSnrLimit);
            this.groupBox3.Controls.Add(this.gaSnrLimit);
            this.groupBox3.Controls.Add(this.glPassCriteria);
            this.groupBox3.Controls.Add(this.bdSnrLimit);
            this.groupBox3.Controls.Add(this.gpPassCriteria);
            this.groupBox3.Controls.Add(this.iniFileName);
            this.groupBox3.Location = new System.Drawing.Point(12, 217);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(299, 276);
            this.groupBox3.TabIndex = 12;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Test Module Setting";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(237, 99);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(49, 12);
            this.label16.TabIndex = 10;
            this.label16.Text = "second(s)";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(10, 68);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(38, 12);
            this.label14.TabIndex = 3;
            this.label14.Text = "Ini File";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(179, 132);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(34, 12);
            this.label20.TabIndex = 10;
            this.label20.Text = "Upper";
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(125, 132);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(35, 12);
            this.label15.TabIndex = 10;
            this.label15.Text = "Lower";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(233, 132);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 12);
            this.label3.TabIndex = 10;
            this.label3.Text = "SNR Limit";
            // 
            // browseIni
            // 
            this.browseIni.Location = new System.Drawing.Point(254, 61);
            this.browseIni.Name = "browseIni";
            this.browseIni.Size = new System.Drawing.Size(32, 23);
            this.browseIni.TabIndex = 1;
            this.browseIni.Text = "...";
            this.browseIni.UseVisualStyleBackColor = true;
            this.browseIni.Click += new System.EventHandler(this.browseIni_Click);
            // 
            // moduleName
            // 
            this.moduleName.AutoSize = true;
            this.moduleName.Font = new System.Drawing.Font("Segoe UI Symbol", 20.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.moduleName.ForeColor = System.Drawing.SystemColors.HotTrack;
            this.moduleName.Location = new System.Drawing.Point(6, 19);
            this.moduleName.Name = "moduleName";
            this.moduleName.Size = new System.Drawing.Size(167, 37);
            this.moduleName.TabIndex = 9;
            this.moduleName.Text = "TEST2535BD";
            this.moduleName.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            this.moduleName.UseMnemonic = false;
            this.moduleName.TextChanged += new System.EventHandler(this.moduleName_TextChanged);
            // 
            // snrTestPeriod
            // 
            this.snrTestPeriod.Location = new System.Drawing.Point(159, 96);
            this.snrTestPeriod.MaxLength = 4;
            this.snrTestPeriod.Name = "snrTestPeriod";
            this.snrTestPeriod.Size = new System.Drawing.Size(72, 22);
            this.snrTestPeriod.TabIndex = 2;
            this.snrTestPeriod.TextChanged += new System.EventHandler(this.snrTestPeriod_TextChanged);
            // 
            // gaSnrLower
            // 
            this.gaSnrLower.Enabled = false;
            this.gaSnrLower.Location = new System.Drawing.Point(127, 244);
            this.gaSnrLower.MaxLength = 4;
            this.gaSnrLower.Name = "gaSnrLower";
            this.gaSnrLower.Size = new System.Drawing.Size(48, 22);
            this.gaSnrLower.TabIndex = 5;
            this.gaSnrLower.TextChanged += new System.EventHandler(this.gaSnrLower_TextChanged);
            // 
            // bdSnrLower
            // 
            this.bdSnrLower.Enabled = false;
            this.bdSnrLower.Location = new System.Drawing.Point(127, 212);
            this.bdSnrLower.MaxLength = 4;
            this.bdSnrLower.Name = "bdSnrLower";
            this.bdSnrLower.Size = new System.Drawing.Size(48, 22);
            this.bdSnrLower.TabIndex = 5;
            this.bdSnrLower.TextChanged += new System.EventHandler(this.bdSnrLower_TextChanged);
            // 
            // glSnrLower
            // 
            this.glSnrLower.Enabled = false;
            this.glSnrLower.Location = new System.Drawing.Point(127, 181);
            this.glSnrLower.MaxLength = 4;
            this.glSnrLower.Name = "glSnrLower";
            this.glSnrLower.Size = new System.Drawing.Size(48, 22);
            this.glSnrLower.TabIndex = 5;
            this.glSnrLower.TextChanged += new System.EventHandler(this.glSnrLower_TextChanged);
            // 
            // gaSnrUpper
            // 
            this.gaSnrUpper.Enabled = false;
            this.gaSnrUpper.Location = new System.Drawing.Point(181, 245);
            this.gaSnrUpper.MaxLength = 4;
            this.gaSnrUpper.Name = "gaSnrUpper";
            this.gaSnrUpper.Size = new System.Drawing.Size(48, 22);
            this.gaSnrUpper.TabIndex = 5;
            this.gaSnrUpper.TextChanged += new System.EventHandler(this.gaSnrUpper_TextChanged);
            // 
            // bdSnrUpper
            // 
            this.bdSnrUpper.Enabled = false;
            this.bdSnrUpper.Location = new System.Drawing.Point(181, 212);
            this.bdSnrUpper.MaxLength = 4;
            this.bdSnrUpper.Name = "bdSnrUpper";
            this.bdSnrUpper.Size = new System.Drawing.Size(48, 22);
            this.bdSnrUpper.TabIndex = 5;
            this.bdSnrUpper.TextChanged += new System.EventHandler(this.bdSnrUpper_TextChanged);
            // 
            // glSnrUpper
            // 
            this.glSnrUpper.Enabled = false;
            this.glSnrUpper.Location = new System.Drawing.Point(181, 181);
            this.glSnrUpper.MaxLength = 4;
            this.glSnrUpper.Name = "glSnrUpper";
            this.glSnrUpper.Size = new System.Drawing.Size(48, 22);
            this.glSnrUpper.TabIndex = 5;
            this.glSnrUpper.TextChanged += new System.EventHandler(this.glSnrUpper_TextChanged);
            // 
            // gpSnrLower
            // 
            this.gpSnrLower.Enabled = false;
            this.gpSnrLower.Location = new System.Drawing.Point(127, 151);
            this.gpSnrLower.MaxLength = 4;
            this.gpSnrLower.Name = "gpSnrLower";
            this.gpSnrLower.Size = new System.Drawing.Size(48, 22);
            this.gpSnrLower.TabIndex = 5;
            this.gpSnrLower.TextChanged += new System.EventHandler(this.gpSnrLower_TextChanged);
            // 
            // gpSnrUpper
            // 
            this.gpSnrUpper.Enabled = false;
            this.gpSnrUpper.Location = new System.Drawing.Point(181, 151);
            this.gpSnrUpper.MaxLength = 4;
            this.gpSnrUpper.Name = "gpSnrUpper";
            this.gpSnrUpper.Size = new System.Drawing.Size(48, 22);
            this.gpSnrUpper.TabIndex = 5;
            this.gpSnrUpper.TextChanged += new System.EventHandler(this.gpSnrUpper_TextChanged);
            // 
            // gaPassCriteria
            // 
            this.gaPassCriteria.AutoSize = true;
            this.gaPassCriteria.Location = new System.Drawing.Point(12, 246);
            this.gaPassCriteria.Name = "gaPassCriteria";
            this.gaPassCriteria.Size = new System.Drawing.Size(104, 16);
            this.gaPassCriteria.TabIndex = 12;
            this.gaPassCriteria.Text = "Test Galileo SNR";
            this.gaPassCriteria.UseVisualStyleBackColor = true;
            this.gaPassCriteria.CheckedChanged += new System.EventHandler(this.gaPassCriteria_CheckedChanged);
            // 
            // gaSnrLimit
            // 
            this.gaSnrLimit.Enabled = false;
            this.gaSnrLimit.Location = new System.Drawing.Point(235, 244);
            this.gaSnrLimit.MaxLength = 3;
            this.gaSnrLimit.Name = "gaSnrLimit";
            this.gaSnrLimit.Size = new System.Drawing.Size(48, 22);
            this.gaSnrLimit.TabIndex = 14;
            this.gaSnrLimit.TextChanged += new System.EventHandler(this.gaSnrLimit_TextChanged);
            // 
            // iniFileName
            // 
            this.iniFileName.Location = new System.Drawing.Point(51, 63);
            this.iniFileName.MaxLength = 260;
            this.iniFileName.Name = "iniFileName";
            this.iniFileName.Size = new System.Drawing.Size(200, 22);
            this.iniFileName.TabIndex = 0;
            this.iniFileName.TextChanged += new System.EventHandler(this.iniFileName_TextChanged);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.checkPromCrc);
            this.groupBox4.Controls.Add(this.label18);
            this.groupBox4.Controls.Add(this.dlBaudSel);
            this.groupBox4.Controls.Add(this.writeTag);
            this.groupBox4.Controls.Add(this.enableDownload);
            this.groupBox4.Controls.Add(this.testBootStatus);
            this.groupBox4.Controls.Add(this.label17);
            this.groupBox4.Controls.Add(this.label7);
            this.groupBox4.Controls.Add(this.label19);
            this.groupBox4.Controls.Add(this.label8);
            this.groupBox4.Controls.Add(this.label9);
            this.groupBox4.Controls.Add(this.writeClockOffset);
            this.groupBox4.Controls.Add(this.label10);
            this.groupBox4.Controls.Add(this.testDrCyro);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Controls.Add(this.testMiniHommer);
            this.groupBox4.Controls.Add(this.label13);
            this.groupBox4.Controls.Add(this.testEcompass);
            this.groupBox4.Controls.Add(this.testClockOffset);
            this.groupBox4.Controls.Add(this.clockOffsetThreshold);
            this.groupBox4.Controls.Add(this.lslClockWise);
            this.groupBox4.Controls.Add(this.thresholdCogWise);
            this.groupBox4.Controls.Add(this.testDrDuration);
            this.groupBox4.Controls.Add(this.lslAnticlockWise);
            this.groupBox4.Controls.Add(this.uslClockWise);
            this.groupBox4.Controls.Add(this.uslAnticlockWise);
            this.groupBox4.Location = new System.Drawing.Point(317, 12);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(295, 481);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Test Items";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(175, 268);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(49, 12);
            this.label18.TabIndex = 10;
            this.label18.Text = "second(s)";
            // 
            // dlBaudSel
            // 
            this.dlBaudSel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.dlBaudSel.FormattingEnabled = true;
            this.dlBaudSel.Location = new System.Drawing.Point(182, 17);
            this.dlBaudSel.Name = "dlBaudSel";
            this.dlBaudSel.Size = new System.Drawing.Size(100, 20);
            this.dlBaudSel.TabIndex = 0;
            this.dlBaudSel.SelectedIndexChanged += new System.EventHandler(this.dlBaudSel_SelectedIndexChanged);
            // 
            // writeTag
            // 
            this.writeTag.AutoSize = true;
            this.writeTag.Location = new System.Drawing.Point(14, 142);
            this.writeTag.Name = "writeTag";
            this.writeTag.Size = new System.Drawing.Size(67, 16);
            this.writeTag.TabIndex = 0;
            this.writeTag.Text = "Write tag";
            this.writeTag.UseVisualStyleBackColor = true;
            this.writeTag.Visible = false;
            this.writeTag.CheckedChanged += new System.EventHandler(this.writeTag_CheckedChanged);
            // 
            // enableDownload
            // 
            this.enableDownload.AutoSize = true;
            this.enableDownload.Location = new System.Drawing.Point(14, 21);
            this.enableDownload.Name = "enableDownload";
            this.enableDownload.Size = new System.Drawing.Size(169, 16);
            this.enableDownload.TabIndex = 1;
            this.enableDownload.Text = "Enable download in baud rate :";
            this.enableDownload.UseVisualStyleBackColor = true;
            this.enableDownload.CheckedChanged += new System.EventHandler(this.enableDownload_CheckedChanged);
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(175, 95);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(26, 12);
            this.label17.TabIndex = 6;
            this.label17.Text = "ppm";
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(29, 364);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(92, 12);
            this.label19.TabIndex = 6;
            this.label19.Text = "Threshold of COG";
            // 
            // thresholdCogWise
            // 
            this.thresholdCogWise.Enabled = false;
            this.thresholdCogWise.Location = new System.Drawing.Point(132, 361);
            this.thresholdCogWise.MaxLength = 10;
            this.thresholdCogWise.Name = "thresholdCogWise";
            this.thresholdCogWise.Size = new System.Drawing.Size(84, 22);
            this.thresholdCogWise.TabIndex = 14;
            this.thresholdCogWise.TextChanged += new System.EventHandler(this.thresholdCogWise_TextChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(50, 597);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(50, 12);
            this.label4.TabIndex = 3;
            this.label4.Text = "Prom File";
            this.label4.Visible = false;
            // 
            // browseFw
            // 
            this.browseFw.Enabled = false;
            this.browseFw.Location = new System.Drawing.Point(270, 593);
            this.browseFw.Name = "browseFw";
            this.browseFw.Size = new System.Drawing.Size(32, 23);
            this.browseFw.TabIndex = 9;
            this.browseFw.Text = "...";
            this.browseFw.UseVisualStyleBackColor = true;
            this.browseFw.Visible = false;
            this.browseFw.Click += new System.EventHandler(this.browseFw_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(207, 553);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(24, 12);
            this.label5.TabIndex = 6;
            this.label5.Text = "Hex";
            this.label5.Visible = false;
            // 
            // promFileName
            // 
            this.promFileName.Enabled = false;
            this.promFileName.Location = new System.Drawing.Point(106, 592);
            this.promFileName.Name = "promFileName";
            this.promFileName.Size = new System.Drawing.Size(155, 22);
            this.promFileName.TabIndex = 7;
            this.promFileName.Visible = false;
            this.promFileName.TextChanged += new System.EventHandler(this.promFileName_TextChanged);
            // 
            // saveAs
            // 
            this.saveAs.Location = new System.Drawing.Point(298, 503);
            this.saveAs.Name = "saveAs";
            this.saveAs.Size = new System.Drawing.Size(75, 23);
            this.saveAs.TabIndex = 1;
            this.saveAs.Text = "Save As...";
            this.saveAs.UseVisualStyleBackColor = true;
            this.saveAs.Click += new System.EventHandler(this.saveAs_Click);
            // 
            // loadFrom
            // 
            this.loadFrom.Location = new System.Drawing.Point(217, 503);
            this.loadFrom.Name = "loadFrom";
            this.loadFrom.Size = new System.Drawing.Size(75, 23);
            this.loadFrom.TabIndex = 1;
            this.loadFrom.Text = "Load From...";
            this.loadFrom.UseVisualStyleBackColor = true;
            this.loadFrom.Click += new System.EventHandler(this.loadFrom_Click);
            // 
            // checkPromCrc
            // 
            this.checkPromCrc.AutoSize = true;
            this.checkPromCrc.Location = new System.Drawing.Point(14, 46);
            this.checkPromCrc.Name = "checkPromCrc";
            this.checkPromCrc.Size = new System.Drawing.Size(99, 16);
            this.checkPromCrc.TabIndex = 15;
            this.checkPromCrc.Text = "Check prom crc";
            this.checkPromCrc.UseVisualStyleBackColor = true;
            this.checkPromCrc.CheckedChanged += new System.EventHandler(this.checkPromCrc_CheckedChanged);
            // 
            // SettingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(624, 535);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.browseFw);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.testPromCrc);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.dvBaudSel);
            this.Controls.Add(this.Cancel);
            this.Controls.Add(this.loadFrom);
            this.Controls.Add(this.saveAs);
            this.Controls.Add(this.OK);
            this.Controls.Add(this.promFileName);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingForm";
            this.Text = "Test Profile Setting";
            this.Load += new System.EventHandler(this.SettingForm_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button OK;
        private System.Windows.Forms.Button Cancel;
        private System.Windows.Forms.ComboBox dvBaudSel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.RadioButton gpModule;
        private System.Windows.Forms.RadioButton glModule;
        private System.Windows.Forms.ComboBox gpModuleSel;
        private System.Windows.Forms.ComboBox glModuleSel;
        private System.Windows.Forms.RadioButton bdModule;
        private System.Windows.Forms.ComboBox bdModuleSel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox gpSnrLimit;
        private System.Windows.Forms.TextBox glSnrLimit;
        private System.Windows.Forms.TextBox bdSnrLimit;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.CheckBox testBootStatus;
        private System.Windows.Forms.CheckBox testPromCrc;
        private System.Windows.Forms.CheckBox testClockOffset;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox clockOffsetThreshold;
        private System.Windows.Forms.CheckBox writeClockOffset;
        private System.Windows.Forms.CheckBox testEcompass;
        private System.Windows.Forms.CheckBox testMiniHommer;
        private System.Windows.Forms.CheckBox testDrCyro;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox testDrDuration;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.TextBox uslClockWise;
        private System.Windows.Forms.TextBox lslClockWise;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.TextBox uslAnticlockWise;
        private System.Windows.Forms.TextBox lslAnticlockWise;
        private System.Windows.Forms.CheckBox gpPassCriteria;
        private System.Windows.Forms.CheckBox glPassCriteria;
        private System.Windows.Forms.CheckBox bdPassCriteria;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label moduleName;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox gdBaudSel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button browseFw;
        private System.Windows.Forms.CheckBox enableDownload;
        private System.Windows.Forms.TextBox promFileName;
        private System.Windows.Forms.ComboBox gaModuleSel;
        private System.Windows.Forms.RadioButton gaModule;
        private System.Windows.Forms.CheckBox gaPassCriteria;
        private System.Windows.Forms.TextBox gaSnrLimit;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Button browseIni;
        private System.Windows.Forms.CheckBox writeTag;
        private System.Windows.Forms.TextBox iniFileName;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.TextBox thresholdCogWise;
        private System.Windows.Forms.Button saveAs;
        private System.Windows.Forms.Button loadFrom;
        private System.Windows.Forms.ComboBox dlBaudSel;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox gpSnrLower;
        private System.Windows.Forms.TextBox gpSnrUpper;
        private System.Windows.Forms.TextBox glSnrLower;
        private System.Windows.Forms.TextBox gaSnrLower;
        private System.Windows.Forms.TextBox bdSnrLower;
        private System.Windows.Forms.TextBox gaSnrUpper;
        private System.Windows.Forms.TextBox bdSnrUpper;
        private System.Windows.Forms.TextBox glSnrUpper;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.TextBox snrTestPeriod;
        private System.Windows.Forms.CheckBox checkPromCrc;
    }
}