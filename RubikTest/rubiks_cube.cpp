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
    for (int i = 0; i < m_numberOfCubes; i++)
    {
        pCube = &m_cubes[i];
        memset(pCube, 0, sizeof(m_cubes[i]));
        
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
    for (int i = 0; i < m_numberOfCubes; i++)
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

void RubiksCube::Move(void *cursor, bool immediately)
{
    CursorPosition pos = ((Cursor *)cursor)->getPosition();
    
    int deltaAngleX = 0;
    int deltaAngleY = 0;
    int deltaAngleZ = 0;
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
    
    for (int i = 0; i < m_numberOfCubes; i++)
    {
        DrawCube(&m_cubes[i]);
    }
    
    glPopMatrix();
}

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