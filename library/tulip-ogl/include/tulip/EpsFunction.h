/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
///@cond DOXYGEN_HIDDEN

#ifndef EPSFUNCTION
#define EPSFUNCTION
#ifndef DOXYGEN_NOTFOR_DEVEL
//====================================================

#include <cstdio>


#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace tlp {

class Color;

void Line(float x1,float y1,float z1,float x2, float y2 ,  float z2, Color& C1, Color& C2);
void Rect(float x, float y , float z, float w , float h);
GLfloat *spewPrimitiveEPS(FILE * file, GLfloat * loc);
void spewUnsortedFeedback(FILE * file, GLint size, GLfloat * buffer);
int compare(const void *a, const void *b);
void spewSortedFeedback(FILE * file, GLint size, GLfloat * buffer);
void spewWireFrameEPS(FILE * file, int doSort, GLint size, GLfloat * buffer, char *creator);
void print3DcolorVertex(GLint size, GLint * count, GLfloat * buffer);
void printBuffer(GLint size, GLfloat * buffer);

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
