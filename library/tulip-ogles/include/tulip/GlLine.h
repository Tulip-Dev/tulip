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

#ifndef GLLINE_H
#define GLLINE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/tulipconf.h>
#include <tulip/GlEntity.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * \brief This is a general class to render an OpenGL line
 * This class is used to render an OpenGL line
 */
class TLP_GLES_SCOPE GlLine : public GlEntity {
public:
  /**
   * Enpty constructor
   */
  GlLine() : _width(1.0) {
  }
  /**
   * Constructor with a list of coord and a list of color
   */
  GlLine(const std::vector<Coord> &points, const std::vector<Color> &colors);

  /**
   * Constructor with a list of coord and a color
   */
  GlLine(const std::vector<Coord> &points, const Color &color);

  virtual ~GlLine();

  /**
   * Render the line
   */
  virtual void draw(const Camera &camera, const Light &light, bool pickingMode = false);
  virtual void draw(const Camera &camera, bool pickingMode = false);

  /**
   * Change number of points
   */
  virtual void resizePoints(const unsigned int nbPoints);
  /**
   * Change number of colors
   */
  virtual void resizeColors(const unsigned int nbColors);

  virtual const Coord &point(const unsigned int i) const;
  /**
   * Return the coord of point with given id
   */
  virtual Coord &point(const unsigned int i);
  /**
   * Add a new point with given coord and given color
   */
  virtual void addPoint(const Coord &point, const Color &color);

  /**
   * return the ith color used for draw the line
   */
  virtual const Color &color(const unsigned int i) const;
  /**
   * return the ith color used for draw the line
   */
  virtual Color &color(const unsigned int i);
  /**
   * Set line width
   */
  virtual void setLineWidth(float width);

protected:
  std::vector<Coord> _points;
  std::vector<Color> _colors;
  float _width;
};
}

#endif
