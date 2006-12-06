#ifndef GLTOOLS_H
#define GLTOOLS_H
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <GL/gl.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Matrix.h>

namespace tlp {
  typedef Matrix<float, 4> MatrixGL;
  TLP_GL_SCOPE void glTest(std::string message = "(no description)");
  TLP_GL_SCOPE void setColor(const Color &c);
  TLP_GL_SCOPE void setMaterial(const Color &c);
  //  void multMatrix(const GLfloat *projectionMatrix, const GLfloat *modelviewMatrix, GLfloat *result);
  TLP_GL_SCOPE bool projectToScreen(const Coord &obj, 
				    const MatrixGL &,
				    const Vector<int, 4> &viewport,
				    Vector<float, 2> &result);
  TLP_GL_SCOPE double segmentVisible(const Coord &u, const Coord &v, 
				     const MatrixGL &, const Vector<int, 4> &viewport);
  TLP_GL_SCOPE double segmentSize(const Coord &u, const Coord &v, 
				  const MatrixGL &, const Vector<int, 4> &viewport);
  inline GLfloat sqr(const GLfloat f);
  TLP_GL_SCOPE GLfloat projectSize(const Coord& position, const Size &_size, 
		      const MatrixGL &, const MatrixGL &,const Vector<int, 4> &viewport);
  TLP_GL_SCOPE void cube(GLenum type);
  TLP_GL_SCOPE void solidCone();
}


GLfloat tlp::sqr(const GLfloat f) {
  return f*f;
}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
