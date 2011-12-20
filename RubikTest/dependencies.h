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

#ifdef _WIN32
// fix stupid Visual Studio bug with 
// non-standard stdlib.h
// we have to override it here
#include <stdlib.h>
#endif

#include <gl/glut.h>
#endif

#if DEBUG
#include <iostream>
#endif


#ifndef __APPLE__
#include <cstdlib>
#include <algorithm>
#endif

#endif
