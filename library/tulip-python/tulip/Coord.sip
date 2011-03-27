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
// | Original Bindings by the Booggie project developpment team              |
// | (http://booggie.org/)                                                   |
// | Improved and maintained by Antoine Lambert                              |
// | Contact:  antoine.lambert@labri.fr                                      |
// +-------------------------------------------------------------------------+
// | License:                                                                |
// |                                                                         |
// | Tulip Python Bindings is free software; you can redistribute it         |
// | and/or modify  it under the terms of the GNU General Public License     |
// | as published by the Free Software Foundation; either version 2 of the   |
// | License, or (at your option) any later version.                         |
// |                                                                         |
// | Tulip Python Bindings is distributed in the hope that it will be        |
// | useful, but WITHOUT ANY WARRANTY; without even the implied warranty of  |
// | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
// | GNU General Public License for more details.                            |
// |                                                                         |
// | You should have received a copy of the GNU General Public License       |
// | along with this program.  If not, see <http://www.gnu.org/licenses/>.   |
// |                                                                         |
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
