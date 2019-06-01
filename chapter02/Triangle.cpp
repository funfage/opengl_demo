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

	// Load up a triangle
	//������������������ĵѿ��������
	GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f };

	/*
	Begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits = 0);
	�������������г�ʼ��������������������ͼԪ�����а������������Լ�һ���������������
	*/
	triangleBatch.Begin(GL_TRIANGLES, 3);
	//����һ����3������x,y,z��������ɵ�����
	triangleBatch.CopyVertexData3f(vVerts);
	//����End�������Լ���������ݸ��ƹ������ҽ������ڲ������֪ͨ����������Щ����
	//һ������End�������Ͳ��������µ�������
	triangleBatch.End();
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
	//���������鴫�ݵ� GLT_SHADER_IDENTITY ��ɫ��
	//�����ɫ��ֻʹ��ָ����ɫ��Ĭ�ϵѿ�������ϵ����Ļ����Ⱦ����ͼ�Ρ�
	//����Ƭ�ζ�Ӧ��ͬһ����ɫ������ͼ��Ϊʵ��Ϊ��Ⱦ�ġ�
	//vRed����ʹ�õ���ɫ
	shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
	//Draw������ʾ������ͼ���ύ����ɫ��
	triangleBatch.Draw();

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


