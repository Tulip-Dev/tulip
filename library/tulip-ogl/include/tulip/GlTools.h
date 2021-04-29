/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <tulip/OpenGlIncludes.h>

namespace tlp {

struct BoundingBox;

typedef Matrix<float, 4> MatrixGL;
TLP_GL_SCOPE const std::string &glGetErrorDescription(GLuint errorCode);
TLP_GL_SCOPE void glTest(const std::string &message = std::string("(no description)"),
                         int line = -1, bool throwException = true);
#ifndef NDEBUG
#define GL_TEST(throwEx) glTest(__FILE__, __LINE__, throwEx)
#define GL_TEST_ERROR() GL_TEST(false)
#define GL_THROW_ON_ERROR() GL_TEST(true)
#else
#define GL_TEST(throwEx)
#define GL_TEST_ERROR()
#define GL_THROW_ON_ERROR()
#endif
TLP_GL_SCOPE void setColor(const Color &c);
TLP_GL_SCOPE void setColor(GLfloat *);
TLP_GL_SCOPE void setMaterial(const Color &c);
TLP_GL_SCOPE bool cameraIs3D();
/*
 * Project point on screen according to the transformation matrix (modelview * projection)
 * and the viewport (x, y, width, height) given in parameter.
 */
TLP_GL_SCOPE Coord projectPoint(const Coord &obj, const MatrixGL &, const Vector<int, 4> &viewport);
/*
 * UnProject point from screen to 3D world according to the inverse transformation matrix (modelview
 * * projection)^-1
 * and the viewport (x, y, width, height) given in parameter.
 */
TLP_GL_SCOPE Coord unprojectPoint(const Coord &obj, const MatrixGL &,
                                  const tlp::Vector<int, 4> &viewport);
TLP_GL_SCOPE GLfloat projectSize(const BoundingBox &bb, const MatrixGL &, const MatrixGL &,
                                 const Vector<int, 4> &viewport);
TLP_GL_SCOPE GLfloat projectSize(const Coord &position, const Size &size, const MatrixGL &,
                                 const MatrixGL &, const Vector<int, 4> &viewport);
TLP_GL_SCOPE float calculateAABBSize(const BoundingBox &bb, const Coord &eye,
                                     const Matrix<float, 4> &transformMatrix,
                                     const Vector<int, 4> &globalViewport,
                                     const Vector<int, 4> &currentViewport);
TLP_GL_SCOPE float calculate2DLod(const BoundingBox &bb, const Vector<int, 4> &globalViewport,
                                  const Vector<int, 4> &currentViewport);

/**
 * Computes the normals associated to the vertices of a triangulated mesh.
 *
 * @since Tulip 4.8
 *
 * @param vertices a vector containing the vertices to compute associated normals
 * @param facesIndices a vector containing the unsigned short indices of the triangles faces (its
 * size must be a multiple of 3)
 * @return a vector, with the same size as the one holding vertices, filled with the computed
 * normals
 *
 */
TLP_GL_SCOPE std::vector<Coord> computeNormals(const std::vector<Coord> &vertices,
                                               const std::vector<unsigned short> &facesIndices);

/**
 * Computes the normals associated to the vertices of a triangulated mesh.
 *
 * @since Tulip 4.8
 *
 * @param vertices a vector containing the vertices to compute associated normals
 * @param facesIndices a vector containing the unsigned int indices of the triangles faces (its size
 * must be a multiple of 3)
 * @return a vector, with the same size as the one holding vertices, filled with the computed
 * normals
 *
 */
TLP_GL_SCOPE std::vector<Coord> computeNormals(const std::vector<Coord> &vertices,
                                               const std::vector<unsigned int> &facesIndices);

TLP_GL_SCOPE void tesselateFontIcon(const std::string &fontFile, unsigned int iconCodePoint,
                                    GLuint &renderingDataBuffer, GLuint &indicesBuffer,
                                    unsigned int &nbVertices, unsigned int &nbIndices,
                                    unsigned int &nbOutlineIndices, BoundingBox &boundingBox);
} // namespace tlp

#endif // DOXYGEN_NOTFOR_DEVEL
#endif
///@endcond
