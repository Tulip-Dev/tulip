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

%Include Vector.sip


namespace tlp {

class Coord : tlp::Vec3f {
%TypeHeaderCode
#include <tulip/Coord.h>
%End

%Docstring
Bases: :class:`tlp.Vec3f`

This class represents a point (coordinates) in a third dimensional space . Constructors and operators 
are similar to those defined in its base class :class:`tlp.Vec3f`.
%End

public:
	
	Coord();
	
	Coord(const float a0);
%MethodCode
		sipCpp = new tlp::Coord();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a0;
		(*sipCpp)[2] = a0;
%End
	 
	Coord(const float a0, const float a1, const float a2=0); 
	
	Coord(const tlp::Coord &);
	
	Coord(const tlp::Vec3f &);
		
	void set(const float=0,const float=0,const float=0);
%Docstring
tlp.Coord.set(x, y, z)

Sets each coordinate of the point.

:param x: a value
:type x: float
:param y: a value
:type y: float
:param z: a value
:type z: float
%End
	
	void set(const tlp::Coord&);
%Docstring
tlp.Coord.set(c)

Sets each coordinate of the point from another one.

:param c: a coordinate
:type c: :class:`tlp.Coord`
%End
	
	void setX(float);
%Docstring
tlp.Coord.setX(x)

Sets the x coordinate of the point.

:param x: a value
:type x: float
%End
	
	void setY(float);
%Docstring
tlp.Coord.setY(y)

Sets the y coordinate of the point.

:param y: a value
:type y: float
%End
	
	void setZ(float);
%Docstring
tlp.Coord.setZ(z)

Sets the z coordinate of the point.

:param z: a value
:type z: float
%End
	
	float getX() const;
%Docstring
tlp.Coord.getX()

Returns the x coordinate of the point.

:rtype: float
%End
	
	float getY() const;
%Docstring
tlp.Coord.getY()

Returns the y coordinate of the point.

:rtype: float
%End
	
	float getZ() const;
%Docstring
tlp.Coord.getZ()

Returns the z coordinate of the point.

:rtype: float
%End
	
	void get(float & /Out/, float & /Out/, float & /Out/) const;
%Docstring
tlp.Coord.get()

Returns the point as a tuple.

:rtype: (float, float, float)
%End
		
	tlp::Coord operator*(const  tlp::Coord &) const;
	tlp::Coord operator*(const float &) const;
	tlp::Coord operator+(const  tlp::Coord &) const;
	tlp::Coord operator+(const float &) const;
	tlp::Coord operator-(const  tlp::Coord &) const;
	tlp::Coord operator-(const float &) const; 
	tlp::Coord operator/(const  tlp::Coord &) const;
	tlp::Coord operator/(const float &) const;
	tlp::Coord operator^(const  tlp::Coord &) const;
		
	
};
};
