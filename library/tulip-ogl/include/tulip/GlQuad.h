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

#ifndef _GLQUAD_H
#define _GLQUAD_H

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include <tulip/GlPolygon.h>

namespace tlp {
/**
 * @ingroup OpenGL
 * \brief Utility class to render a quadrilateral .
 *
 * This is an utility class class for rendering a quadrilateral.
 */
class TLP_GL_SCOPE GlQuad : public GlPolygon {

public:

  /**
   * Constructor
   */
  GlQuad();


  /**
   * Constructor
   *
   * Initializes a quadrilateral with 4 points winding in clockwise order and a single color.
   *
   * \param p1 the first quad vertex
   * \param p2 the second quad vertex
   * \param p3 the third quad vertex
   * \param p4 the fourth quad vertex
   * \param color A single color indicating the color for every point of the quadrilateral.
   */
  GlQuad(const Coord &p1, const Coord &p2, const Coord &p3, const Coord &p4,
         const Color &color);

  /* Constructor
   *
   * Initializes a quadrilateral with 4 points winding in clockwise order and a color attached to each point.
   *
   * \param p1 the first quad vertex
   * \param p2 the second quad vertex
   * \param p3 the third quad vertex
   * \param p4 the fourth quad vertex
   * \param c1 the color of the first quad vertex
   * \param c2 the color of the second quad vertex
   * \param c3 the color of the third quad vertex
   * \param c4 the color of the fourth quad vertex
   */
  GlQuad(const Coord &p1, const Coord &p2, const Coord &p3, const Coord &p4,
         const Color &c1, const Color &c2, const Color &c3, const Color &c4);

  /**
   * Destructor
   */
  virtual ~GlQuad();

  /**
   * Accessor in writing to the position.
   * \param idPosition Indicates which point we want to move.
   */
  void setPosition(const int idPosition, const Coord &position);

  /**
   * Accessor in writing to the color.
   * \param idColor Indicates which point we want to colorize.
   */
  void setColor(const int idColor, const Color &color);

  /**
   * Accessor in writing to the basic color of GlAugmentedDisplay
   */
  void setColor(const Color& color);

  /**
   * Accessor in reading to the position.
   * \param idPosition Indicates which point we want to get the position.
   */
  const Coord& getPosition(const int idPosition) const;

  /**
   * Accessor in reading to the color.
   * \param idColor Indicates which point we want to get the color.
   */
  const Color& getColor(const int idColor) const;

  /**
   * Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);


};

}
#endif
///@endcond
