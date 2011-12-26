//
//  rubik_cube.cpp
//  RubikTest
//
//  Created by Son Dao Hoang on 11/25/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#include "rubiks_cube.h"
#include "constants.h"
#include "utils.h"

GLint RubiksCubeFaces[6][4] = {
    {0, 1, 5, 4}, // front
    {2, 3, 7, 6}, // back
    {4, 5, 6, 7}, // top
    {0, 1, 2, 3}, // bottom
    {0, 3, 7, 4}, // left
    {1, 2, 6, 5}  // right
};

GLfloat RubiksCubeFacesVertexes[8][3] = {
    {-.5f, -.5f,  .5f},
    { .5f, -.5f,  .5f},
    { .5f, -.5f, -.5f},
    {-.5f, -.5f, -.5f},
    {-.5f,  .5f,  .5f},
    { .5f,  .5f,  .5f},
    { .5f,  .5f, -.5f},
    {-.5f,  .5f, -.5f}
};

RubiksCube::RubiksCube(int size)
{
    m_size = size;
    m_numberOfCubes = (unsigned int) pow((double) size, 3);
    m_numberOfCubesPerLevel = (unsigned int) pow((double) size, 2); // number of cubes in one level
    
#if DEBUG
    std::cout << "Generating Rubik's cube with size == " << m_size << std::endl;
#endif
    
    // generate cubes
    m_cubes = new RubiksCubeSingle[m_numberOfCubes];
    unsigned int tmp[3];
    
    RubiksCubeSingle *pCube;
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        pCube = &m_cubes[i];
        memset(pCube, 0, sizeof(m_cubes[i]));

        // get cube's level, row and column
        GetLvlRowColFromIndex(&tmp[0], i, m_size, m_numberOfCubesPerLevel);
        pCube->nLvl = tmp[0];
        pCube->nRow = tmp[1];
        pCube->nCol = tmp[2];
        
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

void RubiksCube::Move(void *cursor)
{
    CursorPosition pos = ((Cursor *)cursor)->getPosition();

    RubiksCubeSingle *cube;
    
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        cube = &m_cubes[i];
        
        if (cube->isMoving)
        {
            // this cube is in some move
            // reposition immediately
            RePositionCubeFromMoving(cube);
        }
        
        if (cube->nLvl != pos.lvl
            && cube->nRow != pos.row
            && cube->nCol != pos.col)
        {
            // this cube is not selected
            // do nothing with it
            continue;
        }
        
        cube->isMoving = true; // turn on the moving flag
        cube->angle = 0;
        cube->angleDelta = (pos.isCW ? 1 : -1) * (90 / FRAME_PER_MOVE);
        
        if (pos.lvl != -1)
        {
            // we are moving in level mode
            cube->angleType = MOVE_LEVEL;
            cube->angleDelta *= -2; // we have to do this to have correct direction
        }
        else if (pos.row != -1)
        {
            // row mode
            cube->angleType = MOVE_ROW;
			cube->angleDelta *= 2;
        }
        else
        {
            // column mode
            cube->angleType = MOVE_COLUMN;
			cube->angleDelta *= 2;
        }
    }
    
    m_last_position = pos;
}

bool RubiksCube::IsSolved(void)
{
    RubiksCubeColor tmp[6];
    memset(&tmp[0], 0, sizeof(tmp[0]) * 6);
    
    RubiksCubeSingle *cube;
    
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        cube = &m_cubes[i];
        for (int j = 0; j < 6; j++)
        {
            if (cube->f[j] == NONE) continue;

            if (tmp[j] == NONE)
            {
                tmp[j] = cube->f[j];
            }
            else if (tmp[j] != cube->f[j])
            {
                // found a different cube
                // stop checking and return false
                return false;
            }
        }
    }
    
    // found no difference!
    // WOW, CONGRATS!
    return true;
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

void RubiksCube::Draw(bool useSolidColors)
{
    glPushMatrix();
    
    RubiksCubeSingle *cube;
    bool isRePositioned = false;
    
    for (unsigned int i = 0; i < m_numberOfCubes; i++)
    {
        cube = &m_cubes[i];
        
        DrawCube(cube, useSolidColors);
        
        if (cube->isMoving)
        {
            // prepare the next move
            cube->angle += cube->angleDelta;
            
            if (abs(cube->angle) == 90)
            {
                // the move is done
                // re-assign color, position for cubes
                // and stop moving now
                RePositionCubeFromMoving(cube);
                isRePositioned = true;
            }
        }
    }
    
    if (isRePositioned && IsSolved())
    {
        //std::cout << "SOLVED!\n";
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

void RubiksCube::DrawCube(RubiksCubeSingle *cube, bool useSolidColors)
{
#if DEBUG
    // std::cout << "Drawing cube (" << cube->nLvl << ", " << cube->nRow << ", " << cube->nCol << ")" << std::endl;
#endif
    
    glPushMatrix();
    
    // roates the cube (as it's moving)
    if (cube->isMoving)
    {
        switch (cube->angleType) {
            case MOVE_LEVEL:
                glRotatef(cube->angle, 0.f, 1.f, 0.f);
                break;
                
            case MOVE_ROW:
                glRotatef(cube->angle, 0.f, 0.f, -1.f);
                break;
                
            case MOVE_COLUMN:
                glRotatef(cube->angle, 1.f, 0.f, 0.f);
                break;
                
            default:
                break;
        }
    }
    
    // move to the drawing site
    GLfloat x, y, z;
    float sizeOver2 = m_size / 2;
    x = m_x;
    y = m_y;
    z = m_z;
    
    x += (cube->nCol - sizeOver2);
    y += (cube->nLvl - sizeOver2);
    z += (sizeOver2 - cube->nRow);
    
    glTranslatef(x, y, z);

    // prepares the faces' color and draws them
    for (int i = 0; i < 6; i++)
    {
        if (useSolidColors)
        {
            // we are in solid color mode
            // use color to encode cube data
            glColor3f(
                      GetIndexFromLvlRowCol(
                                            cube->nLvl,
                                            cube->nRow,
                                            cube->nCol,
                                            m_size, m_numberOfCubesPerLevel
                                            ) / 255.f,
                      i / 255.f,
                      1.f
                      );
        }
        else
        {
            // select color for each face
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
                default:
                    // draws black face
                    glColor3f(0.f, 0.f, 0.f);
                    break;
            }
        }
                
        DrawTriangleVertex3fv(
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][0]][0],
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][1]][0],
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][2]][0]);
        
        DrawTriangleVertex3fv(
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][0]][0],
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][3]][0],
                              &RubiksCubeFacesVertexes[RubiksCubeFaces[i][2]][0]);
                
        // draw the border
        glColor3f(0., 0., 0.);
        glLineWidth(3.);
        glBegin(GL_LINE_LOOP);
        glVertex3fv(&RubiksCubeFacesVertexes[RubiksCubeFaces[i][0]][0]);
        glVertex3fv(&RubiksCubeFacesVertexes[RubiksCubeFaces[i][1]][0]);
        glVertex3fv(&RubiksCubeFacesVertexes[RubiksCubeFaces[i][2]][0]);
        glVertex3fv(&RubiksCubeFacesVertexes[RubiksCubeFaces[i][3]][0]);
        glEnd();
    }
    
    glPopMatrix();
}

void RubiksCube::RePositionCubeFromMoving(RubiksCubeSingle *cube)
{
    if (!cube->isMoving) return;
    
    int nLvl = cube->nLvl;
    int nRow = cube->nRow;
    int nCol = cube->nCol;

    switch (cube->angleType)
    {
        case MOVE_LEVEL:
            // keep the level, change row and column
            if (cube->angleDelta > 0)
            {
                // counter close-wise
                RotateCubeColors(cube, 0, 4, 1, 5);
                cube->nRow = nCol;
                cube->nCol = m_size - nRow - 1;   
            }
            else
            {
                RotateCubeColors(cube, 0, 5, 1, 4);
                cube->nCol = cube->nRow;
                cube->nRow = m_size - nCol - 1;
            }
            break;
        case MOVE_ROW:
            // keep the row, change level and column
            if (cube->angleDelta > 0)
            {
                // close-wise
                RotateCubeColors(cube, 2, 4, 3, 5);
                cube->nCol = nLvl;
                cube->nLvl = m_size - nCol - 1;
            }
            else
            {
                RotateCubeColors(cube, 2, 5, 3, 4);
                cube->nLvl = nCol;
                cube->nCol = m_size - nLvl - 1;
            }
            break;
        case MOVE_COLUMN:
            // keep the column, change level and row
            if (cube->angleDelta > 0)
            {
                // close-wise
                RotateCubeColors(cube, 0, 2, 1, 3);
                cube->nLvl = nRow;
                cube->nRow = m_size - nLvl - 1;
            }
            else
            {
                RotateCubeColors(cube, 0, 3, 1, 2);
                cube->nRow = nLvl;
                cube->nLvl = m_size - nRow - 1;
            }
            break;
            
        default:
            break;
    }
    
    cube->isMoving = false;
    
#ifdef DEBUG
    std::cout << "RePositioned: (" << nLvl << "," << nRow << "," << nCol << ") -> ("
              << cube->nLvl << "," << cube->nRow << "," << cube->nCol << ").\n";
#endif
}

void RubiksCube::RotateCubeColors(RubiksCubeSingle *cube, int a, int b, int c, int d)
{
    RubiksCubeColor tmp = cube->f[a];
    cube->f[a]          = cube->f[b];
    cube->f[b]          = cube->f[c];
    cube->f[c]          = cube->f[d];
    cube->f[d]          = tmp;
}
