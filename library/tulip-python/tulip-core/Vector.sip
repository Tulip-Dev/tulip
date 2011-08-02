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
namespace tlp {
typedef tlp::Vector<int, 4> Vec4i;
}
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
%MethodCode
		sipCpp = new tlp::Vec3f();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a0;
		(*sipCpp)[2] = a0;
%End
	
	Vec3f(const float a0, const float a1, const float a2);
%MethodCode
		sipCpp = new tlp::Vec3f();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a1;
		(*sipCpp)[2] = a2;
%End
	
	Vec3f(const tlp::Vec3f &);
	
	float operator[](const unsigned int i) const;
	
	void __setitem__(int i, const float &value);
%MethodCode
	if (a0 < 3)
		(*sipCpp)[a0] = a1;
	else
		sipIsErr = 1;	
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
        sipRes = PyString_FromString(s.c_str());
%End

};

class Vec4f  {

%Docstring
This class represents a vector with 4 floating point components.
Constructors, operators and methods are similar to those from the :class:`tlp.Vec3f` class. 
%End

public:

	Vec4f();
	
	Vec4f(const float a0);
%MethodCode
		sipCpp = new tlp::Vec4f();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a0;
		(*sipCpp)[2] = a0;
		(*sipCpp)[3] = a0;
%End
	
	Vec4f(const float a0, const float a1, const float a2, const float a3);
%MethodCode
		sipCpp = new tlp::Vec4f();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a1;
		(*sipCpp)[2] = a2;
		(*sipCpp)[2] = a3;
		
%End
	
	Vec4f(const tlp::Vec4f &);
	
	float operator[](const unsigned int i) const;
	
	void __setitem__(int i, const float &value);
%MethodCode
	if (a0 < 4)
		(*sipCpp)[a0] = a1;
	else
		sipIsErr = 1;	
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
        sipRes = PyString_FromString(s.c_str());
%End

};

class Vec4i  {

%Docstring
This class represents a vector with 4 integer components.
Constructors, operators and methods are similar to those from the :class:`tlp.Vec3f` class. 
%End

public:

	Vec4i();
	
	Vec4i(const int a0);
%MethodCode
		sipCpp = new tlp::Vec4i();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a0;
		(*sipCpp)[2] = a0;
		(*sipCpp)[3] = a0;
%End
	
	Vec4i(const int a0, const int a1, const int a2, const int a3);
%MethodCode
		sipCpp = new tlp::Vec4i();
		(*sipCpp)[0] = a0;
		(*sipCpp)[1] = a1;
		(*sipCpp)[2] = a2;
		(*sipCpp)[2] = a3;
		
%End
	
	Vec4i(const tlp::Vec4i &);
	
	int operator[](const unsigned int i) const;
	
	void __setitem__(int i, const int &value);
%MethodCode
	if (a0 < 4)
		(*sipCpp)[a0] = a1;
	else
		sipIsErr = 1;	
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
        sipRes = PyString_FromString(s.c_str());
%End

};

};

