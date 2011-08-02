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

	float getRGL()const;
%Docstring
tlp.Color.getRGL()

Returns the red component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getGGL()const;
%Docstring
tlp.Color.getGGL()

Returns the green component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getBGL()const;
%Docstring
tlp.Color.getBGL()

Returns the blue component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	float getAGL()const;
%Docstring
tlp.Color.getAGL()

Returns the alpha component of the color in the OpenGL format (a float value between 0.0 and 1.0).

:rtype: float
%End

	unsigned short getR()const;
%Docstring
tlp.Color.getR()

Returns the red component of the color.

:rtype: integer
%End

	unsigned short getG()const;
%Docstring
tlp.Color.getG()

Returns the green component of the color.

:rtype: integer
%End

	unsigned short getB()const;
%Docstring
tlp.Color.getB()

Returns the blue component of the color.

:rtype: integer
%End

	unsigned short getA()const;
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

	void setH(int );
%Docstring
tlp.Color.setH(hue)

Sets the hue component of the color in the HSV color space (must be in the range [0, 360]).

:param hue: the hue component value
:type hue: integer

%End

	void setS(int );
%Docstring
tlp.Color.setS(saturation)

Sets the saturation component of the color in the HSV color space (must be in the range [0, 255]).

:param saturation: the saturation component value
:type saturation: integer

%End

	void setV(int );
%Docstring
tlp.Color.setV(value)

Sets the value component of the color in the HSV color space (must be in the range [0, 255]).

:param value: the value component value
:type value: integer

%End
	
	unsigned short operator[](const unsigned int i) const;
	
	void __setitem__(int i, const unsigned short &value);
%MethodCode
	if (a0 < 4)
		(*sipCpp)[a0] = a1;
	else
		sipIsErr = 1;	
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
		std::ostringstream oss;
		oss << *sipCpp;
		std::string s = oss.str();
		s[0] = '[';
		s[s.size() - 1] = ']';
        sipRes = PyString_FromString(s.c_str());
%End
};
};
