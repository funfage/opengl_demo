// Triangle.cpp
// Our first OpenGL program that will just draw a triangle on the screen.
#pragma comment(lib,"legacy_stdio_definitions.lib")

#include<stdio.h>
#ifdef __cplusplus
extern"C"
#endif //
FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(2),__acrt_iob_func(3) };

#include<GLTools.h>
#include<GLShaderManager.h>
#include<GL/glut.h>

GLBatch triangleBatch;
GLShaderManager shaderManager;

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
{
	/*
	glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
	其中x和y参数代表窗口中视口的左下角坐标，而宽度和高度参数是用像素表示的。
	通常x和y都为0， 但是我们可以使用视口在窗口中的不同区域渲染多个图形
	*/
	glViewport(0, 0, w, h);
}

///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context.
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
{
	/*
	glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	在OpenGL中，某种颜色是由 红，绿，蓝和Alpha（透明度）分量混合而成。每种分量值的
	范围从0.0至1.0
	Blue background
	*/
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	/*
	存储着色器，它们可以用着色器管理器进行管理
	着色器管理器需要编译和链接它自己的着色器，所以必须在OpenGL初始化时调用InitializeStockShaders方法
	*/
	shaderManager.InitializeStockShaders();

	// Load up a triangle
	//设置三角形三个顶点的笛卡尔坐标对
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f };

	/*
	Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits = 0);
	对批次容器进行初始化，告诉这个类代表哪种图元，其中包括顶点数，以及一组或两组纹理坐标
	*/
	triangleBatch.Begin(GL_TRIANGLES, 3);
	//复制一个由3分量（x,y,z）顶点组成的数组
	triangleBatch.CopyVertexData3f(vVerts);
	//调用End来表明以及完成了数据复制工作并且将设置内部标记以通知这个类包含哪些属性
	//一旦调用End函数，就不能增加新的属性了
	triangleBatch.End();
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	/*
	前面已经将清除颜色设置为蓝色，现在执行一个函数真正进行清除
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//设置一组浮点数来表示红色
	GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//将浮点数组传递到 GLT_SHADER_IDENTITY 着色器
	//这个着色器只使用指定颜色以默认笛卡尔坐标系在屏幕上渲染几何图形。
	//所有片段都应用同一种颜色，几何图形为实心为渲染的。
	//vRed代表使用的颜色
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	//Draw方法表示将几何图形提交到着色器
	triangleBatch.Draw();

	// Perform the buffer swap to display back buffer
	//指定一个双缓冲区渲染环境。这意味着将在后台缓冲区进行渲染，然后在结束时交换到前台
	glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	//设置当前工作目录，实际没有用处，因为工作目录默认就是与程序的可执行程序相同的目录
	gltSetWorkingDirectory(argv[0]);

	//传输命令行参数并初始化GLUT库
	glutInit(&argc, argv);
	//告诉GLUT库创建窗口时使用那种类型的显示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	//设置窗口的大小和标题	
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangle");
	//为窗口改变大小而设置一个回调函数，以便设置视点，还要注册一个函数以包含OpenGL渲染代码
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	/*
	检查驱动程序初始化过程中没有出现任何问题
	*/
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	//设置渲染环境， OpenGL函数起作用之前必须创建一个渲染环境
	SetupRC();

	//主消息循环并结束main函数， glutMainLoop被调用后，主窗口被关闭之前都不会返回
	glutMainLoop();
	return 0;
}


