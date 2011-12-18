//
//  dependencies.h
//  RubikTest
//
//  Created by Son Dao Hoang on 11/25/11.
//  Copyright (c) 2011 UET. All rights reserved.
//

#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <string.h>

#define _USE_MATH_DEFINES
#include "math.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <gl/glut.h>
#endif

#if DEBUG
#include <iostream>
#endif


#ifndef __APPLE__
#include <cstdlib>
#include <algoritghm>
#endif

#endif
