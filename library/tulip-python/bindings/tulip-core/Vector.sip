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

%ModuleHeaderCode
#include <tulip/Vector.h>
#include <sstream>
%End

namespace tlp {
class Vec3f  {

%Docstring
This class represents a vector with 3 floating point components.
It can be instantiated as illustrated in the code below: ::

	# Initializes a Vec3f with all components set to 0.0
	v = tlp.Vec3f()
	
	# Initializes a Vec3f with all components set to 1.0
	v2 = tlp.Vec3f(1.0)
	
	# Initializes a Vec3f by specifying the value of each component
	v3 = tlp.Vec3f(1.0, 2.0, 3.0) 
	
	# Initializes a Vec3f by copy
	v4 = tlp.Vec3f(v3)

Numerous mathematical operations are available to work with vectors. The sample code below illustrates them: ::

	# Instantiate two vectors
	v = tlp.Vec3f(4.5, 1.0, 3.0)
	v2 = tlp.Vec3f(1.5, 2.0, 6.0)
	
	# Add two vectors, operator += is also available
	v3 = v + v2
	
	# Add a value to all components of a vector, operator += is also available
	v4 = v + 2.0
	
	# Subtract two vectors, operator -= is also available
	v5 = v - v2
	
	# Subtract a value to all components of a vector, operator -= is also available
	v6 = v - 3.0
	
	# Multiply two vectors (not vector product), operator *= is also available
	v7 = v * v2
	
	# Multiply each component of a vector by a value, operator *= is also available
	v8 = v * 2.0
	
	# Divide two vectors, operator /= is also available
	v9 = v / v2
	
	# Divide each component of a vector by a value, operator /= is also available
	v10 = v / 2.0
	
	# Perform a vector product, operator ^= is also available
	v11 = v ^ v2

Each component of the vector can be read / written through the [] operator: ::

	v = tlp.Vec3f(1.0, 2.0, 3.0)
	
	# read first component
	a = v[0]
	
	# write third component
	v[2] = 4.0
	
Operators for vectors equality (==), vectors difference (!=) and vectors comparison (<, >) are also available.
	
%End

public:

	Vec3f();
	
	Vec3f(const float a0);
	
  Vec3f(const float a0, const float a1, const float a2=0);
	
	Vec3f(const tlp::Vec3f &);
	
	float operator[](const unsigned int i) const;
%MethodCode
  if (a0 < 3) {
    sipRes = (*sipCpp)[a0];
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	void __setitem__(int i, const float &value);
%MethodCode
  if (a0 < 3) {
		(*sipCpp)[a0] = a1;
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End
	
	tlp::Vec3f & operator*=(const float &);
  tlp::Vec3f & operator*=(const tlp::Vec3f &);
  tlp::Vec3f & operator/=(const float &);
  tlp::Vec3f & operator/=(const tlp::Vec3f &);
  tlp::Vec3f & operator+=(const float &);
  tlp::Vec3f & operator+=(const tlp::Vec3f &);
  tlp::Vec3f & operator-=(const float &);
  tlp::Vec3f & operator-=(const tlp::Vec3f &);
  tlp::Vec3f & operator^=(const tlp::Vec3f &);

	tlp::Vec3f operator*(const  tlp::Vec3f &) const;
	tlp::Vec3f operator*(const float &) const;
	tlp::Vec3f operator+(const  tlp::Vec3f &) const;
	tlp::Vec3f operator+(const float &) const;
	tlp::Vec3f operator-(const  tlp::Vec3f &) const;
	tlp::Vec3f operator-(const float &) const; 
	tlp::Vec3f operator/(const  tlp::Vec3f &) const;
	tlp::Vec3f operator/(const float &) const;
	tlp::Vec3f operator^(const  tlp::Vec3f &) const;
    
  bool operator>(const tlp::Vec3f &) const;
  bool operator<(const tlp::Vec3f &) const;
  bool operator!=(const tlp::Vec3f &) const;
  bool operator==(const tlp::Vec3f &) const;
    
  void set(const float x=0,const float y=0, const float z=0);
%Docstring
tlp.Vec3f.set(x, y, z)

Sets each component of the vector.

:param x: the first component value
:type x: float
:param y: the second componant value
:type y: float
:param z: the third component value
:type z: float
%End

  void set(const tlp::Vec3f& v);
%Docstring
tlp.Vec3f.set(v)

Sets each component of the vector from another one.

:param v: a vector
:type c: :class:`tlp.Vec3f`
%End

  void setX(float x);
%Docstring
tlp.Vec3f.setX(x)

Convenient method. Sets the first component of the vector.

:param x: a value
:type x: float
%End

  void setY(float y);
%Docstring
tlp.Vec3f.setY(y)

Convenient method. Sets the second component of the vector.

:param y: a value
:type y: float
%End

  void setZ(float z);
%Docstring
tlp.Vec3f.setZ(z)

Convenient method. Sets the third component of the vector.

:param z: a value
:type z: float
%End

  float getX() const;
%Docstring
tlp.Vec3f.getX()

Convenient method. Returns the first component of the vector.

:rtype: float
%End

  float getY() const;
%Docstring
tlp.Vec3f.getY()

Convenient method. Returns the second component of the vector.

:rtype: float
%End

  float getZ() const;
%Docstring
tlp.Vec3f.getZ()

Convenient method. Returns the third component of the vector.

:rtype: float
%End

  float x() const;
%Docstring
tlp.Vec3f.x()

Convenient method. Returns the first component of the vector.

:rtype: float
%End

  float y() const;
%Docstring
tlp.Vec3f.y()

Convenient method. Returns the second component of the vector.

:rtype: float
%End

  float z() const;
%Docstring
tlp.Vec3f.z()

Convenient method. Returns the third component of the vector.

:rtype: float
%End

  void setW(const float width);
%Docstring
tlp.Vec3f.setW(width)

Convenient method. Sets the first component of the vector.

:param width: a value
:type width: float
%End

  void setH(const float height);
%Docstring
tlp.Vec3f.setH(height)

Convenient method. Sets the second component of the vector.

:param height: a value
:type height: float
%End

  void setD(const float depth);
%Docstring
tlp.Vec3f.setD(depth)

Convenient method. Sets the third component of the vector.

:param depth: a value
:type depth: float
%End

  float getW() const;
%Docstring
tlp.Vec3f.getW()

Convenient method. Returns the first component of the vector.

:rtype: float
%End

  float getH() const;
%Docstring
tlp.Vec3f.getH()

Convenient method. Returns the second component of the vector.

:rtype: float
%End

  float getD() const;
%Docstring
tlp.Vec3f.getD()

Convenient method. Returns the third component of the vector.

:rtype: float
%End

  float width() const;
%Docstring
tlp.Vec3f.width()

Convenient method. Returns the first component of the vector.

:rtype: float
%End

  float height() const;
%Docstring
tlp.Vec3f.height()

Convenient method. Returns the second component of the vector.

:rtype: float
%End

  float depth() const;
%Docstring
tlp.Vec3f.depth()

Convenient method. Returns the third component of the vector.

:rtype: float
%End

  void get(float & /Out/, float & /Out/, float & /Out/) const;
%Docstring
tlp.Vec3f.get()

Returns the vector as a tuple.

:rtype: (float, float, float)
%End

  tlp::Vec3f & fill(const float &obj);
%Docstring
tlp.Vec3f.fill(val)

Set the same value to each component of the vector.

:param val: a value
:type val: float
%End
    
  float norm () const;
%Docstring
tlp.Vec3f.norm()

Computes and returns the euclidean norm of the vector.

:rtype: float
%End    
    
  float dist (const tlp::Vec3f &) const;
%Docstring
tlp.Vec3f.dist(v)

Computes and returns the distance between the vector (3d point) and another one.

:param v: a vector
:type v: :class:`tlp.Vec3f`
:rtype: float
%End    
    
  float dotProduct(const tlp::Vec3f &) const;
%Docstring
tlp.Vec3f.dotProduct(v)

Computes and returns the dot product (scalar product) of the vector with another one

:param v: a vector
:type v: :class:`tlp.Vec3f`
:rtype: float
%End
   
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

};

tlp::Vec3f operator*(const float &, const tlp::Vec3f &) const;

class Vec4f  {

%Docstring
This class represents a vector with 4 floating point components.
Constructors, operators and methods are similar to those from the :class:`tlp.Vec3f` class. 
%End

public:

	Vec4f();
	
	Vec4f(const float a0);

  Vec4f(const float a0, const float a1);

  Vec4f(const float a0, const float a1, const float a2);

	Vec4f(const float a0, const float a1, const float a2, const float a3);
	
	Vec4f(const tlp::Vec4f &);
	
	float operator[](const unsigned int i) const;
%MethodCode
  if (a0 < 4) {
    sipRes = (*sipCpp)[a0];
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	void __setitem__(int i, const float &value);
%MethodCode
  if (a0 < 4) {
		(*sipCpp)[a0] = a1;
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End
	
	tlp::Vec4f & operator*=(const float &);
  tlp::Vec4f & operator*=(const tlp::Vec4f &);
  tlp::Vec4f & operator/=(const float &);
  tlp::Vec4f & operator/=(const tlp::Vec4f &);
  tlp::Vec4f & operator+=(const float &);
  tlp::Vec4f & operator+=(const tlp::Vec4f &);
  tlp::Vec4f & operator-=(const float &);
  tlp::Vec4f & operator-=(const tlp::Vec4f &);
  tlp::Vec4f & operator^=(const tlp::Vec4f &);

	tlp::Vec4f operator*(const  tlp::Vec4f &) const;
	tlp::Vec4f operator*(const tlp::Mat4f &mat) const;
	tlp::Vec4f operator*(const float &) const;
	tlp::Vec4f operator+(const  tlp::Vec4f &) const;
	tlp::Vec4f operator+(const float &) const;
	tlp::Vec4f operator-(const  tlp::Vec4f &) const;
	tlp::Vec4f operator-(const float &) const; 
	tlp::Vec4f operator/(const  tlp::Vec4f &) const;
	tlp::Vec4f operator/(const float &) const;
	tlp::Vec4f operator^(const  tlp::Vec4f &) const;
    
  bool operator>(const tlp::Vec4f &) const;
  bool operator<(const tlp::Vec4f &) const;
  bool operator!=(const tlp::Vec4f &) const;
  bool operator==(const tlp::Vec4f &) const;
    
  void set(const float x=0,const float y=0, const float z=0, const float w=0);

  void set(const tlp::Vec4f& v);

  void setX(float x);

  void setY(float y);

  void setZ(float z);

  float getX() const;

  float getY() const;

  float getZ() const;

  float x() const;

  float y() const;

  float z() const;

  float w() const;
%Docstring
tlp.Vec4f.w()

Convenient method. Returns the fourth component of the vector.

:rtype: float
%End

  void setW(const float width);

  void setH(const float height);

  void setD(const float depth);

  float getW() const;

  float getH() const;

  float getD() const;

  float width() const;

  float height() const;

  float depth() const;

  void get(float & /Out/, float & /Out/, float & /Out/, float & /Out/) const;

  tlp::Vec4f & fill(const float &obj);
  float norm () const;
  float dist (const tlp::Vec4f &) const;
  float dotProduct(const tlp::Vec4f &) const;
   
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

};

tlp::Vec4f operator*(const float &, const tlp::Vec4f &) const;

class Vec4i  {

%Docstring
This class represents a vector with 4 integer components.
Constructors, operators and methods are similar to those from the :class:`tlp.Vec3f` class. 
%End

public:

	Vec4i();
	
	Vec4i(const int a0);
	
  Vec4i(const int a0, const int a1);

  Vec4i(const int a0, const int a1, const int a2);

	Vec4i(const int a0, const int a1, const int a2, const int a3);
	
	Vec4i(const tlp::Vec4i &);
	
	int operator[](const unsigned int i) const;
%MethodCode
  if (a0 < 4) {
    sipRes = (*sipCpp)[a0];
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End

	void __setitem__(int i, const int &value);
%MethodCode
  if (a0 < 4) {
		(*sipCpp)[a0] = a1;
  } else {
    sipIsErr = -1;
    PyErr_SetNone(PyExc_IndexError);
  }
%End
	
	tlp::Vec4i & operator*=(const int &);
  tlp::Vec4i & operator*=(const tlp::Vec4i &);
  tlp::Vec4i & operator/=(const int &);
  tlp::Vec4i & operator/=(const tlp::Vec4i &);
  tlp::Vec4i & operator+=(const int &);
  tlp::Vec4i & operator+=(const tlp::Vec4i &);
  tlp::Vec4i & operator-=(const int &);
  tlp::Vec4i & operator-=(const tlp::Vec4i &);
  tlp::Vec4i & operator^=(const tlp::Vec4i &);

	tlp::Vec4i operator*(const  tlp::Vec4i &) const;
	tlp::Vec4i operator*(const int &) const;
	tlp::Vec4i operator+(const  tlp::Vec4i &) const;
	tlp::Vec4i operator+(const int &) const;
	tlp::Vec4i operator-(const  tlp::Vec4i &) const;
	tlp::Vec4i operator-(const int &) const; 
	tlp::Vec4i operator/(const  tlp::Vec4i &) const;
	tlp::Vec4i operator/(const int &) const;
	tlp::Vec4i operator^(const  tlp::Vec4i &) const;
    
  bool operator>(const tlp::Vec4i &) const;
  bool operator<(const tlp::Vec4i &) const;
  bool operator!=(const tlp::Vec4i &) const;
  bool operator==(const tlp::Vec4i &) const;
    
  tlp::Vec4i & fill(const int &obj);
    
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

};

tlp::Vec4i operator*(const int &, const tlp::Vec4i &) const;

};

