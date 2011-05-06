
%ModuleHeaderCode
#include <tulip/Matrix.h>
namespace tlp {
typedef tlp::Matrix<float, 4> Mat4f;
};
%End

namespace tlp {

class Mat4f {
%TypeHeaderCode
#include <tulip/Matrix.h>
%End
  public:
  
    Mat4f();
    
    Mat4f(const tlp::Mat4f&a);
      
   	//Mat4f(const std::vector<std::vector<float> > &covarianceMatrix);
     
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
    
    tlp::Mat4f operator*(const tlp::Mat4f &mat);
    
    tlp::Mat4f operator*(const float obj);
    
    tlp::Mat4f & operator/=(const tlp::Mat4f &mat);
    
    tlp::Mat4f & operator/=(const float obj);
    
    tlp::Mat4f operator/(const tlp::Mat4f &mat);
    
    tlp::Mat4f operator/(const float obj);
    
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
        sipRes = PyString_FromString(repr.c_str());
%End

    };
};
