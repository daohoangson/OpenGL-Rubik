//
//  rubik_cube.h
//  RubikTest
//
//  Created by Son Dao Hoang on 11/25/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

#include "dependencies.h"
#include "cursor.h"

typedef enum _RubiksCubeColor
{
    NONE = 0,
    WHITE,
    RED,
    BLUE,
    ORANGE,
    GREEN,
    YELLOW
} RubiksCubeColor;
#define RUBIK_CUBE_COLOR_FRONT WHITE
#define RUBIK_CUBE_COLOR_BACK RED
#define RUBIK_CUBE_COLOR_TOP BLUE
#define RUBIK_CUBE_COLOR_BOTTOM ORANGE
#define RUBIK_CUBE_COLOR_LEFT GREEN
#define RUBIK_CUBE_COLOR_RIGHT YELLOW

typedef enum _RubiksMoveType
{
    MOVE_NONE = 0,
    MOVE_LEVEL,
    MOVE_ROW,
    MOVE_COLUMN
} RubiksMoveType;

typedef struct _RubiksCubeSingle
{
    unsigned int    nLvl;    // level of the cube
                             // 0 == lowest
    
    unsigned int    nRow;    // row of the cube
                             // 0 == in the front
    
    unsigned int    nCol;    // column of the cube
                             // 0 == in the left
    
    RubiksCubeColor f[6];    // faces
                             // f[0] = front
                             // f[1] = back
                             // f[2] = top
                             // f[3] = bottom
                             // f[4] = left
                             // f[5] = right

    int             angle;
    int             angleDelta;
    RubiksMoveType  angleType;
    bool            isMoving;
} RubiksCubeSingle;

class RubiksCube
{
public:
    RubiksCube(int size);
    ~RubiksCube();
    
    void             Move(void *cursor, bool immediately);
    
    void             PreDraw(void);
    void             Draw(void);
	void			 DrawPickMode(void);
    
    void             RotateX(int deltaX);
    void             RotateY(int deltaY);

    unsigned int     GetSize(void);
private:
    GLfloat          m_x;
    GLfloat          m_y;
    GLfloat          m_z;
    int              m_angle_x;
    int              m_angle_y;
    
    unsigned int     m_size;
    unsigned int     m_numberOfCubes;
    RubiksCubeSingle *m_cubes;
    CursorPosition   m_last_position;
    
    void             DrawCube(RubiksCubeSingle *cube);
	void			 DrawCubePickMode(RubiksCubeSingle *cube, int color);
    
    void             RePositionCubeFromMoving(RubiksCubeSingle *cube);
    void             RotateCubeColors(RubiksCubeSingle *cube, int a, int b, int c, int d);
};

#endif
