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

class Color {
%TypeHeaderCode
#include <tulip/Color.h>
%End

%Docstring
This class represents a color using the RGBA color space. It can be instantiated the following way::
	
	# Each component must be in the range [0, 255]
	redOpaque = tlp.Color(255, 0, 0)
	
	blueTranslucent = tlp.Color(0, 0, 255, 100)

The value of each component can be read / written using the [] operator.	

%End

public:

	Color(const unsigned short red=0 ,const unsigned short green=0 ,const unsigned short blue=0, const unsigned short alpha=255);

	void set(const unsigned short red=0,const unsigned short green=0 ,const unsigned short blue=0, const unsigned short alpha=255);
%Docstring
tlp.Color.set(red, green, blue, alpha)

Sets each component of the color. Each component must be in the range [0, 255].

:param red: the red component value
:type red: integer
:param green: the green component value
:type green: integer
:param blue: the blue component value
:type blue: integer
:param alpha: the alpha component value
:type alpha: integer
%End

	float getRGL() const;
%Docstring
tlp.Color.getRGL()

Returns the red component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getGGL() const;
%Docstring
tlp.Color.getGGL()

Returns the green component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getBGL() const;
%Docstring
tlp.Color.getBGL()

Returns the blue component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getAGL() const;
%Docstring
tlp.Color.getAGL()

Returns the alpha component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	unsigned short getR() const;
%Docstring
tlp.Color.getR()

Returns the red component of the color.

:rtype: integer
%End

	unsigned short getG() const;
%Docstring
tlp.Color.getG()

Returns the green component of the color.

:rtype: integer
%End

	unsigned short getB() const;
%Docstring
tlp.Color.getB()

Returns the blue component of the color.

:rtype: integer
%End

	unsigned short getA() const;
%Docstring
tlp.Color.getA()

Returns the alpha component of the color.

:rtype: integer
%End

	void setR(const unsigned short red);
%Docstring
tlp.Color.setR(red)

Sets the red component of the color (must be in the range [0, 255]).

:param red: the red component value
:type red: integer
%End

	void setG(const unsigned short green);
%Docstring
tlp.Color.setG(green)

Sets the green component of the color (must be in the range [0, 255]).

:param green: the green component value
:type green: integer
%End

	void setB(const unsigned short blue);
%Docstring
tlp.Color.setB(blue)

Sets the blue component of the color (must be in the range [0, 255]).

:param blue: the blue component value
:type blue: integer
%End

	void setA(const unsigned short alpha);
%Docstring
tlp.Color.setA(alpha)

Sets the alpha component of the color (must be in the range [0, 255]).

:param alpha: the alpha component value
:type alpha: integer
%End

	int getH() const;
%Docstring
tlp.Color.getH()

Returns the hue component (in the range [0, 360]) of the color in the HSV color space.

:rtype: integer
%End

	int getS() const;
%Docstring
tlp.Color.getS()

Returns the saturation component (in the range [0, 255]) of the color in the HSV color space.

:rtype: integer
%End

	int getV() const;
%Docstring
tlp.Color.getH()

Returns the value component (in the range [0, 255]) of the color in the HSV color space.

:rtype: integer
%End

	void setH(int hue);
%Docstring
tlp.Color.setH(hue)

Sets the hue component of the color in the HSV color space (must be in the range [0, 360]).

:param hue: the hue component value
:type hue: integer

%End

	void setS(int saturation);
%Docstring
tlp.Color.setS(saturation)

Sets the saturation component of the color in the HSV color space (must be in the range [0, 255]).

:param saturation: the saturation component value
:type saturation: integer

%End

	void setV(int value);
%Docstring
tlp.Color.setV(value)

Sets the value component of the color in the HSV color space (must be in the range [0, 255]).

:param value: the value component value
:type value: integer

%End
	
	unsigned short operator[](const unsigned int i) const;
%MethodCode
  if (a0 < 4) {
    sipRes = (*sipCpp)[a0];
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	void __setitem__(int i, const unsigned short &value);
%MethodCode
  if (a0 < 4) {
		(*sipCpp)[a0] = a1;
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	bool operator!=(const tlp::Color &) const;
  bool operator==(const tlp::Color &) const;

	SIP_PYOBJECT __repr__() const;
%MethodCode
  std::ostringstream oss;
  oss << *sipCpp;
  std::string s = oss.str();
  s[0] = '[';
  s[s.size() - 1] = ']';
#if PY_MAJOR_VERSION >= 3
  sipRes = PyUnicode_FromString(s.c_str());
#else
  sipRes = PyString_FromString(s.c_str());
#endif
%End

  static const tlp::Color Amaranth;
  static const tlp::Color Amber;
  static const tlp::Color Apricot;
  static const tlp::Color Aquamarine;
  static const tlp::Color Azure;
  static const tlp::Color BabyBlue;
  static const tlp::Color Beige;
  static const tlp::Color Black;
  static const tlp::Color Blue;
  static const tlp::Color BlueGreen;
  static const tlp::Color BlueViolet;
  static const tlp::Color Blush;
  static const tlp::Color Bronze;
  static const tlp::Color Brown;
  static const tlp::Color Burgundy;
  static const tlp::Color Byzantium;
  static const tlp::Color Carmine;
  static const tlp::Color Cerise;
  static const tlp::Color Cerulean;
  static const tlp::Color Champagne;
  static const tlp::Color ChartreuseGreen;
  static const tlp::Color Chocolate;
  static const tlp::Color Coffee;
  static const tlp::Color Copper;
  static const tlp::Color Coral;
  static const tlp::Color Crimson;
  static const tlp::Color Cyan;
  static const tlp::Color DesertSand;
  static const tlp::Color ElectricBlue;
  static const tlp::Color Erin;
  static const tlp::Color Gold;
  static const tlp::Color Gray;
  static const tlp::Color Green;
  static const tlp::Color Harlequin;
  static const tlp::Color Indigo;
  static const tlp::Color Ivory;
  static const tlp::Color Jade;
  static const tlp::Color JungleGreen;
  static const tlp::Color Lavender;
  static const tlp::Color Lemon;
  static const tlp::Color Lilac;
  static const tlp::Color Lime;
  static const tlp::Color Magenta;
  static const tlp::Color MagentaRose;
  static const tlp::Color Maroon;
  static const tlp::Color Mauve;
  static const tlp::Color NavyBlue;
  static const tlp::Color Olive;
  static const tlp::Color Orange;
  static const tlp::Color OrangeRed;
  static const tlp::Color Orchid;
  static const tlp::Color Peach;
  static const tlp::Color Pear;
  static const tlp::Color Periwinkle;
  static const tlp::Color PersianBlue;
  static const tlp::Color Pink;
  static const tlp::Color Plum;
  static const tlp::Color PrussianBlue;
  static const tlp::Color Puce;
  static const tlp::Color Purple;
  static const tlp::Color Raspberry;
  static const tlp::Color Red;
  static const tlp::Color RedViolet;
  static const tlp::Color Rose;
  static const tlp::Color Salmon;
  static const tlp::Color Sapphire;
  static const tlp::Color Scarlet;
  static const tlp::Color Silver;
  static const tlp::Color SlateGray;
  static const tlp::Color SpringBud;
  static const tlp::Color SpringGreen;
  static const tlp::Color Tan;
  static const tlp::Color Taupe;
  static const tlp::Color Teal;
  static const tlp::Color Turquoise;
  static const tlp::Color Violet;
  static const tlp::Color Viridian;
  static const tlp::Color White;
  static const tlp::Color Yellow;
};
};
