#include <cstdlib>
#include <iostream>
#include <fstream>

#include "dependencies.h"
#include "constants.h"
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
void callbackTimer(int value);

void initRubik(void);
void drawRubik(void);

// duc
#define RENDER					1
#define SELECT					2
int mode = RENDER;
void drawRubikPickMode(void);
void processPickMode();
bool change = true;
// end

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

// duc
void drawRubikPickMode( void )
{
	glPushMatrix();
    
    rubik->PreDraw();
    rubik->DrawPickMode();
       
    glPopMatrix();
}
// end
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
    glutTimerFunc(MILLISECONDS_PER_FRAME, callbackTimer, 0);
    
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

// duc
void callbackDisplay( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if (mode == SELECT)
		drawRubikPickMode();
	else {
		drawRubik();
	}
	if (mode == SELECT)
	{
		processPickMode();
		mode = RENDER;
	}
	else
		glutSwapBuffers();
}
// end

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
			rubik->Move(cursor, cursor->isCW());
            break;
    }
}

void callbackMouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
		px = x;
		py = y;

		// 
		mode = SELECT;
		// end
	}

}

void callbackMotion(int x, int y)
{
	if (change == true)
	{
    if (x != px)
    {
        rubik->RotateY(x - px);
    }
    if (y != py)
    {
        rubik->RotateX(y - py);
    }
    }
	px = x;
	py = y;
}

void callbackTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(MILLISECONDS_PER_FRAME, callbackTimer, value);
}


// duc
void processPickMode()
{
	GLint viewport[4];
	GLubyte pixel[3];

	glGetIntegerv(GL_VIEWPORT,viewport);

	glReadPixels(px,viewport[3]-py,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

	printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	
	printf ("\n");

	if ( pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0 )
		change = true;
	else
		change = false;
}
// end