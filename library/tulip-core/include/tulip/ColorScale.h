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

#ifndef COLORSCALE_H_
#define COLORSCALE_H_

#include <tulip/Observable.h>
#include <tulip/Color.h>

#include <vector>
#include <map>

namespace tlp {

/**
 * @brief Use this class to create a color scale and perform color mapping.
 * The color scale can be either gradient or predefined colors steps.
 * If the color scale is a gradient, returned color are interpolated in function of the position.
 * If the color scale isn't a gradient returned colors are the predefined colors steps.
 * @code
 * \// Creating the color scale.
 * ColorScale colorScale;
 * \// Color scale initialization : from blue to red with gradient.
 * vector<color> colors;
 * colors.push_back(Color(0,0,255));
 * colors.push_back(Color(255,0,0));
 * colorScale.setColorScale(colors,true);
 *
 * \//Get the color for the position 0.5 eq Color(127,0,127).
 * colorScale.getColorAtPos(0.5);
 * \//Reinit the color scale : from blue to red without gradient.
 * vector<color> newColors;
 * newColors.push_back(Color(0,0,255));
 * newColors.push_back(Color(255,0,0));
 * colorScale.setColorScale(colors,false);
 * \//Get the color for the position 0.3 eq Color(0,0,255).
 * colorScale.getColorAtPos(0.3);
 * \//Get the color for the position 0.7 eq Color(255,0,0).
 * colorScale.getColorAtPos(0.7);
 * @endcode
 */
class TLP_SCOPE ColorScale : public Observable {

public:

	ColorScale(const bool gradient = true);
	ColorScale(const std::vector<Color> &colors, const bool gradient = true);
	ColorScale(const ColorScale& scale);
        ColorScale& operator=(const ColorScale& scale);
	virtual ~ColorScale();

	/**
	 * @brief Configure the color scale.
	 * This method is used to configure the color scale. If the scale was previously configured the old configuration is lost.
	 * @param colors The colors to use in the color scale.
	 * @param gradient If set to true, color scale is a gradient
	 */
	virtual void setColorScale(const std::vector<Color> colors, const bool gradient = true);

	/**
	 * @brief Add a color to color scale at specific position.
	 * This method is used to add a color to the color scale at a specific position
	 * @param pos the position in the color scale (0 <= pos <= 1)
	 * @param gradient If set to true, color scale is a gradient
	 */
	virtual void setColorAtPos(const float pos, const Color &color);

	/**
	 * @brief Compute the color for the given value.
	 * @param pos This value defines the position of the color in the scale and must be between 0 and 1.0.
	 * @return The color corresponding to the position in the scale if the position is beetween 0 and 1.0. If the position is smaller than 0 the color corresponding to the position 0 is returned and if the position is greater than 1.0 the color corresponding to the position 1.0 is returned.
	 */
	virtual Color getColorAtPos(const float pos) const;

	/**
	 * @brief Return true is the color scale was initialized.
	 */
	bool colorScaleInitialized() const {return colorScaleSet;}
	/**
	 * @brief Return a map corresponding to the color scale.
	 * The index of the map is the position for the corresponding color in the color scale.
	 */
	std::map<float, Color> getColorMap() const {return colorMap;}
	/**
	 * @brief Return true if the color scale is a gradient.
	 */
	bool isGradient() const {return gradient;}

protected:

	std::map<float, Color> colorMap;
	bool gradient;
	bool colorScaleSet;

};

}

#endif /* COLORSCALE_H_ */
