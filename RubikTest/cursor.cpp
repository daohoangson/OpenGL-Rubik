//
//  cursor.cpp
//  RubikTest
//
//  Created by Son Dao Hoang on 12/16/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#include "cursor.h"
#include "rubiks_cube.h"
#include "utils.h"

GLfloat CursorV[7][3] = {
    {2.5, 0, 1},
    {2.5, 0.75, 0},
    {2.5, -0.75, 0},
    {2.5, 0.5, 0},
    {2.5, -0.5, 0},
    {2.5, -0.5, -1},
    {2.5, 0.5, -1}
};

Cursor::Cursor(void *rubik)
{
    m_rubik = rubik;
    
    m_lvl = 0;
    m_row = -1;
    m_col = -1;
    m_isCW = true;
}

void Cursor::SwitchMode(void)
{
    if (m_lvl != -1)
    {
        m_row = m_lvl;
        m_lvl = -1;
    }
    else if (m_row != -1)
    {
        m_col = m_row;
        m_row = -1;
    }
    else if (m_col != -1)
    {
        m_lvl = m_col;
        m_col = -1;
    }
    else
    {
        // this should not happen
        // but... what the hell, I don't want 
        // this small app to be stuck
        // so...
        m_lvl = 0;
        m_row = -1;
        m_col = -1;
    }
}

void Cursor::ToggleDirection(void)
{
    m_isCW = !m_isCW;
}

void Cursor::Move(int delta)
{
    int imax = ((RubiksCube *)m_rubik)->GetSize() - 1;
    
    if (m_lvl != -1)
    {
        m_lvl += delta;
        m_lvl = std::max(0, std::min<int>(imax, m_lvl));
    }
    else if (m_row != -1)
    {
        m_row += delta;
        m_row = std::max(0, std::min<int>(imax, m_row));
    }
    else
    {
        m_col += delta;
        m_col = std::max(0, std::min<int>(imax, m_col));
    }
}

void Cursor::GoTo(unsigned int lvl, unsigned int row, unsigned int col)
{
    if (m_lvl != -1)
    {
        m_lvl = lvl;
    }
    else if (m_row != -1)
    {
        m_row = row;
    }
    else
    {
        m_col = col;
    }
}

void Cursor::GoToAndSetDirection(unsigned int index1,
                                 unsigned int index2,
                                 unsigned int face,
                                 unsigned int size)
{
    m_lvl = -1;
    m_row = -1;
    m_col = -1;
    
    unsigned int pos1[3]; GetLvlRowColFromIndex(&pos1[0], index1, size, size*size);
    unsigned int pos2[3]; GetLvlRowColFromIndex(&pos2[0], index2, size, size*size);

    unsigned int lvl1 = pos1[0];
    unsigned int row1 = pos1[1];
    unsigned int col1 = pos1[2];
    unsigned int lvl2 = pos2[0];
    unsigned int row2 = pos2[1];
    // unsigned int col2 = pos2[2]; // not used
    
    // each face can only be 2 kinds of move
    // so we check for them here, manually
    switch (face)
    {
        case 0 /* front  */:
        case 1 /* back   */:
            if (lvl1 == lvl2)
            {
                m_lvl = lvl1;
            }
            else
            {
                m_col = col1;
            }
            
            m_isCW = (index1 > index2);
            if (face == 1) m_isCW = !m_isCW; // revert
            
            break;
        case 2 /* top    */:
        case 3 /* bottom */:
            if (row1 == row2)
            {
                m_row = row1;
                m_isCW = (index1 < index2);
            }
            else
            {
                m_col = col1;
                m_isCW = (index1 > index2);
            }
            
            if (face == 3) m_isCW = !m_isCW; // revert
            break;
        case 4 /* left   */:
        case 5 /* right  */:
            if (lvl1 == lvl2)
            {
                m_lvl = lvl1;
            }
            else
            {
                m_row = row1;
            }
            
            m_isCW = (index1 < index2);
            if (face == 5) m_isCW = !m_isCW; // revert
            break;
        default:
            // what?!
            // simply reset to default
            m_lvl = 0;
            break;
    }
}

CursorPosition Cursor::getPosition(void)
{
    CursorPosition pos = {0};
    pos.lvl  = m_lvl;
    pos.row  = m_row;
    pos.col  = m_col;
    pos.isCW = m_isCW;
    
    return pos;
}

bool Cursor::isCW(void)
{
    return m_isCW;
}

void Cursor::Draw(void)
{
    glPushMatrix();
    
    RotateAndTranslate();
    
    DrawOne();
    
    glRotatef(90, 0., 1., 0.);
    DrawOne();
    
    glRotatef(90, 0., 1., 0.);
    DrawOne();
    
    glRotatef(90, 0., 1., 0.);
    DrawOne();
    
    glPopMatrix();
}

void Cursor::RotateAndTranslate(void)
{
    int sizeHalf = floor((double)(((RubiksCube *)m_rubik)->GetSize() / 2.f));
    
    if (m_lvl != -1)
    {
        // in level mode
        glTranslatef(0, m_lvl - sizeHalf, 0);
    }
    else if (m_row != -1)
    {
        // in row mode, we should rotate by x axis
        glRotatef(90, 1., 0., 0.);
        glTranslatef(0, sizeHalf - m_row, 0);
    }
    else
    {
        // in col mode, we should rotate by z axis
        glRotatef(90, 0., 0., 1.);
        glTranslatef(0, sizeHalf - m_col, 0);
    }
}

void Cursor::DrawOne(void)
{
    glPushMatrix();
    
    if (!m_isCW)
    {
        // couter clock-wise
        glRotatef(180, 1., 0, 0);
    }
    
    glColor3f(1. ,1. ,1.);
    DrawTriangleVertex3fv(&CursorV[0][0], &CursorV[1][0], &CursorV[2][0]);
    DrawTriangleVertex3fv(&CursorV[3][0], &CursorV[4][0], &CursorV[5][0]);
    DrawTriangleVertex3fv(&CursorV[3][0], &CursorV[6][0], &CursorV[5][0]);
    
    glColor3f(0., 0., 0.);
    glLineWidth(2.);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(&CursorV[0][0]);
    glVertex3fv(&CursorV[1][0]);
    glVertex3fv(&CursorV[3][0]);
    glVertex3fv(&CursorV[6][0]);
    glVertex3fv(&CursorV[5][0]);
    glVertex3fv(&CursorV[4][0]);
    glVertex3fv(&CursorV[2][0]);
    glEnd();
    
    glPopMatrix();
}

