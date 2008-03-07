//-*-c++-*-
/*
 * Author: Delorme Maxime
 * Email : Maxime.Delorme@gmail.com
 * Last modification : 08/07/2005 (fr) 
 * Author: Auber David
 * Last modification : 08/10/2006 (fr) 
 * Author: Mathiaut Morgan
 * Last modification : 15/11/2007
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation; either version 2 of the License, or     
 * (at your option) any later version.
*/

#ifndef _Tulip_GLGRID_H_
#define _Tulip_GLGRID_H_

#include <vector>

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/Size.h>

#include "tulip/GlSimpleEntity.h"

namespace tlp {
/** \brief General class used to render grids as augmented displays.
 * 
 * This class is a generic class to render grids as augmented displays.
 * It does not uses the basic parameters of the GlAugmentedDisplay(position and color).
 */
class TLP_GL_SCOPE GlGrid : public GlSimpleEntity {

 public:

  /**
   * Constructor
   */
  GlGrid() {}

  /**
   * Constructor
   *
   * \param frontTopLeft The minimum of the bounding box of the grid.
   * \param backBottomRight The maximum of the bounding box of the grid.
   * \param cell The size of a single cell of the grid.
   * \param color The color of the lines of the grid.
   * \param displays The dimensions of the grid to display.
   * \param hollowGrid Indicates if the grid chould be hollowed.
   */
  GlGrid(const Coord& frontTopLeft, const Coord& backBottomRight, 
	   const Size& cell, const Color& color, bool displays[3]);
  /**
   * Virtual function used to draw the grid.
   */
  virtual void draw(float lod,Camera *camera);
  /**
   * Accessor in reading to the dimensions to display.
   */
  void getDisplayDim(bool displayDim[3]) const;
  /**
   * Accessor in writing to the dimensions to display.
   */
  void setDisplayDim(bool displayDim[3]);

  /**
   * Translate entity
   */
  virtual void translate(const Coord& mouvement);

  /**
   * Get the data in XML form
   */
  void getXML(xmlNodePtr rootNode);
  
  /**
   * Set the data with XML
   */
  void setWithXML(xmlNodePtr rootNode);
  

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
