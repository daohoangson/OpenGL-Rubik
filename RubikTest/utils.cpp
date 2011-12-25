//
//  utils.cpp
//  RubikTest
//
//  Created by Son Dao Hoang on 12/16/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#include "utils.h"

void DrawTriangleVertex3fv(const GLfloat *v1, const GLfloat *v2, const GLfloat *v3, int countDown)
{
    GLfloat v4[3];
    
    v4[0] = (v1[0] + v3[0])/2;
    v4[1] = (v1[1] + v3[1])/2;
    v4[2] = (v1[2] + v3[2])/2;
    
    if (countDown == 1)
    {
        glBegin(GL_TRIANGLES);
        // glBegin(GL_LINE_LOOP);
        glVertex3fv(v1);
        glVertex3fv(v2);
        glVertex3fv(v4);
        glEnd();
        
        glBegin(GL_TRIANGLES);
        // glBegin(GL_LINE_LOOP);
        glVertex3fv(v3);
        glVertex3fv(v2);
        glVertex3fv(v4);
        glEnd();
    }
    else
    {
        // recursively call ourself
        DrawTriangleVertex3fv(v1, &v4[0], v2, countDown - 1);
        DrawTriangleVertex3fv(v3, &v4[0], v2, countDown - 1);
    }
}

void GetLvlRowColFromIndex(unsigned int *result, unsigned int i, unsigned int size, unsigned int sizePow2)
{
    /* lvl */ result[0] = i / sizePow2;
    /* row */ result[1] = (i - (result[0] * sizePow2)) / size;
    /* col */ result[2] = i - (result[0] * sizePow2)
                            - (result[1] * size);
}

unsigned int GetIndexFromLvlRowCol(unsigned int lvl, unsigned int row, unsigned int col, unsigned int size, unsigned int sizePow2)
{
    return lvl * sizePow2 + row * size + col;
}