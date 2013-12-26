using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Windows.Forms;
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
        //private int[] light_ambient = { 1, 1, 1, 1 };
        //private int[] light_diffuse = { 1, 1, 1, 1 };
        //private int[] light_specular = { 1, 1, 1, 1 };
        //private int[] light_position = { 0, 0, 20 }; 

        private void Start_Click(object sender, EventArgs e)
        {
            float[] light_ambient  = { 1.0f, 1.0f, 1.0f, 0.1f };
            float[] light_diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
            float[] light_specular = { 1.0f, 1.0f, 1.0f, 1.0f };
            float[] light_position = { 0.0f, 0.0f, 50.0f, 1.0f }; 

 
             gl.Light(OpenGL.GL_LIGHT0, OpenGL.GL_POSITION, light_position);      //光的座標
           // 設定發光體的光源的特性
            //gl.Light(OpenGL.GL_LIGHT0, OpenGL.GL_AMBIENT, light_ambient);      //環境光(Ambient Light)
            //gl.Light(OpenGL.GL_LIGHT0, OpenGL.GL_DIFFUSE, light_diffuse);      //散射光(Diffuse Light)
            gl.Light(OpenGL.GL_LIGHT0, OpenGL.GL_SPECULAR, light_specular);      //反射光(Specular Light)

            float[] light0Direction = { 0.0f, 0.0f, -1.0f };
            //gl.Light(OpenGL.GL_LIGHT0, OpenGL.GL_SPOT_DIRECTION, light0Direction);


            gl.Enable(OpenGL.GL_LIGHT0);
            gl.Enable(OpenGL.GL_LIGHTING);                           //開燈
            gl.Enable(OpenGL.GL_DEPTH_TEST);                        //深度測試
            gl1.Refresh();

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

        double xrot = 0.0, yrot = 0.0, zrot = 0.0;
        private void gl1_OpenGLDraw(object sender, RenderEventArgs args)
        {
	        gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度緩存
	        gl.LoadIdentity();							// 重置當前的模型觀察矩陣

           
            gl.Translate(0.0, 0.0, -5.0);          // 左移 1.5 單位，並移入屏幕 6.0
            gl.Rotate(xrot, 1.0, 0.0, 0.0);
            gl.Rotate(yrot, 0.0, 1.0, 0.0);
            gl.Rotate(zrot, 0.0, 0.0, 1.0);
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[0]);

            gl.Begin(OpenGL.GL_QUADS);      //  繪製立方體
            // 前面
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的左上
            // 後面
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的左下
            // 頂面
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            // 底面
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            // 右面
            gl.TexCoord(1.0, 0.0); gl.Vertex(1.0, -1.0, -1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(1.0, 1.0, -1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(1.0, 1.0, 1.0);	// 紋理和四邊形的左上
            gl.TexCoord(0.0, 0.0); gl.Vertex(1.0, -1.0, 1.0);	// 紋理和四邊形的左下
            // 左面
            gl.TexCoord(0.0, 0.0); gl.Vertex(-1.0, -1.0, -1.0);	// 紋理和四邊形的左下
            gl.TexCoord(1.0, 0.0); gl.Vertex(-1.0, -1.0, 1.0);	// 紋理和四邊形的右下
            gl.TexCoord(1.0, 1.0); gl.Vertex(-1.0, 1.0, 1.0);	// 紋理和四邊形的右上
            gl.TexCoord(0.0, 1.0); gl.Vertex(-1.0, 1.0, -1.0);	// 紋理和四邊形的左上

            gl.End();





            return;
        }

        private bool glRotateEnable = false;
        private void gl1_MouseUp(object sender, MouseEventArgs e)
        {
            this.gl1.Capture = false;
            glRotateEnable = false;
        }

        private void gl1_MouseMove(object sender, MouseEventArgs e)
        {
            if (!glRotateEnable)
                return;

            xrot += 0.3 * 5;						
            yrot += 0.2 * 5;					
            zrot += 0.4 * 5;
            gl1.Refresh();
        }

        private void gl1_MouseDown(object sender, MouseEventArgs e)
        {
            this.gl1.Capture = true;
            glRotateEnable = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
        }

        Bitmap textureImage;
        uint[] textures = new uint[1];
 
        private void gl1_OpenGLInitialized(object sender, EventArgs e)
        {
            gl = this.gl1.OpenGL;
            textureImage = new Bitmap("Textures\\Texture06-s.bmp");
            gl.Enable(OpenGL.GL_TEXTURE_2D);        // 啟用紋理映射
            gl.GenTextures(1, textures);
            //  Bind the texture.
            gl.BindTexture(OpenGL.GL_TEXTURE_2D, textures[0]);
            //  Tell OpenGL where the texture data is.
            gl.TexImage2D(OpenGL.GL_TEXTURE_2D, 0, 3, textureImage.Width, textureImage.Height, 0, OpenGL.GL_BGR, OpenGL.GL_UNSIGNED_BYTE,
                textureImage.LockBits(new Rectangle(0, 0, textureImage.Width, textureImage.Height),
                ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb).Scan0);
            //  Specify linear filtering.
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MIN_FILTER, OpenGL.GL_LINEAR);
            gl.TexParameter(OpenGL.GL_TEXTURE_2D, OpenGL.GL_TEXTURE_MAG_FILTER, OpenGL.GL_LINEAR);

            gl.ShadeModel(OpenGL.GL_SMOOTH);        // 啟用陰影平滑
            gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 黑色背景
            gl.ClearDepth(1.0f);					// 設置深度緩存
            gl.Enable(OpenGL.GL_DEPTH_TEST);		// 啟用深度測試
            gl.DepthFunc(OpenGL.GL_LEQUAL);			// 所作深度測試的類型
            gl.Hint(OpenGL.GL_PERSPECTIVE_CORRECTION_HINT, OpenGL.GL_NICEST);    // 告訴系統對透視進行修正
        }
    }
}
