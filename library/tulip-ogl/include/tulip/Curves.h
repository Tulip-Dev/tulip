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

#ifndef CURVES_H
#define CURVES_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/tulipconf.h>

#include <vector>

namespace tlp {

template<typename T, unsigned int N>
class Matrix;
//====================================================================
//return vertices, update startN and endN to prevent bad edge drawing
TLP_GL_SCOPE void computeCleanVertices(const std::vector<Coord> &bends,
                                       const Coord &startPoint, const Coord &endPoint,
                                       Coord &startN, Coord &endN,std::vector<Coord> &);
TLP_GL_SCOPE void polyLine(const std::vector<Coord> &,/* polyline vertices */
                           const Color &,             /* start color */
                           const Color &);            /* end color */
TLP_GL_SCOPE void polyQuad(const std::vector<Coord> &, /* polyline vertces */
                           const Color &,            /* start color */
                           const Color &,            /* end color */
                           const float,              /* start size */
                           const float,              /* end size */
                           const Coord &,            /* normal to the begin of the curve */
                           const Coord &,            /* normal to the end curve */
                           bool,         /* if true : use start and end color to border lines, if false : use borderColor*/
                           const Color &,      /* border color */
                           const std::string &textureName="", /* textureName */
                           const float outlineWidth=0);
TLP_GL_SCOPE void simpleQuad(const std::vector<Coord> &, /* quad vertces */
                             const Color &,            /* start color */
                             const Color &,            /* end color */
                             const float,              /* start size */
                             const float,              /* end size */
                             const Coord &,            /* normal to the begin of the curve */
                             const Coord &,            /* normal to the end curve */
                             const Coord &,            /* direction of the camera*/
                             bool,         /* if true : use start and end color to border lines, if false : use borderColor*/
                             const Color &,      /* border color */
                             const std::string &textureName="");     /* textureName */
TLP_GL_SCOPE void splineQuad(const std::vector<Coord> &,  /* polyline vertces */
                             const Color &,          /* start color */
                             const Color &,          /* end color */
                             const float,            /* start size */
                             const float,            /* end size */
                             const Coord &,          /* nomal to the begin of the curve */
                             const Coord &);         /* nomal to the end curve */
TLP_GL_SCOPE void splineLine(const std::vector<Coord> &,  /* polyline vertces */
                             const Color &,          /* start color */
                             const Color &);          /* end color */
//====================================================================
TLP_GL_SCOPE void getColors(const Coord *line,unsigned int lineSize, const Color &c1, const Color &c2,std::vector<Color> &);
TLP_GL_SCOPE void getSizes(const std::vector<Coord> &line, float s1, float s2, std::vector<float> &);
TLP_GL_SCOPE GLfloat* buildCurvePoints (const std::vector<Coord> &vertices,
                                        const std::vector<float> &sizes,
                                        const Coord &startN,
                                        const Coord &endN,
                                        unsigned int &resultSize,
                                        std::vector<unsigned int> *dec=NULL);
TLP_GL_SCOPE void buildCurvePoints (const std::vector<Coord> &vertices,
                                    const std::vector<float> &sizes,
                                    const Coord &startN,
                                    const Coord &endN,
                                    std::vector<Coord> &result);

}
#endif //DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
