//
//  cursor.h
//  RubikTest
//
//  Created by Son Dao Hoang on 12/16/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#ifndef RubikTest_cursor_h
#define RubikTest_cursor_h

#include "dependencies.h"
#include "rubiks_cube.h"

typedef struct _CursorPosition
{
    int  lvl;
    int  row;
    int  col;
    bool isCW;
} CursorPosition;

class Cursor
{
public:
    Cursor(void *rubik);
    
    void            SwitchMode(void);
    void            ToggleDirection(void);
    void            Move(int delta);
    
    CursorPosition  getPosition(void);
    bool            isCW(void);
    
    void            Draw(void);
private:
    void            *m_rubik;
    int             m_lvl;
    int             m_row;
    int             m_col;
    bool            m_isCW;
    
    void            RotateAndTranslate(void);
    void            DrawOne(void);
};

#endif
