#include <GL/gl.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>

namespace tlp {
  bool glError();
  void setColor(const Color &c);
  void setMaterial(const Color &c);
  void multMatrix(const GLdouble *projectionMatrix, const GLdouble *modelviewMatrix, GLdouble *result);
  bool projectToScreen(const GLdouble objx, const GLdouble objy, const GLdouble objz, 
		       const GLdouble *transform,
		       const GLint *viewport,
		       GLdouble &winx, GLdouble &winy);
  bool segmentVisible(const Coord &u, const Coord &v, 
		      const GLdouble *transform, const GLint *viewport);
  inline GLdouble sqr(const GLdouble f) {return f*f;}
  GLdouble projectSize(const Coord& position, const Size &_size, const GLdouble *transformMatrix, const GLint *viewport);
}
