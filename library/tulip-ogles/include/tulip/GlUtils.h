/**
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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLUTILS_H
#define GLUTILS_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Graph.h>
#include <tulip/Color.h>
#include <tulip/Matrix.h>
#include <tulip/BoundingBox.h>

#include <vector>
#include <sstream>

#define ushort_cast(x) static_cast<unsigned short>((x))

namespace tlp {

typedef Matrix<float, 4> MatrixGL;
class SizeProperty;

class GlGraphRenderingParameters;

template <unsigned int SIZE> void addTlpVecToVecFloat(const tlp::Vector<float, SIZE> &c, std::vector<float> &v) {
  for (size_t i = 0; i < SIZE; ++i) {
    v.push_back(c[i]);
  }
}

template <typename T> std::string toString(const T &val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

template <typename T> const T &clamp(const T &n, const T &lower, const T &upper) {
  return std::max(lower, std::min(n, upper));
}

TLP_GLES_SCOPE void addColorToVecFloat(const tlp::Color &c, std::vector<float> &v);

TLP_GLES_SCOPE std::vector<float> getSizes(const std::vector<tlp::Coord> &line, float s1, float s2);

TLP_GLES_SCOPE void getColors(const std::vector<tlp::Coord> &line, const tlp::Color &c1, const tlp::Color &c2, std::vector<tlp::Color> &result);

TLP_GLES_SCOPE std::vector<tlp::Coord> buildCurvePoints(const std::vector<tlp::Coord> &vertices, const std::vector<float> &sizes,
                                                        const tlp::Coord &startN, const tlp::Coord &endN);

TLP_GLES_SCOPE std::vector<tlp::Coord> computeRegularPolygon(unsigned int numberOfSides, float startAngle = 0.f,
                                                             const tlp::Coord &position = tlp::Coord(0.f, 0.f),
                                                             const tlp::Size &size = tlp::Size(0.5f, 0.5f));

TLP_GLES_SCOPE bool clockwiseOrdering(const std::vector<tlp::Vec2f> &points);

TLP_GLES_SCOPE float compute2DVectorsAngle(const tlp::Vec2f &a, const tlp::Vec2f &b);

TLP_GLES_SCOPE bool isConvexPolygon(const std::vector<tlp::Vec2f> &vertices);

TLP_GLES_SCOPE bool convexPolygonsIntersect(const std::vector<tlp::Vec2f> &convexPolygonA, const std::vector<tlp::Vec2f> &convexPolygonB);

TLP_GLES_SCOPE unsigned int nearestPOT(unsigned int x);

struct TLP_GLES_SCOPE TextureData {

  TextureData(unsigned int width, unsigned int height, unsigned int nbBytesPerPixel, unsigned char *pixels)
      : width(width), height(height), nbBytesPerPixel(nbBytesPerPixel), pixels(pixels) {
  }

  ~TextureData() {
    free(pixels);
  }

  unsigned int width;
  unsigned int height;
  unsigned int nbBytesPerPixel;
  unsigned char *pixels;
};

TLP_GLES_SCOPE TextureData *loadTextureData(const char *file);

TLP_GLES_SCOPE GLuint loadTexture(const char *file);

TLP_GLES_SCOPE tlp::Color uintToColor(const unsigned int n);

TLP_GLES_SCOPE tlp::Color genRandomColor(const unsigned char alpha = 255);

TLP_GLES_SCOPE unsigned int colorToUint(tlp::Color c);

TLP_GLES_SCOPE tlp::Coord projectPoint(const tlp::Coord &obj, const tlp::MatrixGL &transform, const tlp::Vec4i &viewport);

TLP_GLES_SCOPE tlp::Coord unprojectPoint(const tlp::Coord &obj, const tlp::MatrixGL &invtransform, const tlp::Vec4i &viewport);

TLP_GLES_SCOPE float calculateAABBSize(const tlp::BoundingBox &bb, const tlp::Coord &eye, const tlp::MatrixGL &transformMatrix,
                                       const tlp::Vec4i &globalViewport, const tlp::Vec4i &currentViewport);

TLP_GLES_SCOPE float calculate2DLod(const tlp::BoundingBox &bb, const tlp::Vec4i &globalViewport, const tlp::Vec4i &currentViewport);

TLP_GLES_SCOPE GLfloat projectSize(const tlp::Coord &position, const tlp::Size &size, const tlp::MatrixGL &projectionMatrix,
                                   const tlp::MatrixGL &modelviewMatrix, const tlp::Vec4i &viewport);

TLP_GLES_SCOPE GLfloat projectSize(const tlp::BoundingBox &bb, const tlp::MatrixGL &projectionMatrix, const tlp::MatrixGL &modelviewMatrix,
                                   const tlp::Vec4i &viewport);

TLP_GLES_SCOPE const std::string &glGetErrorDescription(GLuint errorCode);

TLP_GLES_SCOPE void checkOGLError(const std::string &function, const std::string &file, const unsigned int line);

#define checkOpenGLError() checkOGLError(__PRETTY_FUNCTION__, __FILE__, __LINE__)

TLP_GLES_SCOPE bool fileExists(const char *fileName);

TLP_GLES_SCOPE void tokenize(const std::string &str, std::vector<std::string> &tokens, const std::string &delimiters);

TLP_GLES_SCOPE tlp::Size getEdgeSize(tlp::Graph *graph, tlp::edge e, tlp::SizeProperty *viewSize, GlGraphRenderingParameters *renderingParameters);

TLP_GLES_SCOPE std::vector<tlp::Coord> simplifyCurve(const std::vector<tlp::Coord> &in);
}

#endif // GLUTILS_H
