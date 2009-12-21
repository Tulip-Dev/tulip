//@TLPGEOLICENCE#

#ifndef _TLP_GEO_VECTOR_H
#define _TLP_GEO_VECTOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <tulip/Array.h>

#define VECTOR Vector<Obj,SIZE>
#define TEMPLATEVECTOR template <typename Obj, unsigned int SIZE>

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
  TEMPLATEVECTOR
    class Vector:public Array<Obj,SIZE> {
    public:

    inline VECTOR & operator*=(const Obj &);
    inline VECTOR & operator*=(const VECTOR &);
    inline VECTOR & operator/=(const Obj &);
    inline VECTOR & operator/=(const VECTOR &);
    inline VECTOR & operator+=(const Obj &);
    inline VECTOR & operator+=(const VECTOR &);
    inline VECTOR & operator-=(const Obj &);
    inline VECTOR & operator-=(const VECTOR &);
    inline VECTOR & operator^=(const VECTOR &);
    
    inline bool operator!=(const VECTOR &) const;
    inline bool operator==(const VECTOR &) const;
    inline VECTOR & fill(const Obj &obj);
    inline Obj norm () const;
    inline Obj dist (const VECTOR &) const;
    inline Obj dotProduct(const VECTOR &) const;
  };

  TEMPLATEVECTOR
    inline VECTOR operator*(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator*(const Obj &   , const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator*(const VECTOR &, const Obj &);
  TEMPLATEVECTOR
    inline VECTOR operator+(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator+(const VECTOR &, const Obj &);
  TEMPLATEVECTOR
    inline VECTOR operator-(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator-(const VECTOR &, const Obj &);
  TEMPLATEVECTOR
    inline VECTOR operator/(const VECTOR &, const VECTOR &);
  TEMPLATEVECTOR
    inline VECTOR operator/(const VECTOR &, const Obj &);
  TEMPLATEVECTOR
    inline VECTOR operator^(const VECTOR &, const VECTOR &);

  /*@}*/
}


#include "./cxx/Vector.cxx"
#endif
