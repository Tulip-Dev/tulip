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

// +-------------------------------------------------------------------------+
// | Tulip Python Bindings                                                   |
// | inspired from bindings by the Booggie project development team          |
// | (https://sourceforge.net/projects/booggie/)                             |
// +-------------------------------------------------------------------------+

namespace tlp {
 
class ColorScale {
%TypeHeaderCode
#include <tulip/ColorScale.h>
%End

%ConvertToTypeCode

  PyObject *key = NULL;
  PyObject *val = NULL;
  Py_ssize_t pos = 0;

  if (sipIsErr == NULL) {
    if (sipCanConvertToType(sipPy, sipFindType("tlp::ColorScale"), SIP_NOT_NONE|SIP_NO_CONVERTORS)) {
      return 1;
    } else if (PyDict_Check(sipPy)) {
      while (PyDict_Next(sipPy, &pos, &key, &val)) {
        if (!PyNumber_Check(key)) {
          printErrorMessage("Error : when defining a Tulip color scale through a dictionnary, its keys must have a number type.");
          return 0;
        }
        double step = PyFloat_AsDouble(key);
        if (step < 0 || step > 1) {
          printErrorMessage("Error : color scale step must be must greater or equal than 0.0 and lesser or equal than 1.0.");
          return 0;
        }
        if (!sipCanConvertToType(val, sipFindType("tlp::Color"), SIP_NOT_NONE)) {
          printErrorMessage("Error : when defining a Tulip color scale through a dictionnary, its values must have the tlp.Color type.");
          return 0;
        }
      }
      return 1;
    } else if (PyList_Check(sipPy)) {
        for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
          PyObject *item = PyList_GET_ITEM(sipPy, i);
          if (!sipCanConvertToType(item, sipFindType("tlp::Color"), SIP_NOT_NONE)) {
            printErrorMessage("Error : when defining a Tulip color scale through a list, it must only contain instances of the tlp.Color type.");
            return 0;
          }
        }
        return 1;
    }
    return 0;
  }

  int state=0, err=0;

  if (PyDict_Check(sipPy)) {
    std::map<float, tlp::Color> colorMap;
    while (PyDict_Next(sipPy, &pos, &key, &val)) {
      double step = PyFloat_AsDouble(key);
      tlp::Color *color = static_cast<tlp::Color*>(sipConvertToType(val, sipFindType("tlp::Color"), Py_None, SIP_NOT_NONE, &state, &err));
      colorMap[step] = *color;
    }
    *sipCppPtr = new tlp::ColorScale();
    (*sipCppPtr)->setColorMap(colorMap);
  } else if (PyList_Check(sipPy)) {
    std::vector<tlp::Color> colors;
    for (SIP_SSIZE_T i = 0; i < PyList_GET_SIZE(sipPy); ++i) {
      PyObject *item = PyList_GET_ITEM(sipPy, i);
      tlp::Color *color = static_cast<tlp::Color*>(sipConvertToType(item, sipFindType("tlp::Color"), Py_None, SIP_NOT_NONE, &state, &err));
      colors.push_back(*color);
    }
    *sipCppPtr = new tlp::ColorScale(colors);
  } else {
    *sipCppPtr = new tlp::ColorScale(*static_cast<tlp::ColorScale*>(sipConvertToType(sipPy, sipFindType("tlp::ColorScale"), sipTransferObj, SIP_NOT_NONE|SIP_NO_CONVERTORS, &state, &err)));
  }

  if (!*sipCppPtr) {
    *sipIsErr = 1;
  }

  return sipGetState(sipTransferObj);

%End

public:

%Docstring
This class represents a color scale to perform color mapping.
The color scale can be either a gradient or defined by colors associated to consecutive intervals.
If the color scale is a gradient, returned colors are interpolated in function of a position between 0.0 and 1.0.
If the color scale is not a gradient returned colors are computed according to the interval the position belongs to.

The sample code below illustrates how to create and manipulate a color scale::

  # Creates an empty color scale (default constructor creates a heatmap from blue to red).
  colorScale = tlp.ColorScale([])
  
  # Color scale initialization : from blue to red with gradient.
  colorScale.setColorAtPos(0.0, tlp.Color.Blue)
  colorScale.setColorAtPos(1.0, tlp.Color.Red)

  # The following syntaxes could also be used
  # colors = [tlp.Color.Blue, tlp.Color.Red]
  # colorScale.setColorScale(colors)
  #    or
  # colorScale = tlp.ColorScale(colors)
  #
  # colorsMap = {0: tlp.Color.Blue, 1: tlp.Color.Red}
  # colorScale.setColorMap(colors)
  #   or
  # colorScale = tlp.ColorScale(colorsMap)

  # Gets the color for the position 0.5, i.e. tlp.Color(127,0,127).
  color = colorScale.getColorAtPos(0.5);

  # Reinitializes the color scale : from blue to red without gradient.
  newColors = [tlp.Color.Blue, tlp.Color.Red]
  colorScale.setColorScale(newColors,False)
  
  # Gets the color for the position 0.3, i.e. tlp.Color(0,0,255).
  color = colorScale.getColorAtPos(0.3)
  
  # Gets the color for the position 0.7, i.e. tlp.Color(255,0,0).
  color = colorScale.getColorAtPos(0.7)

Since Tulip 4.10, color scales can also be defined in a more Pythonic way through
the use of a list or a dictionnary. (see :ref:`No need to use the tlp.ColorScale class directly as plugin parameter anymore <colorScalesNewSyntax>`).

%End

//==========================================================================================

  ColorScale();
  ColorScale(const std::vector<tlp::Color> &colors, const bool gradient = true);
  ColorScale(const std::map<float, tlp::Color> &colors, const bool gradient = true);
  ColorScale(const tlp::ColorScale& scale);
  virtual ~ColorScale();

//==========================================================================================

  void clear();
%Docstring
tlp.ColorScale.clear()

.. versionadded:: 4.10

Clears the color scale.
%End

//==========================================================================================

  virtual void setColorScale(const std::vector<tlp::Color> colors, const bool gradient = true);
%Docstring
tlp.ColorScale.setColorScale(colors, gradient=True)

This method configures the color scale from a list of colors and
associates regular stop points to them.

.. warning:: If the scale was previously configured the old configuration is lost.

:param colors: The colors to use in the color scale.
:type colors: list of :class:`tlp.Color`
:param gradient: If set to :const:`True`, the color scale is a gradient
:type gradient: boolean
%End

//==========================================================================================

  virtual void setColorAtPos(const float pos, const tlp::Color &color);
%Docstring
tlp.ColorScale.setColorAtPos(pos, color)

This method adds a color to the color scale at a specific position.

:param pos: the position in the color scale (0.0 <= pos <= 1.0)
:type pos: float
:param color: the color to add at the specified position
:type color: :class:`tlp.Color`
%End

//==========================================================================================

  virtual tlp::Color getColorAtPos(const float pos) const;
%Docstring
tlp.ColorScale.getColorAtPos(pos)

This method computes the color associated to a specific position in the color scale and returns it.

:param pos: This value defines the position of the color in the scale and must be between 0.0 and 1.0 (it will be clamped otherwise).
:type pos: float
:rtype: :class:`tlp.Color`
%End

//==========================================================================================

  bool isGradient() const;
%Docstring
tlp.ColorScale.isGradient()

Returns :const:`True` if the color scale is a gradient.

:rtype: boolean 
%End  
  
//==========================================================================================

  void setGradient(const bool gradient);
%Docstring
tlp.ColorScale.setGradient(gradient)

Specify whether the color scale must be considered as a gradient.

:param gradient: the flag value
:type gradient: boolean
%End

//==========================================================================================

  std::map<float, tlp::Color> getColorMap() const;
%Docstring
tlp.ColorScale.getColorMap()

Returns a dictionary corresponding to the color scale.

:rtype: a dictionary whose indexes are of type float and the values of type :class:`tlp.Color`
%End  

//==========================================================================================

  bool hasRegularStops() const;
%Docstring
tlp.ColorScale.hasRegularStops()

.. versionadded:: 4.10

Returns :const:`True` if the color scale has regular stop points, meaning the distance between each consecutive stop is constant.

:rtype: boolean
%End

//==========================================================================================

  bool colorScaleInitialized() const;
%Docstring
tlp.ColorScale.colorScaleInitialized()

.. versionadded:: 4.10

Returns :const;`True` if the color scale is initialized, i.e. the underlying color map is not empty.

:rtype: boolean
%End

//==========================================================================================

  void setColorMap(const std::map<float, tlp::Color>& colorMap);
%Docstring
tlp.ColorScale.setColorMap(colorMap)

.. versionadded:: 4.10

Sets the map of stop points and colors used to perform color mapping.

.. warning:: The keys of the map must be between 0.0 and 1.0, other values will be ignored.

:param colorMap: the color map that defines the color scale
:type colorMap: a dictionnary with float keys and :class:`tlp.Color` values

%End

//==========================================================================================

  void setColorMapTransparency(unsigned char transparency);
%Docstring
tlp.ColorScale.setColorMapTransparency(alpha)

.. versionadded:: 4.10

Sets the transparency of all the colors in the underlying map.

:param alpha: the alpha value of each colors (0 <= alpha <= 255)
:type alpha: integer
%End

//==========================================================================================

  bool operator==(const tlp::ColorScale &) const;

  std::string __repr__() const;
%MethodCode
  std::ostringstream oss;
  std::vector<float> v;
  std::map<float, tlp::Color> colorMap = sipCpp->getColorMap();
  std::map<float, tlp::Color>::const_iterator it = colorMap.begin();

  for(; it != colorMap.end() ; ++it) {
    v.push_back(it->first);
  }

  std::sort(v.begin(), v.end());
  oss << "{";
  for (size_t i = 0 ; i < v.size() ; ++i) {
    oss << v[i] << ": " << colorMap[v[i]];
    if (i != v.size() - 1) {
      oss << ", ";
    }
  }
  oss << "}";
  sipRes = new std::string(oss.str());
%End

};
};
