using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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
            gl = this.gl1.OpenGL;
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

        double rtri = 0.0, rquad = 0.0;
        private void gl1_OpenGLDraw(object sender, RenderEventArgs args)
        {
	        gl.Clear(OpenGL.GL_COLOR_BUFFER_BIT | OpenGL.GL_DEPTH_BUFFER_BIT);			// 清除屏幕和深度緩存
	        gl.LoadIdentity();							// 重置當前的模型觀察矩陣

            gl.Translate(-1.5, 0.0, -6.0);          // 左移 1.5 單位，並移入屏幕 6.0
            gl.Rotate(rtri, 0.0, 1.0, 0.0);

            gl.Begin(OpenGL.GL_TRIANGLES);  // 開始繪製金字塔的各個面
            //正前面, 面向螢幕
            gl.Color(1.0, 0.0, 0.0);     // 設置當前色為紅色
            gl.Vertex(0.0, 1.0, 0.0);    // 上頂點, 金字塔尖端
            gl.Color(0.0, 1.0, 0.0);		// 設置當前色為綠色
            gl.Vertex(-1.0, -1.0, 1.0);  // 左下
            gl.Color(0.0, 0.0, 1.0);		// 設置當前色為藍色
            gl.Vertex(1.0, -1.0, 1.0);   // 右下
            //右側面, 面向螢幕右方
            gl.Color(1.0, 0.0, 0.0);     // 設置當前色為紅色
            gl.Vertex(0.0, 1.0, 0.0);    // 上頂點, 金字塔尖端
            gl.Color(0.0, 0.0, 1.0);		// 設置當前色為藍色
            gl.Vertex(1.0, -1.0, 1.0);  // 左下
            gl.Color(0.0, 1.0, 0.0);		// 設置當前色為綠色
            gl.Vertex(1.0, -1.0, -1.0);   // 右下
            //後面, 面向螢幕後方
            gl.Color(1.0, 0.0, 0.0);     // 設置當前色為紅色
            gl.Vertex(0.0, 1.0, 0.0);    // 上頂點, 金字塔尖端
            gl.Color(0.0, 1.0, 0.0);		// 設置當前色為綠色
            gl.Vertex(1.0, -1.0, -1.0);  // 左下
            gl.Color(0.0, 0.0, 1.0);		// 設置當前色為藍色
            gl.Vertex(-1.0, -1.0, -1.0);   // 右下
            //右側面, 面向螢幕右方
            gl.Color(1.0, 0.0, 0.0);     // 設置當前色為紅色
            gl.Vertex(0.0, 1.0, 0.0);    // 上頂點, 金字塔尖端
            gl.Color(0.0, 0.0, 1.0);		// 設置當前色為藍色
            gl.Vertex(-1.0, -1.0, -1.0);  // 左下
            gl.Color(0.0, 1.0, 0.0);		// 設置當前色為綠色
            gl.Vertex(-1.0, -1.0, 1.0);   // 右下
            gl.End();   // 金字塔繪製結束

            gl.LoadIdentity();					// 重置模型觀察矩陣
            gl.Translate(1.5, 0.0, -7.0);		// 右移1.5單位,並移入屏幕 6.0
            gl.Rotate(rquad, 1.0, 1.0, 1.0);	//  繞X軸旋轉四邊形

            gl.Begin(OpenGL.GL_QUADS);      //  繪製立方體
            //繪製頂面，上方
            gl.Color(0.0, 1.0, 0.0);	    // 一次性將當前色設置為綠色
            gl.Vertex(1.0, 1.0, -1.0);   
            gl.Vertex(-1.0, 1.0, -1.0);   
            gl.Vertex(-1.0, 1.0, 1.0);   
            gl.Vertex(1.0, 1.0, 1.0);
            //繪製底面，下方
            gl.Color(1.0, 0.5, 0.0);	    // 一次性將當前色設置為澄色
            gl.Vertex(1.0, -1.0, 1.0);
            gl.Vertex(-1.0, -1.0, 1.0);
            gl.Vertex(-1.0, -1.0, -1.0);
            gl.Vertex(1.0, -1.0, -1.0);
            //繪製前面
            gl.Color(1.0, 0.0, 0.0);	    // 一次性將當前色設置為紅色
            gl.Vertex(1.0, 1.0, 1.0);
            gl.Vertex(-1.0, 1.0, 1.0);
            gl.Vertex(-1.0, -1.0, 1.0);
            gl.Vertex(1.0, -1.0, 1.0);
            //繪製後面
            gl.Color(1.0, 1.0, 0.0);	    // 一次性將當前色設置為黃色
            gl.Vertex(1.0, -1.0, -1.0);
            gl.Vertex(-1.0, -1.0, -1.0);
            gl.Vertex(-1.0, 1.0, -1.0);
            gl.Vertex(1.0, 1.0, -1.0);
            //繪製左面
            gl.Color(0.0, 0.0, 1.0);	    // 一次性將當前色設置為藍色
            gl.Vertex(-1.0, 1.0, 1.0);
            gl.Vertex(-1.0, 1.0, -1.0);
            gl.Vertex(-1.0, -1.0, -1.0);
            gl.Vertex(-1.0, -1.0, 1.0);
            //繪製右面
            gl.Color(1.0, 0.0, 1.0);	    // 一次性將當前色設置為紫羅蘭色
            gl.Vertex(1.0, 1.0, -1.0);
            gl.Vertex(1.0, 1.0, 1.0);
            gl.Vertex(1.0, -1.0, 1.0);
            gl.Vertex(1.0, -1.0, -1.0);  

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

            rtri += 0.2 * 20;						// 增加三角形的旋轉變量
            rquad -= 0.15 * 20;						// 減少四邊形的旋轉變量
            gl1.Refresh();
        }

        private void gl1_MouseDown(object sender, MouseEventArgs e)
        {
            this.gl1.Capture = true;
            glRotateEnable = true;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            gl.ShadeModel(OpenGL.GL_SMOOTH);        // 啟用陰影平滑
            gl.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 黑色背景
            gl.ClearDepth(1.0f);					// 設置深度緩存
            gl.Enable(OpenGL.GL_DEPTH_TEST);		// 啟用深度測試
            gl.DepthFunc(OpenGL.GL_LEQUAL);			// 所作深度測試的類型
            gl.Hint(OpenGL.GL_PERSPECTIVE_CORRECTION_HINT, OpenGL.GL_NICEST);    // 告訴系統對透視進行修正
        }
    }
}
