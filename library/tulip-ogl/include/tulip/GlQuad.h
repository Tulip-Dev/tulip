/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#ifndef _GLQUAD_H
#define _GLQUAD_H

#include <tulip/Coord.h>
#include <tulip/Color.h>

#include "tulip/GlSimpleEntity.h"

#define N_QUAD_POINTS 4 /**< Constant indicating the number of points in a quad (stupid isn't it ? :D) */

namespace tlp {
/** \addtogroup GlEntities
 * \brief General class to render quads as augmented displays.
 *
 * This class is a generic class to render quads as augmented displays.
 * It doens't uses the basic parameters of the GlAugmentedDisplay(position and color)
 * but it's own set of positions and colors.
 */
class TLP_GL_SCOPE GlQuad : public GlSimpleEntity
{
 protected:
  Coord* positions[N_QUAD_POINTS]; /**< The position of the points of the Quad */
  Color* colors[N_QUAD_POINTS]; /**< The colors of the points of the Quad */
  std::string textureName;

  /**
   * Accessor in writing to the basic position of GlAugmentedDisplay
   *
   * \attention As you can see, this function is forbidden to the user to avoid him setting an unused parameter.
   */
  void setPosition(const Coord& position);

 public:

  /**
   * Constructor
   */
  GlQuad();

  /**
   * Constructor
   *
   * \param positions Array indicating the position of the points. The first point is the top-left one. The next points are winding in clockwise order.
   * \param color A single color indicating the color for every point of the Quad.
   */
  GlQuad(Coord positions[N_QUAD_POINTS], const Color &color);

  /**
   * Constructor
   * 
   * \param positions Array indicating the position of the points. The first point is the top-left one. The next points are winding in clockwise order.
   * \param colors Array indicating the color of the points. The order is the same as for the positions.
   */
  GlQuad(Coord positions[N_QUAD_POINTS], Color colors[N_QUAD_POINTS]);

  /**
   * Destructor
   */
  virtual ~GlQuad();

  /**
   * Virtual function used to draw the quad.
   */
  virtual void draw(float lod,Camera *camera);

  /**
   * Accessor in writing to the position.
   * \param idPosition Indicates which point we want to move.
   */
  void setPosition(int idPosition, const Coord &position);

  /**
   * Accessor in writing to the color.
   * \param idColor Indicates which point we want to colorize.
   */
  void setColor(int idColor, const Color &color);

  /**
   * Accessor in writing to the basic color of GlAugmentedDisplay
   *
   * \attention As you can see, this function is forbidden to the user to avoid him setting an unused parameter.
   */
  void setColor(const Color& color);

  /**
   * Accessor in reading to the position.
   * \param idPosition Indicates which point we want to get the position.
   */
  Coord* getPosition(int idPosition) const;

  /**
   * Accessor in reading to the color.
   * \param idColor Indicates which point we want to get the color.
   */
  Color* getColor(int idColor) const;

  /**
   * Accessor in reading to the texture.
   */
  void setTextureName(const std::string &name);

  /**
   * Accessor in reading to the texture.
   */
  std::string getTextureName() const;

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Function to export data in XML
   */
  virtual void getXML(xmlNodePtr rootNode);
  
  /**
   * Function to set data with XML
   */
  virtual void setWithXML(xmlNodePtr rootNode);
  
};
}
#endif
