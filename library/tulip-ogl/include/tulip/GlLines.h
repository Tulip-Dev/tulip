#ifndef _TLP_GLLINES
#define _TLP_GLLINES
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <GL/gl.h>
#include <vector>

#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Color.h>

namespace tlp {

struct GlLines
{
  //Curves types: linear, bezier, spline order 3, spline order 4
  enum InterpolationMethod {LINEAR=0, BEZIER, SPLINE3, SPLINE4};
  enum StippleType {TLP_PLAIN=0, TLP_DOT, TLP_DASHED, TLP_ALTERNATE};
  
  static void glDrawPoint(const Coord &p);

  static void glDrawLine(const Coord &startPoint,const Coord &endPoint,const double width,const unsigned int stippleType,
			 const Color &startColor,const Color &endColor,const bool arrow=false,const double arrowWidth=1,
			 const double arrowHeight=1);
  static void glDrawCurve(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint,const double width,
			  const unsigned int stippleType,const Color &startColor,const Color &endColor,const bool arrow=false,
			  const double arrowWidth=1,const double arrowHeight=1);
  static void glDrawBezierCurve(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint,unsigned int steps,
				const double width,const unsigned int stippleType,const Color &startColor,const Color &endColor,
				const bool arrow=false,const double arrowWidth=1,const double arrowHeight=1);
  static void glDrawSplineCurve(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint,unsigned int steps,
				const double width,const unsigned int stippleType,const Color &startColor,const Color &endColor,
				const bool arrow=false,const double arrowWidth=1,const double arrowHeight=1);
  static void glDrawSpline2Curve(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint,unsigned int steps,
				const double width,const unsigned int stippleType,const Color &startColor,const Color &endColor,
				const bool arrow=false,const double arrowWidth=1,const double arrowHeight=1);

  static void glDrawExtrusion(const Coord &startNode, const Coord &finalNode,
                              const Coord &startPoint, const std::vector<Coord> &bends, const Coord &endPoint,
			      unsigned int steps, const Size &size, const StippleType stippleType,
			      InterpolationMethod interpolation,
			      const Color &startColor,const Color &endColor);
  
  private:
  static void glDisableLineStipple(unsigned int stippleType);
  static void glEnableLineStipple(unsigned int stippleType);
  static GLfloat *buildCurvePoints(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint);
  static GLfloat *buildCurvePoints(const Coord &p0,const Coord &p1,const Coord &p2,const Coord &p3);
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
