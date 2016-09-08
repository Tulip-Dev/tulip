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

#ifndef GLCONCAVEPOLYGON_H
#define GLCONCAVEPOLYGON_H

#include <vector>
#include <map>
#include <set>

#include <tulip/Color.h>
#include <tulip/Coord.h>

#include <tulip/Camera.h>
#include <tulip/GlBuffer.h>
#include <tulip/GlEntity.h>

namespace tlp {

class TLP_GLES_SCOPE GlConcavePolygon : public GlEntity {

public:
  /**
   * @brief Default constructor
   * @warning don't use this constructor if you want to create a complex polygon, see others constructors
   */
  GlConcavePolygon() {}
  /**
   * @brief Constructor with a vector of coords, a fill color, and a textureName if you want
   */
  GlConcavePolygon(const std::vector<tlp::Coord> &coords,tlp::Color fcolor, const std::string &textureName = "");
  /**
   * @brief Constructor with a vector of coords, a fill color, an outline color, a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves) and a textureName if you want
   */
  GlConcavePolygon(const std::vector<tlp::Coord> &coords,tlp::Color fcolor,tlp::Color ocolor, const std::string &textureName = "");
  /**
   * @brief Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color, a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves) and a textureName if you want
   */
  GlConcavePolygon(const std::vector<std::vector<tlp::Coord> >&coords,tlp::Color fcolor, const std::string &textureName = "");
  /**
   * @brief Constructor with a vector of vector of coords (the first vector of coord is the polygon and others vectors are holes in polygon), a fill color, an outline color a polygon edges type(0 -> straight lines, 1 -> catmull rom curves, 2 -> bezier curves) and a textureName if you want
   */
  GlConcavePolygon(const std::vector<std::vector<tlp::Coord> >&coords,tlp::Color fcolor,tlp::Color ocolor, const std::string &textureName = "");

  ~GlConcavePolygon();

  /**
   * @brief Draw the complex polygon
   */
  void draw(const Camera &camera, const Light &, bool pickingMode=false);
  void draw(const Camera &camera, bool pickingMode=false);

  /**
   * @brief Set if the polygon is outlined or not
   */
  void setOutlineMode(const bool);

  /**
   * @brief Set size of outline
   */
  void setOutlineSize(double size);

  /**
   * @brief Get fill color of GlConcavePolygon
   */
  tlp::Color getFillColor() const {
    return _fillColor;
  }

  /**
   * @brief Set fill color of GlConcavePolygon
   */
  void setFillColor(const tlp::Color &color) {
    _fillColor=color;
  }

  /**
   * @brief Get outline color of GlConcavePolygon
   */
  tlp::Color getOutlineColor() const {
    return _outlineColor;
  }

  /**
   * @brief Set outline color of GlConcavePolygon
   */
  void setOutlineColor(const tlp::Color &color) {
    _outlineColor=color;
  }

  void setTexture(const std::string &texture) {
    _texture = texture;
  }

  /**
   * @brief Translate entity
   */
  virtual void translate(const tlp::Coord& mouvement);

  const std::vector<std::vector<tlp::Coord> > &getPolygonSides() const {
    return _points;
  }

protected:

  /**
   * @brief Add a new point in polygon
   */
  virtual void addPoint(const tlp::Coord& point);
  /**
   * @brief Begin a new hole in the polygon
   */
  virtual void beginNewHole();

  void runTesselation();
  void createPolygon(const std::vector<tlp::Coord> &coords);

  int _currentVector;
  bool _outlined;
  tlp::Color _fillColor;
  tlp::Color _outlineColor;
  double _outlineSize;

  std::vector<std::vector<tlp::Coord> > _points;
  std::vector<GlBuffer*> _pointsVBOs;

  GlBuffer *_polygonVerticesDataVBO;
  GlBuffer *_polygonIndicesVBO;
  unsigned int _nbIndices;

  std::string _texture;

};

}

#endif // GLCONCAVEPOLYGON_H
