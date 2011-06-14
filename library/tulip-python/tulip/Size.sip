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
class Size : tlp::Vec3f {
%TypeHeaderCode
#include <tulip/Size.h>
%End

%Docstring
Bases: :class:`tlp.Vec3f`

This class represents a size in a third dimensional space (width, height, depth). Constructors and operators 
are similar to those defined in its base class :class:`tlp.Vec3f`.
%End

public:

	Size(const float a0);
%MethodCode
		sipCpp = new tlp::Size();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a0;
		(*sipCpp)[2] = a0;
%End

	Size(const float width=0,const float height=0,const float depth=0);
	
	Size(const tlp::Size &);

	Size(const tlp::Vec3f &);

	void set(const float width=0,const float height=0,const float depth=0);
%Docstring
tlp.Size.set(width, height, depth)

Sets the three component of the size.

:param width: a value
:type width: float
:param height: a value
:type height: float
:param depth: a value
:type depth: float
%End	
	
	void set(const tlp::Size&);
%Docstring
tlp.Size.set(size)

Sets the three component of the size from another one.

:param size: a size
:type size: :class:`tlp.Size`
%End	
	
	void setW(const float width);
%Docstring
tlp.Size.setW(width)

Sets the width component of the size.

:param width: a value
:type width: float
%End	
	
	void setH(const float height);
%Docstring
tlp.Size.setH(height)

Sets the height component of the size.

:param height: a value
:type height: float
%End
	
	void setD(const float depth);
%Docstring
tlp.Size.setD(depth)

Sets the depth component of the size.

:param depth: a value
:type depth: float
%End
	
	float getW() const;
%Docstring
tlp.Size.getW()

Returns the width component of the size.

:rtype: float
%End
	
	float getH() const;
%Docstring
tlp.Size.getH()

Returns the height component of the size.

:rtype: float
%End
	
	float getD() const;
%Docstring
tlp.Size.getD()

Returns the depth component of the size.

:rtype: float
%End	
	
	void get(float &width /Out/, float &height /Out/, float &depth /Out/) const;
%Docstring
tlp.Size.get()

Returns the component of the size as a tuple.

:rtype: (float, float, float)
%End

	tlp::Size operator*(const  tlp::Size &) const;
	tlp::Size operator*(const float &) const;
	tlp::Size operator+(const  tlp::Size &) const;
	tlp::Size operator+(const float &) const;
	tlp::Size operator-(const  tlp::Size &) const;
	tlp::Size operator-(const float &) const; 
	tlp::Size operator/(const  tlp::Size &) const;
	tlp::Size operator/(const float &) const;
	tlp::Size operator^(const  tlp::Size &) const;

};
};
