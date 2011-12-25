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

typedef enum _PickingMode
{
    PICKING_NONE = 0,
    PICKING,
    PICKED,
    PICKING2,
    PICKING_FINISHED
} PickingMode;
PickingMode  pickingMode = PICKING_NONE;
unsigned int pickedPosition[3];

void initGlut(void);
void callbackDisplay(void);
void callbackReshape(int w, int h);
void callbackKeyboard(unsigned char c, int x, int y);
void callbackMouse(int button, int state, int x, int y);
void callbackMotion(int x, int y);
void callbackTimer(int value);

void initRubik(void);
void drawRubik(void);
bool processPicking(void);

#pragma mark - Implementation

void initRubik(void)
{
    rubik = new RubiksCube(3);
    cursor = new Cursor(rubik);
}

void drawRubik(void)
{
    glPushMatrix();
    
    rubik->PreDraw();
    
    bool useSolidColors = pickingMode == PICKING || pickingMode == PICKING2;
    
    rubik->Draw(useSolidColors);
    if (!useSolidColors)
    {
        cursor->Draw();
    }
    
    glPopMatrix();
}

bool processPicking(void)
{
    GLint viewport[4];
	GLubyte pixel[3];
    bool processed = false;
    
	glGetIntegerv(GL_VIEWPORT, viewport);
    
	glReadPixels(px, viewport[3]-py,
                 1, 1, GL_RGB,
                 GL_UNSIGNED_BYTE, (void *)pixel
                 );
    
    switch (pickingMode) {
        case PICKING:
            if (pixel[0] > 0 && pixel[1] > 0 && pixel[2] > 0)
            {
                // user selected a cube!
                pickingMode = PICKED;
                // cursor->GoTo(pixel[0] - 1, pixel[1] - 1, pixel[2] - 1);
                pickedPosition[0] = pixel[0] - 1;
                pickedPosition[1] = pixel[1] - 1;
                pickedPosition[2] = pixel[2] - 1;
            }
            else
            {
                pickingMode = PICKING_NONE;
            }
            processed = true;
            break;
            
        case PICKING2:
            if (pixel[0] > 0 && pixel[1] > 0 && pixel[2] > 0)
            {
                unsigned int tmp[3];
                tmp[0] = pixel[0] - 1;
                tmp[1] = pixel[1] - 1;
                tmp[2] = pixel[2] - 1;
                if (tmp[0] != pickedPosition[0]
                    || tmp[1] != pickedPosition[1]
                    || tmp[2] != pickedPosition[2])
                {
                    std::cout << "Ho ho ho!\n";
                    pickingMode = PICKING_FINISHED;
                }
            }
            else
            {
                pickingMode = PICKED;
            }
            processed = true;
            break;
            
        default:
            break;
    }
    
#ifdef DEBUG
    if (processed)
    {
        std::cout << "Pixel values: " << (int) pixel[0] << ", " << (int) pixel[1] << ", " << (int) pixel[2] << "\n";
    }
#endif
    
    return processed;
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

void callbackDisplay( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    drawRubik();
	
	if (processPicking())
	{
		// do not swap buffer
	}
	else
    {
		glutSwapBuffers();
    }
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
			rubik->Move(cursor, cursor->isCW());
            break;
    }
}

void callbackMouse(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN)
        {
            px = x;
            py = y;

            pickingMode = PICKING;
        }
        else if (state == GLUT_UP)
        {
            pickingMode = PICKING_NONE;
        }
	}

}

void callbackMotion(int x, int y)
{
    if (pickingMode == PICKING_NONE)
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
    
    if (pickingMode == PICKED)
    {
        pickingMode = PICKING2;
    }
}

void callbackTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(MILLISECONDS_PER_FRAME, callbackTimer, value);
}
