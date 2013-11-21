/*  Project 3
Hammad Mazhar CS 559
*/

#include "include.h"
#include "render_pipeline.h"

using namespace std;
using namespace glm;

RenderPipeline window;


void CloseFunc()
{
	window.window_handle = -1;

}

void ReshapeFunc(int w, int h)
{
	// Question for reader: Why is this 'if' statement here?
	if (h > 0) {
		window.size = ivec2(w, h);
		window.window_aspect = float(w) / float(h);
	}


}

void KeyboardFunc(unsigned char c, int x, int y)
{
	switch (c) {
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	default:
		break;
	}
}

void SpecialFunc(int c, int x, int y) {}
void CallBackMouseFunc (int button, int state, int x, int y)
{


}

void CallBackMotionFunc (int x, int y)
{


}
void CallBackPassiveFunc (int x, int y)
{
}
void DisplayFunc()
{
	glEnable(GL_CULL_FACE);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, window.size.x, window.size.y);


	//DisplayInstructions();
	glutSwapBuffers();
}

void TimerFunc(int value)
{
	// Question for reader: Why is this 'if' statement here?
	if (window.window_handle != -1) {
		glutTimerFunc(window.interval, TimerFunc, value);
		glutPostRedisplay();
	}
}

int main(int argc, char * argv[])
{


	ilInit();

	glutInit(&argc, argv);
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	window.window_handle = glutCreateWindow("Hammad Mazhar CS559 Project 2");
	glutReshapeFunc(ReshapeFunc);
	glutCloseFunc(CloseFunc);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutMouseFunc(CallBackMouseFunc);
	glutMotionFunc(CallBackMotionFunc);
	glutPassiveMotionFunc(CallBackPassiveFunc);
	glutTimerFunc(window.interval, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);


	glEnable( GL_POINT_SMOOTH );
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		return 0;
	}

	







	glutMainLoop();
}
