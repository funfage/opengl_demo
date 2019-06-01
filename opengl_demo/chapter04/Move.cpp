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
	����x��y�������������ӿڵ����½����꣬����Ⱥ͸߶Ȳ����������ر�ʾ�ġ�
	ͨ��x��y��Ϊ0�� �������ǿ���ʹ���ӿ��ڴ����еĲ�ͬ������Ⱦ���ͼ��
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
	��OpenGL�У�ĳ����ɫ���� �죬�̣�����Alpha��͸���ȣ�������϶��ɡ�ÿ�ַ���ֵ��
	��Χ��0.0��1.0
	Blue background
	*/
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	/*
	�洢��ɫ�������ǿ�������ɫ�����������й���
	��ɫ����������Ҫ������������Լ�����ɫ�������Ա�����OpenGL��ʼ��ʱ����InitializeStockShaders����
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

	//����glut��Ҫ���´�������
	glutPostRedisplay();
}


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
{
	// Clear the window with current clearing color
	/*
	ǰ���Ѿ��������ɫ����Ϊ��ɫ������ִ��һ�����������������
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//����һ�鸡��������ʾ��ɫ
	GLfloat vRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	M3DMatrix44f mFinalTransform, mTranslationMatrix, mRotationMatrix;
	m3dTranslationMatrix44(mTranslationMatrix, xPos, yPos, 0.0f);

	//ÿ���ػ�ʱ��ת5��
	static float yRot = 0.0f;
	yRot += 5.0f;
	m3dRotationMatrix44(mRotationMatrix, m3dDegToRad(yRot), 0.0f, 0.0f, 1.0f);

	//��ƽ�ƾ������ת������˵õ�Ŀ�����
	m3dMatrixMultiply44(mFinalTransform, mTranslationMatrix, mRotationMatrix);

	//ƽ����ɫ��������Ϊ����ͼ�α任ָ��һ��4x4�任����������ɫ��ֻʹ��һ�����ԣ�GLT_ATTRIBUTE_VERTEX
	shaderManager.UseStockShader(GLT_SHADER_FLAT, mFinalTransform, vRed);
	//Draw������ʾ������ͼ���ύ����ɫ��
	squareBatch.Draw();

	// Perform the buffer swap to display back buffer
	//ָ��һ��˫��������Ⱦ����������ζ�Ž��ں�̨������������Ⱦ��Ȼ���ڽ���ʱ������ǰ̨
	glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
{
	//���õ�ǰ����Ŀ¼��ʵ��û���ô�����Ϊ����Ŀ¼Ĭ�Ͼ��������Ŀ�ִ�г�����ͬ��Ŀ¼
	gltSetWorkingDirectory(argv[0]);

	//���������в�������ʼ��GLUT��
	glutInit(&argc, argv);
	//����GLUT�ⴴ������ʱʹ���������͵���ʾģʽ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	//���ô��ڵĴ�С�ͱ���
	glutInitWindowSize(800, 600);
	glutCreateWindow("Triangle");
	//Ϊ���ڸı��С������һ���ص��������Ա������ӵ㣬��Ҫע��һ�������԰���OpenGL��Ⱦ����
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);

	/*
	������������ʼ��������û�г����κ�����
	*/
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}

	//������Ⱦ������ OpenGL����������֮ǰ���봴��һ����Ⱦ����
	SetupRC();

	//����Ϣѭ��������main������ glutMainLoop�����ú������ڱ��ر�֮ǰ�����᷵��
	glutMainLoop();
	return 0;
}


