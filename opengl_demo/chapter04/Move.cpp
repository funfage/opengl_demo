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


GLBatch squareBatch;
GLShaderManager shaderManager;

GLfloat blockSize = 0.1f;
GLfloat vVerts[] = { -blockSize, -blockSize, 0.0f,
					  blockSize, -blockSize, 0.0f,
					  blockSize,  blockSize, 0.0f,
					 -blockSize,  blockSize, 0.0f };

GLfloat xPos = 0.0f;
GLfloat yPos = 0.0f;
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

	squareBatch.Begin(GL_TRIANGLE_FAN, 4);
	squareBatch.CopyVertexData3f(vVerts);
	squareBatch.End();
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
	GLfloat stepSize = 0.025f;

	GLfloat blockX = vVerts[0];   // Upper left X
	GLfloat blockY = vVerts[7];  // Upper left Y

	if (key == GLUT_KEY_UP)
		blockY += stepSize;

	if (key == GLUT_KEY_DOWN)
		blockY -= stepSize;

	if (key == GLUT_KEY_LEFT)
		blockX -= stepSize;

	if (key == GLUT_KEY_RIGHT)
		blockX += stepSize;

	// Collision detection
	if (blockX < -1.0f) blockX = -1.0f;
	if (blockX > (1.0f - blockSize * 2)) blockX = 1.0f - blockSize * 2;;
	if (blockY < -1.0f + blockSize * 2)  blockY = -1.0f + blockSize * 2;
	if (blockY > 1.0f) blockY = 1.0f;

	// Recalculate vertex positions
	vVerts[0] = blockX;
	vVerts[1] = blockY - blockSize * 2;

	vVerts[3] = blockX + blockSize * 2;
	vVerts[4] = blockY - blockSize * 2;

	vVerts[6] = blockX + blockSize * 2;
	vVerts[7] = blockY;

	vVerts[9] = blockX;
	vVerts[10] = blockY;

	squareBatch.CopyVertexData3f(vVerts);

	//告诉glut需要更新窗口内容
	glutPostRedisplay();
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

	M3DMatrix44f mFinalTransform, mTranslationMatrix, mRotationMatrix;
	m3dTranslationMatrix44(mTranslationMatrix, xPos, yPos, 0.0f);

	//每次重绘时旋转5度
	static float yRot = 0.0f;
	yRot += 5.0f;
	m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(yRot), 0.0f, 0.0f, 1.0f);

	//将平移矩阵和旋转矩阵相乘得到目标矩阵
	m3dMatrixMultiply44(mFinalTransform, mTranslationMatrix, mRotationMatrix);

	//平面着色器，允许为几何图形变换指定一个4x4变换矩阵，这种着色器只使用一个属性，GLT_ATTRIBUTE_VERTEX
	shaderManager.UseStockShader(GLT_SHADER_FLAT, mFinalTransform, vRed);
	//Draw方法表示将几何图形提交到着色器
	squareBatch.Draw();

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
	glutSpecialFunc(SpecialKeys);

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


