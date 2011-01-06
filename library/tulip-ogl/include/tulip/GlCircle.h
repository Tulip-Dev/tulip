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

#ifndef GLCIRCLE_H
#define GLCIRCLE_H

#include <tulip/Color.h>
#include <tulip/Coord.h>

#include "tulip/GlRegularPolygon.h"

namespace tlp {

  /**
   * \addtogroup GlEntities
   */
  /*@{*/
  /**
   * \brief General class used to render circles as GlEntity.
   *
   * This class is a generic class to render circles as GlEntity.
   */
  class TLP_GL_SCOPE GlCircle : public GlRegularPolygon {
  public:
    /**
     * Constructor
     */
    GlCircle(const Coord &center = Coord(0,0,0),
		float radius = 1.,
		const Color &outlineColor = Color(255, 0 , 0 , 255),
		const Color &fillColor = Color (0, 0, 255,255),
		bool filled = false,
		bool outlined = true,
		float startAngle = 0.0,
		unsigned int segments = 10);

    /**
     * Write-acess Accessor to the center, the radius and the startAngle of the circle.
     */
    void set(const Coord &center, float radius, float startAngle);

    /**
     * return the fill color of the circle (i parameter is not used)
     * This is an overload of the GlPolygon function to dismiss its behavior.
     */
    virtual const Color& fcolor(const unsigned int) const {return GlRegularPolygon::fcolor(0);}
    /**
     * return the fill color of the circle (i parameter is not used)
     * This is an overload of the GlPolygon function to dismiss its behavior.
     */
    virtual Color& fcolor(const unsigned int) {return GlRegularPolygon::fcolor(0);}

    /**
     * return the outline color of the circle (i parameter is not used)
     * This is an overload of the GlPolygon function to dismiss its behavior.
     */
    virtual const Color& ocolor(const unsigned int) const {return GlRegularPolygon::ocolor(0);}
    /**
     * return the outline color of the circle (i parameter is not used)
     * This is an overload of the GlPolygon function to dismiss its behavior.
     */
    virtual Color& ocolor(const unsigned int) {return GlRegularPolygon::ocolor(0);}

    /**
     * Function to export data in XML
     */
    virtual void getXML(xmlNodePtr rootNode);
  };
  /*@}*/
}

#endif
