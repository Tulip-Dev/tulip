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


#ifndef Tulip_GLCURVE_H
#define Tulip_GLCURVE_H

#include <tulip/GlSimpleEntity.h>

namespace tlp {
/** \brief This class is use to display an OpenGl curve
 *
 */
class TLP_GL_SCOPE GlCurve : public GlSimpleEntity {
public:
  /**
   * Basic constructor with vector of coord, begin/end color and begin/end size
   */
  GlCurve(const std::vector<tlp::Coord> &points,
          const Color &beginFColor,
          const Color &endFColor,
          const float &beginSize=0.,
          const float &endSize=0.);

  /**
   * Basic constructor with number of points
   */
  GlCurve(const unsigned int nbPoints = 3u);
  virtual ~GlCurve();

  /**
   * Draw the curve
   */
  void draw(float lod,Camera *camera);

  /**
   * Set the texture of the curve (if you want texture)
   */
  void setTexture(const std::string &texture);

  /**
   * Change the number of points
   */
  virtual void resizePoints(const unsigned int nbPoints);

  /**
   * Return the ith coord
   */
  virtual const tlp::Coord& point(const unsigned int i) const;
  /**
   * Return the ith coord
   */
  virtual tlp::Coord& point(const unsigned int i);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);

protected:
  std::vector<tlp::Coord> _points;
  Color _beginFillColor;
  Color _endFillColor;
  float _beginSize;
  float _endSize;
  std::string texture;
};
}
#endif
///@endcond
