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
    float norm () const;
    float dist (const tlp::Vec3f &) const;
    float dotProduct(const tlp::Vec3f &) const;
   
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

