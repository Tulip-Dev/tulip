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

#ifndef SOMMAPELEMENT_H_
#define SOMMAPELEMENT_H_

#include <tulip/GlComposite.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Node.h>

/**
 * @brief GlComposite used to visualize a SOM.
 * GlComposite used to visualize a SOM. This object take a SOM and the the ColorProperty defining the color of each node of the map and print them.
 * If the SOM is hexagonal kind print hexagonal glyph else print sqaure.
 */
namespace tlp {

class SOMMap;
class ColorProperty;

class SOMMapElement: public tlp::GlComposite {
public:
  SOMMapElement(Coord position, Size size, SOMMap *map,
                ColorProperty *colorProperty);
  virtual ~SOMMapElement();

  tlp::Coord getTopLeftPositionForElement(unsigned int x, unsigned int y);
  tlp::Size getNodeAreaSize();

  /**
   * Change the SOM and its color property.
   * @param map the new SOM
   * @param cp the new color property
   */
  void setData(SOMMap* map, ColorProperty* cp);


  /**
   * Change the colors of the som representation with those in the given property;
   * @param newColor The new colors.
   */
  void updateColors(ColorProperty *newColor);

protected:

  void computeNodeAreaSize();

  float computeMaximizedRadiusForHexagone(unsigned int width,
                                          unsigned int height, Size& size);

  void buildMainComposite(Coord topLeft, Size elementSize,
                          SOMMap* map);

  SOMMap* som;

  std::map<node, GlSimpleEntity*> nodesMap;

  Coord position;
  Size size;
  Size nodeAreaSize;
};
}
#endif /* SOMMAPELEMENT_H_ */
