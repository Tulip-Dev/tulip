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

#include <GL/glew.h>

#include <fstream>

#include <tulip/BoundingBox.h>
#include <tulip/Rectangle.h>
#include <tulip/Matrix.h>
#include <tulip/SizeProperty.h>
#include <tulip/TlpTools.h>

#include <tulip/GlUtils.h>
#include <tulip/GlGraphRenderingParameters.h>

#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

using namespace std;
using namespace tlp;

typedef Matrix<float, 2> Mat2f;

//===========================================================================================================
void tlp::addColorToVecFloat(const tlp::Color &c, std::vector<float> &v) {
  v.push_back(c.getRGL());
  v.push_back(c.getGGL());
  v.push_back(c.getBGL());
  v.push_back(c.getAGL());
}
//===========================================================================================================
inline float sqrnorm(const Coord &v) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
//===========================================================================================================
static float lineLength(const vector<Coord> &line) {
  float result = 0;

  for (unsigned int i = 1; i < line.size(); ++i) {
    result += sqrnorm(line[i - 1] - line[i]);
  }

  return result;
}
//===========================================================================================================
std::vector<float> tlp::getSizes(const vector<Coord> &line, float s1, float s2) {
  vector<float> result;
  result.resize(line.size());
  result[0] = s1;
  result[line.size() - 1] = s2;
  s2 -= s1;
  s2 /= lineLength(line);

  for (unsigned int i = 1; i < line.size() - 1; ++i) {
    float delta = sqrnorm(line[i - 1] - line[i]);
    s1 += s2 * delta;
    result[i] = s1;
  }
  return result;
}
//================================================
void tlp::getColors(const vector<Coord> &line, const Color &c1, const Color &c2, vector<Color> &result) {
  tlp::Vec4f _c1, _c2;

  for (unsigned int i = 0; i < 4; ++i) {
    _c1[i] = c1[i];
    _c2[i] = c2[i];
  }

  result.resize(line.size());
  result[0] = c1;
  result[line.size() - 1] = c2;

  if (line.size() == 2)
    return;

  _c2 -= _c1;
  _c2 /= lineLength(line);

  for (unsigned int i = 1; i < line.size() - 1; ++i) {
    float delta = sqrnorm(line[i - 1] - line[i]);
    _c1 += _c2 * delta;
    result[i] = Color(static_cast<unsigned char>(_c1[0]), static_cast<unsigned char>(_c1[1]), static_cast<unsigned char>(_c1[2]),
                      static_cast<unsigned char>(_c1[3]));
  }
}
//===========================================================================================================
static int computeExtrusion(const Coord &pBefore, const Coord &pCurrent, const Coord &pAfter, float size, float inversion, vector<Coord> &result,
                            bool lastPoint = false, bool twoPointsCurve = false) {

  Coord u = pBefore - pCurrent;
  Coord v = pAfter - pCurrent;

  if (fabs(u[2]) < 1e-3)
    u[2] = 0;

  if (fabs(v[2]) < 1e-3)
    v[2] = 0;

  Coord xu(u);

  if (u.norm() != 0)
    xu /= u.norm();

  Coord xv(v);

  if (v.norm() != 0)
    xv /= v.norm();

  Coord bi_xu_xv = xu + xv;

  float newSize = size;
  float angle = 0;

  if (!result.empty() && !lastPoint && bi_xu_xv.norm() < 1e-3) {
    return inversion;
  }

  if (bi_xu_xv.norm() != 0) {
    bi_xu_xv /= bi_xu_xv.norm();
  }

  angle = static_cast<float>(M_PI - atan2((u ^ v).norm(), u.dotProduct(v)));

  bool angleOk = true;

  if (isnan(angle) || fabs(angle) < 1e-3) {
    angleOk = false;

    if (!lastPoint)
      bi_xu_xv = xv;
    else
      bi_xu_xv = xu;

    if (bi_xu_xv[0] == 0 && bi_xu_xv[1] == 0 && fabs(bi_xu_xv[2]) == 1) {
      bi_xu_xv[0] = bi_xu_xv[2];
      bi_xu_xv[2] = 0;
    }
  } else {
    newSize = newSize / static_cast<float>(cos(angle / 2.0));
  }

  if (angleOk && angle < M_PI / 2 + M_PI / 4) {
    if ((xu ^ xv)[2] > 0) {
      result.push_back(pCurrent + bi_xu_xv * newSize * static_cast<float>(inversion));
      result.push_back(pCurrent - bi_xu_xv * newSize * static_cast<float>(inversion));
    } else {
      result.push_back(pCurrent - bi_xu_xv * newSize * static_cast<float>(inversion));
      result.push_back(pCurrent + bi_xu_xv * newSize * static_cast<float>(inversion));
    }
  } else {
    // broken form

    Coord vectUnit(-bi_xu_xv[1], bi_xu_xv[0], bi_xu_xv[2]);

    if (angleOk && !twoPointsCurve && !(newSize > u.norm() || newSize > v.norm() || fabs(angle - M_PI) < 1E-3)) {

      if ((xu ^ xv)[2] > 0) {
        result.push_back(pCurrent + bi_xu_xv * newSize * inversion);
        result.push_back(pCurrent - vectUnit * size * inversion);
        result.push_back(pCurrent + bi_xu_xv * newSize * inversion);
        result.push_back(pCurrent + vectUnit * size * inversion);
      } else {
        result.push_back(pCurrent + vectUnit * size * inversion);
        result.push_back(pCurrent + bi_xu_xv * newSize * inversion);
        result.push_back(pCurrent - vectUnit * size * inversion);
        result.push_back(pCurrent + bi_xu_xv * newSize * inversion);
      }
    } else {

      result.push_back(pCurrent + vectUnit * size * static_cast<float>(inversion));
      result.push_back(pCurrent - vectUnit * size * static_cast<float>(inversion));
      inversion *= -1;
    }
  }

  return inversion;
}
//===========================================================================================================
vector<Coord> tlp::buildCurvePoints(const vector<Coord> &vertices, const vector<float> &sizes, const Coord &startN, const Coord &endN) {

  int inversion = 1;
  bool twoPointsCurve = (vertices.size() == 2);
  vector<Coord> result;

  result.reserve(vertices.size() * 2);

  if (startN != vertices[0]) {
    inversion = computeExtrusion(startN, vertices[0], vertices[1], sizes[0], inversion, result, false, twoPointsCurve);
  } else {
    inversion =
        computeExtrusion(vertices[0] - (vertices[1] - vertices[0]), vertices[0], vertices[1], sizes[0], inversion, result, false, twoPointsCurve);
  }

  for (unsigned int i = 1; i < vertices.size() - 1; ++i) {
    inversion = computeExtrusion(vertices[i - 1], vertices[i], vertices[i + 1], sizes[i], inversion, result, false, twoPointsCurve);
  }

  if (endN != vertices[vertices.size() - 1]) {
    inversion = computeExtrusion(vertices[vertices.size() - 2], vertices[vertices.size() - 1], endN, sizes[sizes.size() - 1], inversion, result, true,
                                 twoPointsCurve);
  } else {
    inversion = computeExtrusion(vertices[vertices.size() - 2], vertices[vertices.size() - 1],
                                 vertices[vertices.size() - 1] + (vertices[vertices.size() - 1] - vertices[vertices.size() - 2]),
                                 sizes[sizes.size() - 1], inversion, result, true, twoPointsCurve);
  }
  return result;
}
//===========================================================================================================
vector<Coord> tlp::computeRegularPolygon(unsigned int numberOfSides, float startAngle, const Coord &position, const Size &size) {

  BoundingBox box;
  vector<Coord> points;
  float delta = (2.0f * M_PI) / float(numberOfSides);

  for (unsigned int i = 0; i < numberOfSides; ++i) {
    float deltaX = cos(i * delta + startAngle);
    float deltaY = sin(i * delta + startAngle);
    points.push_back(Coord(deltaX, deltaY, position[2]));
    box.expand(points.back());
  }

  for (vector<Coord>::iterator it = points.begin(); it != points.end(); ++it) {
    (*it)[0] = position[0] + (((*it)[0] - ((box[1][0] + box[0][0]) / 2.)) / ((box[1][0] - box[0][0]) / 2.)) * size[0];
    (*it)[1] = position[1] + (((*it)[1] - ((box[1][1] + box[0][1]) / 2.)) / ((box[1][1] - box[0][1]) / 2.)) * size[1];
    //(*it)[2]=position[1]+(((*it)[1]-((box[1][1]+box[0][1])/2.))/((box[1][1]-box[0][1])/2.))*size[2];
  }

  return points;
}
//===========================================================================================================
bool tlp::clockwiseOrdering(const vector<Vec2f> &points) {
  float signedArea = 0;
  for (size_t i = 0; i < points.size() - 1; ++i) {
    signedArea += (points[i][0] * points[i + 1][1] - points[i + 1][0] * points[i][1]);
  }
  signedArea += (points.back()[0] * points.front()[1] - points.front()[0] * points.back()[1]);
  return signedArea < 0;
}
//===========================================================================================================
float tlp::compute2DVectorsAngle(const Vec2f &a, const Vec2f &b) {
  float s = (a ^ b).norm();
  float c = a.dotProduct(b);
  float angle = atan2(s, c);
  Mat2f m;
  m[0][0] = a[0];
  m[1][0] = a[1];
  m[0][1] = b[0];
  m[1][1] = b[1];
  float d = m.determinant();
  if (d == 0) {
    if ((a[0] / b[0]) > 0) {
      angle = 0;
    } else {
      angle = M_PI;
    }
  } else if (d < 0) {
    angle = 2 * M_PI - angle;
  }
  return angle;
}
//===========================================================================================================
bool tlp::isConvexPolygon(const vector<Vec2f> &vertices) {
  assert(vertices.size() > 2 && !clockwiseOrdering(vertices));
  for (size_t i = 0; i < vertices.size() - 2; ++i) {
    Vec2f a = vertices[i] - vertices[i + 1];
    Vec2f b = vertices[i + 2] - vertices[i + 1];
    if (compute2DVectorsAngle(a, b) > M_PI)
      return false;
  }
  if (compute2DVectorsAngle((vertices[vertices.size() - 2] - vertices.back()), vertices.front() - vertices.back()) > M_PI)
    return false;
  if (compute2DVectorsAngle(vertices.back() - vertices.front(), vertices[1] - vertices.front()) > M_PI)
    return false;
  return true;
}
//===========================================================================================================
// Gather up one-dimensional extents of the projection of the polygon onto this axis.
static void gatherPolygonProjectionExtents(const std::vector<tlp::Vec2f> &convexPolygon, const Vec2f &axis, float &outMin, float &outMax) {

  // Initialize extents to a single point, the first vertex
  outMin = outMax = axis.dotProduct(convexPolygon[0]);

  // Now scan all the rest, growing extents to include them
  for (size_t i = 1; i < convexPolygon.size(); ++i) {
    float d = axis.dotProduct(convexPolygon[i]);
    if (d < outMin)
      outMin = d;
    else if (d > outMax)
      outMax = d;
  }
}

// Helper routine: test if two convex polygons overlap, using only the edges of
// the first polygon (polygon A) to build the list of candidate separating axes.
static bool findSeparatingAxis(const std::vector<tlp::Vec2f> &convexPolygonA, const std::vector<tlp::Vec2f> &convexPolygonB) {

  // Iterate over all the edges
  size_t prev = convexPolygonA.size() - 1;
  for (size_t cur = 0; cur < convexPolygonA.size(); ++cur) {

    // Get edge vector
    Vec2f edge = convexPolygonA[cur] - convexPolygonA[prev];

    // Rotate vector 90 degrees (doesn't matter which way) to get candidate separating axis.
    Vec2f axis;
    axis[0] = edge[1];
    axis[1] = -edge[0];

    // Gather extents of both polygons projected onto this axis
    float aMin, aMax, bMin, bMax;
    gatherPolygonProjectionExtents(convexPolygonA, axis, aMin, aMax);
    gatherPolygonProjectionExtents(convexPolygonB, axis, bMin, bMax);

    // Is this a separating axis ?
    if (aMax < bMin)
      return true;
    if (bMax < aMin)
      return true;

    // Next edge
    prev = cur;
  }

  // Failed to find a separating axis
  return false;
}

// High level entry point.  It tests whether two convex polygons intersect.
bool tlp::convexPolygonsIntersect(const std::vector<tlp::Vec2f> &convexPolygonA, const std::vector<tlp::Vec2f> &convexPolygonB) {
  assert(isConvexPolygon(convexPolygonA) && isConvexPolygon(convexPolygonB));

  // First, use all of A's edges to get candidate separating axes
  if (findSeparatingAxis(convexPolygonA, convexPolygonB))
    return false;

  // Now swap roles, and use B's edges
  if (findSeparatingAxis(convexPolygonB, convexPolygonA))
    return false;

  // No separating axis found.  They must overlap
  return true;
}

//===========================================================================================================
static unsigned char *invertImage(int pitch, int height, unsigned char *imagePixels) {
  unsigned char *tempRow = new unsigned char[pitch];
  unsigned int heightDiv2 = static_cast<unsigned int>(height * .5);
  for (unsigned int index = 0; index < heightDiv2; ++index) {
    memcpy(tempRow, imagePixels + pitch * index, pitch);
    memcpy(imagePixels + pitch * index, imagePixels + pitch * (height - index - 1), pitch);
    memcpy(imagePixels + pitch * (height - index - 1), tempRow, pitch);
  }
  delete[] tempRow;
  return imagePixels;
}
//===========================================================================================================
unsigned int tlp::nearestPOT(unsigned int x) {
  return pow(2, ceil(log(x) / log(2)));
}
//===========================================================================================================
tlp::GlTextureData *tlp::GlTextureLoader::loadTexture(const std::string &filename) {
  tlp_stat_t buf;
  if (tlp::statPath(filename.c_str(), &buf) < 0)
    return nullptr;
  int w, h, n;
  static const unsigned int nbBytesPerPixel = 4;
  unsigned char *pixels = stbi_load(filename.c_str(), &w, &h, &n, nbBytesPerPixel);
  if (pixels) {
    int nearestpotW = nearestPOT(w);
    int nearestpotH = nearestPOT(h);
    if (nearestpotW != w || nearestpotH != h) {
      unsigned char *newPixels = new unsigned char[nearestpotW * nearestpotH * nbBytesPerPixel];
      stbir_resize_uint8(pixels, w, h, 0, newPixels, nearestpotW, nearestpotH, 0, nbBytesPerPixel);
      delete[] pixels;
      pixels = newPixels;
      w = nearestpotW;
      h = nearestpotH;
    }
    return new GlTextureData(w, h, invertImage(w * nbBytesPerPixel, h, pixels));
  } else {
    std::cerr << "Unable to load image file " << filename << std::endl;
    return nullptr;
  }
}
//===========================================================================================================
tlp::Color tlp::uintToColor(const unsigned int n) {
  unsigned char red = static_cast<unsigned char>(n & 0x000000FF);
  unsigned char green = static_cast<unsigned char>((n & 0x0000FF00) >> 8);
  unsigned char blue = static_cast<unsigned char>((n & 0x00FF0000) >> 16);
  unsigned char alpha = static_cast<unsigned char>((n & 0xFF000000) >> 24);
  return tlp::Color(red, green, blue, alpha);
}
//===========================================================================================================
unsigned int tlp::colorToUint(tlp::Color c) {
  unsigned int red = c.getR();
  unsigned int green = c.getG();
  unsigned int blue = c.getB();
  unsigned int alpha = c.getA();
  unsigned int n = red | (green << 8) | (blue << 16) | (alpha << 24);
  return n;
}
//===========================================================================================================
tlp::Color tlp::genRandomColor(const unsigned char alpha) {
  unsigned char r = static_cast<unsigned char>(rand() / static_cast<float>(RAND_MAX) * 255);
  unsigned char g = static_cast<unsigned char>(rand() / static_cast<float>(RAND_MAX) * 255);
  unsigned char b = static_cast<unsigned char>(rand() / static_cast<float>(RAND_MAX) * 255);
  return tlp::Color(r, g, b, alpha);
}
//===========================================================================================================
Coord tlp::projectPoint(const Coord &obj, const MatrixGL &transform, const Vec4i &viewport) {
  Vec4f point;
  point[0] = obj[0];
  point[1] = obj[1];
  point[2] = obj[2];
  point[3] = 1.0f;
  point = point * transform;

  Coord result(point[0], point[1], point[2]);
  result /= point[3];

  result[0] = viewport[0] + (1.0f + result[0]) * viewport[2] * 0.5f;
  result[1] = viewport[1] + (1.0f + result[1]) * viewport[3] * 0.5f;
  result[2] = (1.0f + result[2]) * 0.5f;
  return result;
}
//===========================================================================================================
Coord tlp::unprojectPoint(const Coord &obj, const MatrixGL &invtransform, const Vec4i &viewport) {
  Vec4f point;

  point[0] = (obj[0] - viewport[0]) / viewport[2] * 2.0f - 1.0f;
  point[1] = (obj[1] - viewport[1]) / viewport[3] * 2.0f - 1.0f;
  point[2] = 2.0f * obj[2] - 1.0f;
  point[3] = 1.0f;

  point = point * invtransform;

  Coord result(point[0], point[1], point[2]);
  result /= point[3];

  return result;
}
//===========================================================================================================
static char hullVertexTable[][7] = {
    {0, 0, 0, 0, 0, 0, 0}, // 0
    {4, 0, 4, 7, 3, 0, 0}, // 1
    {4, 1, 2, 6, 5, 0, 0}, // 2
    {0, 0, 0, 0, 0, 0, 0}, // 3
    {4, 0, 1, 5, 4, 0, 0}, // 4
    {6, 0, 1, 5, 4, 7, 3}, // 5
    {6, 0, 1, 2, 6, 5, 4}, // 6
    {0, 0, 0, 0, 0, 0, 0}, // 7
    {4, 2, 3, 7, 6, 0, 0}, // 8
    {6, 4, 7, 6, 2, 3, 0}, // 9
    {6, 2, 3, 7, 6, 5, 1}, // 10
    {0, 0, 0, 0, 0, 0, 0}, // 11
    {0, 0, 0, 0, 0, 0, 0}, // 12
    {0, 0, 0, 0, 0, 0, 0}, // 13
    {0, 0, 0, 0, 0, 0, 0}, // 14
    {0, 0, 0, 0, 0, 0, 0}, // 15
    {4, 0, 3, 2, 1, 0, 0}, // 16
    {6, 0, 4, 7, 3, 2, 1}, // 17
    {6, 0, 3, 2, 6, 5, 1}, // 18
    {0, 0, 0, 0, 0, 0, 0}, // 19
    {6, 0, 3, 2, 1, 5, 4}, // 20
    {6, 2, 1, 5, 4, 7, 3}, // 21
    {6, 0, 3, 2, 6, 5, 4}, // 22
    {0, 0, 0, 0, 0, 0, 0}, // 23
    {6, 0, 3, 7, 6, 2, 1}, // 24
    {6, 0, 4, 7, 6, 2, 1}, // 25
    {6, 0, 3, 7, 6, 5, 1}, // 26
    {0, 0, 0, 0, 0, 0, 0}, // 27
    {0, 0, 0, 0, 0, 0, 0}, // 28
    {0, 0, 0, 0, 0, 0, 0}, // 29
    {0, 0, 0, 0, 0, 0, 0}, // 30
    {0, 0, 0, 0, 0, 0, 0}, // 31
    {4, 4, 5, 6, 7, 0, 0}, // 32
    {6, 4, 5, 6, 7, 3, 0}, // 33
    {6, 1, 2, 6, 7, 4, 5}, // 34
    {0, 0, 0, 0, 0, 0, 0}, // 35
    {6, 0, 1, 5, 6, 7, 4}, // 36
    {6, 0, 1, 5, 6, 7, 3}, // 37
    {6, 0, 1, 2, 6, 7, 4}, // 38
    {0, 0, 0, 0, 0, 0, 0}, // 39
    {6, 2, 3, 7, 4, 5, 6}, // 40
    {6, 0, 4, 5, 6, 2, 3}, // 41
    {6, 1, 2, 3, 7, 4, 5}  // 42
};

float tlp::calculateAABBSize(const BoundingBox &bb, const Coord &eye, const MatrixGL &transformMatrix, const Vec4i &globalViewport,
                             const Vec4i &currentViewport) {
  BoundingBox bbTmp(bb);
  Coord src[8];
  Coord dst[8];
  int pos;
  int num;

  for (int i = 0; i < 3; i++) {
    if (bbTmp[0][i] > bbTmp[1][i]) {
      float tmp = bbTmp[0][i];
      bbTmp[0][i] = bbTmp[1][i];
      bbTmp[1][i] = tmp;
    }
  }

  bbTmp.getCompleteBB(src);
  pos = ((eye[0] < src[0][0])) + ((eye[0] > src[6][0]) << 1) + ((eye[1] < src[0][1]) << 2) + ((eye[1] > src[6][1]) << 3) +
        ((eye[2] < src[0][2]) << 4) + ((eye[2] > src[6][2]) << 5);
  assert(pos <= 42);

  // If pos==0 : camera are inside the entity so we return a arbitrary lod
  if (pos == 0)
    return 10.;

  num = hullVertexTable[pos][0];

  if (num == 0)
    return -1;

  for (int i = 0; i < num; i++) {
    dst[i] = projectPoint(src[(int)hullVertexTable[pos][i + 1]], transformMatrix, globalViewport);
    // dst[i] -= Coord(globalViewport[0], globalViewport[1]);
  }

  bool inScreen = false;
  float bbBox[4] = {0.f, 0.f, 0.f, 0.f};

  for (int i = 0; i < num; i++) {
    if ((dst[i][0] >= currentViewport[0]) && (dst[i][0] <= currentViewport[0] + currentViewport[2]) && (dst[i][1] >= currentViewport[1]) &&
        (dst[i][1] <= currentViewport[1] + currentViewport[3])) {
      inScreen = true;
    }

    if (i == 0) {
      bbBox[0] = dst[i][0];
      bbBox[2] = dst[i][0];
      bbBox[1] = dst[i][1];
      bbBox[3] = dst[i][1];
    } else {
      if (dst[i][0] < bbBox[0])
        bbBox[0] = dst[i][0];

      if (dst[i][0] > bbBox[2])
        bbBox[2] = dst[i][0];

      if (dst[i][1] < bbBox[1])
        bbBox[1] = dst[i][1];

      if (dst[i][1] > bbBox[3])
        bbBox[3] = dst[i][1];
    }

    if (bbBox[0] < currentViewport[0] + currentViewport[2] && bbBox[2] > currentViewport[0] && bbBox[1] < currentViewport[1] + currentViewport[3] &&
        bbBox[3] > currentViewport[1]) {
      inScreen = true;
    }
  }

  if (!inScreen) {
    return -1;
  } else {
    return sqrt((bbBox[2] - bbBox[0]) * (bbBox[2] - bbBox[0]) + (bbBox[3] - bbBox[1]) * (bbBox[3] - bbBox[1])) * 2;
  }
}
//===========================================================================================================
float tlp::calculate2DLod(const BoundingBox &bb, const Vec4i &, const Vec4i &) {
  return (bb[1][0] - bb[0][0]) * (bb[1][1] - bb[0][1]);
}
//===========================================================================================================
GLfloat tlp::projectSize(const Coord &position, const Size &size, const MatrixGL &projectionMatrix, const MatrixGL &modelviewMatrix,
                         const Vec4i &viewport) {
  BoundingBox box;
  box.expand(position - size / 2.f);
  box.expand(position + size / 2.f);
  return projectSize(box, projectionMatrix, modelviewMatrix, viewport);
}
//===========================================================================================================
GLfloat tlp::projectSize(const BoundingBox &bb, const MatrixGL &projectionMatrix, const MatrixGL &modelviewMatrix, const Vec4i &viewport) {
  Coord bbSize(bb[1] - bb[0]);
  float nSize = bbSize.norm(); // Enclosing bounding box

  MatrixGL translate;
  translate.fill(0);

  for (unsigned int i = 0; i < 4; ++i)
    translate[i][i] = 1;

  for (unsigned int i = 0; i < 3; ++i)
    translate[3][i] = bb[0][i] + bbSize[i] / 2;

  MatrixGL tmp(translate * modelviewMatrix);

  tmp[0][0] = nSize;
  tmp[0][1] = 0;
  tmp[0][2] = 0;
  tmp[1][0] = 0;
  tmp[1][1] = 0;
  tmp[1][2] = 0;
  tmp[2][0] = 0;
  tmp[2][1] = 0;
  tmp[2][2] = 0;

  tmp *= projectionMatrix;

  Vector<float, 4> vect1;
  vect1[0] = 0.5;
  vect1[1] = 0;
  vect1[2] = 0;
  vect1[3] = 1.0;
  Vector<float, 4> proj1 = vect1 * tmp;

  Vector<float, 4> vect2;
  vect2.fill(0);
  vect2[3] = 1.0;
  Vector<float, 4> proj2 = vect2 * tmp;

  float x1 = (proj1[0] / proj1[3] * 0.5 + 0.5) * viewport[2];
  float x2 = (proj2[0] / proj2[3] * 0.5 + 0.5) * viewport[2];

  float width = fabs(x1 - x2);
  float size = pow(2. * width, 2);

  // Test of visibily
  x2 += viewport[0];
  float y2 = (proj2[1] / proj2[3] * 0.5 + 0.5) * viewport[3] + viewport[1];
  Vector<float, 2> upleft;
  upleft[0] = x2 - width;
  upleft[1] = y2 - width;
  Vector<float, 2> downright;
  downright[0] = x2 + width;
  downright[1] = y2 + width;
  Rectangle<float> r1;
  r1[0] = upleft;
  r1[1] = downright;

  Vector<float, 2> upleftV;
  upleftV[0] = viewport[0];
  upleftV[1] = viewport[1];

  Vector<float, 2> downrightV;
  downrightV[0] = viewport[0] + viewport[2];
  downrightV[1] = viewport[1] + viewport[3];

  Rectangle<float> r2;
  r2[0] = upleftV;
  r2[1] = downrightV;

  if (!r1.intersect(r2)) {
    size *= -1.0;
  }

  return size;
}
//===========================================================================================================
// simple structure to embed an error code and its description
struct glErrorStruct {
  GLuint code;
  const std::string description;
};

// the known gl errors
static const struct glErrorStruct glErrorStructs[] = {
    {GL_NO_ERROR, "no error"},
    {GL_INVALID_ENUM, "invalid enumerant"},
    {GL_INVALID_VALUE, "invalid value"},
    {GL_INVALID_OPERATION, "invalid operation"},
    {GL_STACK_OVERFLOW, "stack overflow"},
    {GL_STACK_UNDERFLOW, "stack underflow"},
    {GL_OUT_OF_MEMORY, "out of memory"},
#ifdef GL_EXT_framebuffer_object
    {GL_INVALID_FRAMEBUFFER_OPERATION_EXT, "invalid framebuffer operation"},
#endif
    {GL_TABLE_TOO_LARGE, "table too large"},
    {UINT_MAX, "unknow error"} /* end of list indicator */
};

// the function to retrieve
const std::string &tlp::glGetErrorDescription(GLuint errorCode) {
  unsigned int i = 0;

  while (glErrorStructs[i].code != errorCode && glErrorStructs[i].code != UINT_MAX)
    ++i;

  return glErrorStructs[i].description;
}

void tlp::checkOGLError(const std::string &function, const std::string &file, const unsigned int line) {
  GLuint error = glGetError();
  while (error != GL_NO_ERROR) {
    std::cout << "[OpenGL Error] " << glGetErrorDescription(error) << " in function \"" << function << "\" from file \"" << file << "\" at line "
              << line << std::endl;
    error = glGetError();
  }
}
//===========================================================================================================
bool tlp::fileExists(const char *fileName) {
  std::ifstream infile(fileName);
  return infile.good();
}
//===========================================================================================================
void tlp::tokenize(const string &str, vector<string> &tokens, const string &delimiters) {

  tokens.clear();
  // Skip delimiters at beginning.
  string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (string::npos != pos || string::npos != lastPos) {

    // Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));

    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
}
//===========================================================================================================
tlp::Size tlp::getEdgeSize(tlp::Graph *graph, edge e, tlp::SizeProperty *viewSize, GlGraphRenderingParameters *renderingParameters) {
  node src = graph->source(e);
  node tgt = graph->target(e);
  const Size &srcSize = viewSize->getNodeValue(src);
  const Size &tgtSize = viewSize->getNodeValue(tgt);
  const Size &edgeSize = viewSize->getEdgeValue(e);

  float startSize = 0;
  float endSize = 0;
  if (!renderingParameters->interpolateEdgesSizes()) {
    startSize = edgeSize[0] / 4.f;
    endSize = edgeSize[1] / 4.f;
    if (renderingParameters->maxEdgesSizesToNodesSizes()) {
      float maxSrcSize = max(srcSize[0], srcSize[1]);
      float maxTgtSize = max(tgtSize[0], tgtSize[1]);
      startSize = min(startSize, maxSrcSize);
      endSize = min(endSize, maxTgtSize);
    }
  } else {
    if (srcSize[0] < srcSize[1]) {
      startSize = srcSize[0] / 16.f;
    } else {
      startSize = srcSize[1] / 16.f;
    }
    if (tgtSize[0] < tgtSize[1]) {
      endSize = tgtSize[0] / 16.f;
    } else {
      endSize = tgtSize[1] / 16.f;
    }
  }
  return Size(startSize, endSize);
}
//===========================================================================================================
class CellCBuilder {
public:
  CellCBuilder() {
  }
  CellCBuilder(const std::vector<tlp::Coord> *data, int id, int next, int prev) : _data(data), _id(id) {
    set(prev, next);
  }
  const std::vector<tlp::Coord> *_data;
  int _id;
  int _next, _prev;
  double _sin;

  void set(int prev, int next) {
    _prev = prev;
    _next = next;
    if (prev > -1 && next < (int)_data->size()) {
      Vec3f v1((*_data)[_prev] - (*_data)[_id]);
      Vec3f v2((*_data)[_next] - (*_data)[_id]);
      v1.normalize();
      v2.normalize();
      _sin = abs((v1 ^ v2)[2]);
    } else
      _sin = 0; // we can't compute sin for extremities.
  }
  void setNext(int next) {
    set(_prev, next);
  }
  void setPrev(int prev) {
    set(prev, _next);
  }

  bool operator()(const CellCBuilder &a, const CellCBuilder &b) const {
    if (a._sin < b._sin)
      return true;
    if (a._sin > b._sin)
      return false;
    return a._id < b._id;
  }
};

std::vector<tlp::Coord> tlp::simplifyCurve(const std::vector<Coord> &in) {
  vector<CellCBuilder> cells;
  for (unsigned int i = 0; i < in.size(); ++i) {
    cells.push_back(CellCBuilder(&in, i, i + 1, i - 1));
  }
  set<CellCBuilder, CellCBuilder> order;
  for (unsigned int i = 1; i < in.size() - 1; ++i) {
    order.insert(cells[i]);
  }
  set<CellCBuilder, CellCBuilder>::iterator it;

  const unsigned int MAX_BENDS = 10;       // we remove bends until there is atmost MAX_BENDS
  const double MIN_DEV = sin(M_PI / 360.); // 0.5 degree, if there only MIN_DEV we remove the bend even if nbBends < MAX_BENDS
  const double MAX_DEV = sin(M_PI / 36.);  // 5 degree, if there is more than MAX_DEV degree wee keep the bend even if nbBends > MAX_BEND;

  while ((order.size() > 0) && ((order.begin()->_sin) < MAX_DEV) && ((order.size() > MAX_BENDS) || ((order.begin())->_sin < MIN_DEV))) {
    it = order.begin();
    int prev = it->_prev;
    int next = it->_next;
    order.erase(it);

    if (prev > 0) {
      order.erase(cells[prev]);
      cells[prev].setNext(next);
      order.insert(cells[prev]);
    }
    if (next < ((int)in.size()) - 1) {
      order.erase(cells[next]);
      cells[next].setPrev(prev);
      order.insert(cells[next]);
    }
  }

  std::vector<tlp::Coord> result;
  result.push_back(in[0]);
  for (unsigned int i = 1; i < in.size() - 1; ++i) {
    if (order.find(cells[i]) != order.end())
      result.push_back(in[i]);
  }
  result.push_back(in[in.size() - 1]);
  return result;
}

double tlp::degreeToRadian(double degree) {
  return degree * M_PI / 180.0;
}

double tlp::radianToDegree(double radian) {
  return radian * 180.0 / M_PI;
}
