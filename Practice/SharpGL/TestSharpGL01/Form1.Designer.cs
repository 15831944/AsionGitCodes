namespace TestSharpGl_01
{
    partial class Form1
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
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器
        /// 修改這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.gl1 = new SharpGL.OpenGLControl();
            this.Start = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.gl1)).BeginInit();
            this.SuspendLayout();
            // 
            // gl1
            // 
            this.gl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.gl1.DrawFPS = false;
            this.gl1.Location = new System.Drawing.Point(12, 38);
            this.gl1.Name = "gl1";
            this.gl1.RenderContextType = SharpGL.RenderContextType.DIBSection;
            this.gl1.RenderTrigger = SharpGL.RenderTrigger.TimerBased;
            this.gl1.Size = new System.Drawing.Size(260, 212);
            this.gl1.TabIndex = 0;
            this.gl1.OpenGLInitialized += new System.EventHandler(this.gl1_OpenGLInitialized);
            this.gl1.OpenGLDraw += new SharpGL.RenderEventHandler(this.gl1_OpenGLDraw);
            this.gl1.SizeChanged += new System.EventHandler(this.gl1_SizeChanged);
            this.gl1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.gl1_MouseDown);
            this.gl1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.gl1_MouseMove);
            this.gl1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.gl1_MouseUp);
            // 
            // Start
            // 
            this.Start.Location = new System.Drawing.Point(12, 9);
            this.Start.Name = "Start";
            this.Start.Size = new System.Drawing.Size(75, 23);
            this.Start.TabIndex = 1;
            this.Start.Text = "start";
            this.Start.UseVisualStyleBackColor = true;
            this.Start.Click += new System.EventHandler(this.Start_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.Start);
            this.Controls.Add(this.gl1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.gl1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private SharpGL.OpenGLControl gl1;
        private System.Windows.Forms.Button Start;

    }
}

