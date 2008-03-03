#ifndef EPSFUNCTION
#define EPSFUNCTION
#ifndef DOXYGEN_NOTFOR_DEVEL
//====================================================
#include <stdio.h>
#include <tulip/Color.h>
#include <GL/gl.h>

namespace tlp {

void Line(float x1,float y1,float z1,float x2, float y2 ,  float z2, Color C1, Color C2);
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
