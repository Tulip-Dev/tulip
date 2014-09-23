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

#ifndef _TLP_GLLINES
#define _TLP_GLLINES
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <vector>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/tulipconf.h>

namespace tlp {

struct TLP_GL_SCOPE GlLines {
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

private:
  static void glDisableLineStipple(unsigned int stippleType);
  static void glEnableLineStipple(unsigned int stippleType);
  static GLfloat *buildCurvePoints(const Coord &startPoint,const std::vector<Coord> &bends,const Coord &endPoint);
  static GLfloat *buildCurvePoints(const Coord &p0,const Coord &p1,const Coord &p2,const Coord &p3);
};

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
