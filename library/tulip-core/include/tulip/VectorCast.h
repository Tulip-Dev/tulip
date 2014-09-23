/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
///@cond DOXYGEN_HIDDEN

//@TLPGEOLICENCE#

#ifndef _TLP_VECTORCAST_H
#define _TLP_VECTORCAST_H

#include <cassert>
#include <tulip/Vector.h>

#define TEMPLATEVECTORCAST template <typename Obj, unsigned int SIZE, typename TYPE >

#define VECTORCAST tlp::VectorCast<Obj, SIZE, TYPE>

namespace tlp {
/**
 * @ingroup Structures
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
class TLP_SCOPE VectorCast : public Vector<Obj,SIZE> {
public:
  VectorCast() {}
  inline explicit VectorCast(const tlp::Vector<float,3> &v):Vector<Obj,SIZE>(v) {
  }

  inline TYPE & operator*=(const Obj obj) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator*=(obj);
  }
  inline TYPE & operator*=(const Vector<Obj,SIZE> &vec) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator*=(vec);
  }
  inline TYPE & operator/=(const Obj obj) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator/=(obj);
  }
  inline TYPE & operator/=(const Vector<Obj,SIZE> &vec) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator/=(vec);
  }
  inline TYPE & operator+=(const Obj obj) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator+=(obj);
  }
  inline TYPE & operator+=(const Vector<Obj,SIZE> &vec) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator+=(vec);
  }
  inline TYPE & operator-=(const Obj obj) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator-=(obj);
  }
  inline TYPE & operator-=(const Vector<Obj,SIZE> &vec) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator-=(vec);
  }
  inline TYPE & operator^=(const Vector<Obj,SIZE> &vec) {
    return (TYPE &)this->Vector<Obj,SIZE>::operator^=(vec);
  }
  inline TYPE & fill(const Obj obj) {
    return (TYPE &)this->Vector<Obj,SIZE>::fill(obj);
  }

  inline TYPE operator*(const Vector<Obj,SIZE> &v) const {
    return VECTORCAST(*this)*=v;
  }
  inline TYPE operator*(const Obj v) const {
    return VECTORCAST(*this)*=v;
  }
  inline TYPE operator+(const Vector<Obj,SIZE> &v) const {
    return VECTORCAST(*this)+=v;
  }
  inline TYPE operator+(const Obj v) const {
    return VECTORCAST(*this)+=v;
  }
  inline TYPE operator-(const Vector<Obj,SIZE> &v) const {
    return VECTORCAST(*this)-=v;
  }
  inline TYPE operator-(const Obj v)const {
    return VECTORCAST(*this)-=v;
  }
  inline TYPE operator/(const Vector<Obj,SIZE> &v) const {
    return VECTORCAST(*this)/=v;
  }
  inline TYPE operator/(const Obj v) const {
    return VECTORCAST(*this)/=v;
  }
  inline TYPE operator^(const Vector<Obj,SIZE> &v) const {
    return VECTORCAST(*this)^=v;
  }

};




}

#endif
///@endcond
