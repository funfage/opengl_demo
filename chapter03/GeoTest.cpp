/*
深度测试和多边形模式
*/
#pragma comment(lib,"legacy_stdio_definitions.lib")

#include<stdio.h>
#ifdef __cplusplus
extern"C"
#endif //
FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(2),__acrt_iob_func(3) };


#include<GLTools.h>
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLGeometryTransform.h>

#include <math.h>
#include <GL/glut.h>

GLFrame             viewFrame;
GLFrustum           viewFrustum;
GLTriangleBatch     torusBatch;
GLMatrixStack       modelViewMatix;
GLMatrixStack       projectionMatrix;
GLGeometryTransform transformPipeline;
GLShaderManager     shaderManager;

// Flags for effects
int iCull = 0;
int iDepth = 0;

///////////////////////////////////////////////////////////////////////////////
// Reset flags as appropriate in response to menu selections
void ProcessMenu(int value)
{
	switch (value)
	{
	case 1:
		iDepth = !iDepth;
		break;

	case 2:
		iCull = !iCull;
		break;

	case 3:
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 4:
		//线框模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;

	case 5:
		//点模式
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}
	//重新渲染，要求当前的回调函数返回时执行显示回调函数
	glutPostRedisplay();
}


// Called to draw scene
void RenderScene(void)
{
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Turn culling on if flag is set
	if (iCull)
		//开启背面剔除
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	// Enable depth testing if flag is set
	if (iDepth)
		//启用深度测试
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);


	modelViewMatix.PushMatrix(viewFrame);

	GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vRed);
	shaderManager.UseStockShader(GLT_SHADER_DEFAULT_LIGHT, transformPipeline.GetModelViewMatrix(), transformPipeline.GetProjectionMatrix(), vRed);


	torusBatch.Draw();

	modelViewMatix.PopMatrix();


	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
	// Black background
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	shaderManager.InitializeStockShaders();
	viewFrame.MoveForward(7.0f);

	// Make the torus
	gltMakeTorus(torusBatch, 1.0f, 0.3f, 52, 26);

	glPointSize(4.0f);
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		viewFrame.RotateWorld(m3dDegToRad(-5.0), 1.0f, 0.0f, 0.0f);

	if (key == GLUT_KEY_DOWN)
		viewFrame.RotateWorld(m3dDegToRad(5.0), 1.0f, 0.0f, 0.0f);

	if (key == GLUT_KEY_LEFT)
		viewFrame.RotateWorld(m3dDegToRad(-5.0), 0.0f, 1.0f, 0.0f);

	if (key == GLUT_KEY_RIGHT)
		viewFrame.RotateWorld(m3dDegToRad(5.0), 0.0f, 1.0f, 0.0f);

	// Refresh the Window
	glutPostRedisplay();
}


void ChangeSize(int w, int h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 100.0f);

	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	transformPipeline.SetMatrixStacks(modelViewMatix, projectionMatrix);
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	//请求一个深度缓存区 GLUT_DEPTH
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Geometry Test Program");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);

	// Create the Menu
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle depth test", 1);
	glutAddMenuEntry("Toggle cull backface", 2);
	glutAddMenuEntry("Set Fill Mode", 3);
	glutAddMenuEntry("Set Line Mode", 4);
	glutAddMenuEntry("Set Point Mode", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	SetupRC();

	glutMainLoop();
	return 0;
}

