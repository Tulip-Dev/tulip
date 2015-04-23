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

struct BoundingBox {
%TypeHeaderCode
#include <tulip/BoundingBox.h>
%End

%Docstring
This class represents the 3D bounding box of an object.
It is defined by two 3d points, the first one being the lowest point (bottom-left corner), 
the second being the highest (top-right corner).

A bounding box can be instantiated and manipulated the following ways::

	# Initialize a non-valid bounding box.
	# The minimum is (1, 1, 1) and the maximum is (-1, -1, -1).
	bb = tlp.BoundingBox()
	
	# Initialize a bounding box by specifying the lowest and highest point.
	# The validity of the bounding box is checked in debug mode (an assertion is raised if it is not valid). 
	bb2 = tlp.BoundingBox(tlp.Vec3f(0,0,0), tlp.Vec3f(10,10,10))
	
	# Reading / writing the bounding box components can be done through the [] operator
	min = bb2[0]
	max = bb2[1]
%End

  BoundingBox();
      
  BoundingBox(const tlp::Vec3f& min, const tlp::Vec3f& max);
      
  tlp::Vec3f center() const;
%Docstring
tlp.BoundingBox.center()

Returns the geometrical center of the bounding box.

:rtype: :class:`tlp.Vec3f` 
%End

  float width() const;
%Docstring
tlp.BoundingBox.width()

Returns the width of the bounding box.

:rtype: float
%End	  
	  
  float height() const;
%Docstring
tlp.BoundingBox.height()

Returns the height of the bounding box.

:rtype: float
%End

  float depth() const;
%Docstring
tlp.BoundingBox.depth()

Returns the depth of the bounding box.

:rtype: float
%End

  void expand(const tlp::Vec3f& point);
%Docstring
tlp.BoundingBox.expand(point)

Expands the bounding box to one containing the vector passed as parameter.
If the vector is inside the bounding box, it remains unchanged.

:param point: A point in the 3D space we want the bounding box to encompass.
:type point: :class:`tlp.Vec3f`
%End


  void translate(const tlp::Vec3f& displacement);
%Docstring
tlp.BoundingBox.translate(displacement)

Translates the bounding box by the displacement given by the vector passed as parameter.

:param displacement: The displacement vector in 3D space to translate this bounding box by.
:type displacement: :class:`tlp.Vec3f`
%End

  void scale(const tlp::Vec3f& factor);
%Docstring
tlp.BoundingBox.scale(factor)

.. versionadded:: 4.4

Scales the bounding box, i.e. multiplying its components by a factor vector.

:param factor: The factor vector to scale this bounding box by.
:type factor: :class:`tlp.Vec3f`
%End

  bool isValid() const;
%Docstring
tlp.BoundingBox.isValid()

Checks whether the bounding box's lowest point is less than it's highest point.
"Less Than" means axis-by-axis comparison, i.e. x1 < x2 && y1 < y2 && z1 < z2.

:rtype: boolean
%End      
      
  bool contains(const tlp::Vec3f& coord) const;
%Docstring
tlp.BoundingBox.contains(point)

.. versionadded:: 3.7

Checks if the given point is inside the current bounding box.
If the bounding box is invalid the result is always :const:`False`.

:param point: a point in the 3D space.
:type point: :class:`tlp.Vec3f`
:rtype: boolean
%End

  bool intersect(const tlp::BoundingBox &boundingBox) const;
%Docstring
tlp.BoundingBox.intersect(boundingBox)

.. versionadded:: 3.7

Checks if the given bounding box intersects the current one.
If one of the bounding boxes is invalid returns :const:`False`.

:param boundingBox: the bounding box on which to check intersection
:type boundingBox: :class:`tlp.BoundingBox`
:rtype: boolean
%End

  bool intersect(const tlp::Vec3f &segStart, const tlp::Vec3f &segEnd) const;
%Docstring
tlp.BoundingBox.intersect(segStart, segEnd)

.. versionadded:: 3.7

Checks if the bounding box intersects the given line segment.
If the bounding box is invalid the result is always :const:`False`.

:param segStart: the start point of the line segment on which to check intersection
:type segStart: :class:`tlp.Vec3f`
:param segEnd: the end point of the line segment on which to check intersection
:type segEnd: :class:`tlp.Vec3f`
:rtype: boolean
%End

  tlp::Vec3f& operator[](const unsigned int i) const /NoCopy/;
%MethodCode
  if (a0 < 2) {
    sipRes = &(*sipCpp)[a0];
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

  void __setitem__(int i, const tlp::Vec3f &value);
%MethodCode
  if (a0 < 2) {
		(*sipCpp)[a0] = *a1;
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode
  std::ostringstream oss;
  oss << *sipCpp;
  std::string s = oss.str();
  for (size_t i = 0 ; i < s.length() ; ++i) {
    if (s[i] == '(') {
      s[i] = '[';
    } else if (s[i] == ')') {
      s[i] = ']';
    }
  }
#if PY_MAJOR_VERSION >= 3
  sipRes = PyUnicode_FromString(s.c_str());
#else
  sipRes = PyString_FromString(s.c_str());
#endif
%End

};
  
};
