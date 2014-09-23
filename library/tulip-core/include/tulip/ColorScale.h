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


#ifndef COLORSCALE_H_
#define COLORSCALE_H_

#include <tulip/Observable.h>
#include <tulip/Color.h>

#include <vector>
#include <map>

namespace tlp {

/**
 * @brief This class represents a color scale to perform color mapping.
 * The color scale can be either gradient or predefined colors steps.
 * If the color scale is a gradient, returned colors are interpolated in function of the position.
 * If the color scale isn't a gradient returned colors are the predefined colors steps.
 * @code
 * \// Creating the color scale.
 * ColorScale colorScale;
 * \// Color scale initialization : from blue to red with gradient.
 * colorScale.setColorAtPos(0.0, Color(0,0,255));
 * colorScale.setColorAtPos(1.0, Color(255,0,0));
 *
 * \// Get the color for the position 0.5, i.e. Color(127,0,127).
 * colorScale.getColorAtPos(0.5);
 * \// Reinitialize the color scale : from blue to red without gradient.
 * vector<color> newColors;
 * newColors.push_back(Color(0,0,255));
 * newColors.push_back(Color(255,0,0));
 * colorScale.setColorScale(newColors,false);
 * \// Get the color for the position 0.3, i.e. Color(0,0,255).
 * colorScale.getColorAtPos(0.3);
 * \// Get the color for the position 0.7, i.e. Color(255,0,0).
 * colorScale.getColorAtPos(0.7);
 * @endcode
 */
class TLP_SCOPE ColorScale : public Observable {

public:

  /**
   * Initializes a color scale with a default set of colors.
   * @param gradient Specify if the color scale should be a gradient or not.
   */
  ColorScale(const bool gradient = true);

  /**
   * Initializes a color scale with a set of colors passed as parameter
   * @param colors a vector of colors defining the color scale (first color is at position 0, last color at position 1)
   * @param gradient Specify if the color scale should be a gradient or not.
   */
  ColorScale(const std::vector<Color> &colors, const bool gradient = true);


  ColorScale(const ColorScale& scale);
  ColorScale& operator=(const ColorScale& scale);
  virtual ~ColorScale();

  /**
    @brief Gets the number of stops into the color scale.
    */
  unsigned int getStopsCount() {
    return colorMap.size();
  }

  /**
   * @brief Configures the color scale.
   * This method configures the color scale. If the scale was previously configured the old configuration is lost.
   * @param colors The colors to use in the color scale.
   * @param gradient If set to true, color scale is a gradient
   */
  virtual void setColorScale(const std::vector<Color> colors, const bool gradient = true);

  /**
   * @brief Adds a color to the color scale at specific position.
   * This method adds a color to the color scale at a specific position
   * @param pos the position in the color scale (0.0 <= pos <= 1.0)
   * @param color the color to add at the specified position
   */
  virtual void setColorAtPos(const float pos, const Color &color);

  /**
   * @brief Returns the color for a given position in the color scale.
   * This method computes the color associated to a specific position in the color scale and returns it.
   * @param pos This value defines the position of the color in the scale and must be between 0.0 and 1.0 (it will be clamped otherwise).
   * @return The color corresponding to the position in the scale.
   */
  virtual Color getColorAtPos(const float pos) const;

  /**
   * @brief Returns true is the color scale was initialized.
   */
  bool colorScaleInitialized() const {
    return colorScaleSet;
  }
  /**
   * @brief Returns a map corresponding to the color scale.
   * The index of the map is the position for the corresponding color in the color scale. The index is comprised between 0 and 1.
   */
  std::map<float, Color> getColorMap() const {
    return colorMap;
  }
  /**
    * @brief Set the map to configure position to color mapping.
    * The values in the map must be between 0.0 and 1.0, other values will be ignored.
    *
    **/
  void setColorMap(const std::map<float, Color>& colorMap);
  /**
   * @brief Returns true if the color scale is a gradient.
   */
  bool isGradient() const {
    return gradient;
  }
  /**
    * @brief Set the transparency of all the colors in the map
    *
    **/
  void setColorMapTransparency(unsigned char transparency);

protected:

  std::map<float, Color> colorMap;
  bool gradient;
  bool colorScaleSet;

};

}

#endif /* COLORSCALE_H_ */
///@endcond
