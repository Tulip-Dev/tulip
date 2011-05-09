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

class BoundingBox {
%TypeHeaderCode
#include <tulip/BoundingBox.h>
%End

%Docstring
tlp.BoundingBox()
tlp.BoundingBox(tlp.Vec3f min, tlp.Vec3f max)
%End
     
      BoundingBox();
      
      BoundingBox(const tlp::Vec3f& min, const tlp::Vec3f& max);
      
      tlp::Vec3f center() const;
%Docstring
tlp.BoundingBox.center() -> tlp.Vec3f
%End
      void expand(const tlp::Vec3f& coord);
%Docstring
tlp.BoundingBox.expand(tlp.Vec3f)
%End    
      void translate(const tlp::Vec3f& vec);
%Docstring
tlp.BoundingBox.translate(tlp.Vec3f)
%End    
      bool isValid() const;
%Docstring
tlp.BoundingBox.isValid() -> bool
%End      
      tlp::Vec3f operator[](const unsigned int i) const;
	
      void __setitem__(int i, const tlp::Vec3f &value);
%MethodCode
	if (a0 < 2)
		(*sipCpp)[a0] = *a1;
	else
		sipIsErr = 1;	
%End

};
  
};
