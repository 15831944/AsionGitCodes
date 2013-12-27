using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using SharpGL;

namespace TestSharpGl_01
{
    public partial class Form1 : Form
    {
        private SharpGL.OpenGL gl = null;
        public Form1()
        {
            InitializeComponent();
        }

        double xrot = 0;        // X 旋轉
        double yrot = 0;        // Y 旋轉
        double xspeed = 0;      // X 旋轉速度
        double yspeed = 0;      // Y 旋轉速度
        double z = -5.0;	    // 深入屏幕的距離


        int	filter = 0;						    // 濾波類型
        Bitmap textureImage;
        uint[] textures = new uint[3];

        private void Start_Click(object sender, EventArgs e)
        {

        }


        private void gl1_SizeChanged(object sender, EventArgs e)
        {
            if (gl == null)
                return;

            //設置OpenGL場景的大小
            gl.Viewport(0, 0, this.gl1.Width, this.gl1.Height); //當視窗長寬改變時，畫面也跟著變
            gl.MatrixMode(OpenGL.GL_PROJECTION);    // 選擇投影矩陣
            gl.LoadIdentity();                      // 重置投影矩陣
            gl.Perspective(45.0, gl1.Width / gl1.Height, 0.1, 1.0);
            //gl.Ortho(-10, 10, -10, 10, -10, 10);    //正交投影
            gl.MatrixMode(OpenGL.GL_MODELVIEW);       // 選擇模型觀察矩陣
            gl.LoadIdentity();
        }

       // double xrot = 0.0, yrot = 0.0, zrot = 0.0;
        private void gl1_OpenGLDraw(object sender, RenderEventArgs args)
        {
	        gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度緩存
	        gl.LoadIdentity();							// 重置當前的模型觀察矩陣

           
            gl.Translate(0.0, 0.0, z);          // 左移 1.5 單位，並移入屏幕 6.0
            gl.Rotate(xrot, 1.0, 0.0, 0.0);
            gl.Rotate(yrot, 0.0, 1.0, 0.0);
            //gl.Rotate(zrot, 0.0, 0.0, 1.0);
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[filter]);

            gl.Begin(OpenGL.GL_QUADS);      //  繪製立方體
            // 前面
            gl.Normal(0.0, 0.0, 1.0);	
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的左上
            // 後面
            gl.Normal(0.0, 0.0, -1.0);
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的左下
            // 頂面
            gl.Normal(0.0, 1.0, 0.0);
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            // 底面
            gl.Normal(0.0, -1.0, 0.0);
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            // 右面
            gl.Normal(1.0, 0.0, 0.0);
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            // 左面
            gl.Normal(-1.0, 0.0, 0.0);
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的左上
            gl.End();

            xrot += xspeed;								// xrot 增加 xspeed 單位
            yrot += yspeed;								// yrot 增加 yspeed 單位

            return;
        }

        private bool glRotateEnable = false;
        private void gl1_MouseUp(object sender, MouseEventArgs e)
        {
            this.gl1.Capture = false;
            glRotateEnable = false;
            //xrot = 0;
            //yrot = 0;
            gl1.Refresh();
            oldXrot = xrot;
            oldYrot = yrot;

        }

        private void gl1_MouseMove(object sender, MouseEventArgs e)
        {
            if (!glRotateEnable)
                return;

            yrot = oldYrot + 0.2 * (e.X - centerX);
            xrot = oldXrot + 0.2 * (e.Y - centerY);
            gl1.Refresh();
        }
        int centerX, centerY;
        double oldYrot = 0.0, oldXrot = 0.0;
        private void gl1_MouseDown(object sender, MouseEventArgs e)
        {
            this.gl1.Capture = true;
            glRotateEnable = true;
            centerX = e.X;
            centerY = e.Y;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textureMode.SelectedIndex = filter;
        }

 
        private void gl1_OpenGLInitialized(object sender, EventArgs e)
        {
            gl = this.gl1.OpenGL;
            BitmapData bp = null;

            var stream = File.OpenRead(@"Textures\Texture038.bmp");
            textureImage = (Bitmap)Bitmap.FromStream(stream);
            gl.Enable(OpenGL.GL_TEXTURE_2D);        // 啟用紋理映射
            bp = textureImage.LockBits(new Rectangle(0, 0, textureImage.Width, textureImage.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);

            gl.GenTextures(3, textures);
            //  Bind the texture.
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[0]);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MIN_FILTER, OpenGL.GL_LINEAR);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAG_FILTER, OpenGL.GL_LINEAR);
            gl.TexImage2D(OpenGL.GL_TEXTURE_2D, 0, 3, textureImage.Width, textureImage.Height, 0, OpenGL.GL_BGR, OpenGL.GL_UNSIGNED_BYTE, bp.Scan0);

            //  Bind the texture.
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[1]);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MIN_FILTER, OpenGL.GL_NEAREST);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAG_FILTER, OpenGL.GL_NEAREST);
            gl.TexImage2D(OpenGL.GL_TEXTURE_2D, 0, 3, textureImage.Width, textureImage.Height, 0, OpenGL.GL_BGR, OpenGL.GL_UNSIGNED_BYTE, bp.Scan0);

            //  Bind the texture.
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[2]);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MIN_FILTER, OpenGL.GL_LINEAR_MIPMAP_NEAREST);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAG_FILTER, OpenGL.GL_LINEAR);
            gl.Build2DMipmaps(OpenGL.GL_TEXTURE_2D, 3, textureImage.Width, textureImage.Height, OpenGL.GL_BGR, OpenGL.GL_UNSIGNED_BYTE, bp.Scan0);
            textureImage.UnlockBits(bp);

            gl.ShadeModel(OpenGL.GL_SMOOTH);        // 啟用陰影平滑
            gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 黑色背景
            gl.ClearDepth(1.0f);					// 設置深度緩存
            gl.Enable(OpenGL.GL_DEPTH_TEST);		// 啟用深度測試
            gl.DepthFunc(OpenGL.GL_LEQUAL);			// 所作深度測試的類型
            gl.Hint(OpenGL.GL_PERSPECTIVE_CORRECTION_HINT, OpenGL.GL_NICEST);    // 告訴系統對透視進行修正
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            filter = textureMode.SelectedIndex;
            gl1.Refresh();
        }

        //環境光來自於四面八方。所有場景中的對象都處於環境光的照射中。
        float[] LightAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };     // 環境光參數
        //漫射光由特定的光源產生，並在您的場景中的對象表面上產生反射。
        float[] LightDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };     // 漫射光參數
        float[] LightPosition = { 0.0f, 0.0f, 2.0f, 1.0f };     // 光源位置
        private void light_CheckedChanged(object sender, EventArgs e)
        {
            if (light.Checked)
            {
                gl.Light(OpenGL.GL_LIGHT1, OpenGL.GL_AMBIENT, LightAmbient);    // 設置環境光
                gl.Light(OpenGL.GL_LIGHT1, OpenGL.GL_DIFFUSE, LightDiffuse);    // 設置漫射光       
                gl.Light(OpenGL.GL_LIGHT1, OpenGL.GL_POSITION, LightPosition); // 設置光源位置
                gl.Enable(OpenGL.GL_LIGHT1);
                gl.Enable(OpenGL.GL_LIGHTING);
            }
            else
            {
                gl.Disable(OpenGL.GL_LIGHTING);
            }
            gl1.Refresh();
        }
    }
}
