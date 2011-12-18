#include <cstdlib>
#include <iostream>
#include <fstream>

#include "dependencies.h"
#include "rubiks_cube.h"
#include "cursor.h"

int px, py;
GLdouble eyeX, eyeY, eyeZ;
GLdouble upX, upY, upZ;
RubiksCube *rubik;
Cursor *cursor;

void initGlut(void);
void callbackDisplay(void);
void callbackReshape(int w, int h);
void callbackKeyboard(unsigned char c, int x, int y);
void callbackMouse(int button, int state, int x, int y);
void callbackMotion(int x, int y);
void callbackIdle(void);

void initRubik(void);
void drawRubik(void);

void initRubik(void)
{
    rubik = new RubiksCube(3);
    cursor = new Cursor(rubik);
}

void drawRubik(void)
{
    glPushMatrix();
    
    rubik->PreDraw();
    rubik->Draw();
    
    cursor->Draw();
    
    glPopMatrix();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	initGlut();
	initRubik();
    
	glutDisplayFunc(callbackDisplay); 
	glutReshapeFunc(callbackReshape);
    glutKeyboardFunc(callbackKeyboard);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
    glutIdleFunc(callbackIdle);
    
	glutMainLoop();
    
    delete rubik;
    delete cursor;
    
	return 0;
}

void initGlut(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
}

void callbackDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	drawRubik();
	glutSwapBuffers();
}

void callbackReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode(GL_MODELVIEW);

    eyeX = 0.;
    eyeY = 5.;
    eyeZ = 10.;
    upX = 0.;
    upY = 1.;
    upZ = 0.;
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, upX, upY, upZ);
}

void callbackKeyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 'w':
        case 'd':
            cursor->Move(1);
            break;
        case 's':
        case 'a':
            cursor->Move(-1);
            break;
        case ' ':
            cursor->SwitchMode();
            break;
        case 'x':
            cursor->ToggleDirection();
            break;
        case 'r':
            rubik->Move(cursor, false);
            break;
    }
}

void callbackMouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
		px = x;
		py = y;
	}
}

void callbackMotion(int x, int y)
{
    if (x != px)
    {
        rubik->RotateY(x - px);
    }
    if (y != py)
    {
        rubik->RotateX(y - py);
    }
    
	px = x;
	py = y;
}

void callbackIdle(void)
{
    glutPostRedisplay();
}
