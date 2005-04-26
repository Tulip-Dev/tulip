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
VECTORTLP VECTORTLP::operator+(const VECTORTLP &v) const { 
  return VECTORTLP(*this)+=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator+(const Obj& scalaire) const {
  return VECTORTLP(*this)+=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator-(const VECTORTLP &v) const { 
  return VECTORTLP(*this)-=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator-(const Obj& scalaire) const {
  return VECTORTLP(*this)-=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator*(const VECTORTLP &v) const { 
  return VECTORTLP(*this)*=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP  VECTORTLP::operator*(const Obj& scalaire) const {
  return VECTORTLP(*this)*=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator/(const VECTORTLP &v) const { 
  return VECTORTLP(*this)/=v;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator/(const Obj& scalaire) const {
  return VECTORTLP(*this)/=scalaire;
}
//======================================================
template <typename Obj,unsigned int SIZE>
VECTORTLP VECTORTLP::operator^(const VECTORTLP &v) const { 
  VECTORTLP result;
  switch(SIZE){
  case 3:  
    result[0]=VECTORTLP::array[1]*v[2]-VECTORTLP::array[2]*v[1];
    result[1]=VECTORTLP::array[2]*v[0]-VECTORTLP::array[0]*v[2];
    result[2]=VECTORTLP::array[0]*v[1]-VECTORTLP::array[1]*v[0];
    return result;
    break;
  default :
    std::cerr << "cross product not implemented for dimension :" << SIZE << std::endl;
    break;
  }
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
    break;
  case 3:
    return sqrt(VECTORTLP::array[0]*VECTORTLP::array[0]+VECTORTLP::array[1]*VECTORTLP::array[1]+VECTORTLP::array[2]*VECTORTLP::array[2]);
    break;
  default :
    Obj tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=VECTORTLP::array[i]*VECTORTLP::array[i];
    return(sqrt(tmp));
    break;
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
    break;
  case 3:
    return sqrt((VECTORTLP::array[0]-c.VECTORTLP::array[0])*(VECTORTLP::array[0]-c.VECTORTLP::array[0])+(VECTORTLP::array[1]-c.VECTORTLP::array[1])*(VECTORTLP::array[1]-c.VECTORTLP::array[1])+(VECTORTLP::array[2]-c.VECTORTLP::array[2])*(VECTORTLP::array[2]-c.VECTORTLP::array[2]));
    break;
  default :
    Obj tmp=0;
    for (unsigned int i=0;i<SIZE;++i)
      tmp+=(VECTORTLP::array[i]-c.VECTORTLP::array[i])*(VECTORTLP::array[i]-c.VECTORTLP::array[i]);
    return(sqrt(tmp));
    break;
  }
}
//=======================================================================
