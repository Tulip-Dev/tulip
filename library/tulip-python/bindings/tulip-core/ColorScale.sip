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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (http://booggie.org/)                                                   |
// +-------------------------------------------------------------------------+

namespace tlp {
 
class ColorScale {
%TypeHeaderCode
#include <tulip/ColorScale.h>
%End

public:

%Docstring
This class represents a color scale to perform color mapping.
The color scale can be either gradient (default) or predefined colors steps.
If the color scale is a gradient, returned colors are interpolated in function of the position.
If the color scale isn't a gradient returned colors are the predefined colors steps.

The sample code below illustrates how to create and manipulate a color scale::

    # Creating an empty color scale (default constructor creates a heatmap from blue to red).
    colorScale = tlp.ColorScale([])

    # Color scale initialization : from blue to red with gradient.
    colorScale.setColorAtPos(0.0, tlp.Color(0,0,255))
    colorScale.setColorAtPos(1.0, tlp.Color(255,0,0))

    # The following syntax could also be used
    # colors = [tlp.Color(0,0,255), tlp.Color(255,0,0)]
    # colorScale.setColorScale(colors)
    #    or
    # colorScale = tlp.ColorScale(colors)

    # Get the color for the position 0.5, i.e. Color(127,0,127).
    color = colorScale.getColorAtPos(0.5);

    # Reinitialize the color scale : from blue to red without gradient.
    newColors = []
    newColors.append(tlp.Color(0,0,255))
    newColors.append(tlp.Color(255,0,0))
    colorScale.setColorScale(newColors,False)

    # Get the color for the position 0.3, i.e. Color(0,0,255).
    color = colorScale.getColorAtPos(0.3)

    # Get the color for the position 0.7, i.e. Color(255,0,0).
    color = colorScale.getColorAtPos(0.7)
%End


  ColorScale(const bool gradient = true);
  ColorScale(const std::vector<tlp::Color> &colors, const bool gradient = true);
  ColorScale(const tlp::ColorScale& scale);
  virtual ~ColorScale();

  virtual void setColorScale(const std::vector<tlp::Color> colors, const bool gradient = true);
%Docstring
tlp.ColorScale.setColorScale(colors[, gradient = True])

This method configures the color scale. 
If the scale was previously configured the old configuration is lost.

:param colors: The colors to use in the color scale.
:type colors: list of :class:`tlp.Color`
:param gradient: If set to :const:`True`, the color scale is a gradient
:type gradient: boolean
%End

  virtual void setColorAtPos(const float pos, const tlp::Color &color);
%Docstring
tlp.ColorScale.setColorAtPos(pos, color)

This method adds a color to the color scale at a specific position.

:param pos: the position in the color scale (0.0 <= pos <= 1.0)
:type pos: float
:param color: the color to add at the specified position
:type color: :class:`tlp.Color`
%End  	
  	
  virtual tlp::Color getColorAtPos(const float pos) const;
%Docstring
tlp.ColorScale.getColorAtPos(pos)

This method computes the color associated to a specific position in the color scale and returns it.

:param pos: This value defines the position of the color in the scale and must be between 0.0 and 1.0 (it will be clamped otherwise).
:type pos: float
:rtype: :class:`tlp.Color`
%End	
	
  bool isGradient() const;
%Docstring
tlp.ColorScale.isGradient()

Returns :const:`True` if the color scale is a gradient.

:rtype: boolean 
%End  
  
  std::map<float, tlp::Color> getColorMap() const;
%Docstring
tlp.ColorScale.getColorMap()

Returns a dictionary corresponding to the color scale.

:rtype: a dictionary whose indexes are of type float and the values of type :class:`tlp.Color`
%End  
  
};
};
