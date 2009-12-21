//@TLPGEOLICENCE#

#ifndef _TLP_VECTORCAST_H
#define _TLP_VECTORCAST_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <tulip/Vector.h>

#define TEMPLATEVECTORCAST template <typename Obj, unsigned int SIZE, typename TYPE >

namespace tlp {
  /**
   * \addtogroup basic
   */ 

  /*@{*/
  /**
   * \brief class for mathematical vector
   *
   * Enables to create a Vector of Obj with a
   * fixed size and provides Mathematical operation. Mathematical 
   * operators must be defined for Obj. Out of bound accesses are only checked
   * in debug mode.
   *
   * \author : David Auber auber@tulip-software.org
   * \version 0.0.1 24/01/2003 
   */
  TEMPLATEVECTORCAST
    class VectorCast : public Vector<Obj,SIZE> {
    public:
    VectorCast() {}
    inline explicit VectorCast(const tlp::Vector<float,3> &v):Vector<Obj,SIZE>(v) {
    }

    inline TYPE & operator*=(const Obj &obj) {
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator*=(obj));
    }
    inline TYPE & operator*=(const Vector<Obj,SIZE> &vec){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator*=(vec));
    }
    inline TYPE & operator/=(const Obj &obj){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator/=(obj));
    }
    inline TYPE & operator/=(const Vector<Obj,SIZE> &vec){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator/=(vec));
    }
    inline TYPE & operator+=(const Obj &obj){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator+=(obj));
    }
    inline TYPE & operator+=(const Vector<Obj,SIZE> &vec){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator+=(vec));
    }
    inline TYPE & operator-=(const Obj &obj){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator-=(obj));
    }
    inline TYPE & operator-=(const Vector<Obj,SIZE> &vec){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator-=(vec));
    }
    inline TYPE & operator^=(const Vector<Obj,SIZE> &vec){
      return *((TYPE *)&this->Vector<Obj,SIZE>::operator^=(vec));
    }
    inline TYPE & fill(const Obj &obj){
      return *((TYPE *)&this->Vector<Obj,SIZE>::fill(obj));
    }

    inline TYPE operator*(const Vector<Obj,SIZE> &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) * (*((const Vector<Obj,SIZE> *) &v)) ));
    }
    inline TYPE operator*(const Obj &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) * v ));
    }
    inline TYPE operator+(const Vector<Obj,SIZE> &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) + (*((const Vector<Obj,SIZE> *) &v)) ));
    }
    inline TYPE operator+(const Obj &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) + v ));
    }
    inline TYPE operator-(const Vector<Obj,SIZE> &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) - (*((const Vector<Obj,SIZE> *) &v)) ));
    }
    inline TYPE operator-(const Obj &v)const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) - v ));
    }
    inline TYPE operator/(const Vector<Obj,SIZE> &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) / (*((const Vector<Obj,SIZE> *) &v)) ));
    }
    inline TYPE operator/(const Obj &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) / v ));
    }
    inline TYPE operator^(const Vector<Obj,SIZE> &v) const {
      return *((TYPE *)& ( (*(const Vector<Obj,SIZE> *)this) ^ (*((const Vector<Obj,SIZE> *) &v)) ));
    }

  };



  /*@}*/
}

#endif
