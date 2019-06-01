/*
裁剪测试
*/
#pragma comment(lib,"legacy_stdio_definitions.lib")

#include<stdio.h>
#ifdef __cplusplus
extern"C"
#endif //
FILE __iob_func[3] = { __acrt_iob_func(0),__acrt_iob_func(2),__acrt_iob_func(3) };

#include <GLTools.h>
#include <GL/glut.h>

///////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear blue window
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Now set scissor to smaller red sub region
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	/*
	glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
	x和y参数指定了裁剪框的左下角，宽度和高度分别为裁剪框相应的尺寸
	*/
	glScissor(100, 100, 600, 400);
	//开启裁剪测试
	glEnable(GL_SCISSOR_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	// Finally, an even smaller green rectangle
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glScissor(200, 200, 400, 200);
	glClear(GL_COLOR_BUFFER_BIT);

	// Turn scissor back off for next render
	glDisable(GL_SCISSOR_TEST);

	glutSwapBuffers();
}


///////////////////////////////////////////////////////////
// Set viewport and projection
void ChangeSize(int w, int h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
}


///////////////////////////////////////////////////////////
// Program entry point
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL Scissor");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMainLoop();

	return 0;
}