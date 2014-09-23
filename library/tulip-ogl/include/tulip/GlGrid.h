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


#ifndef _Tulip_GLGRID_H_
#define _Tulip_GLGRID_H_

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

#include <tulip/GlSimpleEntity.h>

namespace tlp {
/**
 * @ingroup OpenGL
 * @brief General class used to render grids as GlSimpleEntity.
 *
 */
class TLP_GL_SCOPE GlGrid : public GlSimpleEntity {

public:

  /**
   * @brief Constructor
   * @warning Don't use this constructor : see other constructor
   */
  GlGrid() {}

  /**
   * @brief Constructor
   *
   * @param frontTopLeft The minimum of the bounding box of the grid.
   * @param backBottomRight The maximum of the bounding box of the grid.
   * @param cell The size of a single cell of the grid.
   * @param color The color of the lines of the grid.
   * @param displays The dimensions of the grid to display.
   * @param hollowGrid Indicates if the grid chould be hollowed.
   */
  GlGrid(const Coord& frontTopLeft, const Coord& backBottomRight,
         const Size& cell, const Color& color, bool displays[3]);
  /**
   * @brief Virtual function used to draw the grid.
   */
  virtual void draw(float lod,Camera *camera);
  /**
   * @brief Accessor in reading to the dimensions to display.
   */
  void getDisplayDim(bool displayDim[3]) const;
  /**
   * @brief Accessor in writing to the dimensions to display.
   */
  void setDisplayDim(bool displayDim[3]);

  /**
   * @brief Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * @brief Function to export data in outString (in XML format)
   */
  virtual void getXML(std::string &outString);

  /**
   * @brief Function to set data with inString (in XML format)
   */
  virtual void setWithXML(const std::string &inString, unsigned int &currentPosition);


protected:
  bool displayDim[3]; /**< Which dimensions should be displayed ? Note that only one dimension at most should be disabled*/
  bool hollowGrid; /**< Should the grid be hollowed ?*/
  Coord frontTopLeft; /**< Front top left point of the grid, the "minimum" */
  Coord backBottomRight; /**< Back bottom right point of the grid, the "maximum" */
  Color color; /**< The color of the grid */
  Size cell; /**< The size of a cell of the grid */
};

}
#endif
