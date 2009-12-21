#include <math.h>

#define VECTORTLP tlp::Vector<Obj,SIZE>

//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const Obj &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]*=scalaire;
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator*=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]*=vecto[i];
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const Obj &scalaire) {
  assert(scalaire!=0);
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]/=scalaire;
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator/=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i) {
    assert(vecto[i]!=0);
    VECTORTLP::array[i]/=vecto[i];
  }
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const Obj &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]+=scalaire;
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator+=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]+=vecto[i];
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const Obj &scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]-=scalaire;
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::operator-=(const VECTORTLP &vecto) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]-=vecto[i];
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
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
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)+=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator+(const VECTORTLP &u, const Obj& scalaire) {
  return VECTORTLP(u)+=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u) -= v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator-(const VECTORTLP &u, const Obj& scalaire) {
  return VECTORTLP(u) -= scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator*(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)*=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP  tlp::operator*(const VECTORTLP &u, const Obj& scalaire) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP  tlp::operator*(const Obj& scalaire, const VECTORTLP &u ) {
  return VECTORTLP(u)*=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const VECTORTLP &v) { 
  return VECTORTLP(u)/=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator/(const VECTORTLP &u, const Obj& scalaire) {
  return VECTORTLP(u)/=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP tlp::operator^(const VECTORTLP &u, const VECTORTLP &v) {
  return VECTORTLP(u) ^= v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
bool VECTORTLP::operator!=(const VECTORTLP &vecto) const {
  for (unsigned int i=0;i<SIZE;++i)
    if (VECTORTLP::array[i]!=vecto[i]) return true;
  return false;
}
//======================================================
template <typename Obj,unsigned int SIZE>
bool VECTORTLP::operator==(const VECTORTLP &vecto) const {
  for (unsigned int i=0;i<SIZE;++i)
    if (VECTORTLP::array[i]!=vecto[i]) return false;
  return true;
}
//======================================================
template <typename Obj,unsigned int SIZE>
Obj VECTORTLP::dotProduct(const VECTORTLP &v) const{
  assert (SIZE>0);
  Obj tmpO= VECTORTLP::array[0] * v[0];
  for (unsigned int i=1;i<SIZE;++i)
    tmpO+= VECTORTLP::array[i] * v[i];
  return tmpO;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP & VECTORTLP::fill(const Obj& scalaire) {
  for (unsigned int i=0;i<SIZE;++i)
    VECTORTLP::array[i]=scalaire;
  return (*this);
}
//======================================================
template <typename Obj,unsigned int SIZE>
Obj  VECTORTLP::norm() const{
  switch(SIZE){
  case 1:  
    return VECTORTLP::array[0];
  case 2:
    return sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]);
  case 3:
    return sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]+VECTORTLP::array[2]*VECTORTLP::array[2]);
  default :
    Obj tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=VECTORTLP::array[i]*VECTORTLP::array[i];
    return(sqrt(tmp));
  }
}
//======================================================
template <typename Obj,unsigned int SIZE>
Obj  VECTORTLP::dist(const VECTOR &c) const{
  switch(SIZE){
  case 1:  
    return 0;
  case 2:
    return sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1]));
   case 3:
    return sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1])+(VECTORTLP::array[2]-c.VECTORTLP::array[2])*(VECTORTLP::array[2]-c.VECTORTLP::array[2]));
   default :
    Obj tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=(VECTORTLP::array[i]-c.VECTORTLP::array[i])*(VECTORTLP::array[i]-c.VECTORTLP::array[i]);
    return(sqrt(tmp));
   }
}
//=======================================================================
