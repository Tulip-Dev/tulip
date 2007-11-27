#ifndef GLTOOLS_H
#define GLTOOLS_H
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <GL/gl.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Matrix.h>
#include <tulip/BoundingBox.h>

namespace tlp {
  typedef Matrix<float, 4> MatrixGL;
  TLP_GL_SCOPE void glTest(std::string message = "(no description)");
  TLP_GL_SCOPE void setColor(const Color &c);
  TLP_GL_SCOPE void setMaterial(const Color &c);
  /*
   * Project point on screnn according to the transformation matrix (modelview * projection)
   * and the viewport (x, y, width, height) given in parmaeter.
   */
  TLP_GL_SCOPE Coord projectPoint(const Coord &obj, 
				  const MatrixGL &,
				  const Vector<int, 4> &viewport);
  /*
   * UnProject point from screen to 3D world according to the inverse transformation matrix (modelview * projection)^-1
   * and the viewport (x, y, width, height) given in parmaeter.
   */
  TLP_GL_SCOPE Coord unprojectPoint(const Coord &obj,
				    const MatrixGL &,
				    const tlp::Vector<int, 4> &viewport);
  TLP_GL_SCOPE double segmentVisible(const Coord &u, const Coord &v, 
				     const MatrixGL &, const Vector<int, 4> &viewport);
  TLP_GL_SCOPE double segmentSize(const Coord &u, const Coord &v, 
				  const MatrixGL &, const Vector<int, 4> &viewport);
  inline GLfloat sqr(const GLfloat f);
  TLP_GL_SCOPE GLfloat projectSize(const BoundingBox& bb, 
		      const MatrixGL &, const MatrixGL &,const Vector<int, 4> &viewport);
  TLP_GL_SCOPE GLfloat projectSize(const Coord& position,const Coord& size, 
		      const MatrixGL &, const MatrixGL &,const Vector<int, 4> &viewport);
  TLP_GL_SCOPE void cube(GLenum type);
  TLP_GL_SCOPE void solidCone();

  TLP_GL_SCOPE MatrixGL makeArrowMatrix(const Coord &A, const Coord &B);
}


GLfloat tlp::sqr(const GLfloat f) {
  return f*f;
}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
