//增加50个浮动球体
#pragma comment(lib,"legacy_stdio_definitions.lib")

#include<stdio.h>
#ifdef __cplusplus
extern"C"
#endif //
FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(2),__acrt_iob_func(3) };

// SphereWorld.cpp
// OpenGL SuperBible
// New and improved (performance) sphere world
// Program by Richard S. Wright Jr.

#include <GLTools.h>
#include <GLShaderManager.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLMatrixStack.h>
#include <GLGeometryTransform.h>
#include <StopWatch.h>

#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

#define NUM_SPHERES 50
GLFrame	spheres[NUM_SPHERES];

GLShaderManager		shaderManager;			// Shader Manager
GLMatrixStack		modelViewMatrix;		// Modelview Matrix 视图模型矩阵
GLMatrixStack		projectionMatrix;		// Projection Matrix 投影矩阵
GLFrustum			viewFrustum;			// View Frustum 视景体/平截头体
GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline 几何换管线

GLTriangleBatch		torusBatch;
GLTriangleBatch     sphereBatch;
GLBatch				floorBatch;			    //创建一个简单批次的容器
GLFrame             cameraFrame;

//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Initialze Shader Manager
	shaderManager.InitializeStockShaders();

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// This makes a torus 创建一个花托
	gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);

	// This make a sphere
	gltMakeSphere(sphereBatch, 0.1f, 26, 13);

	//方格地板批次数据
	floorBatch.Begin(GL_LINES, 324);
	for (GLfloat x = -20.0; x <= 20.0f; x += 0.5) {
		floorBatch.Vertex3f(x, -0.55f, 20.0f);
		floorBatch.Vertex3f(x, -0.55f, -20.0f);
		floorBatch.Vertex3f(20.0f, -0.55f, x);
		floorBatch.Vertex3f(-20.0f, -0.55f, x);
	}
	floorBatch.End();

	// Randomly place the spheres
	for (int i = 0; i < NUM_SPHERES; i++) {
		GLfloat x = ((GLfloat)((rand() % 400) - 200) * 0.1f);
		GLfloat z = ((GLfloat)((rand() % 400) - 200) * 0.1f);
		spheres[i].SetOrigin(x, 0.0f, z);
	}
}

// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
{
	float linear = 0.1f;
	float angular = float(m3dDegToRad(5.0f));

	if (key == GLUT_KEY_UP)
		cameraFrame.MoveForward(linear);

	if (key == GLUT_KEY_DOWN)
		cameraFrame.MoveForward(-linear);

	if (key == GLUT_KEY_LEFT)
		cameraFrame.RotateWorld(angular, 0.0f, 1.0f, 0.0f);

	if (key == GLUT_KEY_RIGHT)
		cameraFrame.RotateWorld(-angular, 0.0f, 1.0f, 0.0f);
}

//	屏幕改变大小或初始化
void ChangeSize(int nWidth, int nHeight) {
	glViewport(0, 0, nWidth, nHeight);

	//创建投影矩阵，并将它载入投影矩阵堆栈中
	/*
	设置透视投影
	SetPerspective(float fFov, float fAspect, float fNear, float fFar)
		fFov:垂直方向上的视场角度
		fAspect:窗口的宽度与高度的纵横
		fNear:近裁剪面距离
		fFar:远裁剪面距离
		纵横  = 宽(w)/ (h)
	*/
	viewFrustum.SetPerspective(35.0f, float(nWidth) / float(nHeight), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

	//设置变换管线以使用两个矩阵堆栈
	//通过将它的内部指针设置为模型视图矩阵和投影矩阵堆栈实例来完成这项任务
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

//进行调用以绘制颜色
void RenderScene(void) {
	//颜色值
	static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static GLfloat vTorusColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat vSphereColor[] = { 0.0f, 0.0f, 1.0f, 1.0f };

	//基于时间的动画
	static CStopWatch rotTimer;
	//rotTimer.GetElapsedSeconds()标示获取上一次渲染到这一次渲染的时间间隔
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

	//清除颜色缓冲区和深度缓冲区
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//保存当前的视图模型矩阵（单位矩阵）
	modelViewMatrix.PushMatrix();

	M3DMatrix44f mCamera;
	cameraFrame.GetCameraMatrix(mCamera);
	modelViewMatrix.PushMatrix(mCamera);

	//绘制背景
	//参数1:平面着色器 
	//参数2:允许变化的4*4矩阵
	//参数3:颜色
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
		//将这个坐标系缩减到单元正方体范围，通过投影矩阵乘以模型视图矩阵来完成这项工作
		transformPipeline.GetModelViewProjectionMatrix(),
		vFloorColor);
	floorBatch.Draw();

	for (int i = 0; i < NUM_SPHERES; i++) {
		modelViewMatrix.PushMatrix();
		modelViewMatrix.MultMatrix(spheres[i]);
		shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(),
			vSphereColor);
		sphereBatch.Draw();
		modelViewMatrix.PopMatrix();
	}

	//绘制选择的花托
	modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);

	//保存平移
	modelViewMatrix.PushMatrix();

	modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f); //围绕Y轴进行旋转
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
		transformPipeline.GetModelViewProjectionMatrix(),
		vTorusColor);
	torusBatch.Draw();
	modelViewMatrix.PopMatrix(); //“消除”以前的旋转

	//应用另一个旋转，然后进行平移，然后再绘制球体
	modelViewMatrix.Rotate(yRot * -2.0f, 0.0f, 1.0f, 0.0f);
	modelViewMatrix.Translate(0.8f, 0.0f, 0.0f);
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
		transformPipeline.GetModelViewProjectionMatrix(),
		vSphereColor);
	sphereBatch.Draw();

	//返回到第一次平移
	modelViewMatrix.PopMatrix();
	// Restore the previous modleview matrix (the identity matrix) 返回到单位矩阵
	modelViewMatrix.PopMatrix();

	//因为是双缓冲区模式，后台缓冲区替换到前台缓存区进行显示
	glutSwapBuffers();

	//刷新窗口，自动触发渲染，达到动画效果
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow("OpenGL SphereWorld2");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}


	SetupRC();
	glutMainLoop();
	return 0;
}