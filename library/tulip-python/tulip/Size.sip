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
	void set(const tlp::Size&);
	void setW(const float width);
	void setH(const float height);
	void setD(const float depth);
	float getW() const;
	float getH() const;
	float getD() const;
	void get(float &width, float &height, float &depth) const;

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
