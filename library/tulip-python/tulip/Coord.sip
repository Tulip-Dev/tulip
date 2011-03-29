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
	
	void set(const tlp::Coord&);
	
	void setX(float);
	
	void setY(float);
	
	void setZ(float);
	
	float getX() const;
	
	float getY() const;
	
	void get(float &, float &, float &) const;
	
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
