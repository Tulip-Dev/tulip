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

#ifndef SOMMAPELEMENT_H_
#define SOMMAPELEMENT_H_

#include <tulip/GlComposite.h>
#include <tulip/ColorProperty.h>
#include <tulip/ForEach.h>
#include <SOMMap.h>
/**
 * @brief GlComposite used to visualize a SOM.
 * GlComposite used to visualize a SOM. This object take a SOM and the the ColorProperty defining the color of each node of the map and print them.
 * If the SOM is hexagonal kind print hexagonal glyph else print sqaure.
 */
class SOMMapElement: public tlp::GlComposite {
public:
  SOMMapElement(tlp::Coord position, tlp::Size size, SOMMap *map,
                tlp::ColorProperty *colorProperty);
  virtual ~SOMMapElement();

  tlp::Coord getTopLeftPositionForElement(unsigned int x, unsigned int y);
  tlp::Size getNodeAreaSize();

  /**
   * Change the SOM and its color property.
   * @param map the new SOM
   * @param cp the new color property
   */
  void setData(SOMMap* map, tlp::ColorProperty* cp);


  /**
   * Change the colors of the som representation with those in the given property;
   * @param newColor The new colors.
   */
  void updateColors(tlp::ColorProperty *newColor);

protected:

  void computeNodeAreaSize();

  float computeMaximizedRadiusForHexagone(unsigned int width,
                                          unsigned int height, tlp::Size& size);

  void buildMainComposite(tlp::Coord topLeft, tlp::Size elementSize,
                          SOMMap* map);

  SOMMap* som;

  std::map<tlp::node, tlp::GlSimpleEntity*> nodesMap;

  tlp::Coord position;
  tlp::Size size;
  tlp::Size nodeAreaSize;
};
#endif /* SOMMAPELEMENT_H_ */
