//
//  utils.h
//  RubikTest
//
//  Created by Son Dao Hoang on 12/16/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#ifndef RubikTest_utils_h
#define RubikTest_utils_h

#include "dependencies.h"

void DrawTriangleVertex3fv(const GLfloat *v1, const GLfloat *v2, const GLfloat *v3, int countDown = 2);

void GetLvlRowColFromIndex(unsigned int *result, unsigned int i, unsigned int size, unsigned int sizePow2);

unsigned int GetIndexFromLvlRowCol(unsigned int lvl, unsigned int row, unsigned int col, unsigned int size, unsigned int sizePow2);

#endif
