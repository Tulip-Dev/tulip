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
// | Tulip Python Bindings                           |
// | inspired from bindings by the Booggie project development team      |
// | (http://booggie.org/)                           |
// +-------------------------------------------------------------------------+


%ModuleHeaderCode
#include <tulip/Matrix.h>
namespace tlp {
typedef tlp::Matrix<float, 3> Mat3f;
typedef tlp::Matrix<float, 4> Mat4f;
};
%End

namespace tlp {

class Mat3f {
%TypeHeaderCode
#include <tulip/Matrix.h>
%End
  public:

    Mat3f();

    Mat3f(const tlp::Mat3f&a);

    tlp::Mat3f& fill(float obj);

    float determinant() const;

    tlp::Mat3f& transpose();

    tlp::Mat3f& inverse();

    tlp::Mat3f operator+(const tlp::Mat3f &mat) const;

    tlp::Mat3f & operator+=(const tlp::Mat3f &mat);

    tlp::Mat3f operator-(const tlp::Mat3f &mat) const;

    tlp::Mat3f & operator-=(const tlp::Mat3f &mat);

    bool operator==(const tlp::Mat3f &) const;

    bool operator!=(const tlp::Mat3f &) const;

    tlp::Mat3f & operator*=(const tlp::Mat3f &mat);

    tlp::Mat3f & operator*=(const float obj);

    tlp::Mat3f operator*(const tlp::Mat3f &mat) const;

    tlp::Vec3f operator*(const tlp::Vec3f &v) const;

    tlp::Mat3f operator*(const float obj) const;

    tlp::Mat3f & operator/=(const tlp::Mat3f &mat);

    tlp::Mat3f & operator/=(const float obj);

    tlp::Mat3f operator/(const tlp::Mat3f &mat) const;

    tlp::Mat3f operator/(const float obj) const;

    tlp::Mat3f cofactor() const;

    tlp::Vec3f powerIteration(const unsigned int nIterations) const;

    tlp::Vec3f & operator[](const unsigned int i)  /NoCopy/;

    void __setitem__(int i, const tlp::Vec3f &value);
%MethodCode
    if (a0 < 3)
        (*sipCpp)[a0] = *a1;
    else
        sipIsErr = 1;
%End

    SIP_PYOBJECT __repr__() const;
%MethodCode
        std::string repr = "";
        for (int i = 0 ; i < 3 ; ++i) {
          std::ostringstream oss;
          oss << (*sipCpp)[i];
          std::string s = oss.str();
          s[0] = '[';
          s[s.size() - 1] = ']';
          repr += (s + "\n");
        }
#if PY_MAJOR_VERSION >= 3
        sipRes = PyUnicode_FromString(repr.c_str());
#else
        sipRes = PyString_FromString(repr.c_str());
#endif
%End

};

class Mat4f {
%TypeHeaderCode
#include <tulip/Matrix.h>
%End
  public:
  
    Mat4f();
    
    Mat4f(const tlp::Mat4f&a);
      
    tlp::Mat4f& fill(float obj);
     
    float determinant() const;
    
    tlp::Mat4f& transpose();
    
    tlp::Mat4f& inverse();
    
    tlp::Mat4f operator+(const tlp::Mat4f &mat) const;
    
    tlp::Mat4f & operator+=(const tlp::Mat4f &mat);
    
    tlp::Mat4f operator-(const tlp::Mat4f &mat) const;
    
    tlp::Mat4f & operator-=(const tlp::Mat4f &mat);
    
    bool operator==(const tlp::Mat4f &) const;
    
    bool operator!=(const tlp::Mat4f &) const;
     
    tlp::Mat4f & operator*=(const tlp::Mat4f &mat);
    
    tlp::Mat4f & operator*=(const float obj);
    
    tlp::Mat4f operator*(const tlp::Mat4f &mat) const;
    
    tlp::Vec4f operator*(const tlp::Vec4f &v) const;
    
    tlp::Mat4f operator*(const float obj) const;
    
    tlp::Mat4f & operator/=(const tlp::Mat4f &mat);
    
    tlp::Mat4f & operator/=(const float obj);
    
    tlp::Mat4f operator/(const tlp::Mat4f &mat) const;
    
    tlp::Mat4f operator/(const float obj) const;
    
    tlp::Mat4f cofactor() const;
    
    tlp::Vec4f powerIteration(const unsigned int nIterations) const;
    
    tlp::Vec4f & operator[](const unsigned int i)  /NoCopy/;
	
	void __setitem__(int i, const tlp::Vec4f &value);
%MethodCode
	if (a0 < 4)
		(*sipCpp)[a0] = *a1;
	else
		sipIsErr = 1;	
%End

	SIP_PYOBJECT __repr__() const;
%MethodCode	
		std::string repr = "";
		for (int i = 0 ; i < 4 ; ++i) {
			std::ostringstream oss;
			oss << (*sipCpp)[i];
			std::string s = oss.str();
			s[0] = '[';
			s[s.size() - 1] = ']';
			repr += (s + "\n");
		}
#if PY_MAJOR_VERSION >= 3
    sipRes = PyUnicode_FromString(repr.c_str());
#else
    sipRes = PyString_FromString(repr.c_str());
#endif
%End

    };
};
