#include <math.h>
#include <limits>

#define VECTORTLP tlp::Vector<TYPE,SIZE>

//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP::Vector(int v) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i] = (TYPE) v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const TYPE &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]*=scalaire;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]*=vecto[i];
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const TYPE &scalaire) {
  assert(scalaire!=0);
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]/=scalaire;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i) {
    assert(vecto[i]!=0);
    VECTORTLP::array[i]/=vecto[i];
  }
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const TYPE &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]+=scalaire;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]+=vecto[i];
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const TYPE &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]-=scalaire;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]-=vecto[i];
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator^=(const VECTORTLP &v) { 
  VECTORTLP tmp(*this);
  switch(SIZE){
  case 3:  
    (*this)[0] = tmp[1]*v[2] - tmp[2]*v[1];
    (*this)[1] = tmp[2]*v[0] - tmp[0]*v[2];
    (*this)[2] = tmp[0]*v[1] - tmp[1]*v[0];
    break;
  default :
    std::cerr << "cross product not implemented for dimension :" << SIZE << std::endl;
    break;
  }
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)+=v;
}
//======================================================
/* @todo find why the compiler do not want that code here
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::minVector(const VECTORTLP &u, const VECTORTLP &v) {
   VECTORTLP tmp;
   for(unsigned int i = 0; i<SIZE; ++i)
       tmp[i] = std::min(u[i], v[i]);
    return tmp;
}
*/
//======================================================
/* @todo find why the compiler do not want that code here
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::maxVector(const VECTORTLP &u, VECTORTLP &v) {
   VECTORTLP tmp;
   for(unsigned int i = 0; i<SIZE; ++i)
       tmp[i] = std::max(u[i], v[i]);
    return tmp;
}
*/
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)+=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u) -= v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)*=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP  tlp::operator*(const TYPE& scalaire, const VECTORTLP &u ) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)/=v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const TYPE& scalaire) {
  return VECTORTLP(u)/=scalaire;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) ^= v;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u) {
  return VECTORTLP(u) *= (TYPE) -1;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator>(const VECTORTLP &vecto) const {
  for (unsigned int i=0;i<SIZE;++i)
    if (VECTORTLP::array[i] <= vecto[i]) return false;
  return true;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator<(const VECTORTLP &vecto) const {
  for (unsigned int i=0;i<SIZE;++i)
    if (VECTORTLP::array[i] >= vecto[i]) return false;
  return true;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  if (std::numeric_limits<TYPE>::is_integer ||
      !std::numeric_limits<TYPE>::is_specialized) {
  for (unsigned int i=0;i<SIZE;++i)
    if (VECTORTLP::array[i]!=vecto[i]) return true;
  } else {
      for (unsigned int i=0;i<SIZE;++i) {
	TYPE tmp = VECTORTLP::array[i] - vecto[i];
	if (tmp > std::numeric_limits<TYPE>::epsilon() ||
	    tmp < -std::numeric_limits<TYPE>::epsilon())
	  return true;
      }
  }
  return false;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
bool VECTORTLP::operator==(const VECTORTLP &vecto) const {
  if (std::numeric_limits<TYPE>::is_integer ||
      !std::numeric_limits<TYPE>::is_specialized) {
    for (unsigned int i=0;i<SIZE;++i)
      if (VECTORTLP::array[i]!=vecto[i]) return false;
  } else {
      for (unsigned int i=0;i<SIZE;++i) {
	TYPE tmp = VECTORTLP::array[i] - vecto[i];
	if (tmp > std::numeric_limits<TYPE>::epsilon() ||
	    tmp < -std::numeric_limits<TYPE>::epsilon())
	  return false;
      }
    }
  return true;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE VECTORTLP::dotProduct(const VECTORTLP &v) const{
  assert (SIZE>0);
  TYPE tmpO= VECTORTLP::array[0] * v[0];
  for (unsigned int i=1;i<SIZE;++i)
    tmpO+= VECTORTLP::array[i] * v[i];
  return tmpO;
}
//======================================================
template <typename TYPE,unsigned int SIZE>
VECTORTLP & VECTORTLP::fill(const TYPE& scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]=scalaire;
  return (*this);
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE  VECTORTLP::norm() const{
  switch(SIZE){
  case 1:  
    return VECTORTLP::array[0];
  case 2:
    return sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]);
  case 3:
    return sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]+VECTORTLP::array[2]*VECTORTLP::array[2]);
  default :
    TYPE tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=VECTORTLP::array[i]*VECTORTLP::array[i];
    return(sqrt(tmp));
  }
}
//======================================================
template <typename TYPE,unsigned int SIZE>
TYPE  VECTORTLP::dist(const VECTOR &c) const{
  switch(SIZE){
  case 1:  
    return 0;
  case 2:
    return sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1]));
   case 3:
    return sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1])+(VECTORTLP::array[2]-c.VECTORTLP::array[2])*(VECTORTLP::array[2]-c.VECTORTLP::array[2]));
   default :
    TYPE tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=(VECTORTLP::array[i]-c.VECTORTLP::array[i])*(VECTORTLP::array[i]-c.VECTORTLP::array[i]);
    return(sqrt(tmp));
   }
}
//=======================================================================
