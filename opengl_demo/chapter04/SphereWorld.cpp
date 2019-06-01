//ʹ�ö�ջ����
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


GLShaderManager		shaderManager;			// Shader Manager
GLMatrixStack		modelViewMatrix;		// Modelview Matrix ��ͼģ�;���
GLMatrixStack		projectionMatrix;		// Projection Matrix ͶӰ����
GLFrustum			viewFrustum;			// View Frustum �Ӿ���/ƽ��ͷ��
GLGeometryTransform	transformPipeline;		// Geometry Transform Pipeline ���λ�����

GLTriangleBatch		torusBatch;
GLBatch				floorBatch;			    //����һ�������ε�����

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

	// This makes a torus ����һ������
	gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);

	//����ذ���������
	floorBatch.Begin(GL_LINES, 324);
	for (GLfloat x = -20.0; x <= 20.0f; x += 0.5) {
		floorBatch.Vertex3f(x, -0.55f, 20.0f);
		floorBatch.Vertex3f(x, -0.55f, -20.0f);
		floorBatch.Vertex3f(20.0f, -0.55f, x);
		floorBatch.Vertex3f(-20.0f, -0.55f, x);
	}
	floorBatch.End();
}

//	��Ļ�ı��С���ʼ��
void ChangeSize(int nWidth, int nHeight) {
	glViewport(0, 0, nWidth, nHeight);

	//����ͶӰ���󣬲���������ͶӰ�����ջ��
	/*
	����͸��ͶӰ
	SetPerspective(float fFov, float fAspect, float fNear, float fFar)
		fFov:��ֱ�����ϵ��ӳ��Ƕ�
		fAspect:���ڵĿ����߶ȵ��ݺ� 
		fNear:���ü������
		fFar:Զ�ü������
		�ݺ�  = ��(w)/ (h)
	*/
	viewFrustum.SetPerspective(35.0f, float(nWidth)/float(nHeight), 1.0f, 100.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());

	//���ñ任������ʹ�����������ջ
	//ͨ���������ڲ�ָ������Ϊģ����ͼ�����ͶӰ�����ջʵ���������������
	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);
}

//���е����Ի�����ɫ
void RenderScene(void) {
	//��ɫֵ
	static GLfloat vFloorColor[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	static GLfloat vTorusColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	//����ʱ��Ķ���
	static CStopWatch rotTimer;
	//rotTimer.GetElapsedSeconds()��ʾ��ȡ��һ����Ⱦ����һ����Ⱦ��ʱ����
	float yRot = rotTimer.GetElapsedSeconds() * 60.0f;

	//�����ɫ����������Ȼ�����
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���浱ǰ����ͼģ�;��󣨵�λ����
	modelViewMatrix.PushMatrix();

	//���Ʊ���
	//����1:ƽ����ɫ�� 
	//����2:����仯��4*4����
	//����3:��ɫ
	shaderManager.UseStockShader(GLT_SHADER_FLAT,
		//���������ϵ��������Ԫ�����巶Χ��ͨ��ͶӰ�������ģ����ͼ��������������
								transformPipeline.GetModelViewProjectionMatrix(),
								vFloorColor);
	floorBatch.Draw();

	//����ѡ��Ļ���
	modelViewMatrix.Translate(0.0f, 0.0f, -2.5f);
	modelViewMatrix.Rotate(yRot, 0.0f, 1.0f, 0.0f); //Χ��Y�������ת
	shaderManager.UseStockShader(GLT_SHADER_FLAT, 
								transformPipeline.GetModelViewProjectionMatrix(),
								vTorusColor);
	torusBatch.Draw();

	//������ǰ����ͼģ�;���
	modelViewMatrix.PopMatrix();

	//��Ϊ��˫������ģʽ����̨�������滻��ǰ̨������������ʾ
	glutSwapBuffers();

	//ˢ�´��ڣ��Զ�������Ⱦ���ﵽ����Ч��
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	gltSetWorkingDirectory(argv[0]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);

	glutCreateWindow("OpenGL SphereWorld");

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
	}


	SetupRC();
	glutMainLoop();
	return 0;
}