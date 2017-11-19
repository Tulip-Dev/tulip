/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef COLORSCALE_H_
#define COLORSCALE_H_

#include <tulip/Observable.h>
#include <tulip/Color.h>

#include <vector>
#include <map>

namespace tlp {

/**
 * @brief This class represents a color scale to perform color mapping.
 * The color scale can be either a gradient or defined by colors associated to consecutive
 * intervals.
 * If the color scale is a gradient, returned colors are interpolated in function of a position
 * between 0.0 and 1.0.
 * If the color scale is not a gradient returned colors are computed according to the interval the
 * position belongs to.
 * @code
 * // Creates the color scale.
 * tlp::ColorScale colorScale;
 * // Clears the color scale as the default constructor initializes a non empty one
 * colorScale.clear();
 * // Color scale initialization : from blue to red with gradient.
 * colorScale.setColorAtPos(0.0, tlp::Color::Blue);
 * colorScale.setColorAtPos(1.0, tlp::Color::Red);
 *
 * // Gets the color for the position 0.5, i.e. tlp::Color(127,0,127).
 * tlp::Color color = colorScale.getColorAtPos(0.5);
 * // Reinitializes the color scale : from blue to red without gradient.
 * std::vector<tlp::Color> newColors;
 * newColors.push_back(tlp::Color::Blue);
 * newColors.push_back(tlp::Color::Red);
 * colorScale.setColorScale(newColors, false);
 * // Gets the color for the position 0.3, i.e. tlp::Color(0,0,255).
 * color = colorScale.getColorAtPos(0.3);
 * // Gets the color for the position 0.7, i.e. tlp::Color(255,0,0).
 * color = colorScale.getColorAtPos(0.7);
 * @endcode
 *
 */
class TLP_SCOPE ColorScale : public Observable {

public:
  /**
   * Initializes a color scale with a default set of colors.
   *
   */
  ColorScale();

  /**
   * Initializes a color scale with a set of colors passed as parameter.
   * @param colors a vector of colors defining the color scale (first color is at position 0.0, last
   * color at position 1.0)
   * @param gradient specifies if the color scale should be a gradient or not
   *
   */
  ColorScale(const std::vector<Color> &colors, const bool gradient = true);

  /**
   * Initializes a color scale with a map of stop points and colors passed as parameter.
   * @since Tulip 4.10
   * @param colorMap a map of stop points and colors defining the color scale (The keys of the map
   * must be between 0.0 and 1.0, other ones will be ignored.)
   * @param gradient specifies if the color scale should be a gradient or not
   *
   */
  ColorScale(const std::map<float, Color> &colorMap, const bool gradient = true);

  ColorScale(const ColorScale &scale);

  ColorScale &operator=(const ColorScale &scale);

  ~ColorScale() override;

  /**
   * @brief Clears the color scale.
   *
   * @since Tulip 4.10
   *
   */
  void clear() {
    colorMap.clear();
  }

  /**
   * @brief Gets the number of stops points into the color scale.
   *
   */
  unsigned int getStopsCount() {
    return colorMap.size();
  }

  /**
   * @brief Configures the color scale with regular stop points.
   *
   * This method configures the color scale from a vector of colors and
   * associates regular stop points to them.
   *
   * @warning If the scale was already configured the previous configuration is lost.
   *
   * @param colors the colors to use in the color scale
   * @param gradient if set to true, color scale is a gradient
   *
   */
  virtual void setColorScale(const std::vector<Color> colors, const bool gradient = true);

  /**
   * @brief Adds a color to the color scale at specific position.
   *
   * This method adds a color to the color scale at a specific position.
   * @param pos the position in the color scale (0.0 <= pos <= 1.0)
   * @param color the color to add at the specified position
   *
   */
  virtual void setColorAtPos(const float pos, const Color &color);

  /**
   * @brief Returns the color for a given position in the color scale.
   *
   * This method computes the color associated to a specific position in the color scale and returns
   * it.
   * @param pos This value defines the position of the color in the scale and must be between 0.0
   * and 1.0 (it will be clamped otherwise)
   * @return the color corresponding to the position in the scale
   *
   */
  virtual Color getColorAtPos(const float pos) const;

  /**
   * @brief Returns true is the color scale was initialized.
   *
   */
  bool colorScaleInitialized() const {
    return !colorMap.empty();
  }

  /**
   * @brief Returns a map corresponding to the color scale.
   * The index of the map is the position for the corresponding color in the color scale. The index
   * is comprised between 0 and 1.
   *
   */
  std::map<float, Color> getColorMap() const {
    return colorMap;
  }

  /**
   * @brief Sets the map of stop points and colors used to perform color mapping.
   *
   * @warning The keys of the map must be between 0.0 and 1.0, other values will be ignored.
   *
   */
  void setColorMap(const std::map<float, Color> &colorMap);

  /**
   * @brief Returns true if the color scale is a gradient.
   *
   */
  bool isGradient() const {
    return gradient;
  }

  /**
   * @brief specify whether the color scale must be considered as a gradient
   */
  void setGradient(const bool g) {
    gradient = g;
  }

  /**
   * @brief Sets the transparency of all the colors in the underlying map.
   *
   */
  void setColorMapTransparency(unsigned char transparency);

  /**
   * @brief Tests color scale equality with another one.
   *
   */
  bool operator==(const ColorScale &cs) const {
    return (gradient == cs.gradient) && (colorMap == cs.colorMap);
  }

  /**
   * @brief Tests color scale equality with a regular one defined by a vector of colors.
   *
   */
  bool operator==(const std::vector<Color> &colors) const;

  /**
   * @brief Tests if the color scale has regular stop points, meaning the distance between each
   * consecutive stop is constant.
   *
   * @since Tulip 4.10
   *
   */
  bool hasRegularStops() const;

protected:
  std::map<float, Color> colorMap;
  bool gradient;
};
}

#endif /* COLORSCALE_H_ */
