#include <GL/gl.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>

namespace tlp {
  void glTest(std::string message = "(no description)");
  void setColor(const Color &c);
  void setMaterial(const Color &c);
  void multMatrix(const GLfloat *projectionMatrix, const GLfloat *modelviewMatrix, GLfloat *result);
  bool projectToScreen(const GLfloat objx, const GLfloat objy, const GLfloat objz, 
		       const GLfloat *transform,
		       const GLint *viewport,
		       GLfloat &winx, GLfloat &winy);
  double segmentVisible(const Coord &u, const Coord &v, 
		      const GLfloat *transform, const GLint *viewport);
  double segmentSize(const Coord &u, const Coord &v, 
		     const GLfloat *transform, const GLint *viewport);
  inline GLfloat sqr(const GLfloat f) {return f*f;}
  GLfloat projectSize(const Coord& position, const Size &_size, const GLfloat *transformMatrix, const GLint *viewport);
  void cube(GLenum type);
  void solidCone();
}
