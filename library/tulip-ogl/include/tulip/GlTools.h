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

#ifndef GLTOOLS_H
#define GLTOOLS_H
#ifndef DOXYGEN_NOTFOR_DEVEL
#include <tulip/tulipconf.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Matrix.h>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace tlp {

struct BoundingBox;

typedef Matrix<float, 4> MatrixGL;
TLP_GL_SCOPE const std::string& glGetErrorDescription(GLuint errorCode);
TLP_GL_SCOPE void glTest(const std::string &message = std::string("(no description)"));
TLP_GL_SCOPE void setColor(const Color &c);
TLP_GL_SCOPE void setColor(GLfloat *);
TLP_GL_SCOPE void setMaterial(const Color &c);
TLP_GL_SCOPE bool cameraIs3D();
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
TLP_GL_SCOPE GLfloat projectSize(const BoundingBox& bb,
                                 const MatrixGL &, const MatrixGL &,const Vector<int, 4> &viewport);
TLP_GL_SCOPE GLfloat projectSize(const Coord& position,const Size& size,
                                 const MatrixGL &, const MatrixGL &,const Vector<int, 4> &viewport);
TLP_GL_SCOPE float calculateAABBSize(const BoundingBox& bb,const Coord& eye,const Matrix<float, 4>& transformMatrix,const Vector<int, 4>& globalViewport,const Vector<int, 4>& currentViewport);
TLP_GL_SCOPE float calculate2DLod(const BoundingBox& bb,const Vector<int, 4>& globalViewport,const Vector<int, 4>& currentViewport);
}

#endif //DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
