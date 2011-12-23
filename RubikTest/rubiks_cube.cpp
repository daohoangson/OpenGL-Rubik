//
//  rubik_cube.cpp
//  RubikTest
//
//  Created by Son Dao Hoang on 11/25/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#include "rubiks_cube.h"
#include "utils.h"

/*
GLfloat RubiksCubeNorm[6][3] = {
    {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
};
// this is no longer needed 
*/

// duc
void copyVertex( GLfloat variable[8][3], GLfloat value[8][3] );
void rotateBlock( GLfloat vertex[8][3] );
void copyArray( GLfloat variable[3], GLfloat value[3] );
void copyCube( RubiksCubeSingle &des, RubiksCubeSingle &src )
{

	des.nLvl	= src.nLvl;
	des.nRow	= src.nRow;
	des.nCol	= src.nCol;

	for ( int i = 0; i < 8; i++ )
		des.p[i] = src.p[i];

	for ( int i = 0; i < 8; i++ )
	{
		des.v[des.p[i]][0] = src.v[src.p[i]][0];
		des.v[des.p[i]][1] = src.v[src.p[i]][1];
		des.v[des.p[i]][2] = src.v[src.p[i]][2];
	}	
}

void TranslateCube( RubiksCubeSingle &src, RubiksCubeSingle &target )
{
	GLfloat center_target[3];
	for ( int i = 0; i < 3; i++ )
	{
		GLfloat total = 0;
		for ( int j = 0; j < 8; j++ )
			total = total + target.v[j][i];
		center_target[i] = total / 8.0;
	}

	GLfloat center_src[3];
	for ( int i = 0; i < 3; i++ )
	{
		GLfloat total = 0;
		for ( int j = 0; j < 8; j++ )
			total = total + src.v[j][i];
		center_src[i] = total / 8.0;
	}

	GLfloat ep_X = center_target[0] - center_src[0];
	GLfloat ep_Y = center_target[1] - center_src[1];
	GLfloat ep_Z = center_target[2] - center_src[2];

	for ( int i = 0; i < 8; i++ )
	{
		src.v[i][0] += ep_X;
		src.v[i][1] += ep_Y;
		src.v[i][2] += ep_Z;
	}

	src.nLvl	= target.nLvl;
	src.nRow	= target.nRow;
	src.nCol	= target.nCol;
}
void RotateCubeLvl( RubiksCubeSingle &cube, bool isCW )
{
	RubiksCubeSingle temp;// = new RubiksCubeSingle;

	copyCube( temp, cube );
	
	if ( isCW ) {
		copyArray( cube.v[cube.p[0]], temp.v[temp.p[3]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[4]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[6]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[3] = tmp[0];
		cube.p[0] = tmp[1];
		cube.p[1] = tmp[2];
		cube.p[2] = tmp[3];
		cube.p[7] = tmp[4];
		cube.p[4] = tmp[5];
		cube.p[5] = tmp[6];
		cube.p[6] = tmp[7];

	} else {
		copyArray( cube.v[cube.p[0]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[3]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[6]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[4]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[1] = tmp[0];
		cube.p[2] = tmp[1];
		cube.p[3] = tmp[2];
		cube.p[0] = tmp[3];
		cube.p[5] = tmp[4];
		cube.p[6] = tmp[5];
		cube.p[7] = tmp[6];
		cube.p[4] = tmp[7];
	}
}
void RotateCubeRow( RubiksCubeSingle &cube, bool isCW )
{
	RubiksCubeSingle temp;// = new RubiksCubeSingle;

	copyCube( temp, cube );
	
	if ( isCW ) {

		copyArray( cube.v[cube.p[0]], temp.v[temp.p[4]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[3]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[6]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[4] = tmp[0];
		cube.p[0] = tmp[1];
		cube.p[3] = tmp[2];
		cube.p[7] = tmp[3];
		cube.p[5] = tmp[4];
		cube.p[1] = tmp[5];
		cube.p[2] = tmp[6];
		cube.p[6] = tmp[7];
	} else {

		copyArray( cube.v[cube.p[0]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[6]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[4]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[3]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[1] = tmp[0];
		cube.p[5] = tmp[1];
		cube.p[6] = tmp[2];
		cube.p[2] = tmp[3];
		cube.p[0] = tmp[4];
		cube.p[4] = tmp[5];
		cube.p[7] = tmp[6];
		cube.p[3] = tmp[7];
	}
}
void RotateCubeCol( RubiksCubeSingle &cube, bool isCW )
{
	RubiksCubeSingle temp;// = new RubiksCubeSingle;

	copyCube( temp, cube );
	
	if ( isCW ) {

		copyArray( cube.v[cube.p[0]], temp.v[temp.p[4]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[6]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[3]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[4] = tmp[0];
		cube.p[5] = tmp[1];
		cube.p[1] = tmp[2];
		cube.p[0] = tmp[3];
		cube.p[7] = tmp[4];
		cube.p[6] = tmp[5];
		cube.p[2] = tmp[6];
		cube.p[3] = tmp[7];
	} else {

		copyArray( cube.v[cube.p[0]], temp.v[temp.p[3]] );
		copyArray( cube.v[cube.p[1]], temp.v[temp.p[2]] );
		copyArray( cube.v[cube.p[2]], temp.v[temp.p[6]] );
		copyArray( cube.v[cube.p[3]], temp.v[temp.p[7]] );
		copyArray( cube.v[cube.p[4]], temp.v[temp.p[0]] );
		copyArray( cube.v[cube.p[5]], temp.v[temp.p[1]] );
		copyArray( cube.v[cube.p[6]], temp.v[temp.p[5]] );
		copyArray( cube.v[cube.p[7]], temp.v[temp.p[4]] );

		int tmp[9];
		for (int i = 0; i < 9; i++)
			tmp[i] = cube.p[i];
		cube.p[3] = tmp[0];
		cube.p[2] = tmp[1];
		cube.p[6] = tmp[2];
		cube.p[7] = tmp[3];
		cube.p[0] = tmp[4];
		cube.p[1] = tmp[5];
		cube.p[5] = tmp[6];
		cube.p[4] = tmp[7];
	}

}

int* getListCube(RubiksCubeSingle *cube, int lvl, int row, int col)
{
	int *res = new int[9];

	int current = 0;
	for (int i = 0; i < 27; i++)
	{
		if( cube[i].nLvl == lvl || cube[i].nRow == row || cube[i].nCol == col ) {
			res[current] = i;
			current = current + 1;
		}
	}

	// sort
	for ( int i = 0; i < 8; i++ )
		for ( int j = i+1; j < 9; j++ )
		{
				if ( (cube[res[i]].nLvl > cube[res[j]].nLvl) || 
					 (cube[res[i]].nLvl == cube[res[j]].nLvl && cube[res[i]].nRow > cube[res[j]].nRow) ||
					 (cube[res[i]].nLvl == cube[res[j]].nLvl && cube[res[i]].nRow == cube[res[j]].nRow && cube[res[i]].nCol > cube[res[j]].nCol ))
				{	 
					int temp = res[i];
					res[i] = res[j];
					res[j] = temp;
				}
		
		}

	//
	return res;
}
// end

GLint RubiksCubeFaces[6][4] = {
    {0, 1, 5, 4}, // front
    {2, 3, 7, 6}, // back
    {4, 5, 6, 7}, // top
    {0, 1, 2, 3}, // bottom
    {0, 3, 7, 4}, // left
    {1, 2, 6, 5}  // right
};

RubiksCube::RubiksCube(int size)
{
    m_size = size;
    m_numberOfCubes = (unsigned int) pow((double) size, 3);
    
#if DEBUG
    std::cout << "Generating Rubik's cube with size == " << m_size << std::endl;
#endif
    
    // generate cubes
    m_cubes = new RubiksCubeSingle[m_numberOfCubes];
    
    unsigned int nNumberOfCubesLevel = (unsigned int) pow((double) size, 2); // number of cubes in one level
    RubiksCubeSingle *pCube;
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        pCube = &m_cubes[i];
        memset(pCube, 0, sizeof(m_cubes[i]));
		//
		for ( int k = 0; k < 8; k++ )
			pCube->p[k] = k;
		//
        // get cube's level, row and column
        pCube->nLvl = i / nNumberOfCubesLevel;
        pCube->nRow = (i - (pCube->nLvl * nNumberOfCubesLevel)) / m_size;
        pCube->nCol = i - (pCube->nLvl * nNumberOfCubesLevel) - (pCube->nRow * m_size);
        
        // set color for cube's faces
        if (pCube->nLvl == 0)
        {
            // this is the bottom cube
            pCube->f[3] = RUBIK_CUBE_COLOR_BOTTOM;
        }
        else if (pCube->nLvl == m_size - 1)
        {
            // this is the top cube
            pCube->f[2] = RUBIK_CUBE_COLOR_TOP;
        }
        if (pCube->nRow == 0)
        {
            // this is the front cube
            pCube->f[0] = RUBIK_CUBE_COLOR_FRONT;
        }
        else if (pCube->nRow == m_size - 1)
        {
            // this is the back cube
            pCube->f[1] = RUBIK_CUBE_COLOR_BACK;
        }
        if (pCube->nCol == 0)
        {
            // this is the left cube
            pCube->f[4] = RUBIK_CUBE_COLOR_LEFT;
        }
        else if (pCube->nCol == m_size - 1)
        {
            // this is the right cube
            pCube->f[5] = RUBIK_CUBE_COLOR_RIGHT;
        }
        
#if DEBUG
        std::cout << "Cube (" << pCube->nLvl << ", " << pCube->nRow << ", " << pCube->nCol << "): ";
        std::cout << pCube->f[0] << " " << pCube->f[1] << " " << pCube->f[2] << " " << pCube->f[3] << " " << pCube->f[4] << " " << pCube->f[5];
        std::cout << std::endl;
#endif
    }
    
    // set all other properties to default value
    m_x = 0;
    m_y = 0;
    m_z = 0;
    m_l = 1;
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        ReCalculateVertexes(&m_cubes[i]);
    }
    
    m_angle_x = 0;
    m_angle_y = 0;
}

RubiksCube::~RubiksCube()
{
    delete[] m_cubes;
#if DEBUG
    std::cout << "RubiksCube deconstructor was called!" << std::endl;
#endif
}
//void RubiksCube::Move(void *cursor, bool immediately)
//{
//    CursorPosition pos = ((Cursor *)cursor)->getPosition();
//    
//    int deltaAngleX = 0;
//    int deltaAngleY = 0;
//    int deltaAngleZ = 0;
//}

void RubiksCube::Move(void *cursor, bool immediately)
{
    CursorPosition pos = ((Cursor *)cursor)->getPosition();

	printf("%i %i %i \n", pos.lvl, pos.row, pos.col);

	printf("is CW: %i \n", pos.isCW);

	printf("\n");

	int *res;
	res = getListCube(m_cubes, pos.lvl, pos.row, pos.col);
	Rotate(res, pos.isCW, pos.lvl, pos.row, pos.col);
	
	printf("%i %i %i %i %i %i %i %i %i \n", res[0], res[1], res[2], res[3], res[4], res[5], res[6], res[7], res[8] );
}

void RubiksCube::PreDraw(void)
{
    if (m_angle_x != 0)
    {
        glRotatef(m_angle_x, 1.f, 0.f, 0.f);
    }
    if (m_angle_y != 0)
    {
        glRotatef(m_angle_y, 0.f, 1.f, 0.f);
    }
}

void RubiksCube::Draw(void)
{
    glPushMatrix();
    
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        DrawCube(&m_cubes[i]);
    }
    
    glPopMatrix();
}

// 22.12
void RubiksCube::DrawPickMode(void)
{
	glPushMatrix();
    
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        DrawCubePickMode(&m_cubes[i], i );
    }
    
    glPopMatrix();
}
// END
void RubiksCube::RotateX(int deltaX)
{
    m_angle_x += deltaX;
    while (m_angle_x < 360)
    {
        m_angle_x += 360;
    }
    m_angle_x = m_angle_x % 360;
}

void RubiksCube::RotateY(int deltaY)
{
    m_angle_y += deltaY;
    while (m_angle_y < 360)
    {
        m_angle_y += 360;
    }
    m_angle_y = m_angle_y % 360;
}

unsigned int RubiksCube::GetSize(void)
{
    return m_size;
}

void RubiksCube::DrawCube(RubiksCubeSingle *cube)
{
#if DEBUG
    // std::cout << "Drawing cube (" << cube->nLvl << ", " << cube->nRow << ", " << cube->nCol << ")" << std::endl;
#endif
    
    glPushMatrix();
    
    if (cube->angleX != 0) glRotatef(cube->angleX, 1., 0., 0.);
    if (cube->angleY != 0) glRotatef(cube->angleY, 0., 1., 0.);
    if (cube->angleZ != 0) glRotatef(cube->angleZ, 0., 0., 1.);
    
    for (int i = 0; i < 6; i++)
    {
        switch (cube->f[i])
        {
            case WHITE:
                glColor3f(1. ,1. ,1.);
                break;
            case RED:
                glColor3f(1., 0., 0.);
                break;
            case BLUE:
                glColor3f(0., 0., 1.);
                break;
            case ORANGE:
                glColor3f(1., 0.6, 0.);
                break;
            case GREEN:
                glColor3f(0., 1., 0.);
                break;
            case YELLOW:
                glColor3f(1., 1., 0.);
                break;
            case NONE:
                // skip if this face has no color
                continue;
        }
        
        DrawTriangleVertex3fv(
                              &cube->v[RubiksCubeFaces[i][0]][0],
                              &cube->v[RubiksCubeFaces[i][1]][0],
                              &cube->v[RubiksCubeFaces[i][2]][0]);
        
        DrawTriangleVertex3fv(
                              &cube->v[RubiksCubeFaces[i][0]][0],
                              &cube->v[RubiksCubeFaces[i][3]][0],
                              &cube->v[RubiksCubeFaces[i][2]][0]);
                
        // draw the border
        glColor3f(0., 0., 0.);
        glLineWidth(4.);
        glBegin(GL_LINE_LOOP);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][0]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][1]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][2]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][3]][0]);
        glEnd();
    }
    
    glPopMatrix();
}

// 22.12
void RubiksCube::DrawCubePickMode(RubiksCubeSingle *cube, int color)
{
#if DEBUG
    // std::cout << "Drawing cube (" << cube->nLvl << ", " << cube->nRow << ", " << cube->nCol << ")" << std::endl;
#endif
    
    glPushMatrix();
    
    if (cube->angleX != 0) glRotatef(cube->angleX, 1., 0., 0.);
    if (cube->angleY != 0) glRotatef(cube->angleY, 0., 1., 0.);
    if (cube->angleZ != 0) glRotatef(cube->angleZ, 0., 0., 1.);
    
    for (int i = 0; i < 6; i++)
    {

		glColor3f( (color+1)/27.0, (color+1)/27.0, (color+1)/27.0 );
        
        DrawTriangleVertex3fv(
                              &cube->v[RubiksCubeFaces[i][0]][0],
                              &cube->v[RubiksCubeFaces[i][1]][0],
                              &cube->v[RubiksCubeFaces[i][2]][0]);
        
        DrawTriangleVertex3fv(
                              &cube->v[RubiksCubeFaces[i][0]][0],
                              &cube->v[RubiksCubeFaces[i][3]][0],
                              &cube->v[RubiksCubeFaces[i][2]][0]);
                
        // draw the border
        glColor3f(0., 0., 0.);
        glLineWidth(4.);
        glBegin(GL_LINE_LOOP);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][0]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][1]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][2]][0]);
        glVertex3fv(&cube->v[RubiksCubeFaces[i][3]][0]);
        glEnd();
    }
    
    glPopMatrix();
}
// end
void RubiksCube::ReCalculateVertexes(RubiksCubeSingle *cube)
{
    GLfloat sizeOver2 = m_size / 2.f;
    
    cube->v[0][0] = m_x + (cube->nCol - sizeOver2) * m_l;
    cube->v[0][1] = m_y + (cube->nLvl - sizeOver2) * m_l;
    cube->v[0][2] = m_z + (sizeOver2 - cube->nRow) * m_l;
    
    cube->v[1][0] = cube->v[0][0] + m_l;
    cube->v[1][1] = cube->v[0][1];
    cube->v[1][2] = cube->v[0][2];
    
    cube->v[2][0] = cube->v[1][0];
    cube->v[2][1] = cube->v[1][1];
    cube->v[2][2] = cube->v[1][2] - m_l;
    
    cube->v[3][0] = cube->v[0][0];
    cube->v[3][1] = cube->v[2][1];
    cube->v[3][2] = cube->v[2][2];
    
    cube->v[4][0] = cube->v[0][0];
    cube->v[4][1] = cube->v[0][1] + m_l;
    cube->v[4][2] = cube->v[0][2];
    
    cube->v[5][0] = cube->v[1][0];
    cube->v[5][1] = cube->v[4][1];
    cube->v[5][2] = cube->v[1][2];
    
    cube->v[6][0] = cube->v[2][0];
    cube->v[6][1] = cube->v[4][1];
    cube->v[6][2] = cube->v[2][2];
    
    cube->v[7][0] = cube->v[3][0];
    cube->v[7][1] = cube->v[4][1];
    cube->v[7][2] = cube->v[3][2];
}

// duc
void RubiksCube::Rotate( int listCube[], bool isCW, int lvl, int row, int col ) 
{
	RubiksCubeSingle *tmp = new RubiksCubeSingle[9];
	for ( int i = 0; i < 9; i++ ) {
		copyCube( tmp[i], m_cubes[listCube[i]] );
	}

	if ( isCW == false) {
		TranslateCube( m_cubes[listCube[0]], tmp[2] );
		TranslateCube( m_cubes[listCube[1]], tmp[5] );
		TranslateCube( m_cubes[listCube[2]], tmp[8] );
		TranslateCube( m_cubes[listCube[3]], tmp[1] );
		TranslateCube( m_cubes[listCube[4]], tmp[4] );
		TranslateCube( m_cubes[listCube[5]], tmp[7] );
		TranslateCube( m_cubes[listCube[6]], tmp[0] );
		TranslateCube( m_cubes[listCube[7]], tmp[3] );
		TranslateCube( m_cubes[listCube[8]], tmp[6] );
	} else {
		TranslateCube( m_cubes[listCube[0]], tmp[6] );
		TranslateCube( m_cubes[listCube[1]], tmp[3] );
		TranslateCube( m_cubes[listCube[2]], tmp[0] );
		TranslateCube( m_cubes[listCube[3]], tmp[7] );
		TranslateCube( m_cubes[listCube[4]], tmp[4] );
		TranslateCube( m_cubes[listCube[5]], tmp[1] );
		TranslateCube( m_cubes[listCube[6]], tmp[8] );
		TranslateCube( m_cubes[listCube[7]], tmp[5] );
		TranslateCube( m_cubes[listCube[8]], tmp[2] );
	}

	for ( int i = 0; i < 9; i++ ) {
	
		if ( lvl >= 0 )
			RotateCubeLvl( m_cubes[listCube[i]], isCW );
		else if ( row >= 0 )
			RotateCubeRow( m_cubes[listCube[i]], isCW );
		else 
			RotateCubeCol( m_cubes[listCube[i]], isCW );
	}

}


void copyVertex( GLfloat variable[8][3], GLfloat value[8][3])
{
	for ( int i = 0; i < 8; i++ )
		for ( int j = 0; j < 3; j++ )
			variable[i][j] = value[i][j];
}

void copyArray( GLfloat variable[3], GLfloat value[3] )
{
	for ( int i = 0; i < 3; i++ )
		variable[i] = value[i];
}

// end