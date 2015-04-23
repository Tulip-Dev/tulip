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

#ifndef SOMPREVIEWCOMPOSITE_H_
#define SOMPREVIEWCOMPOSITE_H_

#include <tulip/GlComposite.h>
#include <tulip/GlLabel.h>
#include <tulip/GlRect.h>
#include "GlLabelledColorScale.h"
#include <SOMMap.h>
#include "SOMMapElement.h"
/**
 * @brief GlComposite used to print SOM preview. Use a SOMMapElement to print the SOM and add other information like property name or color scale and min and max value.
 */
namespace tlp {

class SOMPreviewComposite: public GlComposite {
public:
  /**
    *@brief Constructor for the SOMPreviewComposite.
    * @param position The bottom right corner of the preview.
    * @param size The size of the composite
    * @param propertyName The name of the displayed properties.
    * @param colorProperty The property containing colors for each element in the grid.
    * @param map The SOM grid.
    * @param colorScale The color scale to use in the preview.
    * @param minValue The minium value label.
    * @param maxValue The maximum value label.
    **/
  SOMPreviewComposite(tlp::Coord position, tlp::Size size, const std::string& propertyName, tlp::ColorProperty* colorProperty, SOMMap *map,
                      tlp::ColorScale* colorScale, double minValue, double maxValue);
  virtual ~SOMPreviewComposite();

  inline std::string getPropertyName() const {
    return propertyName;
  }

  /**
   * Change the color of the frame.
   * @param color the new color.
   */
  void setFrameColor(tlp::Color color);

  /**
   * Change the colors of the som representation with those in the given property;
   * @param newColor The new colors.
   */
  void updateColors(ColorProperty *newColor);

  /**
   * @brief Test is the entity is an internal element of the SOM.
   */
  bool isElement(GlEntity* entity);


protected:

  tlp::Size computeAspectRatio(unsigned int width, unsigned int height, float maxWidth, float maxHeight);

  void buildMainComposite(const std::string& colorPropertyName, SOMMap* map, tlp::Coord topLeft, tlp::Size elementSize);

  tlp::GlLabel *label;
  tlp::GlRect *frame;
  SOMMapElement *mapComposite;
  std::string propertyName;
  GlLabelledColorScale *lColorScale;
  tlp::Coord currentPosition;

};
}
#endif /* SOMPREVIEWCOMPOSITE_H_ */
